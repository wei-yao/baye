/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	cityedit.c
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
#undef	CITYCMD_C
#define	CITYCMD_C
#include "enghead.h"
/*U8 MainOrderMenu(void);
U8 InteriorOrderMenu(void);
U8 DiplomatismOrderMenu(void);
U8 ArmamentOrderMenu(void);

 U8 AddPerson(U8 city,U8 person);
 U8 DelPerson(U8 city,U8 person);
 U8 AddOrderHead(OrderType *Order);
 U8 AddOrderEnd(OrderType *Order);
 U8 DelOrder(OrderQueueType *Order);
 U8 GetKingCitys(U8 king,U8 *cqueue);
 U8 GetPersonCity(U8 person);
 U8 GetCityPersons(U8 city,U8 *pqueue);
 U8 GetCityCaptives(U8 city,U8 *pqueue);
 U8 GetCityOutPersons(U8 city,U8 *pqueue);
 U8 MenuComm(MenuType *menu);
 U8 ShowPersonControl(U8 *person,U8 pcount,U8 x0,U8 y0,U8 x1,U8 y1);
 U16 GetFood(U16 min,U16 max);
 U16 GetMoney(U16 min,U16 max);
 U16 GetArmy(U16 min,U16 max);
 U8 GetCitySet(CitySetType *pos);
 U8 SearchRoad(U8 sc,U8 xs,U8 ys,U8 ob,U8 xo,U8 yo);*/

/******************************************************************************
* 函数名:AppointDrv
* 说  明:“任命”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:30	基本功能完成
******************************************************************************/
/*U8 AppointDrv(OrderType *Order)
{
	return(1);
}*/


/******************************************************************************
* 函数名:AssartDrv
* 说  明:“开垦”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:30	基本功能完成
******************************************************************************/
U8 AssartDrv(OrderType *Order)
{
	AddPerson(Order->City,Order->Person);
	return(1);
}

/******************************************************************************
* 函数名:AccractbusinessDrv
* 说  明:“招商”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:30	基本功能完成
******************************************************************************/
U8 AccractbusinessDrv(OrderType *Order)
{
	AddPerson(Order->City,Order->Person);
	return(1);
}

