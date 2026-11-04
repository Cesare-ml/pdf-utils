/* PDF Utils - native Win32, no CRT or external runtime.
 * Target Windows 7 SP1 and Windows 11, x86/x64.
 * Copyright (c) 2026 contributors. MIT License.
 */

typedef unsigned short WCHAR;
typedef char CHAR;
typedef unsigned int UINT, DWORD;
typedef int BOOL, LONG;
typedef unsigned short WORD;
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
typedef __SIZE_TYPE__ SIZE_T;
typedef void *HANDLE, *HWND, *HINSTANCE, *HBRUSH, *HFONT, *HICON, *HCURSOR, *HMENU, *LPVOID, *HGDIOBJ;
typedef const WCHAR *LPCWSTR;
typedef WCHAR *LPWSTR;
typedef const CHAR *LPCSTR;
typedef void *LPSECURITY_ATTRIBUTES;
typedef struct {DWORD nLength;LPVOID lpSecurityDescriptor;BOOL bInheritHandle;} SECURITY_ATTRIBUTES;
typedef __INTPTR_TYPE__ LRESULT;
typedef __UINTPTR_TYPE__ WPARAM;
typedef __INTPTR_TYPE__ LPARAM;
typedef LRESULT (__stdcall *WNDPROC)(HWND,UINT,WPARAM,LPARAM);
#if defined(PDFUTILS_TEST)
#define WINAPI
#define __cdecl
#else
#define WINAPI __stdcall
#endif
#define NULL ((void*)0)
#define TRUE 1
#define FALSE 0
#define INVALID_HANDLE_VALUE ((HANDLE)(LONGLONG)-1)
#define MAX_PATH_CHARS 2048
#define MAX_LOG 4096
#define HEAP_ZERO_MEMORY 8
#define GENERIC_READ 0x80000000u
#define GENERIC_WRITE 0x40000000u
#define FILE_SHARE_READ 1u
#define STARTF_USESHOWWINDOW 0x00000001u
#define STARTF_USESTDHANDLES 0x00000100u
#define OPEN_EXISTING 3u
#define CREATE_ALWAYS 2u
#define OPEN_ALWAYS 4u
#define FILE_END 2u
#define FILE_ATTRIBUTE_NORMAL 0x80u
#define FILE_ATTRIBUTE_DIRECTORY 0x10u
#define INVALID_FILE_ATTRIBUTES 0xffffffffu
#define MOVEFILE_REPLACE_EXISTING 1u
#define CREATE_NO_WINDOW 0x08000000u
#define WAIT_OBJECT_0 0u
#define INFINITE 0xffffffffu
#define WS_OVERLAPPEDWINDOW 0x00cf0000u
#define WS_VISIBLE 0x10000000u
#define WS_CHILD 0x40000000u
#define WS_BORDER 0x00800000u
#define WS_TABSTOP 0x00010000u
#define WS_VSCROLL 0x00200000u
#define WS_HSCROLL 0x00100000u
#define ES_AUTOHSCROLL 0x0080u
#define ES_MULTILINE 0x0004u
#define ES_READONLY 0x0800u
#define ES_AUTOVSCROLL 0x0040u
#define BS_AUTOCHECKBOX 0x0003u
#define BST_CHECKED 1u
#define BM_GETCHECK 0x00f0u
#define BM_SETCHECK 0x00f1u
#define WM_CREATE 1u
#define WM_DESTROY 2u
#define WM_COMMAND 0x0111u
#define WM_SETFONT 0x0030u
#define WM_APP 0x8000u
#define WM_CLOSE 0x0010u
#define EM_SETSEL 0x00b1u
#define EM_REPLACESEL 0x00c2u
#define SW_SHOW 5
#define SW_HIDE 0
#define CW_USEDEFAULT ((int)0x80000000u)
#define IDC_ARROW ((LPCWSTR)32512)
#define COLOR_WINDOW 5
#define DEFAULT_CHARSET 1
#define FW_NORMAL 400
#define FW_BOLD 700
#define OUT_DEFAULT_PRECIS 0
#define CLIP_DEFAULT_PRECIS 0
#define CLEARTYPE_QUALITY 5
#define DEFAULT_PITCH 0
#define FF_SWISS 32
#define BIF_RETURNONLYFSDIRS 0x0001u
#define BIF_USENEWUI 0x0040u
#define OFN_FILEMUSTEXIST 0x1000u
#define OFN_PATHMUSTEXIST 0x0800u
#define OFN_EXPLORER 0x80000u
#define FILE_BEGIN 0u
#define FILE_CURRENT 1u
#define IDC_FOLDER 1001
#define IDC_BROWSE 1002
#define IDC_JPG 1003
#define IDC_PDFA 1004
#define IDC_GS 1005
#define IDC_GS_BROWSE 1006
#define IDC_START 1007
#define IDC_STATUS 1008
#define IDC_LOG 1009
#define WM_LOG (WM_APP+1)
#define WM_DONE (WM_APP+2)

#pragma pack(push, 8)
typedef struct {LONG left, top, right, bottom;} RECT;
typedef struct {DWORD dwLowDateTime, dwHighDateTime;} FILETIME;
typedef struct {DWORD dwFileAttributes; FILETIME ftCreationTime,ftLastAccessTime,ftLastWriteTime; DWORD nFileSizeHigh,nFileSizeLow,dwReserved0,dwReserved1; WCHAR cFileName[260]; WCHAR cAlternateFileName[14]; DWORD dwFileType,dwCreatorType; WORD wFinderFlags;} WIN32_FIND_DATAW;
typedef struct {UINT cbSize; UINT style; WNDPROC lpfnWndProc; int cbClsExtra,cbWndExtra; HINSTANCE hInstance; HICON hIcon; HCURSOR hCursor; HBRUSH hbrBackground; LPCWSTR lpszMenuName,lpszClassName; HICON hIconSm;} WNDCLASSEXW;
typedef struct {HWND hwndOwner; const void *pidlRoot; LPWSTR pszDisplayName; LPCWSTR lpszTitle; UINT ulFlags; void *lpfn; LPARAM lParam; int iImage;} BROWSEINFOW;
typedef struct {DWORD cb; LPCWSTR lStructSize_unused; /* not used */} UNUSED;
typedef struct {DWORD lStructSize; HWND hwndOwner; HINSTANCE hInstance; LPCWSTR lpstrFilter; LPWSTR lpstrCustomFilter; DWORD nMaxCustFilter; DWORD nFilterIndex; LPWSTR lpstrFile; DWORD nMaxFile; LPWSTR lpstrFileTitle; DWORD nMaxFileTitle; LPCWSTR lpstrInitialDir; LPCWSTR lpstrTitle; DWORD Flags; WORD nFileOffset,nFileExtension; LPCWSTR lpstrDefExt; LPARAM lCustData; void *lpfnHook; LPCWSTR lpTemplateName; void *pvReserved; DWORD dwReserved,FlagsEx;} OPENFILENAMEW;
typedef struct {WORD wYear,wMonth,wDayOfWeek,wDay,wHour,wMinute,wSecond,wMilliseconds;} SYSTEMTIME;
typedef struct {DWORD cb; LPWSTR lpReserved,lpDesktop,lpTitle; DWORD dwX,dwY,dwXSize,dwYSize,dwXCountChars,dwYCountChars,dwFillAttribute,dwFlags; WORD wShowWindow,cbReserved2; unsigned char *lpReserved2; HANDLE hStdInput,hStdOutput,hStdError;} STARTUPINFOW;
typedef struct {HANDLE hProcess,hThread; DWORD dwProcessId,dwThreadId;} PROCESS_INFORMATION;
typedef struct {HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam; DWORD time; LONG pt_x, pt_y; DWORD lPrivate;} MSG;
#pragma pack(pop)

