#include "tcpdatastruct.h"
#include <string.h>
#include <time.h>
#if defined(Q_OS_IOS)
//#include <sys/malloc.h>
#else
#include <malloc.h>
#endif

UL ServerGetTransLen(US);
UL ClientGetTransLen(US);

Player g_player;
UL g_clientMsgid;

UT ClientSndMsg(US transType,unsigned char * data,int way)
{
    Tcphead tcphead;
    char senddata[65535];

    tcphead.DataLen = ClientGetTransLen(transType);
    tcphead.TranType = transType;
    memcpy(tcphead.RecvID,"00000000000000000000000000000000",32);
    memcpy(tcphead.SendID,g_player.UUID,(int)32);
    tcphead.Status = way;
    tcphead.MessageID = g_clientMsgid;
    g_clientMsgid ++;
    PackMsg(tcphead,data,senddata);
    SendTcpMsg(senddata,tcphead.DataLen+TCPHEADLEN);
    return OK;
}

//返回0说明目前未收到无此类消息，仅限way=0服务端发起的消息
UT ClientRcvMsg(US transType,unsigned char * data,int way)
{
    Tcphead tcphead ;

    char rcvdata[65535];
    memset(rcvdata,0,sizeof(rcvdata));
    int ret = TCPReadData(rcvdata,ServerGetTransLen(transType)+TCPHEADLEN);

    if(ret == 0 && way == 0)
    {
        return 0;//无消息
    }

    DisPackMsg(rcvdata,&tcphead,data);
    /*switch(tcphead.TranType)
    {
    case TRANS_ACCOUNT:
        break;
    }*/
    if(tcphead.TranType == transType)
    {
        //memcpy(data,rcvdata,ClientGetTransLen(transType));
        return 1;
    }
    else//收到的不是期望的消息？？
    {
        //MsgInQueue(tcphead.TranType,data);
    }
    return 0;

}


UT DisPackMsg(char * rcvdata,Tcphead * tcphead,char * data)
{
    char headdata[TCPHEADLEN+1];
    char tmp[4+1];
    memset(headdata,0,sizeof(headdata));
    memcpy(headdata,rcvdata,TCPHEADLEN);
    memcpy(data,rcvdata+TCPHEADLEN,TCPBUFFMAXLEN);

    memcpy(tcphead->TransFeature,headdata,4);
    memcpy(&tcphead->Version,headdata+4,1);
    memcpy(&tcphead->Version,headdata+5,1);
    memcpy(&tcphead->TranType,headdata+6,2);
    memcpy(tcphead->SendID,headdata+8,32);
    memcpy(tcphead->RecvID,headdata+40,32);
    memcpy(&tcphead->MessageID,headdata+72,4);
    memcpy(&tcphead->Encry,headdata+76,1);
    memcpy(&tcphead->RptFlag,headdata+77,1);
    memcpy(&tcphead->DataLen,headdata+78,4);
    memcpy(&tcphead->CompressFlag,headdata+82,1);

    return OK;
}

UT PackMsg(Tcphead tcphead,char * data,char * senddata)
{
    //unsigned char senddata[tcphead.DataLen+TCPHEADLEN+1];
    memset(senddata,0,sizeof(senddata));

    strcpy(tcphead.TransFeature,"3gby");
    tcphead.Version = 100;
    tcphead.Encry = 0;
    tcphead.RptFlag = 0;
    tcphead.CompressFlag = 0;

    memcpy(senddata,&tcphead,TCPHEADLEN);
    memcpy(senddata+TCPHEADLEN,data,tcphead.DataLen);


    return OK;
}

UT SendTcpMsg(char * senddata,int dataLen)
{
    if(TCPisConnected()== OK)
    {
        TCPWriteData(senddata,dataLen);
    }
    else
    {
        int ret = TCPconnectToServer();
        if(ret == OK)return OK;
        if(ShareHand()==OK)
        {
            TCPWriteData(senddata,dataLen);
        }
    }
    return OK;
}

