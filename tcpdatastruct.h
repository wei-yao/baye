#ifndef TCPDATASTRUCT_H
#define TCPDATASTRUCT_H

#ifdef __cplusplus

extern "C" {
#endif /* __cplusplus */

#include "gameview.h"

#pragma pack(1)

#define TRANS_SHAREHAND     0x0010
#define TRANS_LOGOUT        0x0020
#define TRANS_UPLOADSAV     0x0001
#define TRANS_DOWNLOADSAV   0x0002


#define TCPHEADLEN   96
#define TCPBUFFMAXLEN 65536

#define S_SHAREHAND_LEN     48
#define S_LOGOUT_LEN        16
#define S_UPLOADSAV_LEN     16
#define S_DOWNLOADSAV_LEN   17872

#define C_SHAREHAND_LEN     128
#define C_LOGOUT_LEN        48
#define C_UPLOADSAV_LEN     17872
#define C_DOWNLOADSAV_LEN   32

#define OK          0
#define TCPERROR    1
#define FILEERROR   2
#define DATAERROR   3
#define OTHERERROR  4


#define SHA256_ROTL(a,b) (((a>>(32-b))&(0x7fffffff>>(31-b)))|(a<<b))
#define SHA256_SR(a,b) ((a>>b)&(0x7fffffff>>(b-1)))
#define SHA256_Ch(x,y,z) ((x&y)^((~x)&z))
#define SHA256_Maj(x,y,z) ((x&y)^(x&z)^(y&z))
#define SHA256_E0(x) (SHA256_ROTL(x,30)^SHA256_ROTL(x,19)^SHA256_ROTL(x,10))
#define SHA256_E1(x) (SHA256_ROTL(x,26)^SHA256_ROTL(x,21)^SHA256_ROTL(x,7))
#define SHA256_O0(x) (SHA256_ROTL(x,25)^SHA256_ROTL(x,14)^SHA256_SR(x,3))
#define SHA256_O1(x) (SHA256_ROTL(x,15)^SHA256_ROTL(x,13)^SHA256_SR(x,10))



typedef unsigned char C;
typedef unsigned char UT;
typedef unsigned short US;
typedef unsigned int UL;

typedef struct _Tcphead{//传输自定义数据包头结构
    C TransFeature[4];//交易特征码 4
    UT Version;        //版本号 1
    UT Status;   //握手状态
    US TranType;//消息类型
    C SendID[32];//发送方编码
    C RecvID[32];//接收方编码
    UL MessageID;//报文序号
    UT Encry;//加密方式
    UT RptFlag;//重发标志
    UL DataLen;//数据长度
    UT CompressFlag;//报文体压缩标志
    C Reserve[13];//保留
}Tcphead;

typedef struct _User{
    C UUID[32+1];
    C playerName[30+1];
    C password[30+1];
    UT kingId;
    UT logined;
}Player;



//C g_UUID[32+1];

//C g_userName[30+1];
//C g_userPass[30+1];

UT ClientSndMsg(US transType,unsigned char * data,int way);
UT ClientRcvMsg(US transType,unsigned char * data,int way);

UT GetUUID(UT kingId,char * UUID);
UT SetUUID(char * player,char * UUID);
UT SetKingId(char * player,UT kingId);//扮演君主号

UL ClientGetTransLen(US);

UT PackMsg(Tcphead tcphead,char * data,char * senddata);
UT DisPackMsg(char * rcvdata,Tcphead * tcphead,char * data);
UT SendTcpMsg(char * senddata,int dataLen);

UT ShareHand();//0010
UT Logout();//0020

UT UploadSave(char * filedata);//0001
UT DownloadSave();//0002

UT chartohex(C *instr, C *outchar);
UT hextochar(C *instr, C *outchar);
char* StrSHA256(const char* str, int length, char* sha256);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // TCPDATASTRUCT_H
