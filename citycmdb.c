/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	citycmdb.c
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
*	1.0    2005-6-3 14:45  陈泽伟     基本的功能完成
***********************************************************************/

#include "stdsys.h"
#include "comm.h"
#undef	CITYCMDB_C
#define	CITYCMDB_C
#include "enghead.h"

extern int g_stat;
extern int ngsecond1;

/******************************************************************************
* 函数名:OrderMenu
* 说  明:显示命令选择菜单
*
* 入口参数：无
*
* 出口参数：0xff -- 退出，其他值 -- 选中项
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-13 15:21	基本功能完成
******************************************************************************/
 U8 OrderMenu(void)
{
	U8 rav;
	
	while (1)
	{
        //usleep(1000);
        //GamShowFrame(g_VisScr);
        //ShowCityMap(&g_CityPos,0);
        ShowMapClear();
        g_stat = 0;
		switch(MainOrderMenu())
		{
		case 0:		/*内政指令*/
            g_stat = 0;
            ngsecond1 = 1;
			rav = InteriorOrderMenu();
			if (0xff != rav)
				return(ORDER_INTERIOR + rav);
			break;
		case 1:		/*外交指令*/
            g_stat = 0;
            ngsecond1 = 2;
			rav = DiplomatismOrderMenu();
			if (0xff != rav)
				return(ORDER_DIPLOMATISM + rav);
			break;
		case 2:		/*军备指令*/
            g_stat = 0;
            ngsecond1 = 3;
			rav = ArmamentOrderMenu();
			if (0xff != rav)
				return(ORDER_ARMAMENT + rav);
			break;
		case 3:		/*城市状况*/
			return(0xfe);
			break;
		case 0xff:
			return(0xff);
		}
        ngsecond1 = 0;
	}
}

/******************************************************************************
* 函数名:MainOrderMenu
* 说  明:显示主命令选择菜单
*
* 入口参数：无
*
* 出口参数：0xff -- 退出，其他值 -- 选中项
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-13 15:33	基本功能完成
******************************************************************************/
U8 MainOrderMenu(void)
{

	RECT pRect;
	U8 mstr[25];
	ResLoadToMem(STRING_CONST,MENU_MAINSTR,mstr);
	pRect.sx = WK_SX + WK_EX / 3;
	pRect.ex = WK_SX + WK_EX / 3 + ASC_WID * 4;
	pRect.sy = WK_SY + 10;
#if ((WK_EY - (WK_SY + 10)) > (ASC_HGT * 4 + 4))
	pRect.ey = WK_SY + 10 + ASC_HGT * 4;
#else
	pRect.ey = WK_SY + 10 + ((WK_EY - (WK_SY + 10)) / ASC_HGT) * ASC_HGT;
#endif
	return(PlcSplMenu(&pRect,0,mstr));
	
}

/******************************************************************************
* 函数名:InteriorOrderMenu
* 说  明:显示内政命令菜单
*
* 入口参数：无
*
* 出口参数：0xff -- 退出，其他值 -- 选中项
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-13 15:33	基本功能完成
******************************************************************************/
U8 InteriorOrderMenu(void)
{
	RECT pRect;
	U8 mstr[85];
	ResLoadToMem(STRING_CONST,MENU_INTERIORSTR,mstr);
	pRect.sx = WK_SX + WK_EX / 2;
	pRect.ex = WK_SX + WK_EX / 2 + ASC_WID * 4;
	pRect.sy = WK_SY + 10 + 4;
#if ((WK_EY - (WK_SY + 10 + 4)) > (ASC_HGT * 14 + 4))
	pRect.ey = WK_SY + 10 + ASC_HGT * 14;
#else
	pRect.ey = WK_SY + 10 + 4 + ((WK_EY - (WK_SY + 10 + 4)) / ASC_HGT) * ASC_HGT;
#endif
	return(PlcSplMenu(&pRect,0,mstr));
	
}

