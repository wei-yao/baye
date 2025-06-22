/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	Fight.c
*文件标识：	步步高电子词典的游戏引擎模块
*摘要：		游戏引擎战斗模块
**
*移植性声明:
*	1、符合标准：《游戏设计标准V1.0》
*	2、兼容模式：平移兼容|缩放兼容
*修改历史：
*	版本    日期     作者     改动内容和原因
*   ----------------------------------------------------
*	1.0    2005.5.16  高国军     基本的功能完成
***********************************************************************/
#include "stdsys.h"
#include "comm.h"
#undef	Fight
#define	Fight
#include "enghead.h"
#define		IN_FILE	1	/* 当前文件位置 */
/*本体函数声明*/
/*------------------------------------------*/
U8 FgtIntVar(void);
U8 FgtIntMap(void);
void FgtIntScr(void);
void FgtInit(void);
U8 FgtGetBaseXY(U8 sym);
void FgtShowGen(U8 act);
void FgtShowCursor(void);
void FgtDealMan(void);
void FgtDealCmp(void);
U8 FgtGetControl(void);
U8 FgtGenMove(U8 idx);
void FgtAllRight(bool *flag);
U8 FgtGetFoucs(void (*chkcondition)(bool*));
void FgtMoveBack(bool *flag);
void FgtShowState(void);
void FgtShowInf(void);
void FgtGetPCmd(FGTCMD *pcmd);
U8 FgtExeCmd(FGTCMD *pcmd);
void FgtGetMPos(U8 idx,RECT *pRect);
U8 FgtCmdAimGet(U8 type,U8 param,U8 idx);
void FgtShowAtRng(void);
void FgtCmdBack(bool *flag);
bool FgtChkRng(void);
void FgtRPicShowV(U16 id,U8 idx,U8 sx,U8 sy);
void FgtDealBout(void);
void FgtRefrashMap(void);
void FgtDrvState(void);
void FgtShowSNum(U8 sym,U8 idx,U16 num);
void FgtMapUnitShow(U8 tx,U8 ty,U8 flag);
void FgtShowGetExp(U8 exp);
void FgtShowChgSpe(U8 sfrm,U8 efrm,U8 x,U8 y);
void FgtChkAtkEnd(void);
U8 TransIdxToGen2(U8 idx);
U8 FgtLoadToMem(U8 idx,U8 *buf);
U8 *FgtLoadToCon(U16 ResId,U8 idx);
U8 *FgtConstLoadToCon(U16 ResId,U8 idx);
void FgtShowFrame(void);
void FgtStrShowV(U8 x,U8 y,U8 *buf);
void FgtShowMap(U8 x,U8 y);
void FgtGetMapDat(U8 x,U8 y);
void FgtChkEnd(U8 flag);


extern int g_showPicType;
extern int g_PicId;
extern int flagwar;
extern int ngf;
extern int g_reflushflag;


