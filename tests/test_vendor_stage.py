"""Offline vendor detection/staging tests. No binaries from the internet required."""
import importlib.util
import pathlib
import tempfile
import unittest

SCRIPT=pathlib.Path(__file__).resolve().parents[1]/'scripts'/'vendor-ghostscript.py'
spec=importlib.util.spec_from_file_location('vendor_gs',SCRIPT)
vendor=importlib.util.module_from_spec(spec)
spec.loader.exec_module(vendor)

class TestVendor(unittest.TestCase):
    def test_stage_without_real_runtime_fails(self):
        with tempfile.TemporaryDirectory() as t:
            root=pathlib.Path(t)
            with self.assertRaisesRegex(ValueError, 'exactly one'):
                vendor.stage(root,root/'dest')

    def test_extract_detection_and_legal_notice(self):
        with tempfile.TemporaryDirectory() as t:
            root=pathlib.Path(t)
            gs=root/'unpacked'/'gs10.08.0'
            for p in ('bin','Resource/Init','lib','doc'):
                (gs/p).mkdir(parents=True)
            (gs/'bin'/'gswin32c.exe').write_bytes(b'MZ')
            (gs/'bin'/'gsdll32.dll').write_bytes(b'MZ')
            (gs/'lib'/'test.ps').write_text('%!PS\n')
            (gs/'Resource'/'Init'/'gs_init.ps').write_text('%!PS\n')
            (gs/'doc'/'COPYING').write_text('FAKE LICENSE TEXT - TEST ONLY')
            self.assertEqual(vendor.find_gs_root(root/'unpacked'),gs)
            result=vendor.stage(root/'unpacked',root/'vendor')
            self.assertEqual(result['files'],6)
            self.assertTrue((root/'vendor'/'bin'/'gswin32c.exe').exists())
            self.assertTrue((root/'vendor'/'THIRD-PARTY-NOTICE.txt').exists())

if __name__=='__main__':unittest.main()