/******************************************************************************
* 函数名:SearchDrv
* 说  明:“搜寻”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:30	基本功能完成
******************************************************************************/
U8 SearchDrv(OrderType *Order)
{
	U8 *str;
	U8 *astr;
	U8 *pqptr;
	U8 *infp;
	U8 sp,person,pb;
	U8 p,fo,iq;
	U8 pcount;
	U8 rnd;
	U8 pss;
	U16 count;
	
	ShowMapClear();
	pqptr = SHARE_MEM;
	str = SHARE_MEM;
	astr = SHARE_MEM + 40;
    pss = rand() % 4;
	person = Order->Person;
	pb = g_Persons[person].Belong;
	iq = g_Persons[person].IQ;
	switch (pss)
	{
	case 0:
		break;
	case 1:
		pss = 0;
        rnd = rand() % 150;
		if (rnd < iq)
		{
			switch (rnd % 2)
			{
			case 0:
				pcount = GetCityOutPersons(Order->City,pqptr);
				if (pcount)
				{
                    sp = rand() % pcount;
					p = pqptr[sp];
					count = p;
					count *= 3;
					count += 1;
					infp = ResLoadToCon(GENERAL_CON,g_PIdx,g_CBnkPtr);
					fo = infp[count];
					rnd = 0;
					if (fo - 1 == person)
					{
						rnd = 1;
						g_Persons[p].Belong = pb;
                        g_Persons[p].Devotion = 70 + rand() % 30;
							
						/*搜索到武将*/
					}
					else if (!fo)
					{
                        rnd = rand() % 110;
						if (rnd < iq)
						{
							rnd = 1;
							g_Persons[p].Belong = pb;
                            g_Persons[p].Devotion = 70 + rand() % 30;
						}
						else
						{
							rnd = 2;
						}
					}
					
					if (pb == g_PlayerKing + 1)
					{
						pss = 1;
						if (!rnd)
						{
							ResLoadToMem(STRING_CONST,STR_HEARPERSON,str);
							ShowGReport(person,str);
							break;
						}
						else
						{
							ResLoadToMem(STRING_CONST,STR_GETGOODS,str);
							GetPersonName(p,astr);
							gam_strcat(str,astr);
						}
						ShowGReport(person,str);
						if (rnd == 1)
						{
                            rnd = P_SAY_STR13 + rand() % 4;
						}
						else if (rnd == 2)
						{
                            rnd = P_SAY_STR17 + rand() % 4;
						}
						ResLoadToMem(STRING_CONST,rnd,str);
						ShowGReport(p,str);
					}
				}
				break;
			case 1:
				pcount = GetCityDispGoods(Order->City,pqptr);
				if (pcount)
				{
                    sp = rand() % pcount;
					p = pqptr[sp];
					count = p;
					count *= 3;
					count += 1;
					infp = ResLoadToCon(GOODS_CON,g_PIdx,g_CBnkPtr);
					fo = infp[count];
					/*rnd = 0;*/
					if (!fo || (fo - 1 == person))
					{
						/*搜索到道具*/
						/*rnd = 1;*/
						if (pb == g_PlayerKing + 1)
						{
							ResLoadToMem(STRING_CONST,STR_GETGOODS,str);
							GetGoodsName(p,astr);
							gam_strcat(str,astr);
							ShowGReport(person,str);
							pss = 1;
						}
						SetGoods(p);
					}	
				}
				break;
			}
		}
		
		/*if (rnd)
		{
			pss = 1;
			if (pb == g_PlayerKing + 1)
			{
				ResLoadToMem(STRING_CONST,STR_GETNOTHING,str);
				ShowGReport(person,str);
			}
		}*/
		break;
	case 2:
        rnd = 10 + rand() % (iq * 2);
		g_Cities[Order->City].Money += rnd;
		break;
	case 3:
        rnd = 10 + rand() % (iq * 2);
		g_Cities[Order->City].Food += rnd;
		break;
	}
	while (pb == g_PlayerKing + 1)
	{
		if (1 == pss)
			break;
			
		if (!pss)
		{
			ResLoadToMem(STRING_CONST,STR_GETNOTHING,str);
		}
		else
		{
			ResLoadToMem(STRING_CONST,STR_GETSOME1 - 2 + pss,str);
			gam_itoa(rnd,astr,10);
			gam_strcat(str,astr);
		}
		ShowGReport(person,str);
		break;
	}
	
	AddPerson(Order->City,person);
	return(1);
}

/******************************************************************************
* 函数名:FatherDrv
* 说  明:“治理”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:30	基本功能完成
******************************************************************************/
U8 FatherDrv(OrderType *Order)
{
	AddPerson(Order->City,Order->Person);
	return(1);
}

/******************************************************************************
* 函数名:InspectionDrv
* 说  明:“出巡”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:30	基本功能完成
******************************************************************************/
U8 InspectionDrv(OrderType *Order)
{
	AddPerson(Order->City,Order->Person);
	return(1);
}

/******************************************************************************
* 函数名:SurrenderDrv
* 说  明:“招降”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:30	基本功能完成
******************************************************************************/
U8 SurrenderDrv(OrderType *Order)
{
    U8 rnd;
	U8 rade;
	U8 dev;
	U8 p,ob;
	U8 flag;
	U8 *str;
	PersonType *pptr;
	
	p = Order->Person;
	ob = Order->Object;
	pptr = &g_Persons[ob];
	flag = 0;
	do
	{
		if (pptr->Belong == g_Persons[p].Belong)
		{
			AddPerson(Order->City,p);
			return(1);
		}
			
		rade = g_Persons[p].IQ - pptr->IQ;
		rade += 50;
        rnd = rand() % 100;
        if (rnd > rade)
			break;
			
        /*rand = rand() % 100;
		if (rand < g_Persons[ob].Devotion)
			break;*/
		
		switch (pptr->Character)
		{
		case CHARACTER_LOYALISM:
			rade = SURRENDER_LOYALISM_MOD;
			break;
		case CHARACTER_IDEAL:
			rade = SURRENDER_IDEAL_MOD;
			break;
		case CHARACTER_AVARICE:
			rade = SURRENDER_AVARICE_MOD;
			break;
		case CHARACTER_DREAD:
			rade = SURRENDER_DREAD_MOD;
			break;
		case CHARACTER_TEMERITY:
			rade = SURRENDER_TEMERITY_MOD;
			break;
		}
		dev = pptr->Devotion;
		pptr->Devotion = dev - dev /10;
		if (dev > 60)
			break;
			
		rade = dev / rade;
        rnd = rand() % 100;
        if (rnd < rade)
			break;
		
		/*添加招降成功代码*/
		pptr->Belong = g_Persons[p].Belong;
        pptr->Devotion = 40 + rand() % 40;
		flag = 1;
	}while (0);
	
	if (g_Persons[p].Belong == g_PlayerKing + 1)
	{
		if (flag)
		{
			WonPersonNoet(ob);
		}
		else
		{
			str = SHARE_MEM + 300;
            ResLoadToMem(STRING_CONST,P_SAY_STR1 + (rand() % 3),str);
			ShowMapClear();
			ShowGReport(ob,str);
		}
	}
	
	AddPerson(Order->City,p);
	return(1);
}

