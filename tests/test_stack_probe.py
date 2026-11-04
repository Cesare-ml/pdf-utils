"""Check that stack probing is enabled and compiled into both Windows targets.

This is a static regression guard. It cannot replace real Windows execution.
"""
from pathlib import Path
import subprocess
import unittest

ROOT = Path(__file__).resolve().parents[1]
BUILD = (ROOT / 'scripts' / 'build-cross.sh').read_text('utf-8')
PROBE = (ROOT / 'src' / 'stack_probe.c').read_text('utf-8')
SRC = (ROOT / 'src' / 'pdf_utils.c').read_text('utf-8')

class StackProbeRegression(unittest.TestCase):
    def test_probes_not_disabled_by_build(self):
        self.assertNotIn('-mno-stack-arg-probe', BUILD)
        self.assertIn('src/stack_probe.c', BUILD)
        self.assertIn('stack_probe.obj', BUILD)

    def test_both_abi_variants(self):
        self.assertIn('"__chkstk"', PROBE)
        self.assertIn('pushq %r10', PROBE)
        self.assertIn('movl %ecx, %esp', PROBE)
        self.assertIn('testb $0', PROBE)

    def test_worker_stage_before_other_windows_calls(self):
        self.assertLess(SRC.index('diag_step(100,"Worker entered successfully after stack probe")'),
                        SRC.index('gs_ready=file_exists(job->gs);'))
        self.assertIn('diag_step(14,"Main thread: about to create worker thread")', SRC)

    def test_object_contains_probe_symbol(self):
        for arch in ('x64','x86'):
            obj = ROOT / 'build' / arch / 'stack_probe.obj'
            if not obj.is_file():
                self.skipTest('Build artifacts not present')
            output = subprocess.check_output(['nm', str(obj)], text=True)
            self.assertIn('__chkstk', output)

    def test_compiler_inserts_probe_calls(self):
        for arch in ('x64','x86'):
            obj = ROOT / 'build' / arch / 'pdf_utils.obj'
            if not obj.is_file():
                self.skipTest('Build artifacts not present')
            output = subprocess.check_output(['nm', '-u', str(obj)], text=True)
            self.assertIn('__chkstk', output)

if __name__ == '__main__':
    unittest.main()
