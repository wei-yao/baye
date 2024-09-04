/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	FightSub.c
*文件标识：	步步高电子词典的游戏引擎模块
*摘要：		游戏引擎战斗子模块-单挑和人工智能
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
#undef	FgtPkAi
#define	FgtPkAi
#include "enghead.h"
#define		IN_FILE	1	/* 当前文件位置 */
/*本体函数声明*/
/*------------------------------------------*/
U8 FgtGetMCmdNear(FGTCMD *pcmd);
bool FgtJiNeng(FGTCMD *pcmd);
bool FgtAtkCmd(FGTCMD *pcmd);
void FgtCmpMove(U8 idx);
void FgtMakeSklNam(U8 *buf);
void FgtGetSklBuf(U8 id,U8 *buf);
U8 FgtCanUse(U8 param,U8 idx);
bool FgtChkAkRng(U8 x,U8 y);
U8 FgtCntInterval(U8 x1,U8 y1,U8 x2,U8 y2);
U8 FgtGetGenTer(U8 idx);
void FgtGetHurtMax(U16 *h,U8 *a);
void FgtGetAtkRng(U8 idx,U8 x,U8 y);
U8 FgtGetAimPos(U8 *aimx,U8 *aimy);
U8 FgtGetNearGen(void);
void FgtLoadToMem3(const U8 *idx,U8 *buf);
void FgtViewForce(U8 pForce,U8 pSIdx);
U8 TransIdxToGen3(U8 idx);
U8 FgtStatGen(U8 flag);

extern int ngf;