/******************************************************************************
* 函数名:ExchangeDrv
* 说  明:“交易”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:30	基本功能完成
******************************************************************************/
U8 ExchangeDrv(OrderType *Order)
{
	AddPerson(Order->City,Order->Person);
	return(1);
}

/******************************************************************************
* 函数名:TransportationDrv
* 说  明:“输送”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:30	基本功能完成
******************************************************************************/
U8 TransportationDrv(OrderType *Order)
{
	U8 ob,rpi;
	U8 *str;
	
    if (rand() % 100 > 20)
	{
		ob = Order->Object;
		g_Cities[ob].Food += Order->Food;
		g_Cities[ob].Money += Order->Money;
		g_Cities[ob].MothballArms += Order->Arms;
		rpi = STR_RP9;
	}
	else
	{
		rpi = STR_RP10;
	}
	str = SHARE_MEM;
	ResLoadToMem(STRING_CONST,rpi,str);
	ShowGReport(Order->Person,str);
			
	
	AddPerson(Order->City,Order->Person);
	return(1);
}

/******************************************************************************
* 函数名:MoveDrv
* 说  明:“移动”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:31	基本功能完成
******************************************************************************/
U8 MoveDrv(OrderType *Order)
{
	U8 ob,pb;
	U8 *b;
	
	ob = Order->Object;
	b = &g_Cities[ob].Belong;
	pb = g_Persons[Order->Person].Belong;
	if (!(*b))
	{
		*b = pb;
		/*g_Cities[ob].SatrapId = Order->Person + 1;*/
	}
	else if (*b != pb)
	{
		ob = Order->City;
	}
		
	AddPerson(ob,Order->Person);
	return(1);
}

		
/******************************************************************************
* 函数名:AlienateDrv
* 说  明:“离间”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:31	基本功能完成
******************************************************************************/
U8 AlienateDrv(OrderType *Order)
{
    U8 rnd;
	U8 rade;
	U8 ob;
	
	ob = Order->Object;
	
	do
	{
        rade = g_Persons[Order->Person].IQ - g_Persons[ob].IQ;
        rade += 50;
        rnd = rand() % 100;
        if (rnd > rade)
            break;
			
        rnd = rand() % 100;
        if (rand < g_Persons[ob].Devotion)
            break;
			
		switch (g_Persons[ob].Character)
		{
		case CHARACTER_LOYALISM:
			rade = ALIENATE_LOYALISM;
			break;
		case CHARACTER_IDEAL:
			rade = ALIENATE_IDEAL;
			break;
		case CHARACTER_AVARICE:
			rade = ALIENATE_AVARICE;
			break;
		case CHARACTER_DREAD:
			rade = ALIENATE_DREAD;
			break;
		case CHARACTER_TEMERITY:
			rade = ALIENATE_TEMERITY;
			break;
		}
        rnd = rand() % 100;
        if (rnd > rade)
            break;
		
		/*添加离间成功代码*/
		if (g_Persons[ob].Devotion > 4)
			g_Persons[ob].Devotion -= 4;
		else
			g_Persons[ob].Devotion = 0;
	}while (0);
	
	AddPerson(Order->City,Order->Person);
	return(1);
}