#if defined(PDFUTILS_TEST)
#define DLL extern
#else
#define DLL extern __declspec(dllimport)
#endif
DLL HANDLE WINAPI GetProcessHeap(void);
DLL LPVOID WINAPI HeapAlloc(HANDLE,DWORD,SIZE_T);
DLL BOOL WINAPI HeapFree(HANDLE,DWORD,LPVOID);
DLL HANDLE WINAPI CreateFileW(LPCWSTR,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE);
DLL BOOL WINAPI ReadFile(HANDLE,LPVOID,DWORD,DWORD*,void*);
DLL BOOL WINAPI WriteFile(HANDLE,const void*,DWORD,DWORD*,void*);
DLL BOOL WINAPI CloseHandle(HANDLE);
DLL DWORD WINAPI GetFileSize(HANDLE,DWORD*);
DLL DWORD WINAPI SetFilePointer(HANDLE,LONG,LONG*,DWORD);
DLL DWORD WINAPI GetFileAttributesW(LPCWSTR);
DLL BOOL WINAPI DeleteFileW(LPCWSTR);
DLL BOOL WINAPI MoveFileExW(LPCWSTR,LPCWSTR,DWORD);
DLL HANDLE WINAPI FindFirstFileW(LPCWSTR,WIN32_FIND_DATAW*);
DLL BOOL WINAPI FindNextFileW(HANDLE,WIN32_FIND_DATAW*);
DLL BOOL WINAPI FindClose(HANDLE);
DLL DWORD WINAPI GetModuleFileNameW(HINSTANCE,LPWSTR,DWORD);
DLL UINT WINAPI GetSystemDirectoryW(LPWSTR,UINT);
DLL BOOL WINAPI GetColorDirectoryW(LPCWSTR,LPWSTR,DWORD*);
DLL DWORD WINAPI GetEnvironmentVariableW(LPCWSTR,LPWSTR,DWORD);
DLL DWORD WINAPI SearchPathW(LPCWSTR,LPCWSTR,LPCWSTR,DWORD,LPWSTR,LPWSTR*);
DLL BOOL WINAPI CreateProcessW(LPCWSTR,LPWSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,BOOL,DWORD,void*,LPCWSTR,STARTUPINFOW*,PROCESS_INFORMATION*);
DLL DWORD WINAPI WaitForSingleObject(HANDLE,DWORD);
DLL BOOL WINAPI GetExitCodeProcess(HANDLE,DWORD*);
DLL BOOL WINAPI TerminateProcess(HANDLE,UINT);
DLL HANDLE WINAPI CreateThread(LPSECURITY_ATTRIBUTES,SIZE_T,DWORD (WINAPI *)(LPVOID),LPVOID,DWORD,DWORD*);
DLL DWORD WINAPI GetLastError(void);
DLL void *WINAPI SetUnhandledExceptionFilter(void *);
DLL HINSTANCE WINAPI GetModuleHandleW(LPCWSTR);
DLL void WINAPI ExitProcess(UINT);
DLL unsigned short WINAPI RegisterClassExW(const WNDCLASSEXW*);
DLL HWND WINAPI CreateWindowExW(DWORD,LPCWSTR,LPCWSTR,DWORD,int,int,int,int,HWND,HMENU,HINSTANCE,LPVOID);
DLL LRESULT WINAPI DefWindowProcW(HWND,UINT,WPARAM,LPARAM);
DLL BOOL WINAPI ShowWindow(HWND,int);
DLL BOOL WINAPI UpdateWindow(HWND);
DLL BOOL WINAPI GetMessageW(MSG*,HWND,UINT,UINT);
DLL BOOL WINAPI TranslateMessage(const MSG*);
DLL LRESULT WINAPI DispatchMessageW(const MSG*);
DLL void WINAPI PostQuitMessage(int);
DLL HCURSOR WINAPI LoadCursorW(HINSTANCE,LPCWSTR);
DLL LRESULT WINAPI SendMessageW(HWND,UINT,WPARAM,LPARAM);
DLL BOOL WINAPI PostMessageW(HWND,UINT,WPARAM,LPARAM);
DLL BOOL WINAPI SetWindowTextW(HWND,LPCWSTR);
DLL int WINAPI GetWindowTextW(HWND,LPWSTR,int);
DLL int WINAPI MessageBoxW(HWND,LPCWSTR,LPCWSTR,UINT);
DLL BOOL WINAPI EnableWindow(HWND,BOOL);
DLL int __cdecl wsprintfA(CHAR*,LPCSTR,...);
DLL int __cdecl wsprintfW(WCHAR*,LPCWSTR,...);
DLL HFONT WINAPI CreateFontW(int,int,int,int,int,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,LPCWSTR);
DLL BOOL WINAPI DeleteObject(HGDIOBJ);
DLL void *WINAPI SHBrowseForFolderW(BROWSEINFOW*);
DLL BOOL WINAPI SHGetPathFromIDListW(const void*,LPWSTR);
DLL void WINAPI CoTaskMemFree(LPVOID);
DLL BOOL WINAPI GetOpenFileNameW(OPENFILENAMEW*);

static HINSTANCE appInstance;
static HWND mainWindow, folderControl, gsControl, jpgCheck, pdfaCheck, startButton, logControl, statusControl;
static HFONT uiFont, headingFont;
static int busy = 0;
static const WCHAR APP_NAME[] = {'P','D','F',' ','U','t','i','l','s',0};

static UINT wl(const WCHAR *s) {UINT n=0; if(s) while(s[n]) ++n; return n;}
static UINT al(const CHAR *s) {UINT n=0; if(s) while(s[n]) ++n; return n;}
static WCHAR up(WCHAR x){return x>= 'a' && x<='z' ? (WCHAR)(x-32) : x;}
static BOOL eqw(const WCHAR *a,const WCHAR *b) {UINT i=0; while(a[i] && b[i] && up(a[i])==up(b[i])) ++i; return a[i]==0 && b[i]==0;}
static BOOL endw(const WCHAR *s,const WCHAR *end) {UINT n=wl(s),m=wl(end),i; if(m>n) return FALSE; for(i=0;i<m;i++) if(up(s[n-m+i])!=up(end[i])) return FALSE; return TRUE;}
static BOOL catw(WCHAR *dst,UINT cap,const WCHAR *src){UINT i=wl(dst),j=0; if(i>=cap) return FALSE; while(src[j]) {if(i+1>=cap) return FALSE; dst[i++]=src[j++];} dst[i]=0; return TRUE;}
static void cpyw(WCHAR *dst,UINT cap,const WCHAR *src){UINT i=0; if(!cap)return; while(src[i] && i+1<cap) {dst[i]=src[i];i++;}dst[i]=0;}
static void cpya(CHAR *dst,UINT cap,const CHAR *src){UINT i=0;if(!cap)return;while(src[i]&&i+1<cap){dst[i]=src[i];i++;}dst[i]=0;}
static void zmem(void *dst,UINT n){unsigned char *p=(unsigned char*)dst; UINT i;for(i=0;i<n;i++)p[i]=0;}
/* The diagnostic is a plain ASCII text file beside input documents. This is
   deliberately independent from Ghostscript, Windows Event Viewer and CRT. */