/******************************************************************************
* 函数名:DiplomatismOrderMenu
* 说  明:显示外交命令菜单
*
* 入口参数：无
*
* 出口参数：0xff -- 退出，其他值 -- 选中项
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-13 15:36	基本功能完成
******************************************************************************/
U8 DiplomatismOrderMenu(void)
{
	
	RECT pRect;
	U8 mstr[30];
	ResLoadToMem(STRING_CONST,MENU_DIPLOMATISMSTR,mstr);
	pRect.sx = WK_SX + WK_EX / 2;
	pRect.ex = WK_SX + WK_EX / 2 + ASC_WID * 4;
	pRect.sy = WK_SY + 10 + 4;
#if ((WK_EY - (WK_SY + 10 + 4)) > (ASC_HGT * 5 + 4))
	pRect.ey = WK_SY + 10 + 4 + ASC_HGT * 5;
#else
	pRect.ey = WK_SY + 10 + 4 + ((WK_EY - (WK_SY + 10 + 4)) / ASC_HGT) * ASC_HGT;
#endif
	return(PlcSplMenu(&pRect,0,mstr));
	
}

/******************************************************************************
* 函数名:ArmamentOrderMenu
* 说  明:显示军备命令菜单
*
* 入口参数：无
*
* 出口参数：0xff -- 退出，其他值 -- 选中项
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-13 15:36	基本功能完成
******************************************************************************/
U8 ArmamentOrderMenu(void)
{
	
	
	RECT pRect;
	U8 mstr[30];
	ResLoadToMem(STRING_CONST,MENU_ARMAMENTSTR,mstr);
	pRect.sx = WK_SX + WK_EX / 2;
	pRect.ex = WK_SX + WK_EX / 2 + ASC_WID * 4;
	pRect.sy = WK_SY + 10 + 4;
#if ((WK_EY - (WK_SY + 10 + 4)) > (ASC_HGT * 5 + 4))
	pRect.ey = WK_SY + 10 + 4 + ASC_HGT * 5;
#else
	pRect.ey = WK_SY + 10 + 4 + ((WK_EY - (WK_SY + 10 + 4)) / ASC_HGT) * ASC_HGT;
#endif
	return(PlcSplMenu(&pRect,0,mstr));
	
}

/******************************************************************************
* 函数名:CityCommon
* 说  明:选中城市下达命令
*
* 入口参数：city -- 城市编号，order -- 城市编号
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-16 14:39	基本功能完成
******************************************************************************/
 U8 CityCommon(U8 city,U8 cmd)
{
	
	if (CITY_MAX <= city)
		return(0);
	
	/*while (1)
	{
		switch(OrderMenu())*/
		switch(cmd)
		{
		/*内政指令*/
		/*case APPOINT:		任命
			break;*/
		case ASSART:		/*开垦*/
			AssartMake(city);
			break;
		case ACCRACTBUSINESS:	/*招商*/
			AccractbusinessMake(city);
			break;
		case SEARCH:		/*搜寻*/
			SearchMake(city);
			break;
		case FATHER:		/*治理*/
			FatherMake(city);
			break;
		case INSPECTION:	/*出巡*/
			InspectionMake(city);
			break;
		case SURRENDER:		/*招降*/
			SurrenderMake(city);
			break;
		case KILL:		/*处斩*/
			KillMake(city);
			break;
		case BANISH:		/*流放*/
			BanishMake(city);
			break;
		case LARGESS:		/*赏赐*/
			LargessMake(city);
			break;
		case CONFISCATE:	/*没收*/
			ConfiscateMake(city);
			break;
		case EXCHANGE:		/*交易*/
			ExchangeMake(city);
			break;
		case TREAT:		/*宴请*/
			TreatMake(city);
			break;
		case TRANSPORTATION:	/*输送*/
			TransportationMake(city);
			break;
		case MOVE:		/*移动*/
			MoveMake(city);
			break;
			
		/*外交指令*/
		case ALIENATE:		/*离间*/
			AlienateMake(city);
			break;
		case CANVASS:		/*招揽*/
			CanvassMake(city);
			break;
		case COUNTERESPIONAGE:	/*策反*/
			CounterespiongeMake(city);
			break;
		case REALIENATE:	/*反间*/
		    RealienateMake(city);
			break;
		case INDUCE:		/*劝降*/
			InduceMake(city);
			break;
		/*case TRIBUTE:		朝贡
			break;*/
	
		/*军备指令*/
		case RECONNOITRE:	/*侦察*/
			ReconnoitreMake(city);
			break;
		case CONSCRIPTION:	/*征兵*/
			ConscriptionMake(city);
			break;
		case DISTRIBUTE:	/*分配*/
			DistributeMake(city);
			break;
		case DEPREDATE:		/*掠夺*/
			DepredateMake(city);
			break;
		case BATTLE:		/*出征*/
			BattleMake(city);
				
			break;
		
		/*case 0xff:
			break;*/
		}
	/*}*/
	return(1);
}

