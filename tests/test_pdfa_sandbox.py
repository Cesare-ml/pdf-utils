"""PDF/A SAFER integration regression, runnable without Windows (optional local gs)."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
SOURCE = (ROOT / 'src/pdf_utils.c').read_text(encoding='utf-8')


class TestPDFASandbox(unittest.TestCase):
    def test_pdfa_uses_narrow_icc_permission_and_diagnostics(self):
        self.assertIn(' --permit-file-read=', SOURCE)
        self.assertIn('STARTF_USESTDHANDLES', SOURCE)
        self.assertIn('.ghostscript.log', SOURCE)
        self.assertIn('si.hStdError=log', SOURCE)
        self.assertIn('si.hStdOutput=log', SOURCE)
        self.assertNotIn(' -dNOSAFER', SOURCE)

    def test_real_ghostscript_sandbox_conversion(self):
        gs = shutil.which('gs')
        icc = Path('/usr/share/color/icc/ghostscript/srgb.icc')
        if not gs or not icc.exists():
            self.skipTest('Linux Ghostscript and sRGB ICC profile not available')
        with tempfile.TemporaryDirectory() as scratch:
            root = Path(scratch)
            psin = root / 'in.ps'
            pdfin = root / 'input.pdf'
            pdflog = root / 'stderr.log'
            psdef = root / 'PDFA_def.ps'
            pdfout = root / 'output_PDFA.pdf'
            psin.write_bytes(b'%!PS\n/Helvetica findfont 20 scalefont setfont 72 720 moveto (PDF Utils test) show showpage\n')
            subprocess.run([gs, '-q', '-dBATCH', '-dNOPAUSE', '-sDEVICE=pdfwrite',
                            '-sOutputFile=' + str(pdfin), str(psin)],
                           check=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=30)
            psdef.write_text(
                '%!PS\n/ICCProfile (' + icc.as_posix() + ') def\n'
                '[/_objdef {icc_PDFA} /type /stream /OBJ pdfmark\n'
                '[{icc_PDFA} <</N 3>> /PUT pdfmark\n'
                '[{icc_PDFA} ICCProfile (r) file /PUT pdfmark\n'
                '[/_objdef {OutputIntent_PDFA} /type /dict /OBJ pdfmark\n'
                '[{OutputIntent_PDFA} <<\n /Type /OutputIntent\n /S /GTS_PDFA1\n'
                ' /DestOutputProfile {icc_PDFA}\n'
                ' /OutputConditionIdentifier (sRGB IEC61966-2.1)\n >> /PUT pdfmark\n'
                '[{Catalog} <</OutputIntents [ {OutputIntent_PDFA} ]>> /PUT pdfmark\n',
                encoding='ascii'
            )
            args = [gs, '-dSAFER', '--permit-file-read=' + str(icc), '-dBATCH',
                    '-dNOPAUSE', '-dPDFA=2', '-dPDFACompatibilityPolicy=1',
                    '-sDEVICE=pdfwrite', '-sColorConversionStrategy=RGB',
                    '-sBlendConversionStrategy=Simple', '-sOutputFile=' + str(pdfout),
                    str(psdef), str(pdfin)]
            with pdflog.open('wb') as log:
                rc = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT, timeout=30)
            self.assertEqual(rc.returncode, 0, pdflog.read_text(errors='replace'))
            pdfbytes = pdfout.read_bytes()
            self.assertTrue(pdfbytes.startswith(b'%PDF-'), pdfbytes[:30])
            self.assertIn(b'pdfaid:part', pdfbytes)
            self.assertIn(b'pdfaid:conformance', pdfbytes)

if __name__ == '__main__':
    unittest.main()