static WCHAR diagnostic_file[MAX_PATH_CHARS];
static volatile LONG diagnostic_stage = 0;
typedef struct {DWORD ExceptionCode,ExceptionFlags;void *ExceptionRecord,*ExceptionAddress;} DIAG_EXCEPTION_RECORD;
typedef struct {DIAG_EXCEPTION_RECORD *ExceptionRecord;void *ContextRecord;} DIAG_EXCEPTION_POINTERS;
static void diag_write(const CHAR *message){
  HANDLE f;DWORD size,written;
  if(!diagnostic_file[0]||!message)return;
  f=CreateFileW(diagnostic_file,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
  if(f==INVALID_HANDLE_VALUE)return;
  size=GetFileSize(f,NULL);
  if(size!=0xffffffffu&&size<262144u){
    SetFilePointer(f,0,NULL,FILE_END);
    WriteFile(f,message,(DWORD)al(message),&written,NULL);
    WriteFile(f,"\r\n",2,&written,NULL);
  }
  CloseHandle(f);
}
static void diag_step(LONG stage,const CHAR *message){diagnostic_stage=stage;diag_write(message);}
static LONG WINAPI diag_unhandled(DIAG_EXCEPTION_POINTERS *p){
  CHAR buf[180];DWORD code=0;
  if(p && p->ExceptionRecord)code=p->ExceptionRecord->ExceptionCode;
  wsprintfA(buf,"UNHANDLED_EXCEPTION=0x%08X STAGE=%u",code,(DWORD)diagnostic_stage);
  diag_write(buf);
  return 1; /* EXCEPTION_EXECUTE_HANDLER, fail closed; diagnostic survives */
}

static BOOL file_exists(const WCHAR *name){DWORD a=GetFileAttributesW(name);return a!=INVALID_FILE_ATTRIBUTES && !(a&FILE_ATTRIBUTE_DIRECTORY);}
static void append_slash(WCHAR *s,UINT cap){UINT n=wl(s);if(n && s[n-1]!='\\' && s[n-1]!='/')catw(s,cap,L"\\");}
static void quote_w(WCHAR *s,UINT cap,const WCHAR *part){catw(s,cap,L"\"");catw(s,cap,part);catw(s,cap,L"\"");}
static void errw(WCHAR *dst,UINT cap,DWORD err){WCHAR num[40];wsprintfW(num,L"Errore Windows %u",err);cpyw(dst,cap,num);}
static void ui_log(const WCHAR *text){UINT n=wl(text);WCHAR *copy=(WCHAR*)HeapAlloc(GetProcessHeap(),0,(n+1)*2);UINT i;if(!copy)return;for(i=0;i<=n;i++)copy[i]=text[i];if(!PostMessageW(mainWindow,WM_LOG,0,(LPARAM)copy))HeapFree(GetProcessHeap(),0,copy);}
static void ui_log_file(const WCHAR *prefix,const WCHAR *file){WCHAR m[MAX_PATH_CHARS+200];cpyw(m,MAX_PATH_CHARS+200,prefix);catw(m,MAX_PATH_CHARS+200,file);catw(m,MAX_PATH_CHARS+200,L"\r\n");ui_log(m);}

/* Filesystem operations. Writes always go to a temporary file, then are atomically replaced. */
static BOOL write_bytes(HANDLE h,const void *v,DWORD n){DWORD written=0;return n==0 || (WriteFile(h,v,n,&written,NULL)&&written==n);}
static BOOL write_ascii(HANDLE h,const CHAR *s){return write_bytes(h,s,al(s));}
static BOOL output_temp_path(const WCHAR *dest,WCHAR *tmp){cpyw(tmp,MAX_PATH_CHARS,dest);return catw(tmp,MAX_PATH_CHARS,L".pdfutils.tmp");}
static DWORD tell_pos(HANDLE h){return SetFilePointer(h,0,NULL,FILE_CURRENT);}
static BOOL pdf_obj(HANDLE h,DWORD *offsets,int n,const CHAR *content){CHAR b[36];offsets[n]=tell_pos(h);wsprintfA(b,"%d 0 obj\n",n);return write_ascii(h,b)&&write_ascii(h,content);}
static BOOL is_own_jpg_pdf(const WCHAR *path){
  HANDLE h;CHAR b[100];DWORD read=0;UINT i,j;
  const CHAR marker[]="PDF-Utils JPEG-to-PDF";
  h=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if(h==INVALID_HANDLE_VALUE)return FALSE;
  if(!ReadFile(h,b,99,&read,NULL)){CloseHandle(h);return FALSE;}
  CloseHandle(h);
  for(i=0;i+al(marker)<=read;i++){
    for(j=0;j<al(marker);j++)if(b[i+j]!=marker[j])break;
    if(j==al(marker))return TRUE;
  }
  return FALSE;
}
static BOOL read_jpeg_info(const unsigned char *d,DWORD n,DWORD *width,DWORD *height,DWORD *comps){
  DWORD p=2;
  if(n<16||d[0]!=0xff||d[1]!=0xd8)return FALSE;
  while(p+3<n) {
    unsigned int marker,sz;
    while(p<n && d[p]!=0xff) p++;
    while(p<n && d[p]==0xff) p++;
    if(p>=n)return FALSE;
    marker=d[p++];
    if(marker==0xd9||marker==0xda)break;
    if(marker==0x01||(marker>=0xd0&&marker<=0xd7))continue;
    if(p+2>n)return FALSE;
    sz=((unsigned)d[p]<<8)|d[p+1];
    if(sz<2||p+sz>n)return FALSE;
    if((marker>=0xc0&&marker<=0xc3)||(marker>=0xc5&&marker<=0xc7)||(marker>=0xc9&&marker<=0xcb)||(marker>=0xcd&&marker<=0xcf)){
      if(sz<8) return FALSE;
      *height=((unsigned)d[p+3]<<8)|d[p+4];
      *width=((unsigned)d[p+5]<<8)|d[p+6];
      *comps=d[p+7];
      return *width>0 && *height>0 && (*comps==1||*comps==3||*comps==4);
    }
    p+=sz;
  }
  return FALSE;
}
static DWORD ratio_mul(DWORD a,DWORD b,DWORD c){
  ULONGLONG numerator=(ULONGLONG)a*b, q=0, rem=0;int bit;
  if(!c)return 0;
  for(bit=63;bit>=0;--bit){rem=(rem<<1)|((numerator>>bit)&1);if(rem>=c){rem-=c;q|=((ULONGLONG)1<<bit);}}
  return (DWORD)q;
}
static BOOL jpeg_to_pdf(const WCHAR *src,const WCHAR *dst,WCHAR *error){
  HANDLE in=INVALID_HANDLE_VALUE,out=INVALID_HANDLE_VALUE;unsigned char *data=NULL;DWORD sz,hi=0,actual=0,w=0,h=0,channels=0;
  DWORD off[6]={0},xref;CHAR buf[1024],stream[320];WCHAR tmp[MAX_PATH_CHARS];BOOL ok=FALSE;DWORD iw,ih,ix,iy,pw,ph;
  if(!output_temp_path(dst,tmp)){cpyw(error,200,L"Percorso di uscita troppo lungo");return FALSE;}
  in=CreateFileW(src,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if(in==INVALID_HANDLE_VALUE){errw(error,200,GetLastError());goto cleanup;}
  sz=GetFileSize(in,&hi);
  if(hi!=0||sz<16||sz>250u*1024u*1024u){cpyw(error,200,L"JPG vuoto o superiore a 250 MB");goto cleanup;}
  data=(unsigned char*)HeapAlloc(GetProcessHeap(),0,sz);
  if(!data){cpyw(error,200,L"Memoria insufficiente");goto cleanup;}
  if(!ReadFile(in,data,sz,&actual,NULL)||actual!=sz){cpyw(error,200,L"Impossibile leggere il JPG");goto cleanup;}
  if(!read_jpeg_info(data,sz,&w,&h,&channels)){cpyw(error,200,L"Formato JPEG non valido o non supportato");goto cleanup;}
  if(w>100000||h>100000){cpyw(error,200,L"Dimensioni immagine eccessive");goto cleanup;}
  pw=w>h?842:595;ph=w>h?595:842;
  /* Maintain aspect ratio, fit inside A4 with a 10pt margin. Units are 1/100 point. */
  if(((ULONGLONG)w*(ph-20))>((ULONGLONG)h*(pw-20))){iw=(pw-20)*100u;ih=ratio_mul(iw,h,w);}else{ih=(ph-20)*100u;iw=ratio_mul(ih,w,h);}
  ix=(pw*100u-iw)/2u;iy=(ph*100u-ih)/2u;
  out=CreateFileW(tmp,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
  if(out==INVALID_HANDLE_VALUE){errw(error,200,GetLastError());goto cleanup;}
  if(!write_bytes(out,"%PDF-1.4\n%\xe2\xe3\xcf\xd3\n",15)||!write_ascii(out,"% PDF-Utils JPEG-to-PDF 1.0\n"))goto io_fail;
  if(!pdf_obj(out,off,1,"<< /Type /Catalog /Pages 2 0 R >>\nendobj\n"))goto io_fail;
  if(!pdf_obj(out,off,2,"<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n"))goto io_fail;
  wsprintfA(buf,"<< /Type /Page /Parent 2 0 R /MediaBox [0 0 %u %u] /Resources << /XObject << /Im0 4 0 R >> >> /Contents 5 0 R >>\nendobj\n",pw,ph);
  if(!pdf_obj(out,off,3,buf))goto io_fail;
  off[4]=tell_pos(out);
  wsprintfA(buf,"4 0 obj\n<< /Type /XObject /Subtype /Image /Width %u /Height %u /ColorSpace %s /BitsPerComponent 8 /Filter /DCTDecode /Length %u",w,h,channels==1?"/DeviceGray":channels==3?"/DeviceRGB":"/DeviceCMYK",sz);
  if(!write_ascii(out,buf))goto io_fail;
  if(channels==4 && !write_ascii(out," /Decode [1 0 1 0 1 0 1 0]"))goto io_fail;
  if(!write_ascii(out," >>\nstream\n")||!write_bytes(out,data,sz)||!write_ascii(out,"\nendstream\nendobj\n"))goto io_fail;
  /* Fixed-point (2 decimals) to avoid floating point and CRT dependencies. */
  wsprintfA(stream,"q\n%u.%02u 0 0 %u.%02u %u.%02u %u.%02u cm\n/Im0 Do\nQ\n",iw/100,iw%100,ih/100,ih%100,ix/100,ix%100,iy/100,iy%100);
  off[5]=tell_pos(out);
  wsprintfA(buf,"5 0 obj\n<< /Length %u >>\nstream\n",al(stream));
  if(!write_ascii(out,buf)||!write_ascii(out,stream)||!write_ascii(out,"endstream\nendobj\n"))goto io_fail;
  xref=tell_pos(out);
  if(!write_ascii(out,"xref\n0 6\n0000000000 65535 f \n"))goto io_fail;
  {int j;for(j=1;j<6;j++){wsprintfA(buf,"%010u 00000 n \n",off[j]);if(!write_ascii(out,buf))goto io_fail;}}
  wsprintfA(buf,"trailer\n<< /Size 6 /Root 1 0 R >>\nstartxref\n%u\n%%%%EOF\n",xref);
  if(!write_ascii(out,buf))goto io_fail;
  CloseHandle(out);out=INVALID_HANDLE_VALUE;
  diag_step(350,"PDF/A: moving successful result into place");
  if(!MoveFileExW(tmp,dst,MOVEFILE_REPLACE_EXISTING)){errw(error,200,GetLastError());goto cleanup;}
  ok=TRUE;goto cleanup;
io_fail:cpyw(error,200,L"Errore scrittura PDF");
cleanup:
  if(in!=INVALID_HANDLE_VALUE)CloseHandle(in);
  if(out!=INVALID_HANDLE_VALUE)CloseHandle(out);
  if(data)HeapFree(GetProcessHeap(),0,data);
  if(!ok)DeleteFileW(tmp);
  return ok;
}

/* Ghostscript discovers default locations, but may also be selected manually. */
static BOOL try_gs_at(const WCHAR *path,WCHAR *out){if(file_exists(path)){cpyw(out,MAX_PATH_CHARS,path);return TRUE;}return FALSE;}
static BOOL check_gs_dir(const WCHAR *base,WCHAR *out){
  WCHAR pattern[MAX_PATH_CHARS],dir[MAX_PATH_CHARS],path[MAX_PATH_CHARS];WIN32_FIND_DATAW f;HANDLE it;
  cpyw(pattern,MAX_PATH_CHARS,base);append_slash(pattern,MAX_PATH_CHARS);catw(pattern,MAX_PATH_CHARS,L"gs\\gs*");
  it=FindFirstFileW(pattern,&f);
  if(it==INVALID_HANDLE_VALUE)return FALSE;
  do{
    if(!(f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))continue;
    cpyw(dir,MAX_PATH_CHARS,base);append_slash(dir,MAX_PATH_CHARS);catw(dir,MAX_PATH_CHARS,L"gs\\");catw(dir,MAX_PATH_CHARS,f.cFileName);catw(dir,MAX_PATH_CHARS,L"\\bin\\gswin64c.exe");
    if(try_gs_at(dir,out)){FindClose(it);return TRUE;}
    cpyw(path,MAX_PATH_CHARS,base);append_slash(path,MAX_PATH_CHARS);catw(path,MAX_PATH_CHARS,L"gs\\");catw(path,MAX_PATH_CHARS,f.cFileName);catw(path,MAX_PATH_CHARS,L"\\bin\\gswin32c.exe");
    if(try_gs_at(path,out)){FindClose(it);return TRUE;}
  }while(FindNextFileW(it,&f));
  FindClose(it);return FALSE;
}
static BOOL find_ghostscript(WCHAR *out){
  WCHAR exe[MAX_PATH_CHARS],path[MAX_PATH_CHARS],base[MAX_PATH_CHARS];UINT n;
  exe[0]=0;n=GetModuleFileNameW(NULL,exe,MAX_PATH_CHARS);
  while(n>0&&exe[n-1]!='\\'&&exe[n-1]!='/')n--;exe[n]=0;
  cpyw(path,MAX_PATH_CHARS,exe);catw(path,MAX_PATH_CHARS,L"gswin64c.exe");if(try_gs_at(path,out))return TRUE;
  cpyw(path,MAX_PATH_CHARS,exe);catw(path,MAX_PATH_CHARS,L"gswin32c.exe");if(try_gs_at(path,out))return TRUE;
  cpyw(path,MAX_PATH_CHARS,exe);catw(path,MAX_PATH_CHARS,L"ghostscript\\bin\\gswin64c.exe");if(try_gs_at(path,out))return TRUE;
  cpyw(path,MAX_PATH_CHARS,exe);catw(path,MAX_PATH_CHARS,L"ghostscript\\bin\\gswin32c.exe");if(try_gs_at(path,out))return TRUE;
  if(SearchPathW(NULL,L"gswin64c.exe",NULL,MAX_PATH_CHARS,path,NULL)){cpyw(out,MAX_PATH_CHARS,path);return TRUE;}
  if(SearchPathW(NULL,L"gswin32c.exe",NULL,MAX_PATH_CHARS,path,NULL)){cpyw(out,MAX_PATH_CHARS,path);return TRUE;}
  if(GetEnvironmentVariableW(L"ProgramFiles",base,MAX_PATH_CHARS) && check_gs_dir(base,out))return TRUE;
  if(GetEnvironmentVariableW(L"ProgramFiles(x86)",base,MAX_PATH_CHARS) && check_gs_dir(base,out))return TRUE;
  return FALSE;
}
static BOOL profile_path(WCHAR *out){
  DWORD len=MAX_PATH_CHARS;
  UINT n;
  /* Windows color management API avoids WOW64 System32 path redirection. */
  if(GetColorDirectoryW(NULL,out,&len)){
    append_slash(out,MAX_PATH_CHARS);
    catw(out,MAX_PATH_CHARS,L"sRGB Color Space Profile.icm");
    if(file_exists(out))return TRUE;
  }
  n=GetSystemDirectoryW(out,MAX_PATH_CHARS);
  if(!n||n>=MAX_PATH_CHARS)return FALSE;
  catw(out,MAX_PATH_CHARS,L"\\spool\\drivers\\color\\sRGB Color Space Profile.icm");
  if(file_exists(out))return TRUE;
  out[0]=0;return FALSE;
}
static BOOL make_ps_safe_string(const WCHAR *src,CHAR *dest,UINT capacity){
  UINT i=0,k=0;WCHAR c;
  for(;src[i];i++){
    c=src[i];
    if(c>127)return FALSE; /* PS path literal is ASCII; Windows standard profile path normally is. */
    if(c=='\\')c='/';
    if(c=='('||c==')'||c=='\\'){if(k+2>=capacity)return FALSE;dest[k++]='\\';}
    if(k+1>=capacity)return FALSE;
    dest[k++]=(CHAR)c;
  }
  dest[k]=0;return TRUE;
}
static BOOL generate_pdfa_setup(const WCHAR *psfile,const WCHAR *iccpath){
  CHAR icc[2048],b[3000];HANDLE h;BOOL ok;
  if(!make_ps_safe_string(iccpath,icc,2048))return FALSE;
  h=CreateFileW(psfile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
  if(h==INVALID_HANDLE_VALUE)return FALSE;
  wsprintfA(b,"%%!PS\n/ICCProfile (%s) def\n[/_objdef {icc_PDFA} /type /stream /OBJ pdfmark\n[{icc_PDFA} <</N 3>> /PUT pdfmark\n[{icc_PDFA} ICCProfile (r) file /PUT pdfmark\n[/_objdef {OutputIntent_PDFA} /type /dict /OBJ pdfmark\n[{OutputIntent_PDFA} <<\n /Type /OutputIntent\n /S /GTS_PDFA1\n /DestOutputProfile {icc_PDFA}\n /OutputConditionIdentifier (sRGB IEC61966-2.1)\n >> /PUT pdfmark\n[{Catalog} <</OutputIntents [ {OutputIntent_PDFA} ]>> /PUT pdfmark\n",icc);
  ok=write_ascii(h,b);CloseHandle(h);if(!ok)DeleteFileW(psfile);return ok;
}
static BOOL pdf_to_pdfa(const WCHAR *gs,const WCHAR *src,const WCHAR *dst,const WCHAR *icc,WCHAR *error){
  WCHAR psfile[MAX_PATH_CHARS],tmp[MAX_PATH_CHARS],logfile[MAX_PATH_CHARS],iccallow[MAX_PATH_CHARS],command[MAX_PATH_CHARS*4];
  HANDLE process=INVALID_HANDLE_VALUE,log=INVALID_HANDLE_VALUE,nul=INVALID_HANDLE_VALUE;
  SECURITY_ATTRIBUTES sa;STARTUPINFOW si;PROCESS_INFORMATION pi;
  DWORD status,exitcode=1;BOOL ok=FALSE;
  if(!output_temp_path(dst,tmp)){cpyw(error,200,L"Percorso di uscita troppo lungo");return FALSE;}
  cpyw(psfile,MAX_PATH_CHARS,tmp);
  if(!catw(psfile,MAX_PATH_CHARS,L".ps")){cpyw(error,200,L"Percorso temporaneo troppo lungo");return FALSE;}
  cpyw(logfile,MAX_PATH_CHARS,dst);
  if(!catw(logfile,MAX_PATH_CHARS,L".ghostscript.log")){cpyw(error,200,L"Percorso log troppo lungo");return FALSE;}
  diag_step(310,"PDF/A: preparing PostScript and ICC definition");
  if(!generate_pdfa_setup(psfile,icc)){cpyw(error,200,L"Impossibile creare la definizione PDF/A");return FALSE;}
  DeleteFileW(tmp);
  command[0]=0;
  quote_w(command,MAX_PATH_CHARS*4,gs);
#if defined(PDFUTILS_PORTABLE)
  /* Portable Ghostscript must search the extracted local resources. */
  catw(command,MAX_PATH_CHARS*4,L" -I\"");
  catw(command,MAX_PATH_CHARS*4,portable_root);
  catw(command,MAX_PATH_CHARS*4,L"\\Resource\\Init;");
  catw(command,MAX_PATH_CHARS*4,portable_root);
  catw(command,MAX_PATH_CHARS*4,L"\\Resource\\Font;");
  catw(command,MAX_PATH_CHARS*4,portable_root);
  catw(command,MAX_PATH_CHARS*4,L"\\lib\"");
#endif
  /* Ghostscript's SAFER sandbox blocks arbitrary ICC profile reads by default.
     The sole extra read permission is for the profile selected by Windows ICM. */
  cpyw(iccallow,MAX_PATH_CHARS,icc);
  {UINT j;for(j=0;iccallow[j];j++)if(iccallow[j]=='\\')iccallow[j]='/';}
  catw(command,MAX_PATH_CHARS*4,L" --permit-file-read=");
  quote_w(command,MAX_PATH_CHARS*4,iccallow);
  catw(command,MAX_PATH_CHARS*4,L" -dSAFER -dBATCH -dNOPAUSE -dPDFA=2 -dPDFACompatibilityPolicy=1 -sDEVICE=pdfwrite -sColorConversionStrategy=RGB -sBlendConversionStrategy=Simple -sOutputFile=\"");
  catw(command,MAX_PATH_CHARS*4,tmp);
  catw(command,MAX_PATH_CHARS*4,L"\" ");
  quote_w(command,MAX_PATH_CHARS*4,psfile);
  catw(command,MAX_PATH_CHARS*4,L" ");quote_w(command,MAX_PATH_CHARS*4,src);

  /* Redirect both stdout and stderr into a per-file diagnostic report.
     Without handles, a GUI program hides all Ghostscript error messages. */
  zmem(&sa,sizeof(sa));sa.nLength=sizeof(sa);sa.bInheritHandle=TRUE;
  log=CreateFileW(logfile,GENERIC_WRITE,FILE_SHARE_READ,&sa,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
  if(log==INVALID_HANDLE_VALUE){errw(error,200,GetLastError());goto cleanup;}
  nul=CreateFileW(L"NUL",GENERIC_READ,FILE_SHARE_READ,&sa,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if(nul==INVALID_HANDLE_VALUE){errw(error,200,GetLastError());goto cleanup;}
  zmem(&si,sizeof(si));zmem(&pi,sizeof(pi));
  si.cb=sizeof(si);si.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;si.wShowWindow=SW_HIDE;
  si.hStdInput=nul;si.hStdOutput=log;si.hStdError=log;
  diag_step(320,"PDF/A: calling CreateProcessW Ghostscript");
  if(!CreateProcessW(gs,command,NULL,NULL,TRUE,CREATE_NO_WINDOW,NULL,NULL,&si,&pi)){
    {CHAR b[130];wsprintfA(b,"CreateProcessW FAILED: GetLastError=%u",GetLastError());diag_write(b);}
    errw(error,200,GetLastError());goto cleanup;
  }
  process=pi.hProcess;CloseHandle(pi.hThread);
  CloseHandle(log);log=INVALID_HANDLE_VALUE;
  CloseHandle(nul);nul=INVALID_HANDLE_VALUE;
  diag_step(330,"PDF/A: Ghostscript process started; waiting");
  status=WaitForSingleObject(process,300000u);
  if(status!=WAIT_OBJECT_0){
    TerminateProcess(process,1);
    WaitForSingleObject(process,10000u);
    cpyw(error,200,L"Timeout/errore di attesa Ghostscript (5 minuti)");goto cleanup;
  }
  diag_step(340,"PDF/A: checking Ghostscript exit code");
  if(!GetExitCodeProcess(process,&exitcode)||exitcode!=0){
    WCHAR m[200];CHAR diag[120];wsprintfA(diag,"Ghostscript exit code=%u",exitcode);diag_write(diag);
    wsprintfW(m,L"Ghostscript: errore, codice %u",exitcode);cpyw(error,200,m);goto cleanup;
  }
  if(!file_exists(tmp)){cpyw(error,200,L"Ghostscript non ha generato un PDF/A");goto cleanup;}
  {HANDLE h=CreateFileW(tmp,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);DWORD size;
    if(h==INVALID_HANDLE_VALUE){cpyw(error,200,L"Impossibile leggere PDF/A generato");goto cleanup;}
    size=GetFileSize(h,NULL);CloseHandle(h);
    if(size==0||size==0xffffffffu){cpyw(error,200,L"PDF/A generato vuoto");goto cleanup;}
  }
  if(!MoveFileExW(tmp,dst,MOVEFILE_REPLACE_EXISTING)){errw(error,200,GetLastError());goto cleanup;}
  ok=TRUE;
cleanup:
  if(process!=INVALID_HANDLE_VALUE)CloseHandle(process);
  if(nul!=INVALID_HANDLE_VALUE)CloseHandle(nul);
  if(log!=INVALID_HANDLE_VALUE)CloseHandle(log);
  DeleteFileW(psfile);
  if(!ok)DeleteFileW(tmp);
  if(ok)DeleteFileW(logfile); /* preserve Ghostscript output on failure */
  return ok;
}

typedef struct FILE_NODE {struct FILE_NODE *next;WCHAR name[260];BOOL is_pdf;} FILE_NODE;
typedef struct {WCHAR folder[MAX_PATH_CHARS];WCHAR gs[MAX_PATH_CHARS];BOOL do_jpg,do_pdfa;} JOB;
static void remove_extension(WCHAR *file){UINT n=wl(file);while(n>0){if(file[n-1]=='.'){file[n-1]=0;return;}if(file[n-1]=='\\')break;n--;}}
static DWORD WINAPI run_conversion(LPVOID context){
  JOB *job=(JOB*)context;FILE_NODE *head=NULL,*tail=NULL,*node;WIN32_FIND_DATAW data;HANDLE fh;
  WCHAR pattern[MAX_PATH_CHARS],src[MAX_PATH_CHARS],dst[MAX_PATH_CHARS],base[MAX_PATH_CHARS],icc[MAX_PATH_CHARS],error[200],m[300];
  int count=0,done=0,failed=0,skipped=0,i;
  BOOL gs_ready=FALSE,icc_ready=FALSE;
  diag_step(100,"Worker entered successfully after stack probe");
  gs_ready=file_exists(job->gs);
  diag_step(105,"Ghostscript executable lookup completed");
  diag_step(110,"Looking up Windows sRGB profile");
  icc_ready=profile_path(icc);
  diag_step(120,"Enumerating input folder");
  cpyw(pattern,MAX_PATH_CHARS,job->folder);append_slash(pattern,MAX_PATH_CHARS);catw(pattern,MAX_PATH_CHARS,L"*");
  fh=FindFirstFileW(pattern,&data);
  if(fh==INVALID_HANDLE_VALUE){ui_log(L"ERRORE: impossibile aprire la cartella.\r\n");goto finish;}
  do{
    BOOL is_pdf,is_jpg;
    if(data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)continue;
    is_jpg=endw(data.cFileName,L".jpg")||endw(data.cFileName,L".jpeg");
    is_pdf=endw(data.cFileName,L".pdf")&&!endw(data.cFileName,L"_PDFA.pdf");
    if(!((job->do_jpg&&is_jpg)||(job->do_pdfa&&is_pdf)))continue;
    node=(FILE_NODE*)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(FILE_NODE));
    if(!node){ui_log(L"ERRORE: memoria insufficiente per la lista file.\r\n");break;}
    cpyw(node->name,260,data.cFileName);node->is_pdf=is_pdf;
    if(tail)tail->next=node;else head=node;tail=node;count++;
  }while(FindNextFileW(fh,&data));
  FindClose(fh);
  wsprintfW(m,L"Trovati %d file compatibili.\r\n",count);ui_log(m);
  {CHAR b[100];wsprintfA(b,"Eligible input file count=%d",count);diag_write(b);}
  if(!count){ui_log(L"Nessun JPG/JPEG o PDF originale trovato.\r\n");goto finish;}
  if(job->do_pdfa && !gs_ready){diag_write("Ghostscript executable path missing");ui_log(L"ATTENZIONE: Ghostscript non trovato. I PDF/A verranno saltati.\r\n");}
  if(job->do_pdfa && !icc_ready){diag_write("Windows sRGB ICC profile missing");ui_log(L"ATTENZIONE: profilo colore sRGB di Windows non trovato. I PDF/A verranno saltati.\r\n");}
  /* JPG first; snapshot prevents newly produced PDFs being reprocessed in the same run. */
  for(i=0;i<=1;i++){
    for(node=head;node;node=node->next){
      BOOL ok=FALSE;
      if(node->is_pdf != (BOOL)i)continue;
      cpyw(src,MAX_PATH_CHARS,job->folder);append_slash(src,MAX_PATH_CHARS);catw(src,MAX_PATH_CHARS,node->name);
      cpyw(base,MAX_PATH_CHARS,node->name);remove_extension(base);
      cpyw(dst,MAX_PATH_CHARS,job->folder);append_slash(dst,MAX_PATH_CHARS);catw(dst,MAX_PATH_CHARS,base);
      if(node->is_pdf)catw(dst,MAX_PATH_CHARS,L"_PDFA.pdf");else {
        catw(dst,MAX_PATH_CHARS,L".pdf");
        if(file_exists(dst)&&!is_own_jpg_pdf(dst)){
          /* Preserve an unrelated original PDF with the same name. */
          cpyw(dst,MAX_PATH_CHARS,job->folder);append_slash(dst,MAX_PATH_CHARS);catw(dst,MAX_PATH_CHARS,base);catw(dst,MAX_PATH_CHARS,L"_JPG.pdf");
          if(file_exists(dst)&&!is_own_jpg_pdf(dst)){
            skipped++;ui_log_file(L"[SALTATO] Nome PDF gia' occupato: ",node->name);continue;
          }
        }
      }
      error[0]=0;
      if(node->is_pdf){
        if(!gs_ready||!icc_ready){skipped++;ui_log_file(L"[SALTATO] ",node->name);continue;}
        diag_step(300,"Starting PDF/A conversion of next input");
        ok=pdf_to_pdfa(job->gs,src,dst,icc,error);
        diag_step(360,"Returned from PDF/A conversion function");
      }else ok=jpeg_to_pdf(src,dst,error);
      if(ok){done++;ui_log_file(L"[OK] ",node->name);}
      else {
        failed++;ui_log_file(L"[ERRORE] ",node->name);ui_log_file(L"    ",error);
        if(node->is_pdf){
          WCHAR diag[MAX_PATH_CHARS];cpyw(diag,MAX_PATH_CHARS,dst);
          if(catw(diag,MAX_PATH_CHARS,L".ghostscript.log")&&file_exists(diag))
            ui_log_file(L"    Dettagli Ghostscript: ",diag);
        }
      }
    }
  }
  wsprintfW(m,L"Completato: %d riusciti, %d errori, %d saltati.\r\n",done,failed,skipped);ui_log(m);
  diag_step(900,"Batch complete");
finish:
  while(head){node=head->next;HeapFree(GetProcessHeap(),0,head);head=node;}
  HeapFree(GetProcessHeap(),0,job);
  PostMessageW(mainWindow,WM_DONE,0,0);
  return 0;
}

static HWND make_ctrl(LPCWSTR classname,LPCWSTR title,DWORD style,int x,int y,int w,int h,int id,BOOL bold){
  HWND win=CreateWindowExW(0,classname,title,WS_CHILD|WS_VISIBLE|style,x,y,w,h,mainWindow,(HMENU)(LONGLONG)id,appInstance,NULL);
  SendMessageW(win,WM_SETFONT,(WPARAM)(bold?headingFont:uiFont),1);
  return win;
}
static void ui_append(WCHAR *s){
  SendMessageW(logControl,EM_SETSEL,(WPARAM)-1,(LPARAM)-1);
  SendMessageW(logControl,EM_REPLACESEL,0,(LPARAM)s);
}
static void choose_folder(void){
  WCHAR display[260],path[MAX_PATH_CHARS];BROWSEINFOW b;void *id;
  zmem(&b,sizeof(b));display[0]=0;
  b.hwndOwner=mainWindow;b.pszDisplayName=display;b.lpszTitle=L"Seleziona la cartella da convertire";b.ulFlags=BIF_RETURNONLYFSDIRS|BIF_USENEWUI;
  id=SHBrowseForFolderW(&b);
  if(id){if(SHGetPathFromIDListW(id,path))SetWindowTextW(folderControl,path);CoTaskMemFree(id);}
}
static void choose_gs(void){
  WCHAR path[MAX_PATH_CHARS],filter[]=L"Ghostscript (gswin*c.exe)\0gswin*c.exe\0Programmi EXE\0*.exe\0\0";
  OPENFILENAMEW f;zmem(&f,sizeof(f));path[0]=0;
  f.lStructSize=sizeof(f);f.hwndOwner=mainWindow;f.lpstrFilter=filter;f.lpstrFile=path;f.nMaxFile=MAX_PATH_CHARS;
  f.lpstrTitle=L"Seleziona gswin64c.exe o gswin32c.exe";f.Flags=OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
  if(GetOpenFileNameW(&f))SetWindowTextW(gsControl,path);
}
static void start_job(void){
  JOB *job;HANDLE thread;DWORD tid;
  if(busy)return;
  job=(JOB*)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(JOB));
  if(!job){MessageBoxW(mainWindow,L"Memoria insufficiente.",APP_NAME,0x10);return;}
  GetWindowTextW(folderControl,job->folder,MAX_PATH_CHARS);
  GetWindowTextW(gsControl,job->gs,MAX_PATH_CHARS);
  job->do_jpg=SendMessageW(jpgCheck,BM_GETCHECK,0,0)==BST_CHECKED;
  job->do_pdfa=SendMessageW(pdfaCheck,BM_GETCHECK,0,0)==BST_CHECKED;
  if(!wl(job->folder)||GetFileAttributesW(job->folder)==INVALID_FILE_ATTRIBUTES){MessageBoxW(mainWindow,L"Seleziona una cartella valida.",APP_NAME,0x30);HeapFree(GetProcessHeap(),0,job);return;}
  if(!job->do_jpg&&!job->do_pdfa){MessageBoxW(mainWindow,L"Seleziona almeno una conversione.",APP_NAME,0x30);HeapFree(GetProcessHeap(),0,job);return;}
  /* Initialize trace BEFORE starting the worker. Leave it on disk even if the
     application terminates while Ghostscript has not yet created a log. */
  diagnostic_file[0]=0;
  cpyw(diagnostic_file,MAX_PATH_CHARS,job->folder);
  append_slash(diagnostic_file,MAX_PATH_CHARS);
  if(!catw(diagnostic_file,MAX_PATH_CHARS,L"PDF-Utils-diagnostica.txt"))diagnostic_file[0]=0;
  diag_step(10,"PDF Utils stack probe fix v4 - start");
  diag_step(11,"Main thread: disabling controls");
  busy=1;EnableWindow(startButton,FALSE);EnableWindow(folderControl,FALSE);EnableWindow(gsControl,FALSE);
  diag_step(12,"Main thread: updating controls");
  SetWindowTextW(statusControl,L"Conversione in corso...");SetWindowTextW(logControl,L"");
  diag_step(13,"Main thread: posting UI log");
  ui_log(L"Avvio elaborazione. I file originali rimangono nella cartella.\r\n");
  diag_step(14,"Main thread: about to create worker thread");
  thread=CreateThread(NULL,0,run_conversion,job,0,&tid);
  diag_step(15,"Main thread: CreateThread call returned");
  if(!thread){busy=0;EnableWindow(startButton,TRUE);EnableWindow(folderControl,TRUE);EnableWindow(gsControl,TRUE);HeapFree(GetProcessHeap(),0,job);MessageBoxW(mainWindow,L"Impossibile avviare la conversione.",APP_NAME,0x10);}
  else CloseHandle(thread);
}
static LRESULT WINAPI window_proc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
  switch(msg){
    case WM_CREATE:{
      WCHAR gs[MAX_PATH_CHARS];mainWindow=hwnd;
      headingFont=CreateFontW(-20,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_SWISS,L"Segoe UI");
      uiFont=CreateFontW(-16,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_SWISS,L"Segoe UI");
      make_ctrl(L"STATIC",L"PDF Utils - fix crash stack v4",0,20,16,550,30,0,TRUE);
      make_ctrl(L"STATIC",L"Cartella di lavoro",0,20,60,560,20,0,FALSE);
      folderControl=make_ctrl(L"EDIT",L"",WS_BORDER|ES_AUTOHSCROLL|WS_TABSTOP,20,83,465,28,IDC_FOLDER,FALSE);
      make_ctrl(L"BUTTON",L"Sfoglia...",WS_TABSTOP,493,83,116,28,IDC_BROWSE,FALSE);
      make_ctrl(L"STATIC",L"Operazioni",0,20,123,540,20,0,TRUE);
      jpgCheck=make_ctrl(L"BUTTON",L"JPG/JPEG -> PDF (un PDF per immagine)",BS_AUTOCHECKBOX|WS_TABSTOP,20,150,570,23,IDC_JPG,FALSE);
      pdfaCheck=make_ctrl(L"BUTTON",L"PDF -> PDF/A-2b (archiviazione)",BS_AUTOCHECKBOX|WS_TABSTOP,20,178,570,23,IDC_PDFA,FALSE);
      SendMessageW(jpgCheck,BM_SETCHECK,BST_CHECKED,0);SendMessageW(pdfaCheck,BM_SETCHECK,BST_CHECKED,0);
      make_ctrl(L"STATIC",L"Ghostscript (necessario solo per PDF/A)",0,20,213,580,20,0,FALSE);
      gsControl=make_ctrl(L"EDIT",L"",WS_BORDER|ES_AUTOHSCROLL|WS_TABSTOP,20,237,465,28,IDC_GS,FALSE);
      make_ctrl(L"BUTTON",L"Sfoglia...",WS_TABSTOP,493,237,116,28,IDC_GS_BROWSE,FALSE);
      if(find_ghostscript(gs))SetWindowTextW(gsControl,gs);
      else SetWindowTextW(gsControl,L"Ghostscript non trovato: scegli gswin64c.exe o gswin32c.exe");
      startButton=make_ctrl(L"BUTTON",L"CONVERTI TUTTI",WS_TABSTOP,20,284,180,38,IDC_START,TRUE);
      statusControl=make_ctrl(L"STATIC",L"Pronto",0,221,294,380,25,IDC_STATUS,FALSE);
      make_ctrl(L"STATIC",L"Risultati",0,20,336,580,21,0,TRUE);
      logControl=make_ctrl(L"EDIT",L"",WS_BORDER|WS_VSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|ES_READONLY,20,361,589,144,IDC_LOG,FALSE);
      make_ctrl(L"STATIC",L"Originali conservati  |  output nella stessa cartella  |  PDF/A con suffisso _PDFA.pdf",0,20,515,600,19,0,FALSE);
      return 0;
    }
    case WM_COMMAND:{
      int id=(int)(wp&0xffff);
      if(id==IDC_BROWSE&&!busy)choose_folder();
      else if(id==IDC_GS_BROWSE&&!busy)choose_gs();
      else if(id==IDC_START&&!busy)start_job();
      return 0;
    }
    case WM_LOG:{WCHAR *s=(WCHAR*)lp;if(s){ui_append(s);HeapFree(GetProcessHeap(),0,s);}return 0;}
    case WM_DONE:{busy=0;EnableWindow(startButton,TRUE);EnableWindow(folderControl,TRUE);EnableWindow(gsControl,TRUE);SetWindowTextW(statusControl,L"Elaborazione terminata");return 0;}
    case WM_CLOSE:if(busy){MessageBoxW(hwnd,L"Attendi la fine della conversione prima di chiudere.",APP_NAME,0x40);return 0;}break;
    case WM_DESTROY:if(uiFont)DeleteObject(uiFont);if(headingFont)DeleteObject(headingFont);PostQuitMessage(0);return 0;
  }
  return DefWindowProcW(hwnd,msg,wp,lp);
}
void WinMainCRTStartup(void){
  WNDCLASSEXW cl;MSG message;HWND win;zmem(&cl,sizeof(cl));
  appInstance=GetModuleHandleW(NULL);
  SetUnhandledExceptionFilter((void*)diag_unhandled);
  cl.cbSize=sizeof(cl);cl.lpfnWndProc=window_proc;cl.hInstance=appInstance;
  cl.hCursor=LoadCursorW(NULL,IDC_ARROW);cl.hbrBackground=(HBRUSH)(LONGLONG)(COLOR_WINDOW+1);
  cl.lpszClassName=L"PdfUtilsNativeWindowV1";
  if(!RegisterClassExW(&cl))ExitProcess(2);
  win=CreateWindowExW(0,cl.lpszClassName,APP_NAME,WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,CW_USEDEFAULT,645,585,NULL,NULL,appInstance,NULL);
  if(!win)ExitProcess(3);
  mainWindow=win;ShowWindow(win,SW_SHOW);UpdateWindow(win);
  while(GetMessageW(&message,NULL,0,0)>0){TranslateMessage(&message);DispatchMessageW(&message);}
  ExitProcess(0);
}