/******************************************************************************
* 函数名:AppointDrv
* 说  明:“任命”命令生成
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
/* U8 AppointMake(U8 city)
{
	return(1);
}*/


/******************************************************************************
* 函数名:AppointMake
* 说  明:“开垦”命令生成
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
 U8 AssartMake(U8 city)
{
	U8 *pqptr;
	U8 pcount;
	U8 pcode;
	U8 p;
	U16 *f,fl,fa;
	OrderType order;
	
	pqptr = SHARE_MEM;
	pcount = GetCityPersons(city,pqptr);
	if (!pcount)
	{
		ShowConstStrMsg(STR_NOPERSON);
		return(1);
	}
	do
    {
		f = &g_Cities[city].Farming;
		fl = g_Cities[city].FarmingLimit;
		if (*f >= fl)
		{
			/*提示已达上限*/
			ShowConstStrMsg(NOTE_STR1);
			return(1);
		}
		
		if (!IsMoney(city,ASSART))
		{
			/*金钱不足*/
			ShowConstStrMsg(NOTE_STR8);
			return(1);
		}
		
		pcount = GetCityPersons(city,pqptr);
		if (!pcount)
		{
			/*ShowConstStrMsg(STR_NOPERSON);*/
			break;
		}
		do
		{
			ShowMapClear();
            g_stat = 0;
			pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
            if (0xff != pcode)
			{
				p = pqptr[pcode];
				if (!IsManual(p,ASSART))
				{
					ShowConstStrMsg(NOTE_STR9);
					continue;
				}
				
				fa = g_Persons[p].IQ / 10 * (rand() % 4 + 2) + (g_Persons[p].IQ >> 1);
				*f += fa;
				if (*f > fl)
				{
					*f = fl;
				}
				ShowDMsg(city,p,fa,0);
				OrderConsumeThew(p,ASSART);
				OrderConsumeMoney(city,ASSART);
				order.OrderId = ASSART;
				order.Person = p;
				order.City = city;
				order.TimeCount = 0;
				AddOrderHead(&order);
				DelPerson(city,p);
				break;
			}
			else
			{
				return(1);
			}
			break;
		} while (1);
	} while (1);
	return(1);
}

