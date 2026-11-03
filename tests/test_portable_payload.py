"""Packaging-format smoke tests; do not test Ghostscript execution or Win7 compatibility."""
from pathlib import Path
import importlib.util
import struct
import tempfile
import unittest

SCRIPT = Path(__file__).resolve().parents[1] / "scripts" / "package-portable.py"
spec = importlib.util.spec_from_file_location("pack", SCRIPT)
pack = importlib.util.module_from_spec(spec)
assert spec.loader
spec.loader.exec_module(pack)


class PortablePayloadTest(unittest.TestCase):
    def test_fail_fast_without_vendor(self):
        base = Path(__file__).resolve().parents[1] / "dist" / "PDF-Utils-Win7-portable-base.exe"
        if not base.exists():
            self.skipTest("portable base not compiled")
        with tempfile.TemporaryDirectory() as tmp:
            with self.assertRaises(ValueError):
                pack.build(base, Path(tmp), Path(tmp) / "portable.exe")

    def test_overlay_has_valid_manifest(self):
        base = Path(__file__).resolve().parents[1] / "dist" / "PDF-Utils-Win7-portable-base.exe"
        if not base.exists():
            self.skipTest("portable base not compiled")
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            vendor = root / "gs"; (vendor / "bin").mkdir(parents=True)
            (vendor / "lib").mkdir()
            (vendor / "Resource" / "Init").mkdir(parents=True)
            # Test-only PE placeholders: never use them as a real GS release.
            fake = base.read_bytes()
            (vendor / "bin" / "gswin32c.exe").write_bytes(fake)
            (vendor / "bin" / "gsdll32.dll").write_bytes(fake)
            (vendor / "lib" / "a.ps").write_text("%!PS\n")
            (vendor / "Resource" / "Init" / "b.ps").write_text("%!PS\n")
            (vendor / "COPYING").write_text("FAKE TEST LICENSE - NOT FOR REDISTRIBUTION")
            out = root / "test.exe"
            pack.build(base, vendor, out)
            data = out.read_bytes()
            magic, start, count = struct.unpack("<8sII", data[-16:])
            self.assertEqual(magic, pack.MAGIC)
            self.assertEqual(start, len(fake))
            self.assertEqual(count, 5)
            pos = start
            names = []
            for _ in range(count):
                n, size = struct.unpack_from("<HI", data, pos); pos += 6
                name = data[pos:pos+n].decode("ascii"); pos += n
                names.append(name)
                self.assertEqual(data[pos:pos+size], (vendor / name).read_bytes())
                pos += size
            self.assertEqual(pos, len(data)-16)
            self.assertIn("bin/gsdll32.dll", names)


if __name__ == "__main__":
    unittest.main()