/******************************************************************************
* 函数名:CanvassDrv
* 说  明:“招揽”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:31	基本功能完成
******************************************************************************/
U8 CanvassDrv(OrderType *Order)
{
    U8 rnd;
	U8 rade;
	U8 c;
	U8 o,p;
	
	o = Order->Object;
	p = Order->Person;
	do
	{
        rade = g_Persons[p].IQ - g_Persons[o].IQ;
        rade += 50;
        rnd = rand() % 100;
        if (rnd > rade)
            break;
			
        rnd = rand() % 100;
        if (rand < g_Persons[o].Devotion)
            break;
		
		switch (g_Persons[o].Character)
		{
		case CHARACTER_LOYALISM:
			rade = CANVASS_LOYALISM;
			break;
		case CHARACTER_IDEAL:
			rade = CANVASS_IDEAL;
			break;
		case CHARACTER_AVARICE:
			rade = CANVASS_AVARICE;
			break;
		case CHARACTER_DREAD:
			rade = CANVASS_DREAD;
			break;
		case CHARACTER_TEMERITY:
			rade = CANVASS_TEMERITY;
			break;
		}
        rnd = rand() % 100;
        if (rnd > rade)
            break;
		
		c = GetPersonCity(o);
		if (0xff == c)
			break;
		/*添加招揽成功代码*/
		DelPerson(c,o);
		AddPerson(Order->City,o);
		g_Persons[o].Belong = g_Persons[p].Belong;
        g_Persons[o].Devotion = 40 + rand() % 40;
		if (g_Persons[p].Belong == g_PlayerKing + 1)
			WonPersonNoet(o);
	}while (0);
	
	AddPerson(Order->City,p);
	return(1);
}

/******************************************************************************
* 函数名:CounterespiongeDrv
* 说  明:“策反”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:31	基本功能完成
******************************************************************************/
U8 CounterespiongeDrv(OrderType *Order)
{
    U8 rnd;
	U8 rade;
	U8 *pqptr;
	U8 i,pcount,c;
	U8 o;
	
	o = Order->Object;
	do
	{
        rade = g_Persons[Order->Person].IQ - g_Persons[o].IQ;
        rade += 50;
        rnd = rand() % 100;
        if (rnd > rade)
            break;
			
        rnd = rand() % 100;
        if (rnd < g_Persons[o].Devotion)
            break;
			
		switch (g_Persons[o].Character)
		{
		case CHARACTER_LOYALISM:
			rade = COUNTERESPIONGE_LOYALISM;
			break;
		case CHARACTER_IDEAL:
			rade = COUNTERESPIONGE_IDEAL;
			break;
		case CHARACTER_AVARICE:
			rade = COUNTERESPIONGE_AVARICE;
			break;
		case CHARACTER_DREAD:
			rade = COUNTERESPIONGE_DREAD;
			break;
		case CHARACTER_TEMERITY:
			rade = COUNTERESPIONGE_TEMERITY;
			break;
		}
        rnd = rand() % 100;
        if (rnd > rade)
            break;
		
		c = GetPersonCity(o);
		if (0xff == c)
			break;
		/*添加策反成功代码*/
		g_Persons[o].Belong = o + 1;
		pqptr = SHARE_MEM;
		pcount = GetCityPersons(c,pqptr);
		g_Cities[c].Belong = o + 1;
		for (i = 0;i < pcount;i ++)
		{
			g_Persons[pqptr[i]].Belong = o + 1;
		}
		BeNewKingNote(o);
	}while (0);
	
	AddPerson(Order->City,Order->Person);
	return(1);
}

/******************************************************************************
* 函数名:RealienateDrv
* 说  明:“反间”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:31	基本功能完成
******************************************************************************/
U8 RealienateDrv(OrderType *Order)
{
	return(1);
}