/******************************************************************************
* 函数名:AppointMake
* 说  明:“招商”命令生成
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
 U8 AccractbusinessMake(U8 city)
{
	U8 *pqptr;
	U8 pcount;
	U8 pcode;
	U8 p;
	U16 *c,cl,ca;
	OrderType order;
	
	pqptr = SHARE_MEM;
	
	pcount = GetCityPersons(city,pqptr);
	if (!pcount)
	{
		ShowConstStrMsg(STR_NOPERSON);
		return(1);
	}
	do
    {
		c = &g_Cities[city].Commerce;
		cl = g_Cities[city].CommerceLimit;
		if (*c >= cl)
		{
			/*提示已达上限*/
			ShowConstStrMsg(NOTE_STR2);
			return(1);
		}
		
		if (!IsMoney(city,ACCRACTBUSINESS))
		{
			/*金钱不足*/
			ShowConstStrMsg(NOTE_STR8);
			return(1);
		}
		
		pcount = GetCityPersons(city,pqptr);
		if (!pcount)
		{
			/*ShowConstStrMsg(STR_NOPERSON);*/
			break;
		}
		do
		{
			ShowMapClear();
			pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
			if (0xff != pcode)
			{
				p = pqptr[pcode];
				if (!IsManual(p,ACCRACTBUSINESS))
				{
					ShowConstStrMsg(NOTE_STR9);
					continue;
				}
				
				ca = g_Persons[p].IQ / 10 * (rand() % 4 + 2) + g_Persons[p].IQ >> 1;
				*c += ca;
				if (*c > cl)
				{
					*c = cl;
				}
				ShowDMsg(city,p,ca,1);
				OrderConsumeThew(p,ACCRACTBUSINESS);
				OrderConsumeMoney(city,ACCRACTBUSINESS);
				order.OrderId = ACCRACTBUSINESS;
				order.Person = p;
				order.City = city;
				order.TimeCount = 0;
				AddOrderHead(&order);
				DelPerson(city,p);
			}
			else
			{
				return(1);
			}
			break;
		} while (1);
	} while (1);
	
	return(1);
}

/******************************************************************************
* 函数名:ShowDMsg
* 说  明:显示开发信息
*
* 入口参数：city -- 城市，person -- 开发人，upval -- 提升值，flag -- （0 -- 农业 1 -- 商业 3 -- 民忠 3 -- 防灾）
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-8-16 15:38	基本功能完成
******************************************************************************/
void ShowDMsg(U8 city,U8 person,U8 upval,U8 flag)
{
	U8 *str,*astr;
	
	str = SHARE_MEM + 300;
	astr = SHARE_MEM + 350;
	
	switch (flag)
	{
	case 0:
		ResLoadToMem(STRING_CONST,STR_RP3,str);
		gam_itoa(g_Cities[city].Farming,astr,10);
		break;
	case 1:
		ResLoadToMem(STRING_CONST,STR_RP6,str);
		gam_itoa(g_Cities[city].Commerce,astr,10);
		break;
	case 2:
		ResLoadToMem(STRING_CONST,STR_RP7,str);
		gam_itoa(g_Cities[city].PeopleDevotion,astr,10);
		break;
	case 3:
		ResLoadToMem(STRING_CONST,STR_RP8,str);
		gam_itoa(g_Cities[city].AvoidCalamity,astr,10);
		break;
	}
	gam_strcat(str,astr);
	ResLoadToMem(STRING_CONST,STR_RP4,astr);
	gam_strcat(str,astr);
	gam_itoa(upval,astr,10);
	gam_strcat(str,astr);
	ResLoadToMem(STRING_CONST,STR_RP5,astr);
	gam_strcat(str,astr);
	ShowMapClear();
	ShowGReport(person,str);
}


