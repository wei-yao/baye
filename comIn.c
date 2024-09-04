/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	comIn.c
*文件标识：	步步高电子词典的游戏引擎模块
*摘要：		封装系统输入及其它和显示无关的功能函数
**
*移植性声明:
*	1、符合标准：《游戏设计标准V1.0》
*	2、兼容模式：本程序和界面无关，无兼容模式。
*	3、使用本封装程序的引擎要移植到其他环境中，系统的输入部分只需在此修改
*修改历史：
*	版本    日期     作者     改动内容和原因
*   ----------------------------------------------------
*	1.0    2005.5.16  高国军     基本的功能完成
***********************************************************************/
/* 声明本文件可以直接调用系统底层函数宏 */
#define		_BE_CHANGED_
#include "stdsys.h"
#include "comm.h"
#include "enghead.h"
//#include <unistd.h>
//#include <synchapi.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

/* 当前所在文件 */
#define		IN_FILE		20

extern U8 libByte[300000+1];
extern U8 fontByte[163840+1];
extern U8 rlbByte[50000000+1];

extern int g_keyCode;
extern int g_keyCodeList[100];
extern int g_keyCodeListMax;
extern int g_timerTick;
extern int g_delayTimes;
extern U8 g_modName[20+1];
extern int g_modVer;
extern U8 g_startName_char[200];
int g_modAIHighAttack;
int g_modAIHeroUpLevel;
int g_modAIAttEmptyCity;
int g_modsupportColor = 0;
extern int titleUpdate;
/***********************************************************************
 * 说明:     初始化游戏引擎所在的机型环境
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 U8 GamConInit(void)
{
    U8	tmpbuf[200];
    memset(tmpbuf,0,sizeof (tmpbuf));
    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //timer->start(TIMER_INT);


    /*文件指针初始化*/
	gam_memcpy(tmpbuf,GAM_FONT_FNAME,10);
    g_FontFp = gam_fopen(tmpbuf,'r');			/*打开字库文件*/
	if(NULL == g_FontFp)
		return false;

    //20190502mod
    if(g_useMOD == 0)
    {
        gam_memcpy(tmpbuf,GAM_LIB_FNAME,10);

        g_LibFp = gam_fopen(tmpbuf,'r');			/*打开资源库文件*/
        if(NULL == g_LibFp)
            return false;
        //g_CBnkPtr = (U8 *)0x9000;				/*常量页面指针*/
        g_CBnkPtr = (U8 *)libByte;				//libByte

        gam_memcpy(tmpbuf,GAM_RLB_FNAME,10);


        g_RlbFp = gam_fopen(tmpbuf,'r');			/*打开资源库文件*/
        if(NULL == g_RlbFp)
            return false;
        g_CBnkPtr2 = (U8 *)rlbByte;
    }
    else
    {
        gam_memcpy(tmpbuf,g_startName_char,200);
        g_ModFp = gam_fopen(tmpbuf,'r');

        if(NULL == g_ModFp)
            return false;
        //g_RlbFp = g_LibFp;
        g_CBnkPtr = (U8 *)libByte;
        g_CBnkPtr2 = (U8 *)rlbByte;
    }


    /*随机函数初始化*/
    //U8 i;
    //i = SysGetSecond();
    //gam_srand(i);						/*初始化随机数产生队列*/

    /*汉字显示初始化*/
    //GetDataBankNumber(9,&c_FontBnkS);			/*获取字库起始地址*/
	c_ReFlag = true;
	c_Sx = WK_SX;
	c_Sy = WK_SY;
	c_Ex = WK_EX;
	c_Ey = WK_EY;

    /*虚拟屏幕缓冲指针*/
	g_VisScr = VS_PTR;
	g_BVisScr = BVS_PTR;
    gam_memset(g_VisScr,0,WK_BLEN);
	
    if(g_useMOD == 1)
    {
        ModAddonReplace();
    }
	return 0;
}
/***********************************************************************
 * 说明:     恢复系统参数
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 U8 GamConRst(void)
{

}
/***********************************************************************
 * 说明:     获取系统消息，并转换为游戏的消息体制
 * 输入参数: pMsg-消息指针
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 U8 GamGetMsg(GMType *pMsg)
{
    #if GAM_VER==GAM_DEBUG_MODE
	if(g_RunErr!=NONE_ERR)
		gamTraceP(0xFF000000+g_RunErr);
	#endif	
    g_timerTick = 0;

	while(1)
	{
        if(g_keyCode >0)
        {

            pMsg->type = VM_CHAR_FUN;
            pMsg->param = g_keyCode;

            if(g_keyCodeListMax>0)
            {
                g_keyCode = g_keyCodeList[0];
                int i = 0;
                for(i = 0 ; i < g_keyCodeListMax ; i ++)
                {
                    g_keyCodeList[i]= g_keyCodeList[i+1];
                }
                g_keyCodeListMax --;

            }
            else
            {
                g_keyCode = 0;
            }
        //if(!GuiGetMsg((PtrMsg)pMsg))		continue;
        //if(!GuiTranslateMsg((PtrMsg)pMsg))	continue;
            break;
        }
        else if(g_timerTick == 1)
        {
            pMsg->type = VM_TIMER;
            pMsg->param = 0;
            g_timerTick = 0;
            break;
        }
        usleep(100001);
        //Sleep(10);
    }
}
/***********************************************************************
 * 说明:     延时程序
 * 输入参数: dly-延时秒(0-有键按下时才返回)	keyflag-延时过程中按键可否返回
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 U8 GamDelay(U16 dly,bool keyflag)
{
    //usleep (dly*10000);
    U8	tInt;
	GMType	pMsg;
    int i_dly = dly;

    //tInt = SysGetTimer1Number();
    //SysTimer1Close();


    if(i_dly == 0)
		GamGetMsg(&pMsg);
	else
	{
        //20180725修正
        if(keyflag)
        {
            i_dly=  i_dly * g_delayTimes / 100;
            if(i_dly== 0)i_dly = 1;
        }
        //SysTimer1Open(TIMER_DLY);
		while(1)
		{
            if(i_dly == 0) break;


            if(keyflag)
            {
                int looptmp = 0;
                while(1)
                {
                    if(g_keyCode >0 && i_dly > 2)
                    {

                        g_keyCode = 0;
                        return 0;

                    }
                    looptmp ++;
                    if(looptmp> 10)break;
                    usleep (1000);
                    //Sleep(1);
                }

            }
            else
            {
                if(g_keyCode >0 && i_dly > 2)
                {
                    g_keyCode = 0;
                }
               usleep (10000);
                //Sleep(10);
            }
            i_dly -= 1;
			pMsg.param = 0;
		}
	}
    //if(tInt)
    //{
    //	SysTimer1Close();
    //	SysTimer1Open(tInt);
    //}
    return (U8)pMsg.param;
}

 void wlogfile(char *str)
 {
     //FILE *fp = NULL;

      //  fp = fopen("D:\\2.txt", "w+");
      //  fprintf(fp, str);
      //  fclose(fp);
 }

 void ModAddonReplace()
 {
     U8 *ptr;	/*inf -- 城市连接信息*/

     U8	modVerA;
     U8	modVerB;
     U8	modVerC;
     U8	modVerId;
     //modName
     ptr = ResLoadToCon(MODADDON,1,g_CBnkPtr);
     gam_memcpy(g_modName,ptr,20);
     ptr +=20;

     // modverA,modVerB,modVerC,modVerId;
     modVerA = (U8) *ptr;
     ptr ++;
     modVerB = (U8) *ptr;
     ptr ++;
     modVerC = (U8) *ptr;
     ptr ++;
     modVerId = (U8) *ptr;
     ptr ++;
     g_modVer = (int)(modVerA)*100 +(int)(modVerB)*10+(int)(modVerC);


     //periodname
     ptr += 32;

     //modaiconfig
     g_modAIHighAttack = (U8) *ptr;
     ptr ++;
     g_modAIHeroUpLevel = (U8) *ptr;
     ptr ++;
     g_modAIAttEmptyCity = (U8) *ptr;
     ptr ++;

     g_modsupportColor = 0;
     g_modsupportColor = (U8) *ptr;//支持彩色版

     titleUpdate = 0;

     int editVer = (U8) (*(ptr + 76 + 108 +38 + 150));//使用修改器版本
     if(editVer == 0)
     {

         return;
     }
     else
     {
         return;
     }


     //dCityPos
     gam_memcpy((U8 *) dCityPos,ptr,76);
     ptr += 76;

     //C_MAP
     gam_memcpy((U8 *) C_MAP,ptr,108);
     ptr += 108;

     //dCityMapId
     gam_memcpy((U8 *) dCityMapId,ptr,38);
     ptr += 38;
     //dFgtAtRange
     gam_memcpy((U8 *) dFgtAtRange,ptr,150);
     ptr += 150;
     //备用

     ptr += 46*6;
     //dFgtLandF
     gam_memcpy((U8 *) dFgtLandF,ptr,48);
     ptr += 48;


 }
