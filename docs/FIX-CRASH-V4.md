# PDF Utils — correzione crash Windows v4

## Evidenza ricevuta dall'utente

Il file `PDF-Utils-diagnostica.txt` contiene soltanto due righe
`PDF Utils crash diagnostics v3 - start`, senza le righe del thread
`Worker started` né i log di Ghostscript. Questo restringe il problema
alla fase di avvio del thread, senza però dimostrare da solo la causa.

## Correzione concreta

Le versioni precedenti venivano compilate con
`-mno-stack-arg-probe`. Su Windows questa opzione sopprime le sonde
che attraversano le pagine guardia dello stack. Il `run_conversion`
utilizza un frame di oltre **55 KiB**, e pertanto un nuovo thread può
andare incontro a un errore di accesso prima di scrivere il primo log.
Anche `window_proc` e `check_gs_dir` creavano frame >4 KiB.

La v4 ripristina le sonde automatiche e implementa localmente
`__chkstk` per x86 e x64 con ABI Microsoft, senza richiedere il runtime
Visual C++ o altre installazioni. È presente un oggetto COFF compilato
da C anche per la versione x86, così resta attivo il controllo SafeSEH.

Inoltre sono stati aggiunti checkpoint 11–15 sul thread grafico e
100–105 all'ingresso del worker, per distinguere problemi UI da errori
successivi di Ghostscript.

## Test eseguiti

- Compilazione PE Win32 x86 e x64, versione minima subsystem Windows 6.1.
- Disassemblaggio del simbolo `__chkstk` in entrambe le architetture.
- Verifica che il codice oggetto invochi davvero `__chkstk`.
- 12 test automatici (incluso test reale della procedura Ghostscript su
  Linux): tutti superati.

**Limitazione:** non sono stati ancora eseguiti test con la GUI su
Windows 11 o su Windows 7 SP1. Non è quindi possibile dichiarare il
crash risolto al 100% senza la prova sul PC destinatario.

## Come provare

1. Avviare `PDF-Utils-StackFix-v4-x64.exe` su Windows 11 x64.
2. Selezionare una cartella scrivibile con un PDF di prova.
3. Deselezionare JPG → PDF e lasciare PDF → PDF/A-2b.
4. Verificare il percorso `gswin64c.exe` e premere **CONVERTI TUTTI**.
5. Se la conversione fallisce, inviare `PDF-Utils-diagnostica.txt` e
   `*_PDFA.pdf.ghostscript.log`, quando presenti.

Questa variante richiede ancora **Ghostscript installato**. La versione
monofile con Ghostscript integrato è sviluppata separatamente nella
PR #1. Nessuna GitHub Action viene utilizzata. Il PDF/A deve essere
validato con veraPDF prima di uso archivistico ufficiale.