UT ShareHand()
{
    //1.		IP地址	IPAdress	C	15	校验用
    //2.		连接日期时间	ConnectTime	UL	4
    //3.		客户端版本	Version	UL	4	客户端版本号
    //4.		用户名	UserName	C	30
    //5.		密码	PassWord	C	30
    //6.		UUID	UUID	C	32
    //7.		保留	Reserve	C	13

    char senddata[65535];

    C data[128+1];
    memset(data,0,sizeof(data));

    char IPAdress[15+1];
    UL ConnectTime;
    UL Version = 400;

    time_t t;
    t = time(NULL);

    TCPgetHostIpAddress(IPAdress);
    ConnectTime = time(&t);

    memcpy(data,IPAdress,15);
    memcpy(data+15,&ConnectTime,4);
    memcpy(data+19,&Version,4);
    memcpy(data+23,g_player.playerName,30);
    memcpy(data+53,g_player.password,64);
    //char uuid[32+1];
    //memcpy(g_player.UUID,NewUUID(uuid),32);
    //memcpy(data+83,g_player.UUID,32);

    Tcphead tcphead;

    tcphead.DataLen = ClientGetTransLen(0x0010);
    tcphead.TranType = 0x0010;
    memcpy(tcphead.RecvID,"00000000000000000000000000000000",32);
    memcpy(tcphead.SendID,g_player.UUID,32);
    tcphead.Status = 0;
    tcphead.MessageID = g_clientMsgid;
    g_clientMsgid ++;
    PackMsg(tcphead,data,senddata);
    SendTcpMsg(senddata,tcphead.DataLen+TCPHEADLEN);

    return OK;
}

UT Logout()
{
    //1.		UUID	UUID	C	32


    char senddata[65535];

    C data[48+1];
    memset(data,0,sizeof(data));
    memcpy(data,g_player.UUID,32);

    Tcphead tcphead;

    tcphead.DataLen = ClientGetTransLen(0x0020);
    tcphead.TranType = 0x0020;
    memcpy(tcphead.RecvID,"00000000000000000000000000000000",32);
    memcpy(tcphead.SendID,g_player.UUID,32);
    tcphead.Status = 0;
    tcphead.MessageID = g_clientMsgid;
    g_clientMsgid ++;
    PackMsg(tcphead,data,senddata);
    SendTcpMsg(senddata,tcphead.DataLen+TCPHEADLEN);
    return OK;
}

UT UploadSave(char * filedata)
{
    char senddata[65535];

    C data[17872+1];
    memset(data,0,sizeof(data));

    memcpy(data,g_player.UUID,32);

    memcpy(data+32,filedata,17859);

    Tcphead tcphead;

    tcphead.DataLen = ClientGetTransLen(0x0001);
    tcphead.TranType = 0x0001;
    memcpy(tcphead.RecvID,"00000000000000000000000000000000",32);
    memcpy(tcphead.SendID,g_player.UUID,32);
    tcphead.Status = 0;
    tcphead.MessageID = g_clientMsgid;
    g_clientMsgid ++;
    PackMsg(tcphead,data,senddata);
    SendTcpMsg(senddata,tcphead.DataLen+TCPHEADLEN);

    return OK;
}
UT DownloadSave()
{
    char senddata[65535];

    C data[32+1];
    memset(data,0,sizeof(data));


    memcpy(data,g_player.UUID,32);

    Tcphead tcphead;

    tcphead.DataLen = ClientGetTransLen(0x0002);
    tcphead.TranType = 0x0002;
    memcpy(tcphead.RecvID,"00000000000000000000000000000000",32);
    memcpy(tcphead.SendID,g_player.UUID,32);
    tcphead.Status = 0;
    tcphead.MessageID = g_clientMsgid;
    g_clientMsgid ++;
    PackMsg(tcphead,data,senddata);
    SendTcpMsg(senddata,tcphead.DataLen+TCPHEADLEN);

    return OK;
}


