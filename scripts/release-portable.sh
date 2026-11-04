#!/usr/bin/env bash
# Reproducible developer-side process for the single-file Windows 7/11 EXE.
# This script is NOT run on the target Windows machines.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

usage() {
  cat <<'EOF'
Usage:
  bash scripts/release-portable.sh [--installer /path/to/gs10080w32.exe]

Requires on the BUILD machine: Python 3.9+, Clang, lld-link, binutils,
7-Zip (7z / 7zz / 7za), and an internet connection if --installer is omitted.

Outputs ONE Windows x86 EXE (usable on both x86 and x64 Windows):
  dist/PDF-Utils-Portable-Win7-Win11.exe

The recipient does NOT need to install Ghostscript or any runtime.
This script does not prove Windows 7 runtime compatibility or PDF/A conformity.
Validate both on a real Win7 SP1 system and check licensing before release.
EOF
}

if [[ $# -eq 1 && "$1" == "--help" ]]; then usage; exit 0; fi
if [[ $# -ne 0 && !( $# -eq 2 && "$1" == "--installer" && -n "$2" ) ]]; then
  usage >&2; exit 2
fi
for command in python3 clang lld-link nm sha256sum; do
  if ! command -v "$command" >/dev/null 2>&1; then
    echo "Missing build prerequisite: $command" >&2; exit 2
  fi
done
if ! command -v 7z >/dev/null 2>&1 && ! command -v 7zz >/dev/null 2>&1 && ! command -v 7za >/dev/null 2>&1; then
  echo "Missing 7-Zip command (7z, 7zz or 7za) on build machine." >&2
  exit 2
fi

echo '1/5 Preparing verified upstream Ghostscript Win32 runtime...'
python3 scripts/vendor-ghostscript.py "$@"

echo '2/5 Compiling native 32-bit Windows 7 launcher...'
PDFUTILS_PORTABLE=1 bash scripts/build-cross.sh

echo '3/5 Checking PE imports and minimum Windows version...'
python3 scripts/check-win7-pe.py \
  --launcher dist/PDF-Utils-Win7-portable-base.exe \
  --ghostscript vendor/ghostscript-win32

echo '4/5 Running offline project tests...'
python3 -m unittest discover -s tests -v

echo '5/5 Packaging verified runtime as one Windows EXE...'
python3 scripts/package-portable.py --ghostscript vendor/ghostscript-win32
sha256sum dist/PDF-Utils-Portable-Win7-Win11.exe

cat <<'EOF'

Built a SINGLE executable. BEFORE providing it to recipients:
- Test on clean Windows 7 SP1 x86 and Windows 11, without Ghostscript installed.
- Convert actual JPEG and PDF files; verify PDF/A-2b output with veraPDF.
- Check and fulfill Ghostscript AGPL or commercial distribution requirements.
- Keep the source, upstream provenance, build instructions and legal notices accessible.
EOF