/***********************************************************************
 * 说明:     获取计算机控制方的战斗命令
 * 输入参数: pcmd-战斗命令
 * 返回值  : 
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 bool FgtGetMCmd(FGTCMD *pcmd)
{
	U8	jb;
	U8	sx,sy;
	
	jb = FgtGetMCmdNear(pcmd);
	if(0xFF == jb)
		return false;
	else
	{
		if(CMD_ATK == pcmd->type || CMD_STGM == pcmd->type)
		{
			jb = pcmd->aIdx;
			FgtSetFocus(jb);
			sx = FgtGetScrX(g_GenPos[jb].x);
			sy = FgtGetScrY(g_GenPos[jb].y);
            PlcMovie(STACHG_SPE,12,17,0,sx,sy);
		}
		return true;
	}
}
/***********************************************************************
 * 说明:     获取计算机控制方的战斗命令
 * 输入参数: pcmd-战斗命令
 * 返回值  : 
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtGetMCmdNear(FGTCMD *pcmd)
{
	U8	i;

	i = FgtGetNearGen();
	if(0xFF != i)
	{
		pcmd->sIdx = i;
		/* 显示选中的将领移动范围 */
		FgtCountPath(i);
		if(g_LookEnemy)
		{
            FgtSetFocus(i);
            FgtShowMvRng();
            FgtFrashGen(i,1);
            GamShowFrame(g_VisScr);
			if(g_MoveSpeed)
				GamDelay(SHOW_DLYBASE * 5,false);
		}
		/* 将领移动 */
		FgtCmpMove(i);
		if(g_LookEnemy)
		{
			FgtSetFocus(i);
			if(g_MoveSpeed)
				GamDelay(SHOW_DLYBASE * 5,false);
		}
		if(!FgtJiNeng(pcmd) && !FgtAtkCmd(pcmd))
			pcmd->type = CMD_REST;		/* 仍没目标，只好休息了 */
	}
	return i;
}
/***********************************************************************
 * 说明:     产生将领的技能
 * 输入参数: pcmd-命令结构
 * 返回值  : true-操作成功	false-操作失败
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
bool FgtJiNeng(FGTCMD *pcmd)
{
	U8	i,id,idx,skidx;
	U8	sklbuf[SKILL_NMAX + 1];
	bool	same;
	U16	arms,ranv;
	PersonType	*per;
	JLPOS		*pos;
	
	idx = pcmd->sIdx;
	if(STATE_JZ == g_GenPos[idx].state)
		return false;
	id = TransIdxToGen3(idx);
	per = &g_Persons[id];
    ranv = rand();
	/* 智力越高的，施展技能的可能性越高 */
	if(ranv % 150 > per->IQ)
		return false;
	/* 兵力剩余越少，施展技能的可能性越高(玄兵除外) */
	if(per->ArmsType != ARM_XUANBING)
	{
		arms = PlcArmsMax(id);
		arms += arms >> 1;		/* arms = max * 1.5 */
        if(rand() % arms < per->Arms)
			return false;
	}
	/* 获取要施展的技能 */
	FgtGetSklBuf(id,sklbuf);
    skidx = ranv % gam_strlen(sklbuf);
	skidx = sklbuf[skidx];
	/* 自己是否符合施展的条件 */
	if(FgtCanUse(skidx,idx))
		return false;
	/* 搜索目标 */
	FgtGetCmdRng(CMD_STGM,skidx,idx);
	for(i = 0;i < FGTA_MAX;i += 1)
	{
		pos = &g_GenPos[i];
		if(STATE_SW == pos->state)
			continue;
		if(!FgtChkAkRng(pos->x,pos->y))
			continue;
		if(i >= FGT_PLAMAX)
			same = true;
		else
			same = false;
		if(!FgtJNChkAim(skidx,same,i))
			continue;
		if(skidx == 17 || skidx == 29)
		{
			id = g_FgtParam.GenArray[i] - 1;
			arms = PlcArmsMax(id) >> 1;
			arms += arms >> 1;		/* arms = max * 3/4 */
			/* 若施展的计谋为恢复的，目标将领的兵力至少要损失1/4 */
			if(per->Arms > arms)
				continue;
		}
		else
		{	/* 施展计谋成功 */
			pcmd->type = CMD_STGM;
			pcmd->aIdx = i;
			pcmd->param = skidx;
			return true;
		}
	}
	return false;
}
/***********************************************************************
 * 说明:     产生将领的攻击命令
 * 输入参数: pcmd-战斗命令结构
 * 返回值  : true-操作成功	false-操作失败
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
bool FgtAtkCmd(FGTCMD *pcmd)
{
	U8	idx,aidx;
	U16	hurt;
	
	idx = pcmd->sIdx;
	FgtGetCmdRng(CMD_ATK,0,idx);
	BuiltAtkAttr(0,idx);
	FgtGetHurtMax(&hurt,&aidx);
	/* 产生攻击的命令 */
	if(aidx != 0xFF)
	{
		pcmd->type = CMD_ATK;
		pcmd->aIdx = aidx;
		return true;
	}
	return false;
}
/***********************************************************************
 * 说明:     移动敌人的将领
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
const U8 dStepNumb[] = {1,0,2,1,2,0,1,0,2,1,1,0};
void FgtCmpMove(U8 idx)
{
	U8	aimx,aimy,aidx;
	U8	x,y,ox,oy,pTer;
	U8	type,min,delta;
	U8	ax,ay,bx,by;
	bool	flag;
	U16	hurtmax,hurt;

	/* 若当前将领是太守，不进行移动 */
	if(FgtGetGenTer(idx) == TERRAIN_CITY)
		return;
	
	FgtGetAimPos(&aimx,&aimy);
	/* 获取移动的目标点 */
	ox = g_GenPos[idx].x;
	oy = g_GenPos[idx].y;
	type = g_Persons[TransIdxToGen3(idx)].ArmsType << 1;
	min = 0xFF;
	hurtmax = 0;
	ax = ox;
	ay = oy;
	for(x = g_PUseSX;x < FGT_MRG;x += 1)
	for(y = g_PUseSY;y < FGT_MRG;y += 1)
	{
		if(g_FightPath[y * FGT_MRG + x] >= MOV_RSTD)
			continue;
		bx = x - g_PUseSX + g_PathSX;
		by = y - g_PUseSY + g_PathSY;
		pTer = FgtGetTerrain(bx,by);
		/* 若电脑为进攻模式，且能进入城池，利马儿执行 */
		if(FGT_DF == g_FgtParam.Mode && TERRAIN_CITY == pTer)
		{
			g_GenPos[idx].x = bx;
			g_GenPos[idx].y = by;
			FgtSetFocus(idx);
			return;
		}
		delta = FgtCntInterval(bx,by,aimx,aimy);

		/* 取伤害最大或距离目标点最近，或有防御加层但有攻击目标的地形优先考虑 */
		FgtGetAtkRng(idx,bx,by);
		FgtGetHurtMax(&hurt,&aidx);
		/* 决定是否更新移动目标点 */		
		if(!hurtmax)
		{	/* 没有能造成伤害的情况下，以趋近目标优先移动 */
			hurtmax = hurt;
			if(hurtmax || min > delta)
				flag = true;
			else if(min == delta)		/* 有距离相同的点，随机更新 */
                flag = rand() % 10;
			else
				flag = false;
		}
		else if(hurt < hurtmax)
			flag = false;
		else if(hurt > hurtmax)
			flag = true;
		else
		{	/* 造成的伤害等于最大伤害，取地形信息 */
			if(TERRAIN_HILL == pTer || TERRAIN_WOOD == pTer ||
	   		   TERRAIN_THORP == pTer || TERRAIN_CITY == pTer || TERRAIN_TENT == pTer)
				flag = true;
			else
			{
				if(FgtCntInterval(bx,by,ox,oy) > FgtCntInterval(ax,ay,ox,oy))
					flag = true;		/* 相同攻击伤害(不为0)时，动比静要好，并且要大动 */
				else
					flag = false;
			}
		}
		if(flag)
		{	/* 更新优先点 */
			min = delta;
			hurtmax = hurt;
			ax = bx;
			ay = by;
		}
	}
	if(min != 0xFF)
	{
		g_GenPos[idx].x = ax;
		g_GenPos[idx].y = ay;
	}
}
/***********************************************************************
 * 说明:     获取技能的序号
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 U8 FgtGetJNIdx(U8 idx,RECT *pRect,U8 *buf)
{
	U8	rngb,param,inf[10];
	
	FgtGetSklBuf(TransIdxToGen3(idx),buf);
	FgtMakeSklNam(buf);
	rngb = gam_strlen(buf + SKILL_NMAX + 1) / SKILL_NAMELEN;
	pRect->sx += 4;
	pRect->ex += 4;
	if(pRect->ex > WK_EX - SKILL_NAMELEN * ASC_WID - 4)
	{
		pRect->sx -= 24;
		pRect->ex -= 24;
	}
	pRect->sy += HZ_HGT / 2;
	pRect->ey -= HZ_HGT / 2;
	if(rngb < 3)
		pRect->ey = pRect->sy + rngb * HZ_HGT;
    ngf = 2;
	while(1)
	{
        //usleep(1000);
		rngb = 0;
		param = PlcSplMenu(pRect,0,buf+SKILL_NMAX + 1);
		if(MNU_EXIT == param)
			break;		
		param = buf[param];
		rngb = FgtCanUse(param,idx);
		if(!rngb) break;
        if(rngb == 1)FgtLoadToMem3(dFgtMpErr,inf);
        if(rngb == 2)FgtLoadToMem3(dFgtEthErr,inf);
        if(rngb == 3)FgtLoadToMem3(dFgtWhrErr,inf);
        //FgtLoadToMem3(rngb,inf);
		GamStrShowS(STA_XXX,STA_XXY,inf);		
	}
    ngf = 3;
	return param;
}
/***********************************************************************
 * 说明:     确定将领本身该技能是否可用
 * 输入参数: param-技能参数	idx-将领序号
 * 返回值  : 0-可用		!0-错误参数
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtCanUse(U8 param,U8 idx)
{
	U8	terrain;
	SKILLEF	*skl;
	JLPOS	*pos;

	pos = &g_GenPos[idx];
	terrain = FgtGetGenTer(idx);
	skl = (SKILLEF	*)FgtGetJNPtr(param);
	if(skl->useMp > pos->mp)
        return 1;//dFgtMpErr;		/*技能点不足*/
	if(!skl->oland[terrain])
        return 2;//dFgtEthErr;		/*地形错误*/
	if(!skl->weather[g_FgtWeather-1])
        return 3;//dFgtWhrErr;		/*天候错误*/
	return 0;
}
/***********************************************************************
 * 说明:     获取技能序号缓冲
 * 输入参数: id-将领ID		buf-技能名字缓冲
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
const U8 dArmsJNNum[]={3,4,3,3,4,9};
void FgtGetSklBuf(U8 id,U8 *buf)
{	
	U8	type,len;
	U8	*sklbuf,*ptr;
	
	gam_memset(buf,0,SKILL_NMAX + 1);
	/* 构造技能缓冲 */
	sklbuf = buf;
	ptr = ResLoadToCon(SPE_SKLID,g_PIdx,g_CBnkPtr);
	if(ptr[id])		/* 特有技能 */
	{
		*sklbuf = ptr[id];
		sklbuf += 1;
	}
	if(g_Persons[id].Belong == id + 1)	/* 君主技能 */
	{
		*sklbuf = 30;
		sklbuf += 1;
	}
    ptr = ResLoadToCon(IFACE_CONID,dFgtJNArray,g_CBnkPtr);
	type = g_Persons[id].ArmsType;
	len = ((float)dArmsJNNum[type] * g_Persons[id].Level / (MAX_LEVEL + 1)) + 1;
	ptr += type * SKILL_NMAX;
	gam_memcpy(sklbuf,ptr,len);
}
/***********************************************************************
 * 说明:     构造技能名字缓冲
 * 输入参数: 无
 * 返回值  : true-正确
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtMakeSklNam(U8 *buf)
{
	U8	i,tmp,*ptr;
	
	ptr = buf + SKILL_NMAX + 1;
	for(i = 0;i < SKILL_NMAX;i += 1)
	{
		tmp = buf[i];
		if(!tmp) break;
		ResLoadToMem(SKL_NAMID,tmp,ptr);
		ptr += SKILL_NAMELEN;
	}
}
/***********************************************************************
 * 说明:     检测目标承受该技能的条件是否满足
 * 输入参数: param-计谋序号	idx-施展者脚标	pidx-目标脚标
 * 返回值  : 0-不满足		!0-满足
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 U8 FgtJNChkAim(U8 param,U8 same,U8 aidx)
{
	U8	terrain,type;
	SKILLEF *skl;

	type = g_Persons[g_FgtParam.GenArray[aidx] - 1].ArmsType;	
	terrain = FgtGetGenTer(aidx);
	skl = (SKILLEF	*)FgtGetJNPtr(param);
	/* 检测目标兵种 */
	if(!skl->earm[type])
		return 0;
	/* 检测目标的地形 */
	if(!skl->eland[terrain])
		return 0;
	/* 施展对象是否符合 */
	if((skl->aim == AIM_OWNER && !same) || (skl->aim == AIM_ENEMY && same))
		return 0;
	return 1;
}
/***********************************************************************
 * 说明:     获取指定将领脚下的地况
 * 输入参数: idx-将领脚标
 * 返回值  : 地况
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtGetGenTer(U8 idx)
{
	return (FgtGetTerrain(g_GenPos[idx].x,g_GenPos[idx].y));
}
/***********************************************************************
 * 说明:     获取最大的攻击对象和攻击值
 * 输入参数: 
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtGetHurtMax(U16 *h,U8 *a)
{
	U8	i,aidx;
	U16	max,hurt;
	JLPOS	*pos;
	
	aidx = 0xFF;
	max = 0;
	for(i = 0;i < FGT_PLAMAX;i += 1)
	{
		pos = &g_GenPos[i];
		if(STATE_SW == pos->state)
			continue;
		if(!FgtChkAkRng(pos->x,pos->y))
			continue;
		if(i == g_MainGenIdx)
		{
			aidx = i;
			max = 0xFFFF;
			break;
		}
		BuiltAtkAttr(1,i);
		hurt = CountAtkHurt();
		if(max < hurt)
		{
			max = hurt;
			aidx = i;
		}
	}
	*h = max;
	*a = aidx;
}
/***********************************************************************
 * 说明:     检查指定的将领的是否在有效范围内
 * 输入参数: x,y-将领在地图中的坐标
 * 返回值  : true-是	false-否
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
bool FgtChkAkRng(U8 x,U8 y)
{
	U8	rng;
	
	rng = g_FgtAtkRng[0];
	x -= g_FgtAtkRng[1];
	y -= g_FgtAtkRng[2];
	if(y >= rng || x >= rng)
		return false;
	return (g_FgtAtkRng[(U16)y * rng + x + 3] == 1);
}
/***********************************************************************
 * 说明:     获取指定将领移动到指定位置时的攻击范围
 * 输入参数: idx-将领脚标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtGetAtkRng(U8 idx,U8 x,U8 y)
{
	U8	bakx,baky;
	JLPOS	*pos;
	
	pos = &g_GenPos[idx];
	bakx = pos->x;
	baky = pos->y;
	pos->x = x;
	pos->y = y;
	BuiltAtkAttr(0,idx);
	FgtGetCmdRng(CMD_ATK,0,idx);
	pos->x = bakx;
	pos->y = baky;
}
/***********************************************************************
 * 说明:     获取命令的攻击范围
 * 输入参数: type-命令类型	param-命令参数	idx-命令执行者
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void FgtGetCmdRng(U8 type,U8 param,U8 idx)
{
	U8	item,rngb,*ptr;
	U16	offset,id;

	switch(type)
	{
		case CMD_ATK:
			offset = ATT_RANGE * (U16)g_Persons[g_FgtParam.GenArray[idx] - 1].ArmsType;
			id = IFACE_CONID;
            //item = dFgtAtRange;
			rngb = ATT_RANGEUNIT;
            ptr = ResLoadToCon(id,dFgtAtRange,g_CBnkPtr) + offset;
			break;
/*		case CMD_PK:
			offset = ATT_RANGE * ARM_QIBING;
			id = IFACE_CONID;
			item = dFgtAtRange;
			rngb = ATT_RANGEUNIT;
			break;*/
		case CMD_STGM:
			offset = (U16)(param - 1) * SKILL_RANGE;
			id = SKL_RNGID;
			item = 1;
			rngb = SKILL_RANGEUNIT;
            ptr = ResLoadToCon(id,item,g_CBnkPtr) + offset;
			break;
	}
    //ptr = ResLoadToCon(id,item,g_CBnkPtr) + offset;
	if(rngb * rngb > SKILL_RANGE + 2)
		return;
    gam_memcpy(g_FgtAtkRng + 3,ptr,rngb * rngb);
	g_FgtAtkRng[0] = rngb;
	g_FgtAtkRng[1] = (U8)(g_GenPos[idx].x - (rngb >> 1));
	g_FgtAtkRng[2] = (U8)(g_GenPos[idx].y - (rngb >> 1));
}
/***********************************************************************
 * 说明:     获取要驱动的将领序号
 * 输入参数: 无
 * 返回值  : 0xFF-驱动完成	!0xFF-将领序号
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtGetNearGen(void)
{
	U8	i,state;
	U8	ax,ay;
	U8	min,idx;
	JLPOS	*pos;
	
	min = 0xFF;
	idx = 0xFF;
	FgtGetAimPos(&ax,&ay);
	for(i = FGT_PLAMAX;i < FGTA_MAX;i += 1)
	{
		pos = &g_GenPos[i];
		state = pos->state;
		if(STATE_SW == state || STATE_HL == state || STATE_SZ == state)
			continue;
		if(STA_END == pos->active)
			continue;
		state = FgtCntInterval(pos->x,pos->y,ax,ay);
		if(min > state)
		{
			min = state;
			idx = i;
		}
	}
	return idx;
}
/***********************************************************************
 * 说明:     获取目标点坐标
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtGetAimPos(U8 *aimx,U8 *aimy)
{
	/* 设定目标点坐标 */
	if(FGT_AT == g_FgtParam.Mode)
	{
		*aimx = g_GenPos[g_MainGenIdx].x;
		*aimy = g_GenPos[g_MainGenIdx].y;
	}
	else
	{
		*aimx = g_CityX;
		*aimy = g_CityY;
	}
}
/***********************************************************************
 * 说明:     计算两个坐标点之间的距离
 * 输入参数: x1,y1-点1		x2,y2-点2
 * 返回值  : 距离
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtCntInterval(U8 x1,U8 y1,U8 x2,U8 y2)
{
	U8	dx,dy;
	
	dx = x1 - x2;
	if(x1 < x2)
		dx = 0 - dx;
	dy = y1 - y2;
	if(y1 < y2)
		dy = 0 - dy;
	
	return (U8)(dx + dy);
}
/***********************************************************************
 * 说明:     将领的可移动范围影射到当前的屏幕上
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void FgtShowMvRng(void)
{
	U8	x,y,dx,dy;
	U8	tmp,*pic;
	U8	sx,sy;

	dx = g_MapSX + g_PUseSX - g_PathSX;
	dy = g_MapSY + g_PUseSY - g_PathSY;
	pic = ResLoadToCon(STEP_PIC,1,g_CBnkPtr);
	pic += PICHEAD_LEN;
	for(x = g_PUseSX;x < FGT_MRG;x += 1)
	for(y = g_PUseSY;y < FGT_MRG;y += 1)
	{
		/* 将x,y转换成屏幕地图坐标 */
		sx = x - dx;
		sy = y - dy;
		/* 若x,y超出屏幕的显示范围 */
		if(sx >= SCR_MAPWID || sy >= SCR_MAPHGT)
			continue;
		tmp = g_FightPath[y * FGT_MRG + x];
		/* 不覆盖将领本身 && 在移动范围内 */
		if(tmp != MOV_SPT && tmp < MOV_RSTD)
		{
			sx <<= 4;
			sy <<= 4;
			sx += WK_SX;
			sy += WK_SY;
			GamMPicShowV(sx,sy,16,16,pic,g_VisScr);
		}
	}
}
/***********************************************************************
 * 说明:     显示战场上的形势
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void FgtShowView(void)
{
	U8	pForce,pSIdx,pPcnt;		/* 当前将领显示起始序号 每页显示将领个数 */
	U8	sx,sy,idx,pcolor;
	U8	key,tbuf[20];
	JLPOS	*pos;
    GMType	pMsg;

	gam_clslcd();
	sx = (SCR_WID / 2 - g_MapWid) / 2 + WK_SX;
	sy = (SCR_HGT - g_MapHgt) / 2 + WK_SY + 20;
	FgtLoadToMem3(dPowerCmp,tbuf);
	GamStrShowS(WK_SX + 5,WK_SY + 3,tbuf);		/* 战场形势图 */
	FgtLoadToMem3(dDaysInf,tbuf);
	gam_ltoa(g_FgtBoutCnt,tbuf + 3,10);
	tbuf[gam_strlen(tbuf)] = ' ';
	GamStrShowS(WK_SX + 5,WK_SY + 5 + HZ_HGT,tbuf);	/* 进行日期 */
	FgtLoadToMem3(dReserve0,tbuf);
	GamStrShowS((SCR_WID / 2 - 4 * HZ_WID) / 2 + WK_SX,sy - (HZ_HGT + 3),tbuf);	/* 将领位置 */

	pPcnt = (WK_EY - (HZ_HGT * 2 + 6)) / HZ_HGT;
	FgtViewForce(1,0);
	gam_rect(sx - 1,sy - 1,sx + g_MapWid,sy + g_MapHgt);
	pcolor = COLOR_BLACK;
	pForce = 1;
	pSIdx = 0;
	while(1)
	{
        //usleep(1000);
		for(idx = 0;idx < FGTA_MAX;idx += 1)
		{
			pos = (JLPOS *)&g_GenPos[idx];
			if(STATE_SW == pos->state)
				continue;		
			gam_putpixel(sx + pos->x,sy + pos->y,pcolor);		
		}
		if(COLOR_BLACK == pcolor)
			pcolor = COLOR_WHITE;
		else
			pcolor = COLOR_BLACK;

        GamGetMsg(&pMsg);
        key = pMsg.param;//GamDelay(50,true);
        if(!key)
            continue;
		switch(key)
		{
		case VK_ENTER:
		case VK_EXIT:
			return;
		case VK_LEFT:
		case VK_RIGHT:
			pForce = !pForce;
			pSIdx = 0;
			break;
		case VK_UP:
			if(pSIdx)
				pSIdx -= pPcnt;
			break;
		case VK_DOWN:
			if(FgtStatGen(pForce) < pPcnt)
				break;
			if(pSIdx < FgtStatGen(pForce) - pPcnt)
				pSIdx += pPcnt;
			break;
		}
		FgtViewForce(pForce,pSIdx);
	}	
}
/***********************************************************************
 * 说明:     显示势力情报
 * 输入参数: pForce-交战的一方	pSIdx-开始序号
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void FgtViewForce(U8 pForce,U8 pSIdx)
{
	U8	i,pPCnt,pGIdx;
	U8	tmp,tbuf[20];
	U16	provender;
	
	pGIdx = pForce * FGT_PLAMAX;
	gam_clrlcd(WK_SX + SCR_WID / 2,WK_SY,WK_EX,WK_EY);
	gam_rect(WK_SX + SCR_WID / 2,WK_SY,WK_EX,WK_EY);
	gam_line(WK_SX + SCR_WID / 2,WK_SY + HZ_HGT + 2,WK_EX,WK_SY + HZ_HGT + 2);
	gam_line(WK_SX + SCR_WID / 2,WK_SY + HZ_HGT * 2 + 4,WK_EX,WK_SY + HZ_HGT * 2 + 4);
	i = TransIdxToGen3(pGIdx);
	GetPersonName(g_Persons[i].Belong - 1,tbuf);
	i = gam_strlen(tbuf);
	tbuf[i] = ' ';
	FgtLoadToMem3(dArmyInf,tbuf + i);
	i = SCR_WID / 2 + SCR_WID / 4 - (i + 2) * ASC_WID / 2;
	GamStrShowS(WK_SX + i,WK_SY + 2,tbuf);				/* 显示军团势力 */
	FgtLoadToMem3(dProvInf,tbuf);
	if(pForce)
	{
		if(!g_EneTmpProv)
			FgtLoadToMem3(dNoView,tbuf + 5);
		else
			gam_ltoa(g_EneTmpProv,tbuf + 5,10);
	}
	else
		gam_ltoa(g_FgtParam.MProvender,tbuf + 5,10);
	GamStrShowS(WK_SX + SCR_WID / 2 + 3,WK_SY + HZ_HGT + 4,tbuf);	/* 显示粮草 */

	pPCnt = (WK_EY - (HZ_HGT * 2 + 6)) / HZ_HGT;
	pSIdx += pGIdx;
	for(i = 0;i < pPCnt;i += 1)
	{
		tmp = i + pSIdx;
		if(tmp - pGIdx > FGT_PLAMAX)
			break;
		tmp = g_FgtParam.GenArray[tmp];
		if(!tmp)
			break;
		tmp -= 1;
		provender = g_Persons[tmp].Arms;		
		GetPersonName(tmp,tbuf);
		tmp = gam_strlen(tbuf);
        gam_memset(tbuf + tmp,' ',20 - tmp);
        gam_ltoa(provender,tbuf + 8,10);
        c_Ex = 160;
		GamStrShowS(WK_SX + SCR_WID / 2 + 3,WK_SY + HZ_HGT * (2 + i) + 6,tbuf);	/* 显示将领 */
	}

}
/***********************************************************************
 * 说明:     获取上战场将领的个数
 * 输入参数: flag-指定战斗的一方
 * 返回值  : 将领个数
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 FgtStatGen(U8 flag)
{
	U8	i,pSIdx;
	
	pSIdx = flag * FGT_PLAMAX;
	for(i = 0;i < FGT_PLAMAX;i += 1)
	{		
		if(!g_FgtParam.GenArray[i + pSIdx])
			break;
	}
	return i;
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
void FgtLoadToMem3(const U8 *idx,U8 *buf)
{
    ResLoadToMem(IFACE_STRID,(U8*)idx,buf);
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
U8 TransIdxToGen3(U8 idx)
{
	return (U8)(g_FgtParam.GenArray[idx] - 1);
}