/******************************************************************************
* 函数名:InduceDrv
* 说  明:“劝降”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:31	基本功能完成
******************************************************************************/
U8 InduceDrv(OrderType *Order)
{
	U8 *str,*astr;
    U8 rnd;
	U8 rade;
	U8 wf;
	U8 *cqptr,*pqptr;
	U8 i,j,count,pcount;
	U8 p,o,pb;
	
	p = Order->Person;
	o = Order->Object;
	wf = 0;
	cqptr = SHARE_MEM;
	do
	{
		if (o == g_PlayerKing)
			break;
		
		pb = g_Persons[p].Belong;
		if (GetKingCitys(pb - 1,cqptr) < (GetKingCitys(g_Persons[o].Belong - 1,cqptr) * 2))
			break;
			
        rade = g_Persons[p].IQ - g_Persons[o].IQ;
        rade += 50;
        rnd = rand() % 100;
        if (rnd > rade)
            break;
			
        /*rand = rand() % 100;
		if (rand < g_Persons[Order->Object].Devotion)
			break;*/
			
		switch (g_Persons[o].Character)
		{
		case CHARACTER_PEACE:
			rade = PERSUADE_PEACE;
			break;
		case CHARACTER_JUSTICE:
			rade = PERSUADE_JUSTICE;
			break;
		case CHARACTER_DUPLICITY:
			rade = PERSUADE_DUPLICITY;
			break;
		case CHARACTER_CRAZY:
			rade = PERSUADE_CRAZY;
			break;
		case CHARACTER_RASH:
			rade = PERSUADE_RASH;
			break;
		}
        rnd = rand() % 100;
        if (rnd > rade)
            break;
		
		/*添加劝降成功代码*/
		pqptr = SHARE_MEM + CITY_MAX;
		count = GetKingCitys(o,cqptr);
		for (i = 0;i < count;i ++)
		{
			pcount = GetCityPersons(cqptr[i],pqptr);
			g_Cities[cqptr[i]].Belong = pb;
			for (j = 0;j < pcount;j ++)
			{
				g_Persons[pqptr[i]].Belong = pb;
			}
		}
		wf = 1;
	}while (0);
	
	str = SHARE_MEM + 300;
	astr = SHARE_MEM + 340;
	if (pb == g_PlayerKing + 1)
	{
		if (wf)
		{
			wf = P_SAY_STR36;
		}
		else
		{
			wf = P_SAY_STR32;
		}
        ResLoadToMem(STRING_CONST,wf + (rand() % 4),str);
		ShowMapClear();
		ShowGReport(o,str);
	}
	else if (wf)
	{
		GetPersonName(o,str);
		ResLoadToMem(STRING_CONST,STR_INDUCENOTE,astr);
		gam_strcat(str,astr);
		GetPersonName(pb - 1,astr);
		gam_strcat(str,astr);
		GamMsgBox(str,2);
	}
		
	
	AddPerson(Order->City,p);
	return(1);
}

/******************************************************************************
* 函数名:TributeDrv
* 说  明:“朝贡”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:31	基本功能完成
******************************************************************************/
/*U8 TributeDrv(OrderType *Order)
{
	return(1);
}*/

/******************************************************************************
* 函数名:ReconnoitreDrv
* 说  明:“侦察”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:31	基本功能完成
******************************************************************************/
U8 ReconnoitreDrv(OrderType *Order)
{
	AddPerson(Order->City,Order->Person);
	return(1);
}

/******************************************************************************
* 函数名:ConscriptionDrv
* 说  明:“征兵”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:31	基本功能完成
******************************************************************************/
U8 ConscriptionDrv(OrderType *Order)
{
	AddPerson(Order->City,Order->Person);
	return(1);
}

/******************************************************************************
* 函数名:DepredateDrv
* 说  明:“掠夺”命令驱动
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 17:31	基本功能完成
******************************************************************************/
U8 DepredateDrv(OrderType *Order)
{
	U8 *str,*astr;
	U16 valf,valm;
	CityType *cptr;
	
	cptr = &g_Cities[Order->City];
	cptr->PeopleDevotion /= 2;
	cptr->Farming /= 2;
	cptr->Commerce /= 2;
	valf = g_Persons[Order->Person].IQ + g_Persons[Order->Person].Force;
	valm = valf * 2;
	valf *= 5;
	cptr->Food += valf;
	cptr->Money += valm;
	if (g_Persons[Order->Person].Belong == g_PlayerKing + 1)
	{
		str = SHARE_MEM;
		astr = SHARE_MEM + 100;
		ResLoadToMem(STRING_CONST,P_SAY_STR28,str);
		gam_itoa(valm,astr,10);
		gam_strcat(str,astr);
		ResLoadToMem(STRING_CONST,P_SAY_STR29,astr);
		gam_strcat(str,astr);
		gam_itoa(valf,astr,10);
		gam_strcat(str,astr);
		ResLoadToMem(STRING_CONST,P_SAY_STR30,astr);
		gam_strcat(str,astr);
		ShowGReport(Order->Person,str);
	}
			
	AddPerson(Order->City,Order->Person);
	return(1);
}