/******************************************************************************
* 函数名:AppointMake
* 说  明:“搜寻”命令生成
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
 U8 SearchMake(U8 city)
{
	U8 *str;
	U8 *pqptr;
	U8 pcount;
	U8 pcode;
	U8 p,psay;
	OrderType order;
	
	pqptr = SHARE_MEM;
	
	pcount = GetCityPersons(city,pqptr);
	if (!pcount)
	{
		ShowConstStrMsg(STR_NOPERSON);
		return(1);
	}
	do
	{
		if (!IsMoney(city,SEARCH))
		{
			/*金钱不足*/
			ShowConstStrMsg(NOTE_STR8);
			return(1);
		}
		
		pcount = GetCityPersons(city,pqptr);
		if (!pcount)
		{
			/*ShowConstStrMsg(STR_NOPERSON);*/
			break;
		}
		do
		{
			ShowMapClear();
			pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
			if (0xff != pcode)
			{
				p = pqptr[pcode];
				if (!IsManual(p,SEARCH))
				{
					ShowConstStrMsg(NOTE_STR9);
					continue;
				}
				
				OrderConsumeThew(p,SEARCH);
				OrderConsumeMoney(city,SEARCH);
				
				psay = rand() % 3;
				if (p == g_PlayerKing)
				{
					psay += P_SAY_STR24;
				}
				else
				{
					psay += P_SAY_STR21;
				}
				str = SHARE_MEM + 300;
				ResLoadToMem(STRING_CONST,psay,str);
				ShowGReport(p,str);
				
				order.OrderId = SEARCH;
				order.Person = p;
				order.City = city;
				order.TimeCount = 0;
				AddOrderHead(&order);
				DelPerson(city,p);
			}
			else
			{
				return(1);
			}
			break;
		} while (1);
	} while (1);
	
	return(1);
}

/******************************************************************************
* 函数名:FatherMake
* 说  明:“治理”命令生成
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
 U8 FatherMake(U8 city)
{
	U8 *pqptr;
	U8 pcount;
	U8 pcode;
	U8 p,rnd;
	OrderType order;
	
	pqptr = SHARE_MEM;
	
	pcount = GetCityPersons(city,pqptr);
	if (!pcount)
	{
		ShowConstStrMsg(STR_NOPERSON);
		return(1);
	}
	do
	{
		if (!IsMoney(city,STATE_NORMAL))
		{
			/*金钱不足*/
			ShowConstStrMsg(NOTE_STR8);
			return(1);
		}
		
		if (STATE_NORMAL == g_Cities[city].Id && g_Cities[city].AvoidCalamity >= 100)
		{
			ShowConstStrMsg(NOTE_STR10);
			return(1);
		}
		
		pcount = GetCityPersons(city,pqptr);
		if (!pcount)
		{
			/*ShowConstStrMsg(STR_NOPERSON);*/
			break;
		}
		do
		{
			ShowMapClear();
			pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
			if (0xff != pcode)
			{
				p = pqptr[pcode];
				if (!IsManual(p,STATE_NORMAL))
				{
					ShowConstStrMsg(NOTE_STR9);
					continue;
				}
				
				OrderConsumeThew(p,STATE_NORMAL);
				OrderConsumeMoney(city,STATE_NORMAL);
				
				rnd = 1 + rand() % 4;
				g_Cities[city].Id = STATE_NORMAL;
				g_Cities[city].AvoidCalamity += rnd;
				if (g_Cities[city].AvoidCalamity > 100)
					g_Cities[city].AvoidCalamity = 100;
					
				ShowDMsg(city,p,rnd,3);
				order.OrderId = FATHER;
				order.Person = p;
				order.City = city;
				order.TimeCount = 0;
				AddOrderHead(&order);
				DelPerson(city,p);
			}
			else
			{
				return(1);
			}
			break;
		} while (1);
	} while (1);
	return(1);
}

