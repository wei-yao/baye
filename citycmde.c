/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	showfun.c
*文件标识：	步步高电子词典的游戏引擎模块
*摘要：		封装引擎对系统显示程序调用的入口
**
*移植性声明:
*	1、符合标准：《游戏设计标准V1.0》
*	2、兼容模式：本程序和界面无关，无兼容模式。
*	3、使用本封装程序的引擎要移植到其他环境中，系统的显示部分只需在此修改
*修改历史：
*	版本    日期     作者     改动内容和原因
*   ----------------------------------------------------
*	1.0    2005-7-12 11:17  陈泽伟     基本的功能完成
***********************************************************************/

#include "stdsys.h"
#include "comm.h"
#undef	CITYCMDE_C
#define	CITYCMDE_C
#include "enghead.h"


/******************************************************************************
* 函数名:ConfiscateMake
* 说  明:“没收”命令生成
*
* 入口参数：所在城市编号
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:30	基本功能完成
******************************************************************************/
 U8 ConfiscateMake(U8 city)
{
	U8 *pqptr;
	U8 *str;
	U8 pcount;
	U8 pcode;
	U8 p,g,c;
	U8 gq[3],*gptr;
	
	pqptr = SHARE_MEM;
	do
	{
		pcount = GetCityPersons(city,pqptr);
		if (!pcount)
		{
			ShowConstStrMsg(STR_NOPERSON);
			break;
		}
		/*gam_clrlcd(WK_SX,WK_SY,WK_EX,WK_EY);*/
		ShowMapClear();
		pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
		if (0xff != pcode)
		{
			p = pqptr[pcode];
			c = 0;
			gptr = g_Persons[p].Equip;
			if (!gptr[0])
			{
				gptr[0] = gptr[1];
				gptr[1] = 0;
			}
			else
			{
				c = 1;
			}
			
			if (gptr[c])
			{
				c += 1;
			}
			gq[0] = gptr[0] - 1;
			gq[1] = gptr[1] - 1;
			if (c)
			{
				/*gam_clrlcd(WK_SX,WK_SY,WK_EX,WK_EY);*/
				ShowMapClear();
				g = ShowGoodsControl(gq,c,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
				if (0xff == g)
				{
					continue;
				}
				else
				{
					/*添加没收成功代码*/
					if (p != g_PlayerKing)
					{
						str = SHARE_MEM + 300;
                        ResLoadToMem(STRING_CONST,P_SAY_STR7 + (rand() % 3),str);
						ShowMapClear();
						ShowGReport(p,str);
						if (g_Persons[p].Devotion < 20)
							g_Persons[p].Devotion = 0;
						else
							g_Persons[p].Devotion -= 20;
					}
					AddGoods(city,gq[g]);
					SetGoods(gq[g]);
					DelGoodsPerson(gq[g],p);
					gptr[g] = 0;
				}
			}
			else
			{
				/*提示该武将没道具*/
				ShowConstStrMsg(STR_NOGOODS);
				continue;
			}
		}
		else
		{
			return(1);
		}
	} while (1);
	return(1);
}

/******************************************************************************
* 函数名:DepredateMake
* 说  明:“掠夺”命令生成
*
* 入口参数：所在城市编号
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:31	基本功能完成
******************************************************************************/
 U8 DepredateMake(U8 city)
{
	U8 *str;
	U8 *pqptr;
	U8 pcount;
	U8 pcode;
	U8 p;
	OrderType order;
	
	pqptr = SHARE_MEM;
	
	do
	{
		pcount = GetCityPersons(city,pqptr);
		if (!pcount)
		{
			ShowConstStrMsg(STR_NOPERSON);
			break;
		}
		pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
		if (0xff != pcode)
		{
			p = pqptr[pcode];
			if (!IsManual(p,CONSCRIPTION))
			{
				ShowConstStrMsg(NOTE_STR9);
				continue;
			}
			OrderConsumeThew(p,CONSCRIPTION);
			
			str = SHARE_MEM + 300;
			ResLoadToMem(STRING_CONST,P_SAY_STR27,str);
			ShowGReport(p,str);
			
			order.OrderId = DEPREDATE;
			order.Person = p;
			order.City = city;
			order.TimeCount = 0;
			AddOrderHead(&order);
			DelPerson(city,p);
		}
		break;
	} while (1);
	
	
	return(1);
}

/******************************************************************************
* 函数名:进攻提示
* 说  明:ShowAttackNote
*
* 入口参数：ap -- 进攻方，city -- 被进攻城市
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-8-23 11:00	基本功能完成
******************************************************************************/
 void ShowAttackNote(U8 ap,U8 city)
{
	U8 *str,*buf;
	
	ShowFightNoteFace(0);

	str = SHARE_MEM + 300;
	buf = SHARE_MEM + 340;
	if (ap == g_PlayerKing)
	{
		ResLoadToMem(STRING_CONST,STR_OURS,str);	
	}
	else
	{
		GetPersonName(ap,str);
	}
	ResLoadToMem(STRING_CONST,STR_ATTACKC,buf);
	gam_strcat(str,buf);
	GetCityName(city,buf);
	gam_strcat(str,buf);
	PlcMidShowStr(WK_SX + (WK_EX  - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 2 + 64 + 2,str);
    //GamDelay(100,false);
    GamDelay(100,true);
}


/******************************************************************************
* 函数名:战斗提示
* 说  明:ShowFightNote
*
* 入口参数：ap -- 进攻方，bc -- 被进攻方
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-8-23 11:19	基本功能完成
******************************************************************************/
 void ShowFightNote(U8 ap,U8 bc)
{
	U8 *str,*buf;
	
	ShowFightNoteFace(1);

	str = SHARE_MEM + 300;
	buf = SHARE_MEM + 340;
	GetPersonName(ap,str);
	ResLoadToMem(STRING_CONST,STR_FIGHT_NOTE1,buf);
	gam_strcat(str,buf);
	GetPersonName(bc,buf);
	gam_strcat(str,buf);
	ResLoadToMem(STRING_CONST,STR_FIGHT_NOTE2,buf);
	gam_strcat(str,buf);
	PlcMidShowStr(WK_SX + (WK_EX  - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 2 + 64 + 2,str);
    //GamDelay(300,false);
    GamDelay(100,true);
}

/******************************************************************************
* 函数名:战斗胜利提示
* 说  明:ShowFightWinNote
*
* 入口参数：战胜方
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-8-23 11:27	基本功能完成
******************************************************************************/
 void ShowFightWinNote(U8 wp)
{
	U8 *str,*buf;
	
	ShowFightNoteFace(2);

	str = SHARE_MEM + 300;
	buf = SHARE_MEM + 340;
	
	if (wp == g_PlayerKing)
	{
		ResLoadToMem(STRING_CONST,STR_OURS,str);	
	}
	else
	{
		GetPersonName(wp,str);
	}
	ResLoadToMem(STRING_CONST,STR_WON,buf);
	gam_strcat(str,buf);
	PlcMidShowStr(WK_SX + (WK_EX  - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 2 + 64 + 2,str);
    //GamDelay(300,false);
    GamDelay(100,true);
}

/******************************************************************************
* 函数名:我方战斗失败提示
* 说  明:ShowFightLossNote
*
* 入口参数：无
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-8-23 11:32	基本功能完成
******************************************************************************/
 void ShowFightLossNote(void)
{
	U8 *str,*buf;
	
	ShowFightNoteFace(3);

	str = SHARE_MEM + 300;
	buf = SHARE_MEM + 340;
	
	ResLoadToMem(STRING_CONST,STR_OURS,str);
	
	ResLoadToMem(STRING_CONST,STR_LOST,buf);
	gam_strcat(str,buf);
	PlcMidShowStr(WK_SX + (WK_EX  - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 2 + 64 + 2,str);
	GamDelay(300,false);
    GamDelay(100,true);
}

/******************************************************************************
* 函数名:ShowFightNoteFace
* 说  明:
*
* 入口参数：图片序号
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005/5/18 11:26AM	基本功能完成
******************************************************************************/
void ShowFightNoteFace(U8 idx)
{
	U8 *pic;
	
	gam_clrlcd(WK_SX + (WK_EX - 120 - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2,WK_SX + (WK_EX - 120 - WK_SX) / 2 + 120,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 84);
	gam_rect(WK_SX + (WK_EX - 120 - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2,WK_SX + (WK_EX - 120 - WK_SX) / 2 + 120,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 84);
	pic = ResLoadToCon(FIGHT_NOTE_ICON,1,g_CBnkPtr);
	GamPicShowExS(WK_SX + (WK_EX - 84 - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 2,84,64,idx,pic);
}

/******************************************************************************
* 函数名:IsMoney
* 说  明:判断指定城市是否有足够的金钱执行指定命令
*
* 入口参数：city -- 指定城市，order -- 指定命令
*
* 出口参数：1 -- 足够，0 -- 不足
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-7-28 21:44	基本功能完成
******************************************************************************/
 U8 IsMoney(U8 city,U8 order)
{
	U8 *ptr;
	
    ptr = ResLoadToCon(IFACE_CONID,ConsumeMoney,g_CBnkPtr);
	if (g_Cities[city].Money >= ptr[order])
		return(1);
	else
		return(0);
}

/******************************************************************************
* 函数名:OrderConsumeMoney
* 说  明:指定城市消耗金钱执行指定命令
*
* 入口参数：city -- 指定成都市，order -- 指定命令
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-7-28 21:49	基本功能完成
******************************************************************************/
 void OrderConsumeMoney(U8 city,U8 order)
{
	U8 *ptr;
	
    ptr = ResLoadToCon(IFACE_CONID,ConsumeMoney,g_CBnkPtr);
	g_Cities[city].Money -= ptr[order];
}

/******************************************************************************
* 函数名:IsManual
* 说  明:判断指定武将是否有足够的体力执行指定命令
*
* 入口参数：person -- 指定武将，order -- 指定命令
*
* 出口参数：1 -- 足够，0 -- 不足
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-7-28 20:18	基本功能完成
******************************************************************************/
 U8 IsManual(U8 person,U8 order)
{
	U8 *ptr;
	
    ptr = ResLoadToCon(IFACE_CONID,ConsumeThew,g_CBnkPtr);
	if (g_Persons[person].Thew >= ptr[order])
		return(1);
	else
		return(0);
}

/******************************************************************************
* 函数名:OrderConsumeThew
* 说  明:指定武将消耗体力执行指定命令
*
* 入口参数：person -- 指定武将，order -- 指定命令
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-7-28 20:18	基本功能完成
******************************************************************************/
 void OrderConsumeThew(U8 person,U8 order)
{
	U8 *ptr;
	
    ptr = ResLoadToCon(IFACE_CONID,ConsumeThew,g_CBnkPtr);
	g_Persons[person].Thew -= ptr[order];
}


// {
// 	U8 c,i,cw;
// 	U8 p,o;
// 	U16 armst,armsw;
// 	CityType *cptr;
// 	OrderType *inode;
	
// 	inode = (OrderType *) ORDERQUEUE;
// 	armsw = 0xffff;
// 	for (c = 0;c < count;c ++)
// 	{
// 		cptr = &g_Cities[cqueue[c]];
// 		armst = 0;
// 		for (i = 0;i < cptr->Persons;i ++)
// 		{
// 			p = g_PersonsQueue[cptr->PersonQueue + i];
// 			if (g_Persons[p].Belong == cptr->Belong)
// 			{
// 				armst += g_Persons[p].Arms;
// 			}
// 		}
// 		for (i = 0;i < ORDER_MAX;i ++)
// 		{
// 			o = inode[i].OrderId;
// 			if (0xff == o)
// 				continue;
			
// 			if ((o != MOVE) && (o != BATTLE) && inode[i].City == cqueue[c])
// 			{
// 				armst += g_Persons[inode[i].Person].Arms;
// 			}
// 		}
// 		if (armsw > armst)
// 		{
// 			armsw = armst;
// 			cw = c;
// 		}
// 	}
// 	return(cqueue[cw]);
// }
