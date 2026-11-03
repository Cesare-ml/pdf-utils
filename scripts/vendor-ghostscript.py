#!/usr/bin/env python3
"""Stage official Ghostscript Win32 for a *developer-built* single EXE.

This tool runs on the BUILD computer, not on Windows 7 recipients.
It downloads the pinned Artifex release (or accepts an offline installer),
checks the official SHA-256, extracts the NSIS installer using 7-Zip,
and stages runtime files plus legal notices under vendor/ghostscript-win32.

No installer is run; no files are installed on the host PC.
The staged files still require a license-compliant distribution and a clean
Windows 7 SP1 compatibility test before publishing.
"""
from __future__ import annotations
import argparse
import hashlib
import shutil
import subprocess
import sys
import tempfile
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
VERSION = "10.08.0"
SOURCE_TAG = "gs10080"
BASENAME = "gs10080w32.exe"
URL = ("https://github.com/ArtifexSoftware/ghostpdl-downloads/"
       f"releases/download/{SOURCE_TAG}/{BASENAME}")
SHA256 = "42f9e97ea2580bc24e2604cc03a2213bdba6094869dbe9f8ede50f8dd86b7463"
SOURCE_URL = f"https://github.com/ArtifexSoftware/ghostpdl/releases/tag/{SOURCE_TAG}"


def sha256_file(path: Path) -> str:
    sha = hashlib.sha256()
    with path.open('rb') as inp:
        for block in iter(lambda: inp.read(1024 * 1024), b''):
            sha.update(block)
    return sha.hexdigest()


def download_official(destination: Path) -> None:
    print(f"Downloading the official Artifex {VERSION} Windows x86 installer")
    request = urllib.request.Request(URL, headers={"User-Agent": "PDF-Utils-Build/1.0"})
    tmp = destination.with_name(destination.name + ".partial")
    try:
        with urllib.request.urlopen(request, timeout=90) as source, tmp.open('wb') as output:
            shutil.copyfileobj(source, output, 1024 * 1024)
        if sha256_file(tmp).lower() != SHA256:
            raise ValueError("Ghostscript download SHA256 mismatch; file NOT trusted")
        tmp.replace(destination)
    finally:
        tmp.unlink(missing_ok=True)


def find_gs_root(extracted: Path) -> Path:
    matches = []
    for exe in extracted.rglob('gswin32c.exe'):
        if exe.is_file() and exe.parent.name.lower() == 'bin':
            root = exe.parent.parent
            if (exe.parent / 'gsdll32.dll').is_file():
                matches.append(root)
    if len(matches) != 1:
        raise ValueError(f"Expected exactly one 32-bit Ghostscript installation tree, found {len(matches)}")
    return matches[0]


def stage(extracted: Path, out: Path) -> dict:
    root = find_gs_root(extracted)
    for name in ('Resource', 'lib'):
        if not (root / name).is_dir():
            raise ValueError(f"Missing Ghostscript {name}/; check installer format")
    # Prefer direct COPYING; include copyright notices and license files.
    legal = []
    for pattern in ('COPYING', 'LICENSE', 'LICENSE.txt', 'AGPL-3.0.txt'):
        legal.extend(p for p in root.rglob(pattern) if p.is_file())
    # Some Ghostscript installers store AGPL as doc/Copying.htm.
    if not legal:
        legal = [p for p in root.rglob('*') if p.is_file() and p.name.lower() in
                 ('copying.htm', 'copying.html', 'agpl-3.0.html')]
    if not legal:
        raise ValueError("Installer lacks readily identifiable AGPL license; do not release without license text")
    if out.exists():
        # Avoid mixing previous version's DLLs with freshly extracted runtime.
        shutil.rmtree(out)
    (out / 'bin').mkdir(parents=True)
    for name in ('gswin32c.exe', 'gsdll32.dll'):
        shutil.copy2(root / 'bin' / name, out / 'bin' / name)
    # Package extra DLL dependencies next to Ghostscript (if supplied by the installer).
    for other in (root / 'bin').glob('*.dll'):
        if other.name.lower() != 'gsdll32.dll':
            shutil.copy2(other, out / 'bin' / other.name)
    for name in ('Resource', 'lib'):
        shutil.copytree(root / name, out / name)
    if (root / 'fonts').is_dir():
        shutil.copytree(root / 'fonts', out / 'fonts')
    # License file required by package-portable.py, plus source provenance.
    shutil.copy2(legal[0], out / 'LICENSE.txt')
    (out / 'THIRD-PARTY-NOTICE.txt').write_text(
        f"Ghostscript {VERSION} (Windows x86), copyright Artifex Software.\n"
        f"Distribution: AGPL-3.0 or separate Artifex commercial license.\n"
        f"Official installer: {URL}\n"
        f"Installer SHA-256: {SHA256}\n"
        f"Corresponding original upstream source: {SOURCE_URL}\n"
        "Distributor must comply with the applicable license, including\n"
        "availability of complete corresponding source and all notices.\n",
        encoding='ascii')
    return {'root':str(root), 'files':sum(1 for p in out.rglob('*') if p.is_file()),
            'total_bytes':sum(p.stat().st_size for p in out.rglob('*') if p.is_file())}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--installer', type=Path, help='Previously downloaded official gs10080w32.exe')
    parser.add_argument('--output', type=Path, default=ROOT / 'vendor' / 'ghostscript-win32')
    parser.add_argument('--keep-installer', action='store_true')
    args = parser.parse_args()
    extractor = shutil.which('7z') or shutil.which('7zz') or shutil.which('7za')
    if not extractor:
        parser.error('7-Zip command line (7z / 7zz / 7za) is needed only on the build computer')
    with tempfile.TemporaryDirectory(prefix='pdfutils-gs-stage-') as t:
        work=Path(t)
        installer=args.installer.resolve() if args.installer else work / BASENAME
        if not args.installer:
            download_official(installer)
        if not installer.is_file() or sha256_file(installer).lower()!=SHA256:
            parser.error('Official Ghostscript installer SHA256 mismatch: aborting')
        directory=work / 'unpacked'
        directory.mkdir()
        subprocess.run([extractor, 'x', '-y', f'-o{directory}', str(installer)], check=True,
                       stdout=subprocess.DEVNULL)
        metadata=stage(directory, args.output.resolve())
        print(f"Staged {metadata['files']} Ghostscript files ({metadata['total_bytes']:,} bytes)")
        print('NOTE: Win7 compatibility and AGPL source-distribution compliance must still be verified.')
        if args.keep_installer and not args.installer:
            target=(ROOT / 'vendor' / BASENAME)
            target.parent.mkdir(exist_ok=True, parents=True)
            shutil.copy2(installer,target)
            print(f"Kept verified installer at {target}")

if __name__=='__main__':
    main()