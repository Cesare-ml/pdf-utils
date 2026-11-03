#!/usr/bin/env python3
"""Make a *single-file* PDF Utils portable Windows 7 exe from verified Ghostscript runtime files.

Usage:
  PDFUTILS_PORTABLE=1 bash scripts/build-cross.sh
  python scripts/package-portable.py --ghostscript vendor/ghostscript-win32

Ghostscript files must be staged as vendor/ghostscript-win32/bin/gswin32c.exe,
.../bin/gsdll32.dll, .../lib/, .../Resource/, and COPYING (or LICENSE.txt).
No installer, online download, or third-party binary is redistributed by this
script unless supplied explicitly by the developer. The combined work must
comply with the Ghostscript AGPL/commercial license before distribution.
"""
from __future__ import annotations
import argparse
import hashlib
import importlib.util
import struct
from pathlib import Path

MAGIC = b"PDUGS001"
ALLOWED = set("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-/ ")
ROOT = Path(__file__).resolve().parent.parent


def pe_machine(path: Path) -> int:
    with path.open("rb") as f:
        dos = f.read(64)
        if len(dos) < 64 or dos[:2] != b"MZ":
            raise ValueError(f"{path} is not a PE executable")
        f.seek(struct.unpack_from("<I", dos, 0x3C)[0])
        hdr = f.read(6)
        if len(hdr) != 6 or hdr[:4] != b"PE\0\0":
            raise ValueError(f"{path} PE signature invalid")
        return struct.unpack_from("<H", hdr, 4)[0]


def build(base: Path, gsdir: Path, out: Path, *, allow_test_fixture: bool = False) -> None:
    base, gsdir, out = map(lambda x: x.resolve(), (base, gsdir, out))
    if out == base or gsdir == base.parent:
        raise ValueError("Output must be separate from base binary and vendor files")
    if pe_machine(base) != 0x14C:
        raise ValueError("The portable launcher must be a 32-bit Windows PE")
    must_exist = ["bin/gswin32c.exe", "bin/gsdll32.dll"]
    for name in must_exist:
        if not (gsdir / name).is_file() or pe_machine(gsdir / name) != 0x14C:
            raise ValueError(f"Missing or non-Win32 Ghostscript component: {name}")
    if not (gsdir / "lib").is_dir() or not (gsdir / "Resource").is_dir():
        raise ValueError("Ghostscript lib/ and Resource/ directories are required")
    if not any((gsdir / name).is_file() for name in ("COPYING", "LICENSE", "LICENSE.txt", "AGPL-3.0.txt")):
        raise ValueError("Include the Ghostscript license text in the vendor directory")
    if not allow_test_fixture:
        # A release must come from the hash-checked upstream installer,
        # not a fake/truncated file or manually copied unverified DLL.
        notice = gsdir / "THIRD-PARTY-NOTICE.txt"
        if not notice.is_file():
            raise ValueError("Missing verified upstream provenance: run scripts/vendor-ghostscript.py")
        expected = "42f9e97ea2580bc24e2604cc03a2213bdba6094869dbe9f8ede50f8dd86b7463"
        if f"Installer SHA-256: {expected}" not in notice.read_text(encoding='ascii'):
            raise ValueError("Unexpected Ghostscript version/checksum provenance")
        if (gsdir / "bin" / "gsdll32.dll").stat().st_size < 1000000:
            raise ValueError("Bundled Ghostscript DLL is implausibly small")
        spec = importlib.util.spec_from_file_location(
            'win7_preflight', ROOT / 'scripts' / 'check-win7-pe.py')
        mod = importlib.util.module_from_spec(spec)
        assert spec.loader is not None
        spec.loader.exec_module(mod)
        issues = mod.verify(gsdir, base)
        if issues:
            raise ValueError('Windows 7 static PE preflight failed:\n' + '\n'.join(issues))
    files = sorted(p for p in gsdir.rglob("*") if p.is_file())
    if not files or len(files) > 4096:
        raise ValueError("Invalid number of Ghostscript runtime files")
    for p in files:
        rel = p.relative_to(gsdir).as_posix()
        if p.is_symlink() or len(rel) > 230 or any(c not in ALLOWED for c in rel):
            raise ValueError(f"Nonportable or dangerous file path: {rel}")
        if any(seg in (".", "..", "") for seg in rel.split("/")):
            raise ValueError(f"Dangerous path: {rel}")
        if p.stat().st_size > 256 * 1024 * 1024:
            raise ValueError(f"Component too large: {rel}")
    out.parent.mkdir(parents=True, exist_ok=True)
    sha = hashlib.sha256()
    with base.open("rb") as src, out.open("wb") as dst:
        while (chunk := src.read(1024 * 1024)):
            dst.write(chunk)
            sha.update(chunk)
        start_offset = dst.tell()
        if start_offset >= 2**31:
            raise ValueError("Launcher too large")
        for p in files:
            name = p.relative_to(gsdir).as_posix().encode("ascii")
            bloblen = p.stat().st_size
            header = struct.pack("<HI", len(name), bloblen) + name
            dst.write(header)
            sha.update(header)
            with p.open("rb") as f:
                while (chunk := f.read(1024 * 1024)):
                    dst.write(chunk)
                    sha.update(chunk)
        footer = MAGIC + struct.pack("<II", start_offset, len(files))
        dst.write(footer)
        sha.update(footer)
    print(f"Portable executable: {out} ({out.stat().st_size:,} bytes)")
    print(f"Components bundled: {len(files)}")
    print(f"SHA256: {sha.hexdigest()}")
    print("WARNING: actual Windows 7 operation and PDF/A conformance still need validation.")


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--base", type=Path, default=ROOT / "dist" / "PDF-Utils-Win7-portable-base.exe")
    p.add_argument("--ghostscript", required=True, type=Path)
    p.add_argument("--output", type=Path, default=ROOT / "dist" / "PDF-Utils-Portable-Win7-Win11.exe")
    a = p.parse_args()
    build(a.base, a.ghostscript, a.output)


if __name__ == "__main__":
    main()