/******************************************************************************
* 函数名:InspectionMake
* 说  明:“出巡”命令生成
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
 U8 InspectionMake(U8 city)
{
	U8 *pqptr;
	U8 pcount;
	U8 pcode;
	U8 p;
	U8 rnd;
	OrderType order;
	U8 *PeopleDevotion;
	U32 *Population;
	
	pqptr = SHARE_MEM;
	
	pcount = GetCityPersons(city,pqptr);
	if (!pcount)
	{
		ShowConstStrMsg(STR_NOPERSON);
		return(1);
	}
	do
	{
		if (!IsMoney(city,INSPECTION))
		{
			/*金钱不足*/
			ShowConstStrMsg(NOTE_STR8);
			return(1);
		}
		
		pcount = GetCityPersons(city,pqptr);
		if (!pcount)
		{
			/*ShowConstStrMsg(STR_NOPERSON);*/
			break;
		}
		do
		{
			ShowMapClear();
			pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
			if (0xff != pcode)
			{
				p = pqptr[pcode];
				if (!IsManual(p,INSPECTION))
				{
					ShowConstStrMsg(NOTE_STR9);
					continue;
				}
			
				OrderConsumeThew(p,INSPECTION);
				OrderConsumeMoney(city,INSPECTION);
				
				/*出巡作用代码*/
				PeopleDevotion = &g_Cities[city].PeopleDevotion;
				rnd = 1 + rand() % 4;
				*PeopleDevotion += rnd;
				if (*PeopleDevotion > 100)
					*PeopleDevotion = 100;
				Population = &g_Cities[city].Population;
				*Population += 100;
				if (*Population > g_Cities[city].PopulationLimit)
					*Population = g_Cities[city].PopulationLimit;
					
				ShowDMsg(city,p,rnd,2);
				order.OrderId = INSPECTION;
				order.Person = p;
				order.City = city;
				order.TimeCount = 0;
				AddOrderHead(&order);
				DelPerson(city,p);
			}
			else
			{
				return(1);
			}
			break;
		} while (1);
	} while (1);
	
	return(1);
}

/******************************************************************************
* 函数名:SurrenderMake
* 说  明:“招降”命令生成
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
 U8 SurrenderMake(U8 city)
{
	U8 *str;
	U8 *pqptr;
	U8 pcount;
	U8 pcode;
	U8 pp,p;
	OrderType order;
	
	pqptr = SHARE_MEM;
	
	pcount = GetCityPersons(city,pqptr);
	if (!pcount)
	{
		ShowConstStrMsg(STR_NOPERSON);
		return(1);
	}
	do
	{
		pcount = GetCityCaptives(city,pqptr);
		if (!pcount)
		{
			/*提示城中无俘虏*/
			ShowConstStrMsg(STR_NOCAPTIVES);
			break;
		}
		
		ShowMapClear();
		pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
		if (0xff != pcode)
		{
			pp = pqptr[pcode];
			
			pcount = GetCityPersons(city,pqptr);
			if (!pcount)
			{
				/*ShowConstStrMsg(STR_NOPERSON);*/
				break;
			}
			do
			{
				ShowMapClear();
				pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
				if (0xff != pcode)
				{
					p = pqptr[pcode];
					if (!IsManual(p,SURRENDER))
					{
						ShowConstStrMsg(NOTE_STR9);
						continue;
					}
				
					OrderConsumeThew(p,SURRENDER);
					
					order.OrderId = SURRENDER;
					order.Person = p;
					order.City = city;
					order.Object = pp;
					order.TimeCount = 0;
					AddOrderHead(&order);
					DelPerson(city,p);
					str = SHARE_MEM;
					ResLoadToMem(STRING_CONST,P_SAY_STR41,str);
					ShowMapClear();
					ShowGReport(p,str);
				}
				break;
			} while (1);
		}
		else
		{
			break;
		}
	} while (1);
	
	return(1);
}

/******************************************************************************
* 函数名:KillMake
* 说  明:“处斩”命令生成
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
 U8 KillMake(U8 city)
{
	U8 *pqptr;
	U8 pcount;
	U8 pcode;
	U8 *gptr;
	U8 *str;
	
	pqptr = SHARE_MEM;
	
	pcount = GetCityCaptives(city,pqptr);
	if (!pcount)
	{
		/*提示城中无俘虏*/
		ShowConstStrMsg(STR_NOCAPTIVES);
	}
	pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
	if (0xff != pcode)
	{
		/*添加处斩提示代码*/
		gptr = g_Persons[pqptr[pcode]].Equip;
		if (gptr[0])
		{
			AddGoods(city,gptr[0] - 1);
			SetGoods(gptr[0] - 1);
		}
		if (gptr[1])
		{
			AddGoods(city,gptr[1] - 1);
			SetGoods(gptr[1] - 1);
		}
		str = SHARE_MEM + 300;
		ResLoadToMem(STRING_CONST,P_SAY_STR4 + (rand() % 3),str);
		ShowMapClear();
		ShowGReport(pqptr[pcode],str);
		DelPerson(city,pqptr[pcode]);
	}
	
	return(1);
}