UL ServerGetTransLen(US transType)
{
    switch (transType)
    {
    case TRANS_SHAREHAND    :return S_SHAREHAND_LEN    ;
    case TRANS_LOGOUT       :return S_LOGOUT_LEN        ;
    case TRANS_UPLOADSAV    :return S_UPLOADSAV_LEN      ;
    case TRANS_DOWNLOADSAV  :return S_DOWNLOADSAV_LEN      ;

    }
}

UL ClientGetTransLen(US transType)
{
    switch (transType)
    {
    case TRANS_SHAREHAND    :return C_SHAREHAND_LEN    ;
    case TRANS_LOGOUT       :return C_LOGOUT_LEN        ;
    case TRANS_UPLOADSAV    :return C_UPLOADSAV_LEN      ;
    case TRANS_DOWNLOADSAV  :return C_DOWNLOADSAV_LEN      ;

    }
}

UT chartohex(C *instr, C *outchar)
{
    int size;
    int idx,idx1,idx2;
    char tmp[3];
    int tmpint;
    int i;
    int longtmp;

    size = strlen(instr);
    if(size<2)
        return(1);

    idx1 = size/2-1;
    idx2 = 0;


    for(idx=0; idx<size; idx+=2)
    {
        memset(tmp, 0, sizeof tmp);
        strncpy(tmp, instr+idx, 2);
        sscanf(tmp, "%d", &tmpint);
        *(outchar+((size - idx)/2 - 1)) = tmpint;
        idx2++;
    }


    printf("outchar=[%02x][%02x][%02x][%02x]\n", outchar[0],outchar[1],outchar[2],outchar[3]);

    return 0 ;
}

UT hextochar(C * inbuf, C * outbuf)
{
    int tmplong;
    int idx,idx1;
    int size;

    size = strlen(inbuf);
    idx1=0;

    for(idx=0; idx<size; idx++)
    {
        sprintf(outbuf+idx1, "%02d",*(inbuf+idx));
        idx1+=2;
    }

    return 0 ;
}

char* StrSHA256(const char* str, int length, char* sha256)
{
    char *pp, *ppend;
    long l, i, W[64], T1, T2, A, B, C, D, E, F, G, H, H0, H1, H2, H3, H4, H5, H6, H7;
    H0 = 0x6a09e667, H1 = 0xbb67ae85, H2 = 0x3c6ef372, H3 = 0xa54ff53a;
    H4 = 0x510e527f, H5 = 0x9b05688c, H6 = 0x1f83d9ab, H7 = 0x5be0cd19;
    long K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
    };
    l = length + ((length % 64 > 56) ? (128 - length % 64) : (64 - length % 64));
    if (!(pp = (char*)malloc((unsigned long)l))) return 0;
    for (i = 0; i < length; pp[i + 3 - 2 * (i % 4)] = str[i], i++);
    for (pp[i + 3 - 2 * (i % 4)] = 128, i++; i < l; pp[i + 3 - 2 * (i % 4)] = 0, i++);
    *((long*)(pp + l - 4)) = length << 3;
    *((long*)(pp + l - 8)) = length >> 29;
    for (ppend = pp + l; pp < ppend; pp += 64){
        for (i = 0; i < 16; W[i] = ((long*)pp)[i], i++);
        for (i = 16; i < 64; W[i] = (SHA256_O1(W[i - 2]) + W[i - 7] + SHA256_O0(W[i - 15]) + W[i - 16]), i++);
        A = H0, B = H1, C = H2, D = H3, E = H4, F = H5, G = H6, H = H7;
        for (i = 0; i < 64; i++){
            T1 = H + SHA256_E1(E) + SHA256_Ch(E, F, G) + K[i] + W[i];
            T2 = SHA256_E0(A) + SHA256_Maj(A, B, C);
            H = G, G = F, F = E, E = D + T1, D = C, C = B, B = A, A = T1 + T2;
        }
        H0 += A, H1 += B, H2 += C, H3 += D, H4 += E, H5 += F, H6 += G, H7 += H;
    }
    free(pp - l);
    sprintf(sha256, "%08x%08x%08x%08x%08x%08x%08x%08x", H0, H1, H2, H3, H4, H5, H6, H7);
    return sha256;
}

