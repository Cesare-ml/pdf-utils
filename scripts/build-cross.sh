#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
mkdir -p dist build
python3 - <<'PY'
import re, pathlib
src=pathlib.Path('src/pdf_utils.c').read_text()
for dll,names in {
    'kernel32':['GetProcessHeap','HeapAlloc','HeapFree','CreateFileW','ReadFile','WriteFile','CloseHandle','GetFileSize','SetFilePointer','GetFileAttributesW','DeleteFileW','MoveFileExW','FindFirstFileW','FindNextFileW','FindClose','GetModuleFileNameW','GetSystemDirectoryW','GetEnvironmentVariableW','SearchPathW','CreateProcessW','WaitForSingleObject','GetExitCodeProcess','TerminateProcess','CreateThread','GetLastError','ExitProcess','GetModuleHandleW','SetUnhandledExceptionFilter'],
    'user32':['RegisterClassExW','CreateWindowExW','DefWindowProcW','ShowWindow','UpdateWindow','GetMessageW','TranslateMessage','DispatchMessageW','PostQuitMessage','LoadCursorW','SendMessageW','PostMessageW','SetWindowTextW','GetWindowTextW','MessageBoxW','EnableWindow','wsprintfA','wsprintfW'],
    'gdi32':['CreateFontW','DeleteObject'],
    'shell32':['SHBrowseForFolderW','SHGetPathFromIDListW'],
    'ole32':['CoTaskMemFree'],
    'comdlg32':['GetOpenFileNameW'],
    'mscms':['GetColorDirectoryW']
}.items():
    pathlib.Path('build/'+dll+'.def').write_text('LIBRARY '+dll+'.dll\nEXPORTS\n'+'\n'.join(names)+'\n')
PY
for arch in x64 x86; do
  if [ "$arch" = x64 ];then target=x86_64-pc-windows-msvc;else target=i686-pc-windows-msvc;fi
  mkdir -p "build/$arch"
  clang -target "$target" -O2 -fshort-wchar -fno-builtin -fno-stack-protector -ffreestanding -Werror -Wall -Wno-unused-function -c src/pdf_utils.c -o "build/$arch/pdf_utils.obj"
  clang -target "$target" -c src/stack_probe.c -o "build/$arch/stack_probe.obj"
  if [ "$arch" = x86 ]; then
     python3 - <<'PY2'
import pathlib,re,subprocess
sym=subprocess.check_output(['nm','-u','build/x86/pdf_utils.obj'],text=True)
imports=set(re.findall(r'__imp__?(\w+)(@\d+)?',sym))
for path in pathlib.Path('build').glob('*.def'):
    if path.stem not in ['kernel32','user32','gdi32','shell32','ole32','comdlg32','mscms']:continue
    lines=path.read_text().splitlines()
    mapping={line.strip().lower():line.strip() for line in lines[2:]}
    new=[]
    for name,argcount in sorted(imports):
        if name.lower() in mapping:
            new.append(('_'+name+argcount+'='+name) if argcount else name)
    pathlib.Path('build/x86/'+path.name).write_text('LIBRARY '+path.stem+'.dll\nEXPORTS\n'+'\n'.join(new)+'\n')
PY2
  fi
  for dll in kernel32 user32 gdi32 shell32 ole32 comdlg32 mscms; do
     definition="build/$dll.def"
     if [ "$arch" = x86 ];then definition="build/x86/$dll.def";fi
     lld-link /lib "/def:$definition" "/machine:$arch" "/out:build/$arch/$dll.lib"
  done
  lld-link /subsystem:windows,6.1 /entry:WinMainCRTStartup /nodefaultlib "/machine:$arch" "/out:dist/PDF-Utils-$arch.exe" "build/$arch/pdf_utils.obj" "build/$arch/stack_probe.obj" build/$arch/*.lib
  file "dist/PDF-Utils-$arch.exe"
done