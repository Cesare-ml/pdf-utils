# PDF Utils — JPG → PDF e PDF → PDF/A (Windows 7 / Windows 11)

Convertitore in blocco con **interfaccia grafica Win32 nativa**. Non richiede Python, Java, .NET, Visual C++ Redistributable o installazioni per creare PDF da JPG. Il repository contiene il sorgente C, lo script di build e due eseguibili compilati per architetture differenti.

## Avvio

1. Su Windows **a 64 bit** (Windows 11 o Windows 7 64 bit), aprire `PDF-Utils-x64.exe`. Su **Windows 7 a 32 bit**, usare `PDF-Utils-x86.exe`.
2. Premere **Sfoglia** accanto alla cartella, scegliere la cartella che contiene le immagini JPG/JPEG e/o i PDF.
3. Spuntare **JPG/JPEG → PDF**, **PDF → PDF/A-2b** oppure entrambe.
4. Selezionare **CONVERTI TUTTI**. Gli esiti per ogni file sono visualizzati nella parte inferiore.

L'app legge i file **solo nella cartella selezionata, non nelle sottocartelle**. L'interfaccia rimane reattiva durante la conversione. I file originali vengono mantenuti. I file di destinazione riconosciuti come prodotti da PDF Utils vengono aggiornati nelle esecuzioni successive.

| File nella cartella | File prodotto |
| --- | --- |
| `foto.jpg` | `foto.pdf` (A4, adatta alla foto senza ritagli) |
| `scansione.jpeg` | `scansione.pdf` |
| `documento.pdf` | `documento_PDFA.pdf` (PDF/A-2b) |

Se `foto.pdf` esiste già ma **non** è stato creato da PDF Utils, il programma lo preserva e usa `foto_JPG.pdf`. Se anche questo nome è occupato da un PDF estraneo, salta la foto invece di cancellare il documento preesistente. I PDF/A già denominati `*_PDFA.pdf` sono esclusi dal trattamento per evitare riconversioni. I PDF generati dai JPG non vengono aggiunti alla lista PDF/A durante la medesima esecuzione, se non erano presenti quando si è selezionato **Converti tutti**.

## Ghostscript per convertire PDF → PDF/A

La generazione PDF/A richiede **Ghostscript**, un programma esterno per elaborare PDF. Questo pacchetto **non** lo distribuisce, per evitare la redistribuzione involontaria di software soggetto a licenza AGPL e per permettere aggiornamenti di sicurezza separati.

- Scaricare Ghostscript da **https://www.ghostscript.com/releases/** e installare una versione adatta al proprio Windows.
- Il programma cerca automaticamente `gswin64c.exe` o `gswin32c.exe` nel PATH, accanto all'eseguibile, in `ghostscript\bin` o nelle installazioni comuni `Program Files\gs\gs*\bin`.
- Se il campo rimane vuoto o mostra *Ghostscript non trovato*, premere il secondo **Sfoglia** e indicare il percorso a `gswin64c.exe` / `gswin32c.exe`.
- La conversione usa il profilo di colore **sRGB ICC già presente in Windows**, l'OutputIntent e il conformità **PDF/A-2b** (`-dPDFA=2`, profilo RGB). L'output mantiene i PDF di input e scrive il risultato con suffisso `_PDFA.pdf`.

**Avvertenza:** la presenza di metadati PDF/A non certifica da sola conformità completa a ISO 19005. Per archivi regolamentati, verificare i PDF/A prodotti con **veraPDF**: https://verapdf.org/. I contenuti PDF con font, trasparenze, firme digitali, moduli o funzionalità non rappresentabili in PDF/A potrebbero essere modificati o non risultare conformi. La conversione non è una firma digitale e può invalidare eventuali firme presenti.

La famiglia Ghostscript è disponibile con licenza AGPL o commerciale; chi la installa/distribuisce deve rispettarne la licenza e considerare l'aggiornamento di sicurezza. La compatibilità specifica con Windows 7 dipende **anche dalla versione di Ghostscript scelta**: i due EXE PDF Utils sono compilati per Windows 7, ma non possono garantire che qualunque Ghostscript moderno si avvii su Windows 7.

## Requisiti e limiti

- `PDF-Utils-x86.exe`: Windows 7 SP1 **32 o 64 bit**; `PDF-Utils-x64.exe`: Windows 7 SP1 **64 bit** o Windows 11 **64 bit**.
- Esclusivamente `.jpg`, `.jpeg`, `.pdf` (maiuscole e minuscole indistinte).
- JPG indipendenti: **1 JPG → 1 PDF**, non un singolo PDF cumulativo. I JPEG originali sono incorporati con filtro DCT senza ricompressione; pagina A4 con margine da 10 punti, orientamento verticale/orizzontale automatico.
- JPEG fino a 250 MB; JPEG in scala di grigi, RGB e CMYK. L'orientamento EXIF non viene attualmente interpretato, quindi per certe foto da telefono potrebbe servire ruotare il JPG prima della conversione.
- Non imposta OCR, non rende ricercabili scansioni raster, non gestisce cartelle annidate, non converte PNG/TIFF e non sostituisce la validazione veraPDF.
- Il file PDF di destinazione viene creato temporaneamente con estensione `.pdfutils.tmp` e soltanto dopo una conversione riuscita sostituisce quello preesistente. In caso di errore, il PDF di destinazione preesistente resta intatto.
- I nomi di file contenenti caratteri internazionali sono passati alle API Unicode di Windows; con versioni obsolete di Ghostscript il supporto Unicode sulla riga di comando può essere limitato.
- Il codice C è distribuito con licenza MIT; Ghostscript non è incluso.

## Build riproducibile da Linux

Occorrono `clang`, `lld-link`, `nm`, `python3` (solo strumenti di sviluppo). Non è necessario Windows per compilare. Il file `scripts/build-cross.sh` crea in `dist/` i due PE Windows senza CRT, con subsystem `6.1` (Windows 7):

```sh
./scripts/build-cross.sh
```

Le DLL importate sono solo componenti Win32 di sistema: `kernel32`, `user32`, `gdi32`, `comdlg32`, `shell32`, `ole32`, `mscms`.

Per verificare l'engine JPG su Linux con librerie PDF disponibili:

```sh
clang -O2 -fshort-wchar -ffunction-sections -fdata-sections \
    -Wl,--gc-sections -Wno-ignored-attributes \
    tests/test_jpeg_native.c -o tests/test_jpeg_native
./tests/test_jpeg_native tests/example.jpg tests/from_jpg.pdf
pdfinfo tests/from_jpg.pdf
```

Per PDF/A si può testare il comando Ghostscript e il profilo ICC locale, verificando che i metadati XMP contengano `pdfaid:part='2'` e `pdfaid:conformance='B'`. Per la conformità normativa usare sempre veraPDF.

## Test

L'engine JPG è stato testato mediante harness nativo su Linux, con PDF risultante leggibile da Poppler e Ghostscript. Il setup Ghostscript PDF/A-2b è stato testato con Ghostscript 10.05.1 su Linux, producendo XMP PDF/A-2b. I due eseguibili Windows sono compilati come PE con subsystem 6.01 e librerie di sistema disponibili su Windows 7. **Non è stato possibile eseguirli su un'installazione reale Windows 7/11 in questa sessione:** serve un test finale sui due sistemi operativi prima di una distribuzione in produzione.