/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	comOut.c
*文件标识：	步步高电子词典的游戏引擎模块
*摘要：		游戏数据管理模块
**
*移植性声明:
*	1、符合标准：《游戏设计标准V1.0》
*	2、兼容模式：本程序和界面无关，无兼容模式。
*修改历史：
*	版本    日期     作者     改动内容和原因
*   ----------------------------------------------------
*	1.0    2005.5.16  高国军     基本的功能完成
***********************************************************************/
#include "stdsys.h"
#include "comm.h"
#include "enghead.h"

U8 libByte[300000+1];
U8 fontByte[163840+1];
U8 rlbByte[50000000+1];

U8 VS_PTR[15360+1];
U8 SHARE_MEM[8000000+1];//2097152+1];
U8 FIGHTERS_IDX[30+1];
U8 FIGHTERS[300+1];
U8 ORDERQUEUE[1200+1];

//全局按键码变量
int g_keyCode;
int g_keyCodeList[100];
int g_keyCodeListMax = 0;
int g_timerTick;
//图片模式
int g_showPicType = 0;//0-原版黑白 1-彩色 2-灰度
int g_stat;//用于选择的值(触屏用)

int libReaded = 0;
int rlbReaded = 0;

/*ng=0//choiceshow
ng=1//新游戏选择历史时期界面
ng=2//读档界面
ng=3//读入存档
nga=1//选择君主界面
ng=10 关于界面
ngb=1 ngsave=0 ngc=0主地图界面
flagwar = 1 战斗
flagshowskill= 1战斗技能菜单
flagmenuhero=1 hero菜单
flagmenushow=1
flagmenuspeed=1
flagmenumov=1
flagmenu=1战斗主菜单
ngc=1  escmenu
ngc=2 ngsecond1=0 citymenu
ngc=3 显示城市状态
ngsave=1 存档界面
ngc=2 ngsecond1=1 ngthird1 == 0 内政菜单
ngc=2 ngsecond1=2 ngthird1 == 0 外交菜单
ngc=2 ngsecond1=3 ngthird1 == 0 军备菜单
ngthird1 >0 武将列表
flagstat 战斗状态显示*/


int ng=0;//用于状态(触屏用)
int nga=0;
int ngb=0;
int ngc=0;
int ngsave=0;
int ngsecond1=0;
int ngthird1=0;
int flagwar=0;
int flagstat=0;
int ngthird2=0;
int ngthird3=0;
int ngthird4=0;
int ngf=0;//flagshowskill;//战斗技能菜单ngf2
int operateflag=0;
//int flagmenuhero;//hero菜单ngf3
//int flagmenushow;//=1ngf4
//int flagmenuspeed;//=1ngf5
//int flagmenumov;//=1ngf6
//int flagmenu;//=1战斗主菜单ngf1
/*本体函数声明*/

int g_ResId;
int g_PicId;
int g_PicStyle;

extern int g_StoredResId[1500];
extern int g_StoredPicId[1500];
extern int g_StoredPicStyle[1500];
extern int g_StoredMax;