/***********************************************************************
 * 说明:     战斗模块入口函数
 * 输入参数: 无
 * 返回值  : 0
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 U8 GamFight(void)
{
    flagwar = 1;

	FgtInit();
	while(!g_FgtOver)
	{


		FgtDealBout();
		FgtDealMan();
		FgtDealCmp();
		CountProvUse();
	}
	/* 战斗只通过g_FgtOver返回胜败，战后处理由外部完成 */
    flagwar = 0;
	return 0;
}
/***********************************************************************
 * 说明:     初始化战斗
 * 输入参数: way-进攻方向
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtInit(void)
{
	FgtIntVar();
	if(FGT_COMON != g_FgtOver)
		return;
	if(FGT_AUTO == g_FgtParam.Mode)
	{
		FgtCountWon();
		return;
	}
	else
	{
		FgtIntMap();
		FgtIntScr();
	}


	FgtChkAtkEnd();
	FgtChkEnd(false);
	FgtChkEnd(true);

}
/***********************************************************************
 * 说明:     处理玩家的操作
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtDealCmp(void)
{
	U8	pTer;
	FGTCMD	fcmd;
	
	while(!g_FgtOver)
	{
		if(!FgtGetMCmd(&fcmd))
			break;
		pTer = FgtExeCmd(&fcmd);
		if(FGT_DF == g_FgtParam.Mode && TERRAIN_CITY == pTer)
			g_FgtOver = FGT_LOSE;
		else
			FgtChkEnd(true);
	}
}
/***********************************************************************
 * 说明:     处理玩家的操作
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtDealMan(void)
{
	U8	idx,pTer;
	FGTCMD	fcmd;

	for(pTer = 0;pTer < FGT_PLAMAX;pTer += 1)
	{
		if(STATE_SW == g_GenPos[pTer].state)
			continue;
		FgtSetFocus(pTer);			/* 敌人处理完后，将光标移回玩家主将 */
		break;
	}
	while(!g_FgtOver)
	{

		idx = FgtGetControl();
		if (idx == 0xff)
			break;
		if(FgtGenMove(idx))
			continue;
		fcmd.sIdx = idx;
		FgtGetPCmd(&fcmd);
		pTer = FgtExeCmd(&fcmd);
		if(FGT_AT == g_FgtParam.Mode && pTer == TERRAIN_CITY)
			g_FgtOver = FGT_WON;
		else
			FgtChkEnd(false);
	}
}
/***********************************************************************
 * 说明:     玩家生成命令
 * 输入参数: pcmd-命令结构
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtGetPCmd(FGTCMD *pcmd)
{	
	U8	idx,sIdx,aIdx;
	U8	lflag,type,param;
	U8	*buf;
	RECT	pRect;
	
	lflag = true;
	idx = pcmd->sIdx;
	sIdx = TransIdxToGen2(idx);
	type = 0;
	buf = (U8*)gam_malloc(100);
	while(lflag)
	{
		/* 选择命令 */
		FgtShowFrame();
		FgtGetMPos(idx,&pRect);
		FgtLoadToMem(dFgtMnuCmd,buf);
        ngf = 3;
		type = PlcSplMenu(&pRect,type,buf);
		pcmd->type = type;
		param = 0;
		/* 获取命令的目标 */
		switch(type)
		{
		case CMD_LOOK:
			FgtShowHlp();
			param = MNU_EXIT;
			break;
		case CMD_STGM:
			if(g_GenPos[idx].state == STATE_JZ)
			{
				/* 若当前将领为禁咒状态，就直接break */
				param = MNU_EXIT;
				break;
			}
			param = FgtGetJNIdx(idx,&pRect,buf);
			break;
		case CMD_REST:
		case MNU_EXIT:
			gam_free(buf);
            ngf = 0;
			return;
		}
		if(param != MNU_EXIT)
		{
            //FgtShowMap(g_MapSX,g_MapSY);//20180223
            //FgtShowGen(0);//20180223
			FgtGetCmdRng(type,param,idx);
			aIdx = FgtCmdAimGet(type,param,idx);
			if(aIdx != MNU_EXIT)
				lflag = false;
		}
	}
    ngf = 0;
	pcmd->param = param;
	pcmd->aIdx = aIdx;
	gam_free(buf);
}
/***********************************************************************
 * 说明:     获取命令执行的对象
 * 输入参数: type-命令类型	param-命令参数	idx-施展者的脚标
 * 返回值  : 0xFF-退出		其它-将领序号
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtCmdAimGet(U8 type,U8 param,U8 idx)
{
	U8	pidx,same;
	U8	naim[10],err[10];
	
	if(param == 22 || param == 30)		/* 天变和谍报-不需要选择目标 */
		return idx;
	FgtLoadToMem(dFgtNoAim,naim);
	FgtLoadToMem(dFgtErrAim,err);
	FgtShowAtRng();	
	while(1)
	{
        //usleep(1000);
		FgtShowFrame();
		if(FgtGetFoucs(FgtCmdBack) == 0xFF)
		{
			FgtSetFocus(idx);
			return 0xFF;
		}
		
		pidx = FgtGetGenIdx(g_FoucsX,g_FoucsY);
		if(0xFF == pidx)
		{
			FgtStrShowV(STA_XXX,STA_XXY,naim);
			continue;
		}
		if(FgtChkRng())
		{
			same = (idx < FGT_PLAMAX && pidx < FGT_PLAMAX) || (idx > FGT_PLAMAX && pidx > FGT_PLAMAX);
			if(CMD_STGM == type)
			{	/* 若是施展计谋 */				
				if(FgtJNChkAim(param,same,pidx))		/* 且符合施展条件-目标的类型、地形 */
					break;
			}
			else
				if(!same) break;
		}
		FgtStrShowV(STA_XXX,STA_XXY,err);
	}
	return pidx;
}
/***********************************************************************
 * 说明:     选择命令执行目标的背景
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtCmdBack(bool *flag)
{	
	if(*flag)
		FgtShowAtRng();
	else
		FgtAllRight(flag);
}
/***********************************************************************
 * 说明:     显示攻击的目标范围
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtShowAtRng(void)
{
	U8	rng;
	U8	x,y,sx,sy;

	/* g_FgtAtkRng中的数据结构：
	[0]：范围的尺寸
	[1]：范围左上角在地图上的起始列坐标
	[2]：范围左上角在地图上的起始行坐标
	[3]~[max]：范围数据 */

	rng = g_FgtAtkRng[0];
	for(y = 0;y < rng;y += 1)
	{
		sy = y + g_FgtAtkRng[2];
		if(sy < g_MapSY || sy >= g_MapSY + SCR_MAPHGT)
			continue;
		sy = FgtGetScrY(sy);
		for(x = 0;x < rng;x += 1)
		{
			sx = x + g_FgtAtkRng[1];
			if(sx < g_MapSX || sx >= g_MapSX + SCR_MAPWID)
				continue;
			if(g_FgtAtkRng[3 + x + y * rng] != 1)
				continue;
			sx = FgtGetScrX(sx);
			FgtRPicShowV(STEP_PIC,2,sx,sy);
		}
	}
}
/***********************************************************************
 * 说明:     获取菜单显示的起始坐标
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtGetMPos(U8 idx,RECT *pRect)
{
	U8	sx,sy,tmp;
	U8	rsy,rey;

	tmp = (WK_EY - WK_SY - 24) / HZ_HGT;
	if(tmp < 4)
		tmp = 6;
	else
		tmp = 0;
	sx = FgtGetScrX(g_GenPos[idx].x) + TIL_WID + 2;
	if(sx + ASC_WID * SKILL_NAMELEN > WK_EX - tmp)
		sx = sx - TIL_WID - tmp  - ASC_WID * SKILL_NAMELEN;
	pRect->sx = sx;
	pRect->ex = sx + ASC_WID * SKILL_NAMELEN;
	
	if((WK_EY - WK_SY - 24) / HZ_HGT < 4)
	{
		rsy = WK_SY + 4;
		rey = WK_EY - 18;
	}
	else
	{
		sy = FgtGetScrY(g_GenPos[idx].y);
		if((WK_EY - sy - 24) / HZ_HGT < 4)
		{
			sy = WK_EY - 18;
			rey = WK_EY - 18;
			rsy = sy - HZ_HGT * 4;
		}
		else
		{
			sy += 4;
			rsy = sy;
			rey = sy + HZ_HGT * 4;
		}
	}
	pRect->sy = rsy;
	pRect->ey = rey;
}
/***********************************************************************
 * 说明:     执行玩家生成的命令
 * 输入参数: pcmd-执行命令结构
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtExeCmd(FGTCMD *pcmd)
{
	U8	idx,exp;
	
	idx = pcmd->sIdx;
	exp = FgtDrvCmd(pcmd);
	if(exp)
	{
		FgtShowMap(g_MapSX,g_MapSY);
		FgtShowGen(0);
		*g_GenAtt[0].exp += exp;
		if(idx < FGT_PLAMAX)
			FgtShowGetExp(exp);
		FgtChkAtkEnd();
	}
	return (FgtGetTerrain(g_GenPos[idx].x,g_GenPos[idx].y));
}
/***********************************************************************
 * 说明:     获取将领移动目标位置
 * 输入参数: idx-被移动将领序号
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtGenMove(U8 idx)
{
	U8	x,y;
	U8	buf[20];
	JLPOS	*pos;

	pos = &g_GenPos[idx];
	if(NO_MOV == pos->move)		/* 被定身，不可移动 */
		return 1;

	FgtCountPath(idx);
	gam_memset(buf,0,20);
	FgtLoadToMem(dFgtCtMove,buf);
	FgtShowMvRng();
	while(1)
	{
        //usleep(1000);
		FgtShowFrame();
		if(FgtGetFoucs(FgtMoveBack) == 0xFF)
		{
			FgtSetFocus(idx);
			return 1;
		}
		x = g_FoucsX - g_PathSX + g_PUseSX;
		y = g_FoucsY - g_PathSY + g_PUseSY;
		if(x > FGT_MRG || y > FGT_MRG || g_FightPath[y * FGT_MRG + x] > MOV_RSTD)
			FgtStrShowV(STA_SX+74,STA_SY+2,buf);
		else
			break;
	}
	g_BakUpX = pos->x;
	g_BakUpY = pos->y;
	pos->x = g_FoucsX;
	pos->y = g_FoucsY;
	FgtSetFocus(idx);
	return 0;
}
/***********************************************************************
 * 说明:     将领移动时的背景显示
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtMoveBack(bool *flag)
{
	if(*flag)
		FgtShowMvRng();
	else
		FgtAllRight(flag);
}
/***********************************************************************
 * 说明:     获取被操作将领的序号
 * 输入参数: 无
 * 返回值  : !0xff-操作序号	0xff-结束操作
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtGetControl(void)
{
	U8	idx;

	FgtShowMap(g_MapSX,g_MapSY);
	FgtShowState();
	FgtShowInf();
	while(1)
	{
        //usleep(1000);

        if(!FgtGetFoucs(FgtAllRight))
        {
            idx = FgtCheckIdx();
            if(idx != 0xFF)
                return idx;
            else
                continue;
        }
		if(FgtMainMenu() == 0xFF)
			return 0xFF;
	}
}
/***********************************************************************
 * 说明:     获取光标位置
 * 输入参数: chkcondition-检测函数
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtGetFoucs(void (*chkcondition)(bool*))
{
	bool	flag,tflag;	
	GMType	pMsg;
    bool hflag = false;

	flag = false;
	while(1)
	{
        //usleep(1000);
        //DebugTime(0);
        if(g_reflushflag == 1)
        {
            InitFgtMap();
            g_reflushflag = 0;
        }

		GamGetMsg(&pMsg);
        //DebugTime(1);
		if(VM_CHAR_FUN == pMsg.type)
		{
			if(VK_ENTER == pMsg.param)
				return 0;
			if(VK_EXIT == pMsg.param)
				return 0xFF;
			FgtMapUnitShow(g_FoucsX,g_FoucsY,0);
        }
        //DebugTime(2);
		switch(pMsg.type)
		{
		case VM_CHAR_FUN:
            hflag = false;
			switch(pMsg.param)
			{
			case VK_UP:
				if (g_FoucsY)
					g_FoucsY -= 1;
				break;
			case VK_DOWN:
				if (g_FoucsY < g_MapHgt-1)
					g_FoucsY += 1;
				break;
			case VK_LEFT:
				if (g_FoucsX)
					g_FoucsX -= 1;
				break;
			case VK_RIGHT:
				if (g_FoucsX < g_MapWid-1)
					g_FoucsX += 1;
				break;
			case VK_HELP:
                hflag = true;
				FgtShowHlp();
                continue;
			case VK_SEARCH:
				FgtShowView();
				break;
			}

			tflag = false;
			(*chkcondition)(&tflag);
			break;
		case VM_TIMER:
            if(hflag)break;
			flag = !flag;
			break;
		}
        //DebugTime(3);
        if(hflag == false)
        {
            //DebugTime(4);
            FgtShowGen(flag);
            //DebugTime(5);
            tflag = true;
            (*chkcondition)(&tflag);
            FgtShowCursor();
            //DebugTime(6);
            FgtShowFrame();
            //DebugTime(7);
        }
        else
        {
            tflag = true;
            (*chkcondition)(&tflag);
        }
	}
}
/***********************************************************************
 * 说明:     显示当前光标所在的位置
 * 输入参数: flag-显示还是消除
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtShowCursor(void)
{
	U8	x,y;

	x = FgtGetScrX(g_FoucsX);
	y = FgtGetScrY(g_FoucsY);	
	FgtRPicShowV(STEP_PIC,5,x,y);
}
/***********************************************************************
 * 说明:     初始化战斗地图
 * 输入参数: mode-玩家战斗模式	way-进攻方向	mapid-地图id
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtIntMap(void)
{
	U8	i,sx,sy;
	U8	tmp,way;
	U16	lp,cx,cy;
	U8	*ptr,*tptr;

	/* 获取地图的基本信息 */
	ptr = FgtLoadToCon(g_FgtParam.MapId,1);
	if (NULL == ptr)
	{
		g_RunErr = MAP_LOAD_ERR;
		return 1;
	}
	g_MapWid = (U8) *ptr;
	g_MapHgt = (U8) *(ptr+2);
