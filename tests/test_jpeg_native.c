/* Host-side tests for the same native JPEG->PDF engine used in Windows EXE.
   Run: clang -fshort-wchar -ffunction-sections -fdata-sections -Wl,--gc-sections ... */
#define PDFUTILS_TEST
#include "../src/pdf_utils.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
static char narrow[4096];
static const char* cvt(const WCHAR *s){int i=0;while(s[i] && i<4090){narrow[i]=(char)(s[i]&127);i++;}narrow[i]=0;return narrow;}
HANDLE GetProcessHeap(void){return (HANDLE)1;}
LPVOID HeapAlloc(HANDLE h,DWORD flags,SIZE_T n){(void)h;return flags&HEAP_ZERO_MEMORY?calloc(1,n):malloc(n);}
BOOL HeapFree(HANDLE h,DWORD flags,LPVOID p){(void)h;(void)flags;free(p);return 1;}
HANDLE CreateFileW(LPCWSTR s,DWORD access,DWORD share,LPSECURITY_ATTRIBUTES sec,DWORD how,DWORD attrs,HANDLE tmpl){
  (void)share;(void)sec;(void)attrs;(void)tmpl;
  FILE*f=fopen(cvt(s),access==GENERIC_READ?"rb":how==CREATE_ALWAYS?"wb":"r+b");return f?(HANDLE)f:INVALID_HANDLE_VALUE;
}
BOOL ReadFile(HANDLE h,LPVOID b,DWORD n,DWORD*out,void*x){(void)x;*out=(DWORD)fread(b,1,n,(FILE*)h);return *out==n;}
BOOL WriteFile(HANDLE h,const void*b,DWORD n,DWORD*out,void*x){(void)x;*out=(DWORD)fwrite(b,1,n,(FILE*)h);return *out==n;}
BOOL CloseHandle(HANDLE h){return fclose((FILE*)h)==0;}
DWORD GetFileSize(HANDLE h,DWORD*high){long n;long cur=ftell((FILE*)h);fseek((FILE*)h,0,SEEK_END);n=ftell((FILE*)h);fseek((FILE*)h,cur,SEEK_SET);if(high)*high=0;return (DWORD)n;}
DWORD SetFilePointer(HANDLE h,LONG d,LONG*high,DWORD mode){(void)high;fseek((FILE*)h,d,mode==FILE_CURRENT?SEEK_CUR:SEEK_SET);return (DWORD)ftell((FILE*)h);}
DWORD GetFileAttributesW(LPCWSTR p){struct stat st;if(stat(cvt(p),&st)==0) return S_ISDIR(st.st_mode)?FILE_ATTRIBUTE_DIRECTORY:FILE_ATTRIBUTE_NORMAL;return INVALID_FILE_ATTRIBUTES;}
BOOL DeleteFileW(LPCWSTR p){return remove(cvt(p))==0;}
BOOL MoveFileExW(LPCWSTR from,LPCWSTR to,DWORD flags){char a[4096],b[4096];(void)flags;strcpy(a,cvt(from));strcpy(b,cvt(to));return rename(a,b)==0;}
DWORD GetLastError(void){return 1;}
int wsprintfA(CHAR*out,LPCSTR f,...){va_list va;va_start(va,f);int n=vsprintf(out,f,va);va_end(va);return n;}
int wsprintfW(WCHAR*out,LPCWSTR fmt,...){(void)fmt;out[0]=0;return 0;}
int main(int argc,char**argv){WCHAR src[2048],dst[2048],err[200];int j;BOOL ok;if(argc!=3)return 2;
  for(j=0;argv[1][j];j++)src[j]=(WCHAR)argv[1][j];src[j]=0;
  for(j=0;argv[2][j];j++)dst[j]=(WCHAR)argv[2][j];dst[j]=0;
  ok=jpeg_to_pdf(src,dst,err);fprintf(stderr,"JPEG conversion success=%d\n",ok);return ok?0:1;
}