/*------------------------------------------*/
U32	GetResStartAddr(U16 id);
U16	GetResItem(U32 addr,U8 idx,RCHEAD *reshead,RIDX *rIdx);
void	ExpDataWithKey(U8 *ptr,U8 key,U16 len);
U32	GetResStartAddrRlb(U16 id);
/***********************************************************************
 * 说明:     获取指定资源项的数据长度
 * 输入参数: ResId-资源id	idx-目标在资源队列中的序号
 * 返回值  : 资源项的数据长度
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
 U16 ResGetItemLen(U16 ResId,U8 idx)
{
	U32	addr;
	RIDX	rIdx;
	RCHEAD	reshead;
	
	addr=GetResStartAddr(ResId);
	if(addr==0)
		return 0;
	return GetResItem(addr,idx,&reshead,&rIdx);
}
/***********************************************************************
 * 说明:     将指定的资源载入到内存中
 * 输入参数: ResId-资源id	idx-目标在资源队列中的序号
 *	   : ptr-内存指针
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
  U8 ResLoadToMem(U16 ResId,U8 idx,U8 *ptr)
  {
      if(g_showPicType == 0)
      {
         /*if(ResId == IFACE_STRID||ResId == IFACE_CONID)
         {
             ResStringLoadToMem(ResId,idx,ptr);
         }
         else*/
         {
             ResLoadToMemOld(ResId,(U8)idx,ptr);
         }
      }
      else //if(g_showPicType == 1)
      {
          /*if(ResId == IFACE_STRID||ResId == IFACE_CONID)
          {
              ResStringLoadToMem(ResId,idx,ptr);
          }
          else*/
          {
              ResLoadToMemOld(ResId,(U8)idx,ptr);
          }

      }

  }

 /* U8 ResStringLoadToMem(U16 ResId,const U8 *idx,U8 *ptr)
 {
    memcpy((char *)ptr,(char *)idx,g_tlen);
     return 0;
 }*/


  U8 ResLoadToMemNew(U16 ResId,U8 idx,U8 *ptr)//用于加载qrc图片
  {
     //char resname[100+1];
     //sprintf(resname,"pic_%03d",(int)ResId);
     //ptr =  (U8*)resname;
      U16	plen;
      U32	addr;
      RIDX	rIdx;
      RCHEAD	reshead;

      addr=GetResStartAddr(ResId);
      if(addr==0)
          return 1;
      GetResItem(addr,idx,&reshead,&rIdx);
      addr+=rIdx.offset;
      plen = rIdx.rlen;
      gam_fseek(g_LibFp,addr,SEEK_SET);
      gam_fread(ptr,1,plen,g_LibFp);
      ptr[plen] = 0;
      if(reshead.ResKey)
          ExpDataWithKey(ptr,reshead.ResKey,plen);

      return 0;
  }

 U8 ResLoadToMemOld(U16 ResId,U8 idx,U8 *ptr)
{
	U16	plen;
	U32	addr;	
	RIDX	rIdx;
	RCHEAD	reshead;	

	addr=GetResStartAddr(ResId);
	if(addr==0)
		return 1;
    GetResItem(addr,idx,&reshead,&rIdx);
    addr+=rIdx.offset;
    plen = rIdx.rlen;
    gam_fseek(g_LibFp,addr,SEEK_SET);
    gam_fread(ptr,1,plen,g_LibFp);
    ptr[plen] = 0;
    if(reshead.ResKey)
        ExpDataWithKey(ptr,reshead.ResKey,plen);

	return 0;
}
/***********************************************************************
 * 说明:     将指定的资源载入到常量页面（没有常量页的系统用堆来模拟）
 * 输入参数: ResId-资源id		idx-目标在资源队列中的序号
 * 返回值  : 资源被载入后的指针(NULL为载入失败)
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
 U8 *ResLoadToCon(U16 ResId,U8 idx,U8 *cbnk)
 {
     if(g_showPicType == 0)
     {
        /*if(ResId == IFACE_STRID||ResId == IFACE_CONID)
        {
            return ResLoadToCon(ResId,idx,cbnk);
        }
        else*/
        {
            return ResLoadToConOld(ResId,(U8)idx,cbnk);
        }
     }
     else //if(g_showPicType == 1)
     {
        /*if(ResId == IFACE_STRID||ResId == IFACE_CONID)
        {
            return ResLoadToCon(ResId,idx,cbnk);
        }
        else*/
        {
            //61		/* 武将资源 */
//62		/* 武将名称一 */
//70		/* 武将名称二 */
//71		/* 武将名称三 */
//72		/* 武将名称四 */
//57		/* 城市资源 */
//58		/* 城市名称 */
//73		/* 道具名称 */
//66		/* 道具资源 */
//GENERAL_CON		63		/* 武将出现条件 */
//GOODS_CON		67		/* 道具出现条件 */
            if(ResId == 1 ||ResId == 2 ||ResId == 10 ||ResId == 11 ||ResId == 12
                    ||ResId == 13 ||ResId == 14 ||ResId == 57 ||ResId == 58 ||ResId == 59
                    ||ResId == 60 ||ResId == 61 ||ResId == 62 ||ResId == 63 ||ResId == 64
                    ||ResId == 65 ||ResId == 66 ||ResId == 67 ||ResId == 68 ||ResId == 70
                    ||ResId == 71 ||ResId == 72 ||ResId == 73 ||ResId == 74 ||ResId == 80
                    ||ResId == 110 ||ResId == 111 ||ResId == 112 ||ResId == 113
                    ||ResId == 114 ||ResId == 115 ||ResId == 116 )
            {
                return ResLoadToConOld(ResId,(U8)idx,cbnk);
            }
            else
            {

                return ResLoadToConNew(ResId,(U8)idx,g_CBnkPtr2);//cbnk);
            }
        }
     }

 }

 /*U8 *ResConstLoadToCon(U16 ResId,const U8 *idx,U8 *cbnk)
{

   //strcpy((char *)cbnk,(char *)idx);
    return (U8 *)idx;
}*/

 U8 *ResLoadToConNew(U16 ResId,U8 idx,U8 *cbnk)//用于加载qrc图片
 {
    // resname[100+1];
    //sprintf(resname,"pic_%03d",(int)ResId);
    //return (U8*)resname;
     U8 * ptr;
     U16	tmp;
     U32	addr;
     RCHEAD	*reshead;

     if(!idx)
         return (U8 *) NULL;
     idx-=1;
     //DebugTime(41);
     addr=GetResStartAddrRlb(ResId);
     if(addr==0)
         return (U8 *) NULL;
     //DebugTime(42);
     //20180226记录资源序号
     g_ResId = ResId;
     g_PicId = idx;

     if(rlbReaded == 0)
     {
        ptr=gam_fload(cbnk,addr,g_RlbFp);
        rlbReaded = 1;
     }
     else
     {
        ptr=gam_fload_mem(cbnk,addr,g_RlbFp);
     }
     //DebugTime(43);
     reshead=(RCHEAD	*)ptr;
     if(reshead->ItmCnt<idx || reshead->ResKey!=0)
         return (U8 *) NULL;
     if(reshead->ItmLen!=0)
     {
         ptr+=sizeof(RCHEAD);
         ptr+=idx*reshead->ItmLen;
     }
     else
     {
         if(reshead->ItmCnt==1)
             ptr+=sizeof(RCHEAD);
         else
         {
             tmp=idx;
             tmp<<=2;
             tmp+=sizeof(RCHEAD);
             ptr+=(U16) *((U16*)(ptr+tmp));
         }
     }
     //DebugTime(44);
     return ptr;
 }

 U8 *ResLoadToConOld(U16 ResId,U8 idx,U8 *cbnk)
{
    U8 * ptr;
	U16	tmp;
	U32	addr;
	RCHEAD	*reshead;
	
	if(!idx)
		return (U8 *) NULL;
	idx-=1;
	addr=GetResStartAddr(ResId);
	if(addr==0)
		return (U8 *) NULL;

    //20180226记录资源序号
    g_ResId = ResId;
    g_PicId = idx;

    //20180316检测是否存在资源，如果已经存在，不再进行重复load
    /*int picStored = 0;
    for(int t_i = 0; t_i < g_StoredMax ; t_i ++)
    {
        if(g_ResId == g_StoredResId[t_i] && g_PicId == g_StoredPicId[t_i] && g_PicStyle == g_StoredPicStyle[t_i])
        {
            picStored = t_i;
            break;
        }
    }
    if(picStored>0)
    {
        return NULL;
    }*/
    if(libReaded == 0)
    {
       ptr=gam_fload(cbnk,addr,g_LibFp);
       libReaded = 1;
    }
    else
    {
       ptr=gam_fload_mem(cbnk,addr,g_LibFp);
    }

	reshead=(RCHEAD	*)ptr;
	if(reshead->ItmCnt<idx || reshead->ResKey!=0)
		return (U8 *) NULL;
	if(reshead->ItmLen!=0)
	{
        ptr+=sizeof(RCHEAD);
        ptr+=idx*reshead->ItmLen;
	}
	else
	{
		if(reshead->ItmCnt==1)
            ptr+=sizeof(RCHEAD);
		else
		{
			tmp=idx;
			tmp<<=2;
			tmp+=sizeof(RCHEAD);
            ptr+=(U16) *((U16*)(ptr+tmp));
		}
	}
    return ptr;
}