/******************************************************************************
* 函数名:OrderExec
* 说  明:执行命令
*
* 入口参数：Order -- 待执行命令
*
* 出口参数：1 -- 成功，0 -- 失败
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-3 16:55	基本功能完成
******************************************************************************/
U8 OrderExec(OrderType *Order)
{
	switch (Order->OrderId)
	{
	/*内政指令*/
	/*case APPOINT:		任命
		AppointDrv(Order);
		break;*/
	case ASSART:		/*开垦*/
		AssartDrv(Order);
		break;
	case ACCRACTBUSINESS:	/*招商*/
		AccractbusinessDrv(Order);
		break;
	case SEARCH:		/*搜寻*/
		SearchDrv(Order);
		break;
	case FATHER:		/*治理*/
		FatherDrv(Order);
		break;
	case INSPECTION:	/*出巡*/
		InspectionDrv(Order);
		break;
	case SURRENDER:		/*招降*/
		SurrenderDrv(Order);
		break;
	case KILL:		/*处斩*/
		break;
	case BANISH:		/*流放*/
		break;
	case LARGESS:		/*赏赐*/
		break;
	case CONFISCATE:	/*没收*/
		break;
	case EXCHANGE:		/*交易*/
		ExchangeDrv(Order);
		break;
	case TREAT:		/*宴请*/
		break;
	case TRANSPORTATION:	/*输送*/
		TransportationDrv(Order);
		break;
	case MOVE:		/*移动*/
		MoveDrv(Order);
		break;
		
	/*外交指令*/
	case ALIENATE:		/*离间*/
		AlienateDrv(Order);
		break;
	case CANVASS:		/*招揽*/
		CanvassDrv(Order);
		break;
	case COUNTERESPIONAGE:	/*策反*/
		CounterespiongeDrv(Order);
		break;
	case REALIENATE:	/*反间*/
		RealienateDrv(Order);
		break;
	case INDUCE:		/*劝降*/
		InduceDrv(Order);
		break;
	/*case TRIBUTE:		朝贡
		TributeDrv(Order);
		break;	*/

	/*军备指令*/
	case RECONNOITRE:	/*侦察*/
		ReconnoitreDrv(Order);
		break;
	case CONSCRIPTION:	/*征兵*/
		ConscriptionDrv(Order);
		break;
	case DISTRIBUTE:	/*分配*/
		break;
	case DEPREDATE:		/*掠夺*/
		DepredateDrv(Order);
		break;
	case BATTLE:		/*出征*/
		BattleDrv(Order);
		break;
	}
	return(1);
}

/******************************************************************************
* 函数名:PolicyExec
* 说  明:命令队列执行
*
* 入口参数：无
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-17 11:22	基本功能完成
******************************************************************************/
 void PolicyExec(void)
{
	/*OrderQueueType *inode,*nnode;
	OrderType eOrder;*/
	
	U8 i;
	OrderType *io;
	
	io = (OrderType *) ORDERQUEUE;
	for (i = 0;i < ORDER_MAX;i ++)
	{
		if (0xff == io[i].OrderId)
			continue;
		
		
		OrderExec(&io[i]);
		DelOrder(&io[i]);
	}
	
	/*inode = g_OrderHead;
	while (1)
	{
		if (NULL == inode)
			break;
		
		nnode = inode->nOrder;
		if (!(inode->iOrder.TimeCount))
		{
			gam_memcpy((U8 *) &eOrder,(U8 *) &(inode->iOrder),sizeof(OrderType));
			DelOrder(inode);
			OrderExec(&eOrder);
		}
		else
		{
			inode->iOrder.TimeCount -= 1;
		}
		inode = nnode;
	}*/
}
