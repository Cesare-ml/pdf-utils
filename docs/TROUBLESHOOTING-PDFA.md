PDF UTILS - VERSIONE DIAGNOSTICA v3
==================================
Questo è un test per individuare un crash durante la conversione PDF -> PDF/A.
NON è ancora verificato su un'installazione Windows reale.

1. Estrai lo ZIP e apri PDF-Utils-Diagnostica-x64.exe su Windows 11 64-bit.
   (Su Windows 7 a 32 bit utilizza PDF-Utils-Diagnostica-x86.exe.)
2. Scegli una cartella SCRIVIBILE con UN SOLO PDF di prova (conserva una copia dell'originale).
3. Deseleziona JPG -> PDF, lascia PDF -> PDF/A selezionato.
4. Verifica il percorso a gswin64c.exe (o gswin32c.exe) già installato.
5. Premi CONVERTI TUTTI.
6. Se il programma si chiude oppure non produce il PDF/A, nella cartella scelta
   controlla e condividi questi file (se presenti):

   PDF-Utils-diagnostica.txt
   nomefile_PDFA.pdf.ghostscript.log

Il primo file è generato dall'app e registra la fase in cui è terminata.
Se si verifica un'eccezione Windows non gestita, dovrebbe contenere una riga:
  UNHANDLED_EXCEPTION=0xXXXXXXXX STAGE=N
Il secondo è l'output di errore di Ghostscript.

NOTA: Se il programma termina per arresto esterno o prima di poter scrivere,
il rapporto può essere assente o incompleto. In quel caso controlla Visualizzatore
Eventi -> Registri di Windows -> Applicazione -> Errore applicazione.
Annota codice eccezione e modulo che ha causato l'errore.

QUESTA BUILD RICHIEDE ANCORA GHOSTSCRIPT INSTALLATO SUL PC.
La futura versione single-EXE senza installazioni resta un lavoro distinto.