/***********************************************************************
 * 说明:     将被加密的数据解密
 * 输入参数: id-资源id
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
void ExpDataWithKey(U8 *ptr,U8 key,U16 len)
{
	U16	i;
	for(i=0;i<len;i++)
		ptr[i]-=key;
}
/***********************************************************************
 * 说明:     获取指定的资源项
 * 输入参数: addr-资源起始地址	idx-目标在资源队列中的序号
 *	   : reshead-资源头	rIdx-资源项索引
 * 返回值  : 资源项数据长度
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
U16 GetResItem(U32 addr,U8 idx,RCHEAD *reshead,RIDX *rIdx)
{	
    gam_fseek(g_LibFp,addr,SEEK_SET);
    gam_fread((U8*)reshead,sizeof(RCHEAD),1,g_LibFp);
    if(reshead->ItmLen!=0)
	{
		rIdx->offset = idx - 1;
		rIdx->offset *= reshead->ItmLen;
		rIdx->offset += sizeof(RCHEAD);
		rIdx->rlen=(U16)(reshead->ItmLen);
	}
	else
	{
        addr=idx-1;
        addr<<=2;
        gam_fseek(g_LibFp,addr,SEEK_CUR);
        gam_fread((U8 *)rIdx,sizeof(RIDX),1,g_LibFp);
	}
	return rIdx->rlen;
}
/***********************************************************************
 * 说明:     获取指定资源的数据起始地址
 * 输入参数: id-资源id
 * 返回值  : 资源地址
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
U32 GetResStartAddr(U16 id)
{
	U32	addr;
	
	if(id==0) return (U32)0;
	
	/* 获取资源的索引地址 addr=(id-1)*4 */
	addr=id-1;
	addr<<=2;

    /* 获取资源索引（实际就是资源存储的数据地址）*/
	gam_fseek(g_LibFp,addr,SEEK_SET);
	gam_fread((U8*)&addr,4,1,g_LibFp);
    return addr;
}

U32 GetResStartAddrRlb(U16 id)
{
    U32	addr;

    if(id==0) return (U32)0;

    /* 获取资源的索引地址 addr=(id-1)*4 */
    addr=id-1;
    addr<<=2;

    /* 获取资源索引（实际就是资源存储的数据地址）*/
    gam_fseek(g_RlbFp,addr,SEEK_SET);
    gam_fread((U8*)&addr,4,1,g_RlbFp);
    return addr;
}
