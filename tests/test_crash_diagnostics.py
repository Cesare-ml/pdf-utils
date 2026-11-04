"""Static smoke regression tests for Windows 7+ diagnostic build."""
from pathlib import Path
import unittest

SOURCE = (Path(__file__).resolve().parents[1] / 'src/pdf_utils.c').read_text('utf-8')

class TestCrashDiagnostics(unittest.TestCase):
    def test_crash_handler_installed(self):
        self.assertIn('SetUnhandledExceptionFilter((void*)diag_unhandled)', SOURCE)
        self.assertIn('UNHANDLED_EXCEPTION=0x%08X STAGE=%u', SOURCE)

    def test_step_log_created_before_thread(self):
        self.assertLess(SOURCE.index('diag_step(10,"PDF Utils stack probe fix v4 - start")'),
                        SOURCE.index('thread=CreateThread('))
        self.assertIn('PDF-Utils-diagnostica.txt', SOURCE)

    def test_does_not_remove_diagnostic_file_on_cleanup(self):
        self.assertNotIn('DeleteFileW(diagnostic_file)', SOURCE)

    def test_windows_icc_api_buffer_is_wchar_count(self):
        self.assertIn('DWORD len=MAX_PATH_CHARS;', SOURCE)
        self.assertNotIn('DWORD len=MAX_PATH_CHARS*2;', SOURCE)

    def test_external_ghostscript_has_stderr_log(self):
        self.assertIn('si.hStdError=log', SOURCE)
        self.assertIn('si.hStdOutput=log', SOURCE)
        self.assertIn('.ghostscript.log', SOURCE)

if __name__ == '__main__':
    unittest.main()