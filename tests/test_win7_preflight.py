"""Test PE32 import preflight against our real built Win7 launcher."""
import importlib.util
import pathlib
import tempfile
import unittest

ROOT=pathlib.Path(__file__).resolve().parents[1]
SCRIPT=ROOT/'scripts'/'check-win7-pe.py'
spec=importlib.util.spec_from_file_location('win7pe',SCRIPT)
pe=importlib.util.module_from_spec(spec)
spec.loader.exec_module(pe)

class TestWin7PE(unittest.TestCase):
    def test_real_launcher_imports_are_only_win7_core(self):
        launcher=ROOT/'dist'/'PDF-Utils-Win7-portable-base.exe'
        if not launcher.exists():self.skipTest('launcher not yet compiled')
        info=pe.parse_pe(launcher)
        self.assertEqual(info['cpu'],0x14c)
        self.assertLessEqual(info['subsystem'],(6,1))
        self.assertLessEqual(info['min_os'],(6,1))
        with tempfile.TemporaryDirectory() as t:
            self.assertEqual(pe.verify(pathlib.Path(t),launcher),[])

    def test_reject_non_pe_runtime(self):
        with tempfile.TemporaryDirectory() as t:
            root=pathlib.Path(t)
            (root/'broken.dll').write_bytes(b'fake PE')
            launcher=ROOT/'dist'/'PDF-Utils-Win7-portable-base.exe'
            if not launcher.exists():self.skipTest('launcher not yet compiled')
            self.assertTrue(any('invalid PE' in e for e in pe.verify(root,launcher)))

if __name__=='__main__':unittest.main()