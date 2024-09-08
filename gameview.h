

#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#ifdef __cplusplus

extern "C" {
#endif /* __cplusplus */

#include "compa.h"
#include "fsys.h"

U8 SysLcdPartClear(U8 l,U8 t,U8 r,U8 b);

U8 GamePictureDummy(U16 l,U16 t,U16 r,U16 b,U8* pic,U8* vscr,U8 mode);

U8 GamePictureSave(U16 l,U16 t,U16 r,U16 b,U8* pic,U8* vscr,U8 mode);

U8 SysLcdReverse(U8 l,U8 t,U8 r,U8 b);


U8 SysPutPixel(U8 x,U8 y,U8 c);


U8 SysLine(U8 l,U8 t,U8 r,U8 b);


U8 SysLineClear(U8 l,U8 t,U8 r,U8 b);

U8 SysRect(U8 l,U8 t,U8 r,U8 b);


U8 SysRectClear(U8 l,U8 t,U8 r,U8 b);


U8 SysGetKey();


U8 SysPicture(U16 l,U16 t,U16 r,U16 b,U8 * pic,U8 zero);


U8 SysAscii(U32 posx,U32 posy,U8 * ascii,U8 trans);
U8 SysChinese(U32 posx,U32 posy,U8 * ascii,U8 trans);

U8 SysCopyToDc();
U8 SysCopyToDcStr();
U8 SysCopyFromDcToStr();
U8 SysCopyToQImage(U8 type);

U8 FileNum(U8 filetype,U16 * filenum);

U8 FileSearch(U8 filetype,U8 fileno,U8 * filename,U8 *out_filename);


FILE *FileOpen(U8 * filename,U8 filetype,const char *openmode,FILE *filehandle,U32 *filelen);


U8 FileDel(gam_FILE * fhandle);


FILE *FileCreat(U8 filetype,U32 filelen,U8 *filename,FILE *filehandle);


U8 FileClose(gam_FILE * fhandle);


U8 FileRead(FILE *fhandle,U8 len,U8* outptr);


U8 FileWrite(FILE *fhandle,U8 len,U8* outptr);

U8 FileChangeInf(gam_FILE *file,U8* newname);

void DebugTime(int a);
//void * GamBaYeEng(void *p);
//void GamBaYeEng(void);


U8 NewTCPClient();

U8 TCPisConnected();

U8 TCPWriteData(char *senddata,int dataLen);
U8 TCPReadData(char * rcvdata,int dataLen);

int TCPconnectToServer();
int TCPdisconnectServer();

U8 TCPgetHostIpAddress(char *outip);

char * NewUUID(char * buffer);
void SetBrush(U8 type,U8 trans);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // GAMEVIEW_H
