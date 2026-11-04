#!/usr/bin/env python3
"""Conservative, offline PE preflight for Windows 7 SP1 no-install distributions.

Checks 32-bit PE CPU/subsystem and imported DLL dependencies of the launcher
and all packaged PE files. Missing/non-standard DLLs and known post-Win7 API
imports fail the check. This is NOT a substitute for a clean Win7 VM test.
"""
from __future__ import annotations
import argparse
import struct
from pathlib import Path

SYSTEM_DLLS = {
    'kernel32.dll','kernelbase.dll','user32.dll','gdi32.dll',
    'shell32.dll','ole32.dll','oleaut32.dll','advapi32.dll','rpcrt4.dll',
    'mscms.dll','comdlg32.dll','comctl32.dll','shlwapi.dll','winspool.drv',
    'ws2_32.dll','wininet.dll','urlmon.dll','dnsapi.dll','wtsapi32.dll',
    'ntdll.dll','version.dll','imm32.dll','winmm.dll','msvcrt.dll',
    'crypt32.dll','bcrypt.dll','secur32.dll','gdi32.dll',
    'setupapi.dll','usp10.dll','dwmapi.dll','iphlpapi.dll',
    'psapi.dll','normaliz.dll','dbghelp.dll','mpr.dll','winhttp.dll',
    'msimg32.dll','netapi32.dll','wsock32.dll','mswsock.dll','sensapi.dll',
}
# This subset catches common mistakes; full API-version correctness needs Win7 VM.
UNSUPPORTED = {
    'GetDpiForWindow', 'SetProcessDpiAwareness', 'GetSystemTimePreciseAsFileTime',
    'GetPackageFamilyName', 'GetCurrentPackageId', 'GetCurrentPackageFullName',
    'IsWow64Process2', 'GetFileInformationByHandleEx2', 'GetThreadInformation',
    'GetSystemTimePreciseAsFileTime',
    'GetProcessInformation', 'GetSystemCpuSetInformation',
}

def _u16(data: bytes, pos: int) -> int:
    if pos < 0 or pos+2 > len(data):raise ValueError('Truncated PE header')
    return struct.unpack_from('<H',data,pos)[0]

def _u32(data: bytes, pos: int) -> int:
    if pos < 0 or pos+4 > len(data):raise ValueError('Truncated PE header')
    return struct.unpack_from('<I',data,pos)[0]

def parse_pe(path: Path) -> dict:
    data=path.read_bytes()
    if len(data)<256 or data[:2]!=b'MZ':raise ValueError('Not an MZ PE binary')
    pe=_u32(data,0x3c)
    if pe+24>len(data) or data[pe:pe+4]!=b'PE\0\0':raise ValueError('No PE header')
    cpu=_u16(data,pe+4)
    count=_u16(data,pe+6)
    sizeopt=_u16(data,pe+20)
    opt=pe+24
    if _u16(data,opt)!=0x10b:raise ValueError('Only 32-bit PE is supported')
    if opt+sizeopt>len(data) or sizeopt < 112:raise ValueError('Incomplete PE option header')
    min_os=(_u16(data,opt+40),_u16(data,opt+42))
    subsystem=(_u16(data,opt+48),_u16(data,opt+50))
    imp_rva=_u32(data,opt+104)
    sections=[]
    for j in range(count):
        h=opt+sizeopt+j*40
        if h+40>len(data):raise ValueError('Truncated section header')
        size=max(_u32(data,h+8),_u32(data,h+16))
        sections.append((_u32(data,h+12),size,_u32(data,h+20)))
    def rva_offset(rva):
        for va,size,ptr in sections:
            if va<=rva<va+size:
                off=ptr+rva-va
                if off<len(data):return off
        raise ValueError(f'Unmapped RVA 0x{rva:x}')
    def cstr_at(off):
        end=data.find(b'\0',off, min(off+1024,len(data)))
        if end<0:raise ValueError('Unterminated import name')
        return data[off:end].decode('ascii',errors='strict')
    dlls={}
    if imp_rva:
        imports=rva_offset(imp_rva)
        for j in range(1024):
            entry=imports+j*20
            if entry+20>len(data):raise ValueError('Truncated import table')
            thunk,_,_,name_rva,iat=struct.unpack_from('<IIIII',data,entry)
            if not (thunk or name_rva or iat):break
            dllname=cstr_at(rva_offset(name_rva)).lower()
            names=[]
            pos=rva_offset(thunk or iat)
            for k in range(30000):
                hint=_u32(data,pos+k*4)
                if hint==0:break
                if hint&0x80000000:names.append(f'ordinal-{hint&0xffff}')
                else:names.append(cstr_at(rva_offset(hint)+2))
            else:raise ValueError('Import entries exceed limit')
            dlls[dllname]=names
        else:raise ValueError('Import descriptors exceed limit')
    return {'cpu':cpu,'min_os':min_os,'subsystem':subsystem,'imports':dlls}


def verify(root: Path, launcher: Path) -> list[str]:
    errors=[]
    files=[launcher]+sorted(p for p in root.rglob('*') if p.is_file() and p.suffix.lower() in ('.exe','.dll'))
    delivered={p.name.lower() for p in files}
    for path in files:
        try:
            info=parse_pe(path)
            if info['cpu']!=0x14c:errors.append(f'{path.name}: non-x86 executable')
            if info['subsystem'] > (6,1):errors.append(f'{path.name}: subsystem requires newer than Win7 {info["subsystem"]}')
            if info['min_os'] > (6,1):errors.append(f'{path.name}: PE requires newer than Win7 {info["min_os"]}')
            for dll,imports in info['imports'].items():
                if dll in ('shcore.dll', 'gdi32full.dll'):
                    errors.append(f'{path.name}: {dll} is not a Windows 7 system DLL')
                if dll.startswith('api-ms-win-') or dll.startswith('ext-ms-'):
                    errors.append(f'{path.name}: API-set import {dll}: compatibility unverified')
                elif dll=='ucrtbase.dll':
                    errors.append(f'{path.name}: UCRT dependency; unsupported on unpatched Win7 without assessment')
                elif dll not in SYSTEM_DLLS and dll not in delivered:
                    errors.append(f'{path.name}: external DLL missing from bundle: {dll}')
                for func in imports:
                    if func in UNSUPPORTED:
                        errors.append(f'{path.name}: {dll}!{func} requires a newer OS')
        except Exception as exc:
            errors.append(f'{path}: invalid PE ({exc})')
    return errors


def main() -> None:
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--launcher',type=Path,required=True)
    parser.add_argument('--ghostscript',type=Path,required=True)
    args=parser.parse_args()
    issues=verify(args.ghostscript,args.launcher)
    if issues:
        for issue in issues:print('ERROR:',issue)
        raise SystemExit(f'Win7 static preflight FAILED ({len(issues)} issues)')
    print('Win7 static PE preflight passed (32-bit, target 6.1, import names present).')
    print('NOTE: Runtime compatibility still requires real Windows 7 SP1 testing.')

if __name__=='__main__':main()