/*	g_TileId = (U16) *(ptr+4);*/
	g_TileId = FIGHT_TILE;
	
	/* 获取城池-防守方起始坐标 */
	ptr += 16;
	lp = 0;
	while(CITY_TIL != ptr[lp])	/* 注意：若地图中没城市地图块，可能造成死循环 */
		lp += 1;
	g_CityX = lp % g_MapWid;
	g_CityY = lp / g_MapWid;
	cx = g_CityX - 2;
	cy = g_CityY - 2;
	
	/* 获取进攻方起始坐标 */
	sx = FgtGetBaseXY('x');
	sy = FgtGetBaseXY('y');
	
	/* 初始化将领坐标 */
	tmp = 8;
	way = g_FgtParam.Way;
	if (FGT_AT == g_FgtParam.Mode)
	{
		tmp = cx;
		cx = sx;
		sx = tmp;
		tmp = cy;
		cy = sy;
		sy = tmp;
		tmp = way;
		way = 8;
	}
	/* 初始化坐标 */	
    ptr = FgtLoadToCon(IFACE_CONID,dFgtIntPos);
	tptr = ptr + way * 20;
	ptr += tmp * 20;
	for (i = 0; i < FGT_PLAMAX; i += 1)
	{
		tmp = i << 1;
		g_GenPos[i].x = cx + ptr[tmp];
		g_GenPos[i].y = cy + ptr[tmp + 1];
		g_GenPos[i+FGT_PLAMAX].x = sx + tptr[tmp];
		g_GenPos[i+FGT_PLAMAX].y = sy + tptr[tmp + 1];
	}
	return 0;
}
/***********************************************************************
 * 说明:     初始化战斗变量
 * 输入参数: mode-玩家的战斗方式	way-进攻方向
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtIntVar(void)
{
	g_FgtWeather = WEATHER_WIND;
	g_FgtOver = FGT_COMON;
	g_FgtBoutCnt = 0;
	if(FGT_AT == g_FgtParam.Mode)
		g_MainGenIdx = 0;
	else
		g_MainGenIdx = FGT_PLAMAX;
	/* 初始化将领基本属性 */
	CountInitGen();
	return 0;
}
/***********************************************************************
 * 说明:     获取攻击方初始化的基准坐标
 * 输入参数: way-进攻方向
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtGetBaseXY(U8 sym)
{	
	U8	x,y;

	x = g_MapWid - 5;
	y = g_MapHgt - 5;
	switch(g_FgtParam.Way)
	{
	case DIRECT_ES:
		break;
	case DIRECT_N:
		x = (g_MapWid>>1)-2;
		y = 0;
		break;
	case DIRECT_EN:
		y = 2;
		break;
	case DIRECT_E:
		y = (g_MapHgt >> 1) - 2;
		break;
	case DIRECT_S:
		x = (g_MapWid >> 1) - 2;
		break;
	case DIRECT_WS:
		x = 2;
		break;
	case DIRECT_W:
		x = 2;
		y = (g_MapHgt >> 1) - 2;
		break;
	case DIRECT_WN:
		x = 0;
		y = 0;
		break;
	}
	if('x' == sym)
		return x;
	else
		return y;
}
/***********************************************************************
 * 说明:     初始化战斗界面
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtIntScr(void)
{
	FgtShowState();
	FgtSetFocus(FGT_PLAMAX);
	GamDelay(SHOW_DLYBASE * 10,false);
	FgtSetFocus(0);
	GamDelay(SHOW_DLYBASE * 10,false);
}
/***********************************************************************
 * 说明:     显示战斗状态
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtShowState()
{
	FgtRPicShowV(STATE_PIC,1,STA_SX,STA_SY);			/* 显示背景图 */
	PlcNumShow(STA_LCX,STA_LCY,g_FgtParam.MProvender,3,g_VisScr);	/* 显示粮草 */
	FgtRPicShowV(WEATHER_PIC,g_FgtWeather,STA_TQX,STA_TQY);		/* 显示天气 */
}
/***********************************************************************
 * 说明:     检查光标所处位置是否在指定范围内的有效区域
 * 输入参数: 无
 * 返回值  : true-是	false-否
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
bool FgtChkRng(void)
{
	U8	x,y,rng;	
	
	rng = g_FgtAtkRng[0];
	x = g_FoucsX - g_FgtAtkRng[1];
	y = g_FoucsY - g_FgtAtkRng[2];
	if(y >= rng || x >= rng)
		return false;
	return (g_FgtAtkRng[(U16)y * rng + x + 3] == 1);
}
/***********************************************************************
 * 说明:     处理回合开始时公共的对象
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtDealBout(void)
{
	U8	idx,numb[5],xof;
	U8	tbuf[30];
    U8 idx1;
		
	g_FgtBoutCnt += 1;	/* 战斗天数增加 */
	g_EneTmpProv = 0;	/* 敌人粮草临时变量 */
	if(g_FgtBoutCnt > 30)
	{
		if(g_FgtParam.Mode == FGT_AT)
			g_FgtOver = FGT_LOSE;
		else
			g_FgtOver = FGT_WON;
		return;
	}
	/* 恢复将领的激活状态 */
	for(idx = 0;idx < FGTA_MAX;idx += 1)
		g_GenPos[idx].active = STA_WAIT;
	
	/* 转变天气 */
	FgtChgWeather();
	PlcRPicShow(DAYS_PIC,1,FGT_SPESX + 46,FGT_SPESY + 18,true);
	gam_itoa(g_FgtBoutCnt,numb,10);
	xof = 64 - (gam_strlen(numb) - 1) * ASC_WID / 2;
	GamStrShowS(FGT_SPESX + xof,FGT_SPESY + 22,(const char*)numb);
	GamDelay(SHOW_DLYBASE * 5,false);
	
	if(g_FgtParam.EProvender == 0)
	{
		g_FgtOver = FGT_WON;
        idx1 = dFgtNoProv1;
	}
	if(g_FgtParam.MProvender == 0)
	{
		g_FgtOver = FGT_LOSE;
        idx1 = dFgtNoProv0;
	}
	if(g_FgtOver)
	{
        FgtLoadToMem(idx1,tbuf);
		GamMsgBox((const char*)tbuf,1);
	}
	
	FgtDrvState();
}
/***********************************************************************
 * 说明:     刷新地图的显示
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtRefrashMap(void)
{
	if (g_FoucsX < g_MapSX)
		g_MapSX -= 1;
	else if (g_FoucsY < g_MapSY)
		g_MapSY -= 1;
	else if (g_FoucsX >= g_MapSX + SCR_MAPWID)
		g_MapSX += 1;
	else if (g_FoucsY >= g_MapSY + SCR_MAPHGT)
		g_MapSY += 1;
	else
		return;
	FgtShowMap(g_MapSX,g_MapSY);
}
/***********************************************************************
 * 说明:     总是返回正确
 * 输入参数: 无
 * 返回值  : true-正确
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtAllRight(bool *flag)
{
	if(*flag) return;
	
	FgtRefrashMap();
	FgtShowState();
	FgtShowInf();
}
/***********************************************************************
 * 说明:     驱动特殊状态
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtDrvState(void)
{
	U8	i,state,tmp;
	U16	arms,hurt;
	bool	rec,update;
	PersonType	*per;
	
	if(g_FgtOver)
		return;
	for(i = 0;i < FGTA_MAX;i += 1)
	{
		state = g_GenPos[i].state;
		if(STATE_SW == state)
			continue;
		tmp = TransIdxToGen2(i);
		per = &g_Persons[tmp];
		rec = (rand() % 60) < (per->IQ >> 1);
		arms = per->Arms;
		hurt = 0;
		update = false;
		switch(state)
		{
		case STATE_ZC:		/* 正常 */
		case STATE_DJ:		/* 遁甲 */
			break;
		case STATE_SZ:		/* 石阵 */
			hurt = arms >> 3;
			arms -= hurt;
		case STATE_HL:		/* 混乱 */
		case STATE_JZ:		/* 禁咒 */
		case STATE_DS:		/* 定身 */
			if(rec)
			{
				CountMoveP(i);
				state = STATE_ZC;
				update = true;
			}
			break;
		case STATE_QM:		/* 奇门 */
		case STATE_QZ:		/* 潜踪 */
			if(!rec)
			{
				state = STATE_ZC;
				update = true;
			}
			break;
		}
		if(hurt)
		{
			FgtSetFocus(i);
			FgtShowSNum('-',i,hurt);	/* 在地图上显示数字 */
			per->Arms = arms;
		}
		if(update)
		{
			g_GenPos[i].state = state;
			FgtSetFocus(i);
			FgtFrashGen(i,4);		/* 刷新将领 */
		}		
	}	
}
/***********************************************************************
 * 说明:     将屏幕的焦点聚集到指定的将领身上
 * 输入参数: foc-将领序号
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void FgtSetFocus(U8 foc)
{
	U8	sx,sy;
	
	sx = g_GenPos[foc].x;
	sy = g_GenPos[foc].y;
	g_FoucsX = sx;
	g_FoucsY = sy;
	/* 初始化显示地图 */
	if (sx < SHOW_MX)
		sx = 0;
	else
		sx -= SHOW_MX;
	if (sy < SHOW_MY)
		sy = 0;
	else
		sy -= SHOW_MY;
	FgtShowMap(sx,sy);
	FgtShowGen(0);
	FgtShowInf();
	FgtShowFrame();
}
/***********************************************************************
 * 说明:     在地图上显示小数字
 * 输入参数: sym-'+','-'	idx-将领脚标	num-显示的数字
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtShowSNum(U8 sym,U8 idx,U16 num)
{
	U8	sx,sy;
	U8	tbuf[10];
	
	tbuf[0] = sym;
	gam_itoa(num,tbuf + 1,10);
	sx = FgtGetScrX(g_GenPos[idx].x);
	sy = FgtGetScrY(g_GenPos[idx].y);
    GamStrShowT(sx,sy,tbuf);
	GamDelay(SHOW_DLYBASE * 5,false);
}
/***********************************************************************
 * 说明:     闪烁指定的将领
 * 输入参数: idx-将领脚标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void FgtFrashGen(U8 idx,U8 cnt)
{
	U8	i;
	
	FgtShowGen(0);
	i = 0;
	while(1)
	{
        //usleep(1000);
		i += 1;
		FgtShowFrame();
		if(i > cnt) break;
		GamDelay(SHOW_DLYBASE,false);
		FgtMapUnitShow(g_GenPos[idx].x,g_GenPos[idx].y,1);
		GamDelay(SHOW_DLYBASE,false);
	}
}
/***********************************************************************
 * 说明:     显示当前地图显示范围内可见的将领
 * 输入参数: act-将领动作
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtShowGen(U8 act)
{
	U8	i,idx,pIdx;
	U8	sx,sy,tx,ty,state;
	JLPOS	*pos;
	
	/* 显示可见的将领 */
	for (i = 0; i < FGTA_MAX; i += 1)
	{
        //DebugTime(11);
		pos = &g_GenPos[i];
		state = pos->state;
		if (STATE_SW == state)
			continue;
		tx = pos->x;
		ty = pos->y;		
		if(tx >= g_MapSX && tx < g_MapSX + SCR_MAPWID &&
		   ty >= g_MapSY && ty < g_MapSY + SCR_MAPHGT)
		{
            //DebugTime(12);
			idx = FgtGenPIdx(i);
            //DebugTime(13);
			if(STA_WAIT == pos->active)
				idx += act;
            //DebugTime(14);
			sx = FgtGetScrX(tx);
            //DebugTime(15);
			sy = FgtGetScrY(ty);
			/* 显示将领 */
            //DebugTime(16);
			FgtMapUnitShow(tx,ty,0);
            //DebugTime(17);
			FgtRPicShowV(BING_PIC,idx + 1,sx,sy);
			/* 显示异常状态 */
			if(state == STATE_JZ)
				pIdx = 2;
			else if(state == STATE_DS)
				pIdx = 4;
			else
				pIdx = 0;
            //DebugTime(18);
			if(pIdx)
			{
				pIdx += act;
				FgtRPicShowV(SPESTA_PIC,pIdx + 1,sx,sy);
			}
            //DebugTime(19);
		}
	}
}
/***********************************************************************
 * 说明:     显示地图块单元
 * 输入参数: tx,ty-地图坐标	flag-true显示到屏幕 false显示到内存
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtMapUnitShow(U8 tx,U8 ty,U8 flag)
{
    U8	*ptr;
    U32	tile;
    U32	poff;
    //DebugTime(21);
    //tile = (ty - g_MapSY) * SCR_MAPWID + tx - g_MapSX;
    tile = ty * 32 + tx;

    ptr = FgtLoadToCon(g_TileId,1) + PICHEAD_LEN;
    if(g_showPicType == 0)
    {
        poff = (U16)g_FightMap[tile] << 5;
    }
    else if(g_showPicType == 1)
    {
        poff = (U16)g_FightMap[tile] * 19200;
    }
    else if(g_showPicType == 2)
    {
        poff = 0;//(U16)g_FightMap[tile] * 19200;
        int ii = 0 ;
       for(ii = 0 ; ii < g_FightMap[tile] ; ii ++)
       {
           U32 size  = ((PictureHeadTypeJPG *)(ptr+poff))->size;
           poff += (size + 4);
       }

    }
    //DebugTime(22);
    ptr+= poff;
    //DebugTime(23);
    g_PicId = poff;

	tx = FgtGetScrX(tx);
    //DebugTime(24);
	ty = FgtGetScrY(ty);
    //DebugTime(25);
	if(flag)
		GamPicShowS(tx,ty,TIL_WID,TIL_HGT,ptr);
	else
		GamPicShowV(tx,ty,TIL_WID,TIL_HGT,ptr,g_VisScr);
    //DebugTime(26);
}
/***********************************************************************
 * 说明:     显示获取的经验值
 * 输入参数: exp-获取的经验值
 * 返回值  : true-是	false-否
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtShowGetExp(U8 exp)
{
	U8	sbuf[20],numb[5];
	
	/* 显示获得的经验 */
	FgtLoadToMem(dFgtGetExp,sbuf);
	gam_itoa(exp,numb,10);
    gam_strcat(sbuf,numb);
	PlcGraMsgBox(sbuf,1,0xFF);
}
/***********************************************************************
 * 说明:     检查并驱动命令的结束（该升的升，该死的死）
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtChkAtkEnd(void)
{
	U8	i,tmp,idx;
	U8	x,y,*buf;
	PersonType	*per;
	JLPOS		*pos;
	
	buf = SHARE_MEM;
	for(i = 0;i < FGTA_MAX; i += 1)
	{
		pos = &g_GenPos[i];
		if(STATE_SW == pos->state)
			continue;
		tmp = TransIdxToGen2(i);
		per = &g_Persons[tmp];
		x = pos->x;
		y = pos->y;
		idx = rand() % 3;
		/* 升的 */
		if(per->Experience > FGT_EXPMAX - 1)
		{
			FgtSetFocus(i);
			per->Experience -= FGT_EXPMAX;
			per->Level += 1;
			if(per->Level > MAX_LEVEL)
				per->Level = MAX_LEVEL;
            FgtShowChgSpe(0,5,x,y);
            if(idx ==0)FgtLoadToMem(dFgtLevUp0,buf);
            if(idx ==1)FgtLoadToMem(dFgtLevUp1,buf);
            if(idx ==2)FgtLoadToMem(dFgtLevUp2,buf);
			ShowGReport(tmp,buf);
		}
		/* 死的 */
		if(!pos->hp || !per->Arms)
		{
			FgtSetFocus(i);
			pos->state = STATE_SW;
			FgtShowChgSpe(6,11,x,y);
            if(idx ==0)FgtLoadToMem(dFgtDead0,buf);
            if(idx ==1)FgtLoadToMem(dFgtDead1,buf);
            if(idx ==2)FgtLoadToMem(dFgtDead2,buf);
            //(dFgtDead0 + idx,buf);
			ShowGReport(tmp,buf);
		}
	}
}
/***********************************************************************
 * 说明:     显示变身动画
 * 输入参数: sfrm,efrm-播放参数	x,y-播放坐标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtShowChgSpe(U8 sfrm,U8 efrm,U8 x,U8 y)
{	
	if(x < g_MapSX || x >= g_MapSX + SCR_MAPWID ||
	   y < g_MapSY || y >= g_MapSY + SCR_MAPHGT)
		return;
	x = FgtGetScrX(x);
	y = FgtGetScrY(y);
    PlcMovie(STACHG_SPE,sfrm,efrm,0,x,y);
}
/***********************************************************************
 * 说明:     显示地图
 * 输入参数: x,y-显示地图的起始坐标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtShowMap(U8 x,U8 y)
{
    U8	i,*ptr;
    U32	count;

    /* 入口参数检查 */
    if(g_MapWid < x + SCR_MAPWID)
        x = g_MapWid - SCR_MAPWID;
    if(g_MapHgt < y + SCR_MAPHGT)
        y = g_MapHgt - SCR_MAPHGT;

    g_MapSX=x;
    g_MapSY=y;


    U16 wid = TIL_WID * SCR_MAPWID;
    U16 hgt = TIL_HGT * SCR_MAPHGT;
    if(g_showPicType > 0)
    {
        wid*=5;
        hgt*=5;
        wid-=1;
        hgt-=1;
        GamePictureDummy(x*80,y*80,wid,hgt,NULL,NULL,11);//10-城市地图 11-战斗地图
    }
    else
    {
        wid-=1;
        hgt-=1;
        GamePictureDummy(x*16,y*16,wid,hgt,NULL,NULL,11);//10-城市地图 11-战斗地图
    }


}
/***********************************************************************
 * 说明:     获取指定位置的地图数据
 * 输入参数: x,y-显示地图的起始坐标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtGetMapDat(U8 x,U8 y)
{
	U8	i,*ptr,*maptr;
	U16	count;

	/* 入口参数检查 */
    /*if(g_MapWid < x + SCR_MAPWID)
		x = g_MapWid - SCR_MAPWID;
	if(g_MapHgt < y + SCR_MAPHGT)
		y = g_MapHgt - SCR_MAPHGT;

	g_MapSX=x;
    g_MapSY=y;*/
	/* 获取要显示的地图数据指针 */
	ptr = FgtLoadToCon(g_FgtParam.MapId,1) + MAP_HEAD_LEN;	
	/* 获取要显示的地图数据 */
	maptr = g_FightMap;
    count = 32;
    //ptr += y * count;
    //ptr += x;
    for(i = 0; i < 32; i += 1)
	{
        gam_memcpy(maptr,ptr,32);
        maptr += 32;
        ptr += 32;
	}
}
/***********************************************************************
 * 说明:     显示当前位置的信息
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtShowInf(void)
{
	U8	idx,buf[40],sbuf[5];

	gam_clrvscr(STA_XXX,STA_XXY,STA_XXX + HZ_WID * 4,STA_XXY + HZ_HGT,g_VisScr);
	idx = FgtGetGenIdx(g_FoucsX,g_FoucsY);
	if(0xFF == idx)
	{
		idx = FgtGetTerrain(g_FoucsX,g_FoucsY) << 2;
		FgtLoadToMem(dFgtTrnInf,buf);
		gam_memcpy(sbuf,buf+idx,4);
		sbuf[4]=0;
	}
	else
	{
		idx = TransIdxToGen2(idx);
		GetPersonName(idx,sbuf);
	}
	FgtStrShowV(STA_XXX,STA_XXY,sbuf);
#if	(GAM_VER == GAM_DEBUG_MODE)
	itoa(g_FoucsX,buf,10);
	FgtStrShowV(STA_XXX + 30,STA_XXY,buf);
	itoa(g_FoucsY,buf,10);
	FgtStrShowV(STA_XXX + 50,STA_XXY,buf);
#endif
}
/***********************************************************************
 * 说明:     检查战斗是否结束
 * 输入参数: flag-
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtChkEnd(U8 flag)
{
	U8	i,start;
	
	if(g_FgtOver)
		return;

	/* 若主将已死 */
	if(STATE_SW ==g_GenPos[g_MainGenIdx].state)
	{
		if(FGT_PLAMAX == g_MainGenIdx)
			g_FgtOver = FGT_WON;
		else
			g_FgtOver = FGT_LOSE;
		return;
	}

	if(flag)
	{
		start = 0;
		g_FgtOver = FGT_LOSE;
	}
	else
	{
		start = FGT_PLAMAX;
		g_FgtOver = FGT_WON;
	}
	/* 全员死亡 */
	for(i = start;i < FGTA_MAX; i += 1)
	{
		if(STATE_SW == g_GenPos[i].state)
			continue;
		g_FgtOver = FGT_COMON;
		return;
	}
}
/***********************************************************************
 * 说明:     将将领的控制脚标转换成将领id
 * 输入参数: idx-控制脚标
 * 返回值  : 将领id
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 TransIdxToGen2(U8 idx)
{
	return (U8)(g_FgtParam.GenArray[idx] - 1);
}
/***********************************************************************
 * 说明:     资源管理函数的本体调用
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtLoadToMem(U8 idx,U8 *buf)
{
    ResLoadToMem(IFACE_STRID,(U8)idx,buf);
}
/***********************************************************************
 * 说明:     初始化游戏引擎所在的机型环境
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 *FgtLoadToCon(U16 ResId,U8 idx)
{
    return (ResLoadToCon(ResId,idx,g_CBnkPtr));
}

U8 *FgtConstLoadToCon(U16 ResId,U8 idx)
{
    return (ResLoadToCon(ResId,idx,g_CBnkPtr));
}
/***********************************************************************
 * 说明:     刷新屏幕函数的本体调用
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtShowFrame(void)
{
    GamShowFrame(g_VisScr);
}
/***********************************************************************
 * 说明:     显示字符串到屏幕函数的本体调用
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtStrShowV(U8 x,U8 y,U8 *buf)
{
	GamStrShowV(x,y,buf,g_VisScr);
}
/***********************************************************************
 * 说明:     封装长调用函数PlcRPicShow
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtRPicShowV(U16 id,U8 idx,U8 sx,U8 sy)
{
	PlcRPicShow(id,idx,sx,sy,false);
}

void InitFgtMap()
{
    U8	*ptr;//= malloc(1048576);
    U32	i,count;
    U16 x,y;

    count = 0;

    FgtGetMapDat(0,0 );
    /* 显示地图 */
    ptr = FgtLoadToCon(FIGHT_TILE,1) + PICHEAD_LEN;
    for(i = 0; i < 32 * 32; i += 1)
    {

        if(g_showPicType == 0)
        {
            count = g_FightMap[i];
            count <<= 5;
        }
        else if(g_showPicType == 1)
        {
            count = g_FightMap[i];
            count *= 19200;
        }
        else if(g_showPicType == 2)
        {
            U32 titleno = g_FightMap[i];//图块编号
            int imax = (int)titleno;
            count = 0;
            int ii = 0 ;
            for(ii = 0 ; ii <imax ; ii ++ )
            {
                U32 size  = ((PictureHeadTypeJPG *)(ptr + count))->size;//size不包含4
                count += (size + 4);
            }

        }

        y = ((i / 32) << 4) + WK_SX;
        x = ((i % 32) << 4) + WK_SY;
        g_PicId = count;
        GamPicSave(x,y,TIL_WID,TIL_HGT,ptr + count,g_VisScr,1);


    }
}
