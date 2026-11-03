/* Win7-compatible single-file Ghostscript overlay runtime. Included by pdf_utils.c
   only when PDFUTILS_PORTABLE is defined. No ZIP, CRT, installer or admin rights. */
#if defined(PDFUTILS_PORTABLE)
#define PORTABLE_FOOTER_BYTES 16u
#define PORTABLE_MAX_FILES 4096u
static WCHAR portable_root[MAX_PATH_CHARS];
static const CHAR portable_magic[8]={'P','D','U','G','S','0','0','1'};
static BOOL portable_read(HANDLE f,void *buf,DWORD n){DWORD read=0;return !n || (ReadFile(f,buf,n,&read,NULL)&&read==n);}
static DWORD portable_le32(const unsigned char *p){return (DWORD)p[0]|((DWORD)p[1]<<8)|((DWORD)p[2]<<16)|((DWORD)p[3]<<24);}
static UINT portable_le16(const unsigned char *p){return (UINT)p[0]|((UINT)p[1]<<8);}
static BOOL portable_valid_name(const CHAR *s,UINT n){
  UINT i=0,start=0;
  if(!n||n>230||s[0]=='/'||s[0]=='\\')return FALSE;
  for(i=0;i<=n;i++){
    CHAR c=i==n?'/':s[i];
    if(c=='/' || c=='\\'){
      if(i==start || (i-start==1 && s[start]=='.') || (i-start==2 && s[start]=='.' && s[start+1]=='.'))return FALSE;
      start=i+1;
    }else if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='.'||c=='_'||c=='-'||c==' '))return FALSE;
  }
  return TRUE;
}
static BOOL portable_write_file(HANDLE src,const WCHAR *dest,DWORD size){
  HANDLE out=CreateFileW(dest,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
  unsigned char block[16384];DWORD done=0,chunk=0;BOOL ok=FALSE;
  if(out==INVALID_HANDLE_VALUE)return FALSE;
  while(done<size){chunk=size-done;if(chunk>sizeof(block))chunk=sizeof(block);
    if(!portable_read(src,block,chunk)||!write_bytes(out,block,chunk))goto stop;
    done+=chunk;
  }
  ok=TRUE;
stop:
  CloseHandle(out);if(!ok)DeleteFileW(dest);return ok;
}
static BOOL portable_mkdir_parents(WCHAR *full,UINT baseLen){
  UINT j;WCHAR old;
  for(j=baseLen+1;full[j];j++)if(full[j]=='\\'){
    old=full[j];full[j]=0;
    if(!CreateDirectoryW(full,NULL)){
      DWORD attr=GetFileAttributesW(full);
      if(attr==INVALID_FILE_ATTRIBUTES || !(attr&FILE_ATTRIBUTE_DIRECTORY)){full[j]=old;return FALSE;}
    }
    full[j]=old;
  }
  return TRUE;
}
static void portable_delete_tree(const WCHAR *dir){
  WCHAR pattern[MAX_PATH_CHARS],item[MAX_PATH_CHARS];WIN32_FIND_DATAW ent;HANDLE it;
  cpyw(pattern,MAX_PATH_CHARS,dir);append_slash(pattern,MAX_PATH_CHARS);catw(pattern,MAX_PATH_CHARS,L"*");
  it=FindFirstFileW(pattern,&ent);
  if(it!=INVALID_HANDLE_VALUE){
    do{
      if(eqw(ent.cFileName,L".")||eqw(ent.cFileName,L".."))continue;
      cpyw(item,MAX_PATH_CHARS,dir);append_slash(item,MAX_PATH_CHARS);catw(item,MAX_PATH_CHARS,ent.cFileName);
      if(ent.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)portable_delete_tree(item);
      else DeleteFileW(item);
    }while(FindNextFileW(it,&ent));
    FindClose(it);
  }
  RemoveDirectoryW(dir);
}
static BOOL portable_prepare(WCHAR *gsPath){
  WCHAR exe[MAX_PATH_CHARS],tmp[MAX_PATH_CHARS],dest[MAX_PATH_CHARS];
  unsigned char footer[PORTABLE_FOOTER_BYTES],head[6],uuid[16];
  HANDLE f=INVALID_HANDLE_VALUE;DWORD len,size,base,count,i;UINT n;BOOL ok=FALSE;
  if(!GetModuleFileNameW(NULL,exe,MAX_PATH_CHARS))return FALSE;
  f=CreateFileW(exe,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if(f==INVALID_HANDLE_VALUE)return FALSE;
  size=GetFileSize(f,NULL);
  if(size<PORTABLE_FOOTER_BYTES+1024)goto end;
  if(SetFilePointer(f,-(LONG)PORTABLE_FOOTER_BYTES,NULL,FILE_END)==0xffffffffu)goto end;
  if(!portable_read(f,footer,sizeof(footer)))goto end;
  for(i=0;i<8;i++)if(footer[i]!=(unsigned char)portable_magic[i])goto end;
  base=portable_le32(footer+8);count=portable_le32(footer+12);
  if(base<1024||base>=size-PORTABLE_FOOTER_BYTES||!count||count>PORTABLE_MAX_FILES)goto end;
  if(SetFilePointer(f,(LONG)base,NULL,FILE_BEGIN)==0xffffffffu)goto end;
  n=GetTempPathW(MAX_PATH_CHARS,tmp);if(!n||n>=MAX_PATH_CHARS-80)goto end;
  if(CoCreateGuid(uuid)!=0)goto end;
  cpyw(portable_root,MAX_PATH_CHARS,tmp);append_slash(portable_root,MAX_PATH_CHARS);
  if(!catw(portable_root,MAX_PATH_CHARS,L"PDFUtilsGS-"))goto end;
  for(i=0;i<16;i++){static const WCHAR hexdigits[]=L"0123456789abcdef";WCHAR hexb[3];
    hexb[0]=hexdigits[(uuid[i]>>4)&15];hexb[1]=hexdigits[uuid[i]&15];hexb[2]=0;
    if(!catw(portable_root,MAX_PATH_CHARS,hexb))goto end;
  }
  if(!CreateDirectoryW(portable_root,NULL))goto end;
  for(i=0;i<count;i++){
    CHAR name[232];DWORD bytes;UINT k;DWORD pos;
    if(!portable_read(f,head,6))goto end;
    len=portable_le16(head);bytes=portable_le32(head+2);
    if(!len||len>=sizeof(name)||bytes>256u*1024u*1024u)goto end;
    pos=SetFilePointer(f,0,NULL,FILE_CURRENT);
    if(pos==0xffffffffu || (ULONGLONG)pos+len+bytes>(ULONGLONG)size-PORTABLE_FOOTER_BYTES)goto end;
    if(!portable_read(f,name,len))goto end;
    if(!portable_valid_name(name,len))goto end;
    cpyw(dest,MAX_PATH_CHARS,portable_root);append_slash(dest,MAX_PATH_CHARS);n=wl(dest);
    for(k=0;k<len;k++){
      if(n+1>=MAX_PATH_CHARS)goto end;
      dest[n++]=name[k]=='/' ? '\\' : (WCHAR)(unsigned char)name[k];
    }
    dest[n]=0;
    if(!portable_mkdir_parents(dest,wl(portable_root)))goto end;
    if(!portable_write_file(f,dest,bytes))goto end;
  }
  if(SetFilePointer(f,0,NULL,FILE_CURRENT)!=size-PORTABLE_FOOTER_BYTES)goto end;
  cpyw(gsPath,MAX_PATH_CHARS,portable_root);catw(gsPath,MAX_PATH_CHARS,L"\\bin\\gswin32c.exe");
  if(!file_exists(gsPath)){gsPath[0]=0;goto end;}
  ok=TRUE;
end:
  if(f!=INVALID_HANDLE_VALUE)CloseHandle(f);
  if(!ok){if(portable_root[0])portable_delete_tree(portable_root);portable_root[0]=0;}
  return ok;
}
static void portable_cleanup(void){if(portable_root[0]){portable_delete_tree(portable_root);portable_root[0]=0;}}
#endif