/******************************************************************************
* 函数名:BanishMake
* 说  明:“流放”命令生成
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
 U8 BanishMake(U8 city)
{
	U8 *pqptr;
	U8 pcount;
	U8 pcode;
	U8 p;
	U8 rnd;
	
	pqptr = SHARE_MEM;
	
	pcount = GetCityPersons(city,pqptr);
	pcount += GetCityCaptives(city,&pqptr[pcount]);
	if (!pcount)
	{
		ShowConstStrMsg(STR_NOPERSON);
		return(1);
	}
	pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
	if (0xff != pcode)
	{	
		p = pqptr[pcode];
		g_Persons[p].Belong = 0;
		rnd = rand() % CITY_MAX;
		if (0xff == g_Persons[p].Belong)
		{
			if (g_Persons[p].Id)
			{
				pcount = GetKingCitys(g_Persons[p].Id - 1,pqptr);
				if (pcount)
				{
					g_Persons[p].Belong = g_Persons[p].Id;
					rnd = rand() % pcount;
				}
			}
		}
		
		DelPerson(city,p);
		AddPerson(rnd,p);
	}
	
	return(1);
}

/******************************************************************************
* 函数名:LargessMake
* 说  明:“赏赐”命令生成
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
 U8 LargessMake(U8 city)
{
	U8 *str;
	U8 *gqptr;
	U8 gcount;
	U8 gcode;
	U8 *pqptr;
	U8 pcount;
	U8 pcode;
	U8 p;
	U8 *eq;
	U8 g,gi;
	
	gqptr = SHARE_MEM + 200;
	gcount = GetCityPGoods(city,gqptr);
	if (gcount)
	{
		gcode = ShowGoodsControl(gqptr,gcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
		if (0xff == gcode)
		{
			return(1);
		}
		
		do
		{
			pqptr = SHARE_MEM;
			pcount = GetCityPersons(city,pqptr);
			if (!pcount)
			{
				ShowConstStrMsg(STR_NOPERSON);
				break;
			}
			pcode = ShowPersonControl(pqptr,pcount,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
			if (0xff != pcode)
			{
				g = gqptr[gcode];
				p = pqptr[pcode];
				eq = g_Persons[p].Equip;
				if (!(eq[0]))
				{
					gi = 0;
				}
				else if (!(eq[1]))
				{
					gi = 1;
				}
				else
				{
					/*提示该武将道具已满*/
					ShowConstStrMsg(STR_FULLGOODS);
					continue;
				}
				switch (AddGoodsPerson(g,p))
				{
				case 0:
					eq[gi] = g + 1;
					break;
				case 1:
					break;
				case 0xff:
					/*提示无法使用*/
					ShowConstStrMsg(STR_CANNOTUSE);
					continue;
					break;
				}
				/*添加赏赐成功代码*/
				DelGoods(city,g);
				if (p != g_PlayerKing)
				{
					str = SHARE_MEM + 300;
					ResLoadToMem(STRING_CONST,P_SAY_STR10 + (rand() % 3),str);
					ShowMapClear();
					ShowGReport(p,str);
					g_Persons[p].Devotion += 8;
					if (g_Persons[p].Devotion > 100)
						g_Persons[p].Devotion = 100;
				}
				break;
			}
			else
			{
				break;
			}
		}while (1);
	
	}
	else
	{
		/*提示城中无道具*/
		ShowConstStrMsg(STR_CNOGOODS);
	}
	return(1);
}
