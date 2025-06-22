/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	tactic.c
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
*	1.0    2005-6-29 14:45  陈泽伟     基本的功能完成
***********************************************************************/

#include "stdsys.h"
#include "comm.h"
#undef TACTIC_C
#define TACTIC_C
#include "enghead.h"
#include "tactic.h"
#include "mainwindow.h"
#include "config.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <stringapiset.h>

#include <iostream>
#include <string>
#include <cstring>
#include <iconv.h>
#include <locale>
#include <sstream>
#include <iomanip>
#include <ctime>
// #include <wincon.h>
using namespace std;

extern int ngsecond1;
extern int ngc;
extern int g_stat;
extern int operateflag;
extern int g_HighAtt;
extern int g_autoSave;
extern int g_keyCodeListMax;
extern int g_modAIHighAttack;
extern int g_modAIHeroUpLevel;
extern int g_startFlag;

// Global flag to control order logging - only log when person belongs to user king
bool g_LogOnlyUserKing = true; // Default to true - only log user king activities

// Functions to control the logging flag
void setLogOnlyUserKing(bool enabled) {
    g_LogOnlyUserKing = enabled;
}

bool getLogOnlyUserKing() {
    return g_LogOnlyUserKing;
}

// Helper function to check if person belongs to user king
bool isPersonUserKing(U8 personId) {
    if (personId >= PERSON_MAX) return false;
    return (g_Persons[personId].Belong == (g_PlayerKing + 1));
}

bool GamSaveRcd(U8 idx);
// U8 o_PersonsQueue[PERSON_MAX];

/******************************************************************************
 * 函数名:PlayerTactic
 * 说  明:玩家策略控制
 *
 * 入口参数：无
 *
 * 出口参数：
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-1 14:31	基本功能完成
 ******************************************************************************/
U8 PlayerTactic(void)
{
	U8 cset;
	U8 cral;

	while (1)
	{
		// usleep(1000);
		// DebugTime(84);

		if (g_startFlag == 3 || g_startFlag == 2)
		{
			return 0xfe;
		}

		cset = GetCitySet(&g_CityPos);
		if (0xff == cset)
		{
			return (1);
		}
		else if (0xfe == cset)
		{
			return (0xfe);
		}
		else
		{
			if (g_Cities[cset].Belong != (g_PlayerKing + 1))
			{
				if (g_Cities[cset].Belong)
				{
					ShowConstStrMsg(NOTE_STR3);
				}
				else
				{
					ShowConstStrMsg(NOTE_STR5);
				}
				continue;
			}
			ngc = 2;
			ngsecond1 = 0;
			cral = OrderMenu();
			switch (cral)
			{
			case 0xff:
				break;
			case 0xfe:
				ngc = 3;
				printCityDebugInfoCout(cset);		
				ShowCityPro(cset);		
				break;
			default:
				CityCommon(cset, cral);
				break;
			}
			ngc = 0;
		}
	}
	return (0);
}

/******************************************************************************
 * 函数名:ComputerTactic
 * 说  明:产生电脑策略
 *
 * 入口参数：无
 *
 * 出口参数：
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-17 15:37	基本功能完成
 ******************************************************************************/
/*君主产生"内政、协调"、"外交"、"军备"策略几率*/
/*const U8 KingTacticOddsIH[5] = {50,40,30,20,10};
const U8 KingTacticOddsD[5] = {80,70,70,40,20};*/

void ComputerTactic(void)
{
	U8 i;
	U8 rnd;
	U8 TacticOddsIH[6], TacticOddsD[6];
	U8 b;
	CityType *cptr;

	// g_tlen = KingTacticOddsIHLen;
	ResLoadToMem(IFACE_CONID, KingTacticOddsIH, TacticOddsIH);
	// g_tlen = KingTacticOddsDLen;
	ResLoadToMem(IFACE_CONID, KingTacticOddsD, TacticOddsD);
	for (i = 0; i < CITY_MAX; i++)
	{
		cptr = &g_Cities[i];
		cptr->Id = STATE_NORMAL;
		b = cptr->Belong;
		if (!(b))
			continue;
		bool isAuto = (b == (g_PlayerKing + 1));

		// if (b == (g_PlayerKing + 1))
		//	continue;

		cptr->AvoidCalamity += 1;
		if (cptr->AvoidCalamity > 100)
			cptr->AvoidCalamity = 100;

		if (cptr->Food < 100)
			cptr->Food = 500;
		if (cptr->Money > 500 && cptr->Food < 5000)
		{
			cptr->Food += (cptr->Money - 500) / 2;
			cptr->Money = 500;
		}

		if (isAuto && cptr->Food >= 1000)
		{
			SmartAssign(i, 5);
		}
		/*添加电脑策略代码*/

		if (cptr->Food < 1000)
		{
			ComputerTacticInterior(i);
			continue;
		}
		int cCount = GetCityCaptives(i, SHARE_MEM);
		if (cCount && isAuto)
		{
			AITacticDiplomatism(i);
			continue;
		}
		rnd = rand() % 100;

		if (TacticOddsIH[g_Persons[b - 1].Character] > rnd) /*执行内政、协调策略*/
		{
			ComputerTacticInterior(i);
			
			// Log computer tactic
			logComputerTacticC(i, "Interior");

			// ComputerTacticHarmonize(i, isAuto);
		}
		else if (TacticOddsD[g_Persons[b - 1].Character] > rnd) /*执行外交策略*/
		{
			if (isAuto)
			{
				AITacticDiplomatism(i);
				logComputerTacticC(i, "AI_Diplomatism");
			}
			else
			{
				ComputerTacticDiplomatism(i);
				logComputerTacticC(i, "Diplomatism");
			}
		}
		else if (!isAuto) /*执行军备策略*/
		{
			ComputerTacticArmament(i, isAuto);
			logComputerTacticC(i, "Armament");
		}
	}
}

/******************************************************************************
 * 函数名:ComputerTacticInterior
 * 说  明:产生城市内政电脑策略
 *
 * 入口参数：要产生电脑策略的城市
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-20 15:32	基本功能完成
 ******************************************************************************/

void ComputerTacticInterior(U8 city)
{
	U8 *pqptr;
	U8 i, rnd;
	U8 pcount;
	OrderType order;
	U16 *c, cl, ca;
	U16 *f, fl, fa;
	U8 *PeopleDevotion;
	U32 *Population;
	CityType *cptr;

    memset(&order, 0, sizeof(OrderType));
	cptr = &g_Cities[city];
	f = &cptr->Farming;
	fl = cptr->FarmingLimit;

	c = &cptr->Commerce;
	cl = cptr->CommerceLimit;

	PeopleDevotion = &cptr->PeopleDevotion;
	Population = &cptr->Population;

	pqptr = SHARE_MEM;
	pcount = GetCityPersons(city, pqptr);
	bool isAuto = (city == (g_PlayerKing + 1));

	for (i = 0; i < pcount; i++)
	{
		rnd = rand() % 5;
		if (cptr->Id != STATE_NORMAL)
		{
			rnd = 3; // 治理
		}
		else if (g_Cities[city].Food < 1000)
		{
			rnd = 0;
		}

		switch (rnd)
		{
		case 0: /*开垦*/
			if (*f < fl)
			{
				fa = 200;
				*f += fa;
				if (*f > fl)
				{
					*f = fl;
				}
				order.OrderId = ASSART;
				break;
			}
		case 1: /*招商*/
			if (*c < cl)
			{
				ca = 200;
				*c += ca;
				if (*c > cl)
				{
					*c = cl;
				}
				order.OrderId = ACCRACTBUSINESS;
				break;
			}
		case 2: /*出巡*/
			if (*PeopleDevotion < 100 || *Population < cptr->PopulationLimit)
			{
				*PeopleDevotion += 4;
				if (*PeopleDevotion > 100)
					*PeopleDevotion = 100;
				*Population += 100;
				if (*Population > cptr->PopulationLimit)
					*Population = cptr->PopulationLimit;

				order.OrderId = INSPECTION;
				break;
			}
		case 3:
			if (cptr->Id != STATE_NORMAL || cptr->AvoidCalamity < 100)
			{
				cptr->Id = STATE_NORMAL; // zhi li
				cptr->AvoidCalamity += 4;
				if (cptr->AvoidCalamity > 100)
					cptr->AvoidCalamity = 100;
				order.OrderId = FATHER;
				break;
			}
		// case 4:
		// 	// if(isAuto){
		// 	// 	int minCity=-1;
		// 	// 	for (int ci = 0;ci < CITY_MAX;ci ++)
		// 	// 	{
		// 	// 		if (g_Cities[ci].Belong == (g_PlayerKing + 1) && ci != city && (minCity==-1 || g_Cities[ci].PersonV))
		// 	// 		{

		// 	// 		}
		// 	// 	}
		// 	// }
		// 	break;
		default: /*搜寻*/
			order.OrderId = SEARCH;
			break;
		}

		order.Person = pqptr[i];
		order.City = city;
		order.Object = city;
		order.TimeCount = 0;
		if (AddOrderHead(&order) == 0) {
			DelPerson(city, pqptr[i]);
		}
		if (cptr->Belong == g_PlayerKing + 1)
		{
			std::cout<< "ComputerTacticInterior: Adding order for person " << (int)pqptr[i] << " in city " << (int)city << "order id" <<order.OrderId<<std::endl;
		}
	}
}

/******************************************************************************
 * 函数名:ComputerTacticDiplomatism
 * 说  明:产生城市外交电脑策略
 *
 * 入口参数：要产生电脑策略的城市
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-20 15:34	基本功能完成
 ******************************************************************************/
void ComputerTacticDiplomatism(U8 city)
{
	U8 *pqptr, *eqptr;
	U8 i, rnd;
	U8 pcount, ecount;
	OrderType order;

	pqptr = SHARE_MEM;
	eqptr = SHARE_MEM + PERSON_MAX;
	pcount = GetCityPersons(city, pqptr);
	for (i = 0; i < pcount; i++)
	{
		rnd = rand() % 8;
		ecount = GetCityCaptives(city, eqptr);
		if (ecount)
		{
			rnd = 0;
		}
		switch (rnd)
		{
		case 0: /*招降*/
			/*ecount = GetCityCaptives(city,eqptr);*/
			if (ecount)
			{
				rnd = rand() % ecount;
				if (g_Persons[eqptr[rnd]].Id != g_PlayerKing + 1)
				{
					g_Persons[eqptr[rnd]].Belong = g_Cities[city].Belong;
					continue;
				}
				DelPerson(city, eqptr[rnd]);
			}
			else
			{
				continue;
			}
			order.OrderId = SURRENDER;
			break;
		case 1: /*处斩*/
			/*ecount = GetCityCaptives(city,eqptr);*/
			if (ecount)
			{
				rnd = rand() % ecount;
				DelPerson(city, eqptr[rnd]);
			}

			continue;
			break;
		case 2: /*流放*/
			/*ecount = GetCityCaptives(city,eqptr);
			if (ecount)
			{
				rnd = rand() % ecount;
				g_Persons[eqptr[rnd]].Belong = 0;
				g_Persons[eqptr[rnd]].Arms = 0;
			}
			order.OrderId = BANISH;*/
			continue;
			break;
		case 3: /*离间*/
			ecount = GetEnemyPersons(city, eqptr);
			if (ecount)
			{
				rnd = rand() % ecount;
				order.Object = eqptr[rnd];
			}
			else
			{
				continue;
			}
			order.OrderId = ALIENATE;
			break;
		case 4: /*招揽*/
			ecount = GetEnemyPersons(city, eqptr);
			if (ecount)
			{
				rnd = rand() % ecount;
				order.Object = eqptr[rnd];
			}
			else
			{
				continue;
			}
			order.OrderId = CANVASS;
			break;
		case 5: /*策反*/
			ecount = GetEnemySatraps(city, eqptr);
			if (ecount)
			{
				rnd = rand() % ecount;
				order.Object = eqptr[rnd];
			}
			else
			{
				continue;
			}
			order.OrderId = COUNTERESPIONAGE;
			break;
		case 6: /*劝降*/
			ecount = GetEnemyKing(city, eqptr);
			if (ecount)
			{
				rnd = rand() % ecount;
				order.Object = eqptr[rnd];
			}
			else
			{
				continue;
			}
			order.OrderId = INDUCE;
			break;
		case 7:
			continue;
			break;
		}

		order.Person = pqptr[i];
		order.City = city;
		order.TimeCount = 0;
		if (AddOrderHead(&order) == 0) {
			DelPerson(city, pqptr[i]);
		}
	}
}

// Get all persons in the city: include active, out, and captives
std::vector<U8> GetAllCityPersonsFromLegacy(U8 city, U8 *o_PersonsQueue)
{
	std::vector<U8> officers;

	// Pre-allocate space based on city's total persons
	officers.reserve(g_Cities[city].Persons);

	// Loop through all persons in the city
	for (U8 i = 0; i < g_Cities[city].Persons; i++)
	{
		U8 p = o_PersonsQueue[g_Cities[city].PersonQueue + i];
		officers.push_back(p);
		// if (g_Persons[p].Belong == g_Cities[city].Belong)
		// {
		// 	officers.push_back(p);
		// }
	}

	return officers;
}

void AITacticDiplomatism(U8 city)
{
	U8 *pqptr, *eqptr;
	U8 i, rnd;
	U8 pcount, ecount;
	OrderType order;

	pqptr = SHARE_MEM;
	eqptr = SHARE_MEM + PERSON_MAX;
	pcount = GetCityPersons(city, pqptr);

	for (i = 0; i < pcount; i++)
	{
		ecount = GetCityCaptives(city, eqptr);
		/*招降*/
		/*ecount = GetCityCaptives(city,eqptr);*/
		if (ecount)
		{
			rnd = rand() % ecount; // random pick one and success
			if (g_Persons[eqptr[rnd]].Id != g_PlayerKing + 1)
			{
				g_Persons[eqptr[rnd]].Belong = g_Cities[city].Belong;
				continue;
			}
			DelPerson(city, eqptr[rnd]);
			order.OrderId = SURRENDER; // should there be an object?
			continue;
		}
		/*招揽*/
		ecount = GetEnemyPersons(city, eqptr);

		if (ecount)
		{
			rnd = rand() % ecount;
			order.Object = eqptr[rnd];
		}
		else
		{
			continue;
		}
		order.OrderId = CANVASS;
		order.Person = pqptr[i];
		order.City = city;
		order.TimeCount = 0;
		logMessageFromCppFormat(" 外交 order type %d", order.OrderId);
		if (AddOrderHead(&order) == 0) {
			DelPerson(city, pqptr[i]);
		}
	}
}

/******************************************************************************
 * 函数名:ComputerTacticHarmonize
 * 说  明:产生城市协调电脑策略
 *
 * 入口参数：要产生电脑策略的城市
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-20 15:34	基本功能完成
 ******************************************************************************/
void ComputerTacticHarmonize(U8 city, bool isAuto)
{
	U8 *pqptr, *cqptr, *eqptr;
	U8 i, j, c, rnd;
	U8 pcount, ccount, ecount;
	OrderType order;
	CityType *cptr;

	cptr = &g_Cities[city];
	pqptr = SHARE_MEM;
	cqptr = SHARE_MEM + PERSON_MAX;
	eqptr = SHARE_MEM + PERSON_MAX + CITY_MAX;
	pcount = GetCityPersons(city, pqptr);
	for (i = 0; i < pcount; i++)
	{
		rnd = rand() % 7;
		if (isAuto)
		{
			rnd = 0;
		}
		switch (rnd)
		{
		case 0: /*治理*/ // todo never go here
			cptr->Id = STATE_NORMAL;
			cptr->AvoidCalamity += 4;
			if (cptr->AvoidCalamity > 100)
				cptr->AvoidCalamity = 100;
			order.OrderId = FATHER;
			break;
		case 1: /*赏赐*/
			continue;
			break;
		case 2: /*没收*/
			continue;
			break;
		case 3: /*交易*/
			/*order.OrderId = EXCHANGE;*/
			continue;
			break;
		case 4: /*输送*/
			/*order.OrderId = TRANSPORTATION;*/
			continue;
			break;
		case 5: /*移动*/
			if (i < 3)
			{
				continue;
			}
			ccount = GetKingCitys(cptr->Belong - 1, cqptr);
			if (ccount < 2)
			{
				continue;
			}
			for (c = 0, j = 0; j < ccount; j++)
			{
				ecount = GetRoundEnemyCity(cqptr[j], eqptr);
				if (ecount)
				{
					c = j;
					rnd = rand() % 2;
					if (rnd)
					{
						break;
					}
				}
			}
			order.Object = cqptr[c];
			order.OrderId = MOVE;
			break;
		case 6:
			continue;
			break;
		}

		order.Person = pqptr[i];
		order.City = city;
		order.TimeCount = 0;
		if (AddOrderHead(&order) == 0) {
			DelPerson(city, pqptr[i]);
		}
	}
}

void RecruitAndAssign(U8 city)
{
}

/// @brief
/// @param person
/// @return max army the person can carry
int getMaxArmy(U8 person)
{
	PersonType *pptr = &g_Persons[person];
	int armys = pptr->Level;
	armys *= 10;
	armys += pptr->Force + pptr->IQ;
	armys *= 10;
	return armys;
}

/// @brief smartAssign the army to top k generals
/// @param city
/// @param k
int SmartAssign(U8 city, int k)
{
	vector<U8> cands = getAllCityPersonsBelonging(city);
	vector<vector<int>> input;
	input.reserve(cands.size());
	int count = 0;
	int quota = g_Cities[city].Food * 5; // how many more armies can support the 10 days, ignore consumption and harvest
	for (auto p : cands)
	{
		int tmp = getMaxArmy(p);
		if (g_Persons[p].Arms == tmp)
		{
			count++;
		}
		input.push_back({p, tmp});
	}
	if (count >= k || count == cands.size())
	{
		return -1;
	}

	std::sort(input.begin(), input.end(), [](const std::vector<int> &a, const std::vector<int> &b)
			  { return a[1] > b[1]; });

	for (int i = 0; i < input.size(); i++)
	{
		if (i < k && quota > 0)
		{
			g_Persons[input[i][0]].Arms = min(input[i][1], quota);
			quota -= g_Persons[input[i][0]].Arms;
		}
		else
		{
			g_Persons[input[i][0]].Arms = 0; // remove the army for others to avoid too much food consumption
		}
	}
	g_Cities[city].MothballArms = 0;
	return 0;
}

// get all persons that belong to the city's king
vector<U8> getAllCityPersonsBelonging(U8 city)
{
	vector<U8> ret;
	// Filter persons that belong to the city's faction
	for (auto p : g_Cities[city].PersonV)
	{
		if (g_Persons[p].Belong == g_Cities[city].Belong)
		{
			ret.push_back(p);
		}
	}
	return ret;
}

/******************************************************************************
 * 函数名:ComputerTacticArmament
 * 说  明:产生城市军备电脑策略
 *
 * 入口参数：要产生电脑策略的城市
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-20 15:35	基本功能完成
 ******************************************************************************/
void ComputerTacticArmament(U8 city, bool isAuto)
{
	U8 *pqptr, *cqptr, fp[10];
	U8 i, j, rnd;
	U8 pcount, fpcount, fcount, t;
	U16 armys;
	OrderType order;
	PersonType *pptr;

	pqptr = SHARE_MEM;
	cqptr = SHARE_MEM + PERSON_MAX;
	pcount = GetCityPersons(city, pqptr);
	if (!pcount)
		return;
	pptr = &g_Persons[pqptr[rand() % pcount]];

	if (!(g_MonthDate % 3))
	{
		if (g_useMOD == 1 && g_modAIHeroUpLevel == 0)
		{
		}
		else
		{
			pptr->Level += 1;
			if (pptr->Level > MAX_LEVEL)
				pptr->Level = MAX_LEVEL;
		}
	}

	SmartAssign(city, 5);

	fcount = GetRoundEnemyCity(city, cqptr);
	rnd = rand() % 10; // 1/10 attack
	if (!fcount || rnd || g_DisableComputerAttacks) // Added global disable check
	{
		return;
	}
	rnd = rand() % (fcount);
	order.Object = GetWeekCity(fcount, cqptr);
	fpcount = GetCityPersons(city, cqptr);
	if (fpcount)
	{
		fcount = 0;
		for (fcount = 0; fcount < fpcount - 1; fcount++)
		{
			for (j = fcount + 1; j < fpcount; j++)
			{
				/*添加武将兵力排序*/
				if (g_Persons[cqptr[fcount]].Arms < g_Persons[cqptr[j]].Arms)
				{
					t = cqptr[j];
					cqptr[j] = cqptr[fcount];
					cqptr[fcount] = t;
				}
				if (!g_Persons[cqptr[pcount - 1]].Arms)
					fpcount -= 1;
			}
		}

		if (g_Persons[cqptr[0]].Arms >= 1000 && fpcount >= 4)
		{
			/*添加出征命今生成代码*/
			order.OrderId = BATTLE;
			gam_memset(fp, 0, 10);
			fpcount -= 1;
			if (fpcount > 10)
			{
				fpcount = 10;
			}
			for (j = 0; j < fpcount; j++)
			{
				DelPerson(city, cqptr[j]);
				fp[j] = cqptr[j] + 1;
			}
			/*order.Person = fpcount;*/
			order.City = city;
			order.Food = g_Cities[city].Food;
			order.TimeCount = 0;
			AddFightOrder(&order, fp);
			return;
		}
	}
}

/******************************************************************************
 * 函数名:ConditionUpdate
 * 说  明:环境更新
 *
 * 入口参数：无
 *
 * 出口参数：
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
void ConditionUpdate(void)
{
	g_MonthDate += 1; /*时间月累加*/
	if (g_MonthDate > 12)
	{
		g_YearDate += 1; /*时间年累加*/
		g_MonthDate = 1;
		GoodsUpDatadate();
		PersonUpDatadate();
	}
	CitiesUpDataDate();
	EventStateDeal();
	RandEvents();
}

/******************************************************************************
 * 函数名:GameDevDrv
 * 说  明:游戏发展驱动
 *
 * 入口参数：无
 *
 * 出口参数：游戏命令
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-1 11:43	基本功能完成
 ******************************************************************************/
U8 GameDevDrv(void)
{
	U8 ral;

	// 20180505初始化大地图(战斗地图初始化在进入战斗时初始化)
	InitCityMap();

	while (1)
	{

		if (g_startFlag == 3 || g_startFlag == 2)
		{
			return 0;
		}

		if (IsLoss())
		{
			/*提示玩家全军覆没*/
			ShowConstStrMsg(STR_GAMELOST);
			return (1);
		}
		if (IsWin())
		{
			/*提示玩家大获全胜*/
			ShowConstStrMsg(STR_GAMEWON);
			return (0);
		}
		SetCitySatrap();
		/*玩家策略*/
		do
		{
			if (g_startFlag == 3 || g_startFlag == 2)
			{
				return 0;
			}

			// DebugTime(82);
			int yt = PlayerTactic();
			if (yt == 0xfe)
				return 0;
			// DebugTime(83);
			ngc = 1;
			ral = FunctionMenu();
			ngc = 0;
			if (!ral)
			{
				continue;
			}
			else if (1 == ral)
			{
				break;
			}
			else if (2 == ral)
			{
				return (2);
			}
			// usleep(1000);
		} while (1);

		if (g_autoSave == 1)
		{
			GamSaveRcd(0);
		}

		/*提示电脑策略中*/
		// ShowTacticNote();
		/*电脑策略*/
		ComputerTactic();

		/*命令队列执行*/
		PolicyExec();

		/*环境更新*/
		ConditionUpdate();
		
		// Log game turn completion
		logGameTurnC();
	}
}

/******************************************************************************
 * 函数名:IsWin
 * 说  明:检测玩家是否取胜
 *
 * 入口参数：无
 *
 * 出口参数：0 -- 否，1 -- 是
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-30 0:35	基本功能完成
 ******************************************************************************/
U8 IsWin(void)
{
	U8 *ptr;

	ptr = SHARE_MEM;
	if (GetEnemyKing(g_PlayerKing, ptr))
		return (0);
	else
		return (1);
}

/******************************************************************************
 * 函数名:IsLoss
 * 说  明:检测玩家是否失败
 *
 * 入口参数：无
 *
 * 出口参数：0 -- 否，1 -- 是
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-30 0:44	基本功能完成
 ******************************************************************************/
U8 IsLoss(void)
{
	U8 *ptr;

	ptr = SHARE_MEM;
	if (GetKingCitys(g_PlayerKing, ptr))
		return (0);
	else
		return (1);
}

/******************************************************************************
 * 函数名:SetCitySatrap
 * 说  明:设置城中最高智力者为太守
 *
 * 入口参数：无
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-30 0:47	基本功能完成
 ******************************************************************************/
void SetCitySatrap()
{
	for (U8 cityId = 0; cityId < CITY_MAX; cityId++)
	{
		if (!g_Cities[cityId].Belong)
		{
			g_Cities[cityId].SatrapId = 0;
			continue;
		}

		// Get all persons belonging to this city's faction
		vector<U8> cityPersons = getAllCityPersonsBelonging(cityId);

		// If no persons, set SatrapId to 0
		if (cityPersons.empty())
		{
			g_Cities[cityId].SatrapId = 0;
			continue;
		}

		// Sort persons by IQ, prioritizing the king if present
		auto bestSatrap = *std::max_element(cityPersons.begin(), cityPersons.end(),
											[cityId](U8 a, U8 b) -> bool
											{
												// If one is the king, they take priority
												if (g_Cities[cityId].Belong == a + 1)
													return false;
												if (g_Cities[cityId].Belong == b + 1)
													return true;
												// Otherwise compare by IQ
												return g_Persons[a].IQ < g_Persons[b].IQ;
											});

		// Set the satrap (+1 because game uses 1-based indices for SatrapId)
		g_Cities[cityId].SatrapId = bestSatrap + 1;
	}
}

/******************************************************************************
 * 函数名:GetPeriodKings
 * 说  明:取得指定时期君主
 *
 * 入口参数：period -- 指定时期，kings -- 君主队列指针
 *
 * 出口参数：君主个数
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-28 19:27	基本功能完成
 ******************************************************************************/
U8 GetPeriodKings(U8 period, U8 *kings)
{
	LoadPeriod(period);
	return (GetAllKing(kings));
}

/******************************************************************************
 * 函数名:GetAllKing
 * 说  明:取得现时期君主
 *
 * 入口参数：kings -- 君主队列指针
 *
 * 出口参数：君主个数
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-28 19:28	基本功能完成
 ******************************************************************************/
U8 GetAllKing(U8 *kings)
{
	U8 c, i, count;
	U8 b;

	count = 0;
	for (c = 0; c < CITY_MAX; c++)
	{
		b = g_Cities[c].Belong;
		if (b)
		{
			for (i = 0; i < count; i++)
			{
				if (kings[i] == b - 1)
					break;
			}
			if (i < count)
			{
				continue;
			}
			kings[count] = b - 1;
			count += 1;
		}
	}
	return (count);
}

/******************************************************************************
 * 函数名:LoadPeriod
 * 说  明:加载特定时期资料
 *
 * 入口参数：period -- 指定时期
 *
 * 出口参数：君主个数
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-29 23:01	基本功能完成
 ******************************************************************************/
void LoadPeriod(U8 period)
{
	U8 i;
	U8 *ptr;

	ptr = ResLoadToCon(CITY_RESID, period, g_CBnkPtr);
	OldCityType old_cities[CITY_MAX];
	memset(old_cities, 0, CITY_MAX * sizeof(OldCityType));
	gam_memcpy((U8 *)old_cities, ptr, sizeof(OldCityType) * CITY_MAX);

	ptr += sizeof(CityType) * CITY_MAX;
	g_YearDate = *((U16 *)ptr);

	ptr = ResLoadToCon(GENERAL_RESID, period, g_CBnkPtr);
	gam_memcpy((U8 *)g_Persons, ptr, sizeof(PersonType) * PERSON_MAX);

	U8 o_PersonsQueue[PERSON_MAX];
	ptr = ResLoadToCon(GENERAL_QUEUE, period, g_CBnkPtr);
	gam_memcpy(o_PersonsQueue, ptr, PERSON_MAX);

	ptr = ResLoadToCon(GOODS_QUEUE, period, g_CBnkPtr);
	gam_memcpy(g_GoodsQueue, ptr, GOODS_MAX);

	gam_memset(FIGHTERS_IDX, 0, FIGHT_ORDER_MAX);
	gam_memset((U8 *)ORDERQUEUE, 0xff, (U16)ORDER_MAX * sizeof(OrderType));

	g_PIdx = period;

	for (i = 0; i < PERSON_MAX; i++)
	{
		g_Persons[i].Thew = 100;
		g_Persons[i].Arms = 100;
		g_Persons[i].Id = 0;
	}

	OldCityToNewCity(old_cities, o_PersonsQueue);
	g_MonthDate = 1;
}



void OldCityToNewCity(OldCityType old_cities[38], U8 o_PersonsQueue[200])
{
	for (int i = 0; i < CITY_MAX; i++)
		g_Cities[i].Id = 0;
	for (int i = 0; i < CITY_MAX; i++)
	{
		g_Cities[i].Id = old_cities[i].Id;
		g_Cities[i].Belong = old_cities[i].Belong;
		g_Cities[i].SatrapId = old_cities[i].SatrapId;
		g_Cities[i].FarmingLimit = old_cities[i].FarmingLimit;
		g_Cities[i].Farming = old_cities[i].Farming;
		g_Cities[i].CommerceLimit = old_cities[i].CommerceLimit;
		g_Cities[i].Commerce = old_cities[i].Commerce;
		g_Cities[i].PeopleDevotion = old_cities[i].PeopleDevotion;
		g_Cities[i].AvoidCalamity = old_cities[i].AvoidCalamity;
		g_Cities[i].PopulationLimit = old_cities[i].PopulationLimit;
		g_Cities[i].Population = old_cities[i].Population;
		g_Cities[i].Money = old_cities[i].Money;
		g_Cities[i].Food = old_cities[i].Food;
		g_Cities[i].MothballArms = old_cities[i].MothballArms;
		g_Cities[i].PersonQueue = old_cities[i].PersonQueue;
		g_Cities[i].Persons = old_cities[i].Persons;
		g_Cities[i].ToolQueue = old_cities[i].ToolQueue;
		g_Cities[i].Tools = old_cities[i].Tools;
		g_Cities[i].autoManage = 0;
		g_Cities[i].PersonV = GetAllCityPersonsFromLegacy(i, o_PersonsQueue);

		U8 tmpName[5];
		GetCityName(i, tmpName);
		g_Cities[i].Name = gbk_to_utf8((const char*)tmpName, 4);
	}
	InitializeToolsVFromOldSystem();
}

// Copy all person IDs  from new city to person queue
// update personsQueue, and g_Cities[].PersonQueue/Persons
void NewCityToOldPersonsQueue(U8 *personsQueue)
{
	int queueIndex = 0;

	// Iterate through all cities
	for (U8 cityId = 0; cityId < CITY_MAX; cityId++)
	{
		// Update city's PersonQueue to point to start of its section
		g_Cities[cityId].PersonQueue = queueIndex;

		// Copy all person IDs from this city's PersonV to queue
		for (const auto &personId : g_Cities[cityId].PersonV)
		{
			personsQueue[queueIndex++] = personId;
		}

		// Update number of persons in this city
		g_Cities[cityId].Persons = g_Cities[cityId].PersonV.size();
	}
}

/******************************************************************************
 * 函数名:FunctionMenu
 * 说  明:功能菜单
 *
 * 入口参数：无
 *
 * 出口参数：
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-13 15:33	基本功能完成
 ******************************************************************************/
U8 FunctionMenu(void)
{
	/*U8 mstr[30];
	MenuType menu;

	menu.top = 0;
	menu.set = 0;
	menu.count = 3;
	menu.sx = WK_SX + WK_EX / 3;
	menu.sy = WK_SY + 10;
	menu.ex = WK_SX + WK_EX / 3 + ASC_WID * 8;
	menu.ey = WK_EY - 10;
	ResLoadToMem(STRING_CONST,FUNC_TEND,mstr);
	ResLoadToMem(STRING_CONST,FUNC_SAVE,&mstr[10]);
	ResLoadToMem(STRING_CONST,FUNC_QUIT,&mstr[20]);
	menu.menustr[0] = mstr;
	menu.menustr[1] = &mstr[10];
	menu.menustr[2] = &mstr[20];

	switch (MenuComm(&menu))
	{
	case 0:
		return(1);
	case 1:
		GamRecordMan(0);
		return(0);
	case 2:
		return(2);
	case 0xff:
		return(0);
	}*/

	RECT pRect;
	U8 mstr[30];
	ResLoadToMem(STRING_CONST, MENU_FUNCSTR, mstr);
	pRect.sx = WK_SX + WK_EX / 3;
	pRect.ex = WK_SX + WK_EX / 3 + ASC_WID * 8;
	pRect.sy = WK_SY + 10;
#if ((WK_EY - (WK_SY + 10)) > (ASC_HGT * 3 + 4))
	pRect.ey = WK_SY + 10 + ASC_HGT * 3;
#else
	pRect.ey = WK_SY + 10 + ((WK_EY - (WK_SY + 10)) / ASC_HGT) * ASC_HGT;
#endif

	g_stat = 0;
	g_keyCodeListMax = 0; // 20180725
	switch (PlcSplMenu(&pRect, 0, mstr))
	{
	case 0:
		return (1);
	case 1:
		GamRecordMan(0);
		return (0);
	case 2:
		return (2);
	case 0xff:
		return (0);
	}
}

/******************************************************************************
 * 函数名:NumOperate
 * 说  明:数字操作
 *
 * 入口参数：min -- 最小值限制，max -- 最大值限制
 *
 * 出口参数：0xffff -- 退出，其他值 -- 操作值
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-23 11:43	基本功能完成
 ******************************************************************************/
U16 NumOperate(U16 min, U16 max)
{
	U8 str[20];
	U8 showflag, i, bit, maxbit;
	U16 donum, tnum, num;
	GMType Msg;

	donum = min;
	num = 1;
	showflag = 1;
	operateflag = 1;
	ResLoadToMem(STRING_CONST, ATRR_STR63, str);
	GamStrShowS((WK_SX + WK_EX - WK_SX - ASC_WID * 12) / 2, (WK_SY + WK_EY - WK_SY - ASC_HGT * 3) / 2 + ASC_HGT, (const char *)str);
	ResLoadToMem(STRING_CONST, ATRR_STR64, str);
	gam_ltoa(max, &str[7], 10);
	GamStrShowS((WK_SX + WK_EX - WK_SX - ASC_WID * 12) / 2, (WK_SY + WK_EY - WK_SY - ASC_HGT * 3) / 2 + ASC_HGT * 2, (const char *)str);
	maxbit = gam_strlen(&str[7]) - 1;
	if (maxbit == 255)
		maxbit = 0;
	bit = maxbit;
	while (1)
	{
		if (showflag)
		{
			SysCopyFromDcToStr();
			tnum = donum;
			for (i = maxbit; i + 1 >= 1; i--)
			{
				if (i > maxbit)
					break;
				GamAsciiS((WK_SX + WK_EX - WK_SX - ASC_WID * 12) / 2 + ASC_WID * 7 + ASC_WID * i, (WK_SY + WK_EY - WK_SY - ASC_HGT * 3) / 2 + ASC_HGT, (tnum % 10) + '0');
				tnum /= 10;
			}
			SysCopyToDcStr();
			gam_revlcd((WK_SX + WK_EX - WK_SX - ASC_WID * 12) / 2 + ASC_WID * 7 + ASC_WID * bit, (WK_SY + WK_EY - WK_SY - ASC_HGT * 3) / 2 + ASC_HGT, (WK_SX + WK_EX - WK_SX - ASC_WID * 12) / 2 + ASC_WID * 7 + ASC_WID * bit + ASC_WID - 1, (WK_SY + WK_EY - WK_SY - ASC_HGT * 3) / 2 + ASC_HGT + ASC_HGT - 1);
			showflag = 0;
		}

		GamGetMsg(&Msg);
		if (VM_CHAR_FUN == Msg.type)
		{
			switch (Msg.param)
			{
			case VK_UP:
				if (donum + num <= max)
				{
					donum += num;
					showflag = 1;
				}
				break;
			case VK_DOWN:
				if (donum >= num && donum - num >= min)
				{
					donum -= num;
					showflag = 1;
				}
				break;
			case VK_LEFT:
				if (bit)
				{
					bit -= 1;
					num *= 10;
					showflag = 1;
				}
				break;
			case VK_RIGHT:
				if (bit < maxbit)
				{
					bit += 1;
					num /= 10;
					showflag = 1;
				}
				break;
			case VK_PGUP:
			{
				donum = min;
				showflag = 1;
			}
			break;
			case VK_PGDN:
			{
				donum = max;
				showflag = 1;
			}
			break;
			case VK_ENTER:
				operateflag = 0;
				return (donum);
				break;
			case VK_EXIT:
				operateflag = 0;
				return (0xffff);
			}
		}
	}
}

/******************************************************************************
 * 函数名:ShowTacticNote
 * 说  明:提示电脑策略中
 *
 * 入口参数：无
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-19 23:49	基本功能完成
 ******************************************************************************/
void ShowTacticNote(void)
{
	U8 *str, *astr;
	U8 *kq;
	U8 i, kc;

	ShowMapClear();
	kq = SHARE_MEM;
	str = SHARE_MEM + 200;
	astr = SHARE_MEM + 250;
	kc = GetEnemyKing(g_PlayerKing, kq);
	for (i = 0; i < kc; i++)
	{
		GetPersonName(kq[i], str);
		ResLoadToMem(STRING_CONST, TACTIC_STR, astr);
		gam_strcat(str, astr);
		gam_clrlcd(WK_SX + WK_EX / 2 - (WK_SX + ASC_WID * 19 + 4) / 2, WK_EY - 2 - ASC_HGT - 2, WK_SX + WK_EX / 2 - (WK_SX + ASC_WID * 19 + 4) / 2 + ASC_WID * 19 + 4, WK_EY - 2);
		gam_rect(WK_SX + WK_EX / 2 - (WK_SX + ASC_WID * 19 + 4) / 2, WK_EY - 2 - ASC_HGT - 2, WK_SX + WK_EX / 2 - (WK_SX + ASC_WID * 19 + 4) / 2 + ASC_WID * 19 + 4, WK_EY - 2);
		GamStrShowS(WK_SX + (WK_EX - (WK_SX + ASC_WID * 19)) / 2, WK_EY - 2 - ASC_HGT, (const char *)str);
		GamDelay(50, true);
		// GamDelay(50,false);
	}
	ShowMapClear();
}

// Get all enemies persons.
std::vector<U8> GetEnemyPersonsV(U8 king)
{
	vector<U8> enemyOfficers;

	// Iterate through all cities
	for (U8 cityId = 0; cityId < CITY_MAX; cityId++)
	{
		U8 cityBelong = g_Cities[cityId].Belong;

		// Check if city belongs to an enemy (not player's faction and not empty)
		if (cityBelong && (cityBelong != (king + 1)))
		{
			// Add all officers from this city's PersonV
			for (auto personId : g_Cities[cityId].PersonV)
			{
				// Only add if they belong to this city's faction and aren't the king
				if ((g_Persons[personId].Belong == cityBelong) &&
					((personId + 1) != cityBelong))
				{
					enemyOfficers.push_back(personId);
				}
			}
		}
	}

	return enemyOfficers;
}

U8 GetEnemyPersons(U8 king, U8 *pqueue)
{
	auto officers = GetEnemyPersonsV(king);

	// Copy to array
	for (size_t i = 0; i < officers.size(); i++)
	{
		pqueue[i] = officers[i];
	}

	return officers.size();
}

/******************************************************************************
 * Function: DelPerson
 * Description: Removes an officer from a city's roster using remove-and-swap idiom
 *
 * Input:
 *   city - City ID to remove officer from
 *   person - Officer ID to remove
 * Returns:
 *   0 - Success
 *   1 - Failure (invalid params or officer not found)
 ******************************************************************************/
U8 DelPerson(U8 city, U8 person)
{
	// Validate inputs
	if (city >= CITY_MAX || person >= PERSON_MAX)
	{
		return 1;
	}

	auto &personV = g_Cities[city].PersonV;

	// Find and remove person using remove-and-swap idiom
	auto newEnd = std::remove(personV.begin(), personV.end(), person);

	// Check if person was found and removed
	if (newEnd != personV.end())
	{
		personV.erase(newEnd, personV.end());
		
		// Log person removal
		logPersonRemovedC(person, city, "Order Assignment");
		
		return 0;
	}

	return 1; // Person not found
}

/******************************************************************************
 * Function: GetPersonCity
 * Description: Gets the city ID where a person/officer is currently located
 *
 * Input: person - Officer ID to find
 * Returns: City ID (0xff if not found in any city)
 ******************************************************************************/
U8 GetPersonCity(U8 person)
{
	// Check each city's PersonV for the officer
	for (U8 cityId = 0; cityId < CITY_MAX; cityId++)
	{
		const auto &personV = g_Cities[cityId].PersonV;

		// Use std::find to search for person in vector
		if (std::find(personV.begin(), personV.end(), person) != personV.end())
		{
			return cityId;
		}
	}

	return 0xff; // Person not found in any city
}

/******************************************************************************
 * Function: GetCityPersons
 * Description: Gets list of active officers in a city (excluding captives)
 *
 * Input:
 *   city - City ID to check
 *   pqueue - Array to store officer IDs
 * Returns: Number of active officers found
 ******************************************************************************/
U8 GetCityPersons(U8 city, U8 *pqueue)
{
	U8 count = 0;

	// Get all officers that belong to city's faction
	for (const auto &personId : g_Cities[city].PersonV)
	{
		if (g_Persons[personId].Belong == g_Cities[city].Belong)
		{
			pqueue[count++] = personId;
		}
	}

	return count;
}

/******************************************************************************
 * Function: GetCityOutPersons
 * Description: Gets unaffiliated officers (在野武将) in a city
 *
 * Input:
 *   city - City ID to check
 *   pqueue - Array to store officer IDs
 * Returns: Number of unaffiliated officers found
 ******************************************************************************/
U8 GetCityOutPersons(U8 city, U8 *pqueue)
{
	U8 count = 0;

	// Get all unaffiliated officers from city's PersonV
	for (const auto &personId : g_Cities[city].PersonV)
	{
		// Check if person is unaffiliated (Belong == 0)
		if (g_Persons[personId].Belong == 0)
		{
			pqueue[count++] = personId;
		}
	}

	return count;
}
/******************************************************************************
 * Function: GetCityCaptives
 * Description: Gets captured officers (俘虏) in a city
 *
 * Input:
 *   city - City ID to check
 *   pqueue - Array to store captured officer IDs
 * Returns: Number of captured officers found
 ******************************************************************************/
U8 GetCityCaptives(U8 city, U8 *pqueue)
{
	U8 count = 0;

	// Get all captured officers from city's PersonV
	for (const auto &personId : g_Cities[city].PersonV)
	{
		// Check if person belongs to a different faction than the city
		if (g_Persons[personId].Belong != 0 &&
			g_Persons[personId].Belong != g_Cities[city].Belong)
		{
			pqueue[count++] = personId;
		}
	}

	return count;
}

/******************************************************************************
 * Function: GetWeekCity
 * Description: Gets the weakest city from a list of cities based on total arms
 *
 * Input:
 *   count - Number of cities to check
 *   cqueue - Array of city IDs to check
 * Returns: ID of the weakest city
 ******************************************************************************/
U8 GetWeekCity(U8 count, U8 *cqueue)
{
	U16 weakestArms = 0xffff;
	U8 weakestCity = 0;

	// Check each city in the queue
	for (U8 c = 0; c < count; c++)
	{
		U16 totalArms = 0;
		const auto &city = g_Cities[cqueue[c]];

		// Sum arms from all officers in city's PersonV
		for (const auto &personId : city.PersonV)
		{
			if (g_Persons[personId].Belong == city.Belong)
			{
				totalArms += g_Persons[personId].Arms;
			}
		}

		// Add arms from officers on missions
		const auto *orders = (OrderType *)ORDERQUEUE;
		for (U8 i = 0; i < ORDER_MAX && orders[i].OrderId != 0xff; i++)
		{
			if ((orders[i].OrderId != MOVE) &&
				(orders[i].OrderId != BATTLE) &&
				(orders[i].City == cqueue[c]))
			{
				totalArms += g_Persons[orders[i].Person].Arms;
			}
		}

		// Update weakest city if this one has fewer arms
		if (totalArms < weakestArms)
		{
			weakestArms = totalArms;
			weakestCity = c;
		}
	}

	return cqueue[weakestCity];
}

std::string gbk_to_utf8(const char* gbk_str, size_t gbk_len) {
    iconv_t cd = iconv_open("UTF-8", "GBK");
    if (cd == (iconv_t)-1) {
        std::cerr << "iconv_open error: " << strerror(errno) << std::endl;
        return "";
    }
    
    // Allocate output buffer (4x input size to be safe - UTF-8 can be longer than GBK)
    size_t out_buf_size = gbk_len * 4;
    char* utf8_buf = new char[out_buf_size];
    
    char* in_ptr = const_cast<char*>(gbk_str);
    char* out_ptr = utf8_buf;
    size_t in_bytes_left = gbk_len;
    size_t out_bytes_left = out_buf_size;
    
    if (iconv(cd, &in_ptr, &in_bytes_left, &out_ptr, &out_bytes_left) == (size_t)-1) {
        std::cerr << "iconv error: " << strerror(errno) << std::endl;
        delete[] utf8_buf;
        iconv_close(cd);
        return "";
    }
    
    // Create string from buffer, using the correct length
    std::string result(utf8_buf, out_buf_size - out_bytes_left);
    
    delete[] utf8_buf;
    iconv_close(cd);
    return result;
}

// Function to get tool name in UTF-8 from tool ID
std::string getGoodGbkName(U8 toolId) {
    // Safety check: ensure toolId is valid (GOODS_MAX = 33)
    if (toolId == 0 || toolId >= 33) {
        return "Unknown";
    }
    
    U8 nameBuf[10];
    // Get the base tool ID (remove high bit if it's discovered)
    U8 baseToolId = toolId & 0x7f;
    
    // Additional safety check for base tool ID
    if (baseToolId >= 33) {
        return "Invalid";
    }
    
    // Clear the buffer first
    memset(nameBuf, 0, sizeof(nameBuf));
    
    // Add extra safety check for the buffer
    if (nameBuf == nullptr) {
        return "BufferError";
    }
    
    GetGoodsName(baseToolId, nameBuf);
    
    // Check if the name buffer is valid after the call
    if (nameBuf[0] == 0) {
        return "Empty";
    }
    
    // Convert GBK to UTF-8 for JSON storage
    std::string utf8Name = gbk_to_utf8((const char*)nameBuf, strlen((const char*)nameBuf));
    if (utf8Name.empty()) {
        return "ConversionError";
    }
    
    return utf8Name;
}

// Function to get person name in UTF-8 from person ID
std::string getPersonGbkName(U8 personId) {
    // Safety check: ensure personId is valid (PERSON_MAX = 200)
    if (personId >= 200) {
        return "Unknown";
    }
    
    U8 nameBuf[10];
    // Clear the buffer first
    memset(nameBuf, 0, sizeof(nameBuf));
    
    // Add extra safety check for the buffer
    if (nameBuf == nullptr) {
        return "BufferError";
    }
    
    GetPersonName(personId, nameBuf);
    
    // Check if the name buffer is valid after the call
    if (nameBuf[0] == 0) {
        return "Empty";
    }
    
    // Convert GBK to UTF-8 for JSON storage
    std::string utf8Name = gbk_to_utf8((const char*)nameBuf, strlen((const char*)nameBuf));
    if (utf8Name.empty()) {
        return "ConversionError";
    }
    
    return utf8Name;
}

// Function to get comprehensive debug information for a city
std::string getCityDebugString(U8 cityId) {
    try {
        if (cityId >= CITY_MAX) {
            return "Invalid city ID";
        }
        
        std::stringstream debug;
        const auto& city = g_Cities[cityId];
        
        // City header
        debug << "=== City " << (int)cityId << " (" << city.Name << ") ===" << std::endl;
        debug << "Belong: " << (int)city.Belong << ", SatrapId: " << (int)city.SatrapId << std::endl;
        
        // Add king's name if city belongs to a faction
        if (city.Belong > 0) {
            U8 kingId = city.Belong - 1;  // Convert from 1-based to 0-based index
            if (kingId < PERSON_MAX) {
                try {
                    std::string kingName = getPersonGbkName(kingId);
                    debug << "King: " << kingName << std::endl;
                } catch (...) {
                    debug << "King: Error getting king name" << std::endl;
                }
            } else {
                debug << "King: Invalid King ID" << std::endl;
            }
        } else {
            debug << "King: None (Unclaimed City)" << std::endl;
        }
        
        debug << "Population: " << city.Population << "/" << city.PopulationLimit << std::endl;
        debug << "Food: " << city.Food << ", Money: " << city.Money << std::endl;
        debug << std::endl;
        
        // Persons section
        debug << "--- PERSONS (" << city.PersonV.size() << ") ---" << std::endl;
        if (city.PersonV.empty()) {
            debug << "No persons in city" << std::endl;
        } else {
            for (const auto& personId : city.PersonV) {
                try {
                    std::string personName = getPersonGbkName(personId);
                    std::string status;
                    
                    if (personId >= 200) {
                        status = "INVALID_ID";
                    } else {
                        const auto& person = g_Persons[personId];
                        if (person.Belong == 0)
                        {
                            status = "OFF_CITY";  // 在野武将
                        }
                        else if (person.Belong == city.Belong)
                        {
                            status = "IN_CITY";   // 属于该城市势力
                        }
                        else
                        {
                            status = "CAPTIVE";   // 俘虏
                        }
                    }
                    
                    debug << "  ID:" << std::setw(3) << (int)personId 
                          << " Name:" << std::setw(10) << personName 
                          << " Status:" << status << std::endl;
                } catch (...) {
                    debug << "  ID:" << std::setw(3) << (int)personId 
                          << " Name:ERROR     Status:ERROR" << std::endl;
                }
            }
        }
        debug << std::endl;
        
        // Tools section
        debug << "--- TOOLS (" << city.ToolsV.size() << ") ---" << std::endl;
        if (city.ToolsV.empty()) {
            debug << "No tools in city" << std::endl;
        } else {
            for (const auto& toolId : city.ToolsV) {
                try {
                    std::string toolName = getGoodGbkName(toolId);
                    std::string discovered = (toolId & 0x80) ? "FOUND" : "NOT_FOUND";
                    
                    debug << "  ID:" << std::setw(3) << (int)toolId 
                          << " Name:" << std::setw(10) << toolName 
                          << " Status:" << discovered << std::endl;
                } catch (...) {
                    debug << "  ID:" << std::setw(3) << (int)toolId 
                          << " Name:ERROR     Status:ERROR" << std::endl;
                }
            }
        }
        debug << std::endl;
        
        return debug.str();
    } catch (const std::exception& e) {
        return "ERROR in getCityDebugString: " + std::string(e.what());
    } catch (...) {
        return "ERROR: Unknown error in getCityDebugString";
    }
}

#include "json.hpp"
using json = nlohmann::json;

bool SaveCityJson(U8 idx)
{
	// Create filename
	std::string filename = "city_" + std::to_string(idx) + ".json";

	// Convert cities to JSON
	json citiesJson;
	for (int i = 0; i < CITY_MAX; i++)
	{
		json city;
		// Basic properties
		city["Id"] = g_Cities[i].Id;
		city["Belong"] = g_Cities[i].Belong;
		city["SatrapId"] = g_Cities[i].SatrapId;
		city["FarmingLimit"] = g_Cities[i].FarmingLimit;
		city["Farming"] = g_Cities[i].Farming;
		city["CommerceLimit"] = g_Cities[i].CommerceLimit;
		city["Commerce"] = g_Cities[i].Commerce;
		city["PeopleDevotion"] = g_Cities[i].PeopleDevotion;
		city["AvoidCalamity"] = g_Cities[i].AvoidCalamity;
		city["PopulationLimit"] = g_Cities[i].PopulationLimit;
		city["Population"] = g_Cities[i].Population;
		city["Money"] = g_Cities[i].Money;
		city["Food"] = g_Cities[i].Food;
		city["MothballArms"] = g_Cities[i].MothballArms;
		city["PersonQueue"] = g_Cities[i].PersonQueue;
		city["Persons"] = g_Cities[i].Persons;
		city["ToolQueue"] = g_Cities[i].ToolQueue;
		city["Tools"] = g_Cities[i].Tools;
		city["autoManage"] = g_Cities[i].autoManage;
		city["Name"] = g_Cities[i].Name;

		// Vector and set properties
		city["PersonV"] = g_Cities[i].PersonV;
		city["ToolsV"] = g_Cities[i].ToolsV;
		city["usedPersonsV"] = json(g_Cities[i].usedPersonsV);

		// Add GBK names for all persons in the city
		std::vector<std::string> personNames;
		for (const auto& personId : g_Cities[i].PersonV)
		{
			std::string personName = getPersonGbkName(personId);
			personNames.push_back(personName);
		}
		city["PersonNames"] = personNames;

		// Add GBK names for all tools in the city
		std::vector<std::string> toolNames;
		
		std::cout << "City " << (int)i << " ToolsV: " << g_Cities[i].ToolsV.size() << std::endl;
		for (const auto& toolId : g_Cities[i].ToolsV) {
			std::cout << (int)toolId << " ";
		}
		std::cout << std::endl;
		for (size_t j = 0; j < g_Cities[i].ToolsV.size(); j++)
		{
			const auto toolId = g_Cities[i].ToolsV[j];
			std::string toolName = getGoodGbkName(toolId);
			toolNames.push_back(toolName);
		}
		city["ToolNames"] = toolNames;
		citiesJson.push_back(city);
	}

	// Write JSON to file with pretty printing
	try {
		std::ofstream o(filename);
		o << citiesJson.dump(4);
		o.close();
	} catch (const std::exception& e) {
		std::cerr << "JSON serialization error: " << e.what() << std::endl;
	}
	writeAllCitiesDebugLog("SaveCityJson");
	printAllPersonsDebugInfo();
	writeAllPersonsDebugLog();
	return true;
}

bool LoadCityJson(U8 idx)
{
	// Create filename
	std::string filename = "city_" + std::to_string(idx) + ".json";

	try
	{
		// Read JSON from file
		std::ifstream i(filename);
		json citiesJson;
		i >> citiesJson;

		// Load cities from JSON
		for (int i = 0; i < CITY_MAX; i++)
		{
			const auto &city = citiesJson[i];

			// Basic properties
			g_Cities[i].Id = city["Id"];
			g_Cities[i].Belong = city["Belong"];
			g_Cities[i].SatrapId = city["SatrapId"];
			g_Cities[i].FarmingLimit = city["FarmingLimit"];
			g_Cities[i].Farming = city["Farming"];
			g_Cities[i].CommerceLimit = city["CommerceLimit"];
			g_Cities[i].Commerce = city["Commerce"];
			g_Cities[i].PeopleDevotion = city["PeopleDevotion"];
			g_Cities[i].AvoidCalamity = city["AvoidCalamity"];
			g_Cities[i].PopulationLimit = city["PopulationLimit"];
			g_Cities[i].Population = city["Population"];
			g_Cities[i].Money = city["Money"];
			g_Cities[i].Food = city["Food"];
			g_Cities[i].MothballArms = city["MothballArms"];
			g_Cities[i].PersonQueue = city["PersonQueue"];
			g_Cities[i].Persons = city["Persons"];
			g_Cities[i].ToolQueue = city["ToolQueue"];
			g_Cities[i].Tools = city["Tools"];
			g_Cities[i].autoManage = city["autoManage"];

			// Vector and set properties
			g_Cities[i].PersonV = city["PersonV"].get<std::vector<U8>>();
			g_Cities[i].ToolsV = city["ToolsV"].get<std::vector<U8>>();
			g_Cities[i].usedPersonsV = city["usedPersonsV"].get<std::set<U8>>();
			g_Cities[i].Name = city["Name"];

			// Load PersonNames if present (optional, for future use)
			if (city.contains("PersonNames"))
			{
				// PersonNames are loaded but not stored in the current data structure
				// They can be used for validation or debugging purposes
				auto personNames = city["PersonNames"].get<std::vector<std::string>>();
				// Note: We don't store PersonNames in the current data structure
				// as they can be reconstructed from PersonV using GetPersonName()
			}

			// Load ToolNames if present (optional, for future use)
			if (city.contains("ToolNames"))
			{
				// ToolNames are loaded but not stored in the current data structure
				// They can be used for validation or debugging purposes
				auto toolNames = city["ToolNames"].get<std::vector<std::string>>();
				// Note: We don't store ToolNames in the current data structure
				// as they can be reconstructed from ToolsV using GetGoodsName()
			}
		}

		return true;
	}
	catch (const std::exception &e)
	{
		// Handle any file reading or parsing errors
		return false;
	}
}

// C wrapper functions for calling from C code
U8 SaveCityJsonC(U8 idx)
{
	return SaveCityJson(idx) ? 1 : 0;
}

U8 LoadCityJsonC(U8 idx)
{
	return LoadCityJson(idx) ? 1 : 0;
}

void printCityDebugInfo(U8 cityId) {
	if (cityId >= CITY_MAX) {
		logMessageFromCppFormat("Invalid city ID %d", cityId);
		return;
	}

	// Print city info
	logMessageFromCppFormat("City %d (%s):", cityId, g_Cities[cityId].Name.c_str());
	
	// Get and print unaffiliated persons in city 
	logMessageFromCppFormat("在野武将:");
	for (const auto& personId : g_Cities[cityId].PersonV) {
		if (g_Persons[personId].Belong) {
			continue;
		}
		U8 nameBuf[10];
		GetPersonName(personId, nameBuf);
		logMessageFromCppFormat("ID: %3d, Name: %s", personId, gbk_to_utf8((char*)nameBuf, 10).c_str());
	}
}
void printCityDebugInfoCout(U8 cityId) {
	if (cityId >= CITY_MAX) {
		std::cout << "Invalid city ID " << (int)cityId << std::endl;
		return;
	}

	// Print city info
	std::cout << "City " << (int)cityId << " (" << g_Cities[cityId].Name << "):" << std::endl;
	
	// Get and print unaffiliated persons in city
	std::cout << "在野武将:" << std::endl; 
	for (const auto& personId : g_Cities[cityId].PersonV) {
		if (g_Persons[personId].Belong) {
			continue;
		}
		U8 nameBuf[10];
		GetPersonName(personId, nameBuf);
		std::cout << "ID: " << std::setw(3) << (int)personId 
				 << ", Name: " << gbk_to_utf8((char*)nameBuf, strlen((const char*)nameBuf)) << std::endl;
	}
}

// Function to write debug information for all cities to debug.log
void writeAllCitiesDebugLog(const std::string& operation) {
    try {
        // Open debug log file
        std::ofstream debugLog("debug.log");
        if (!debugLog.is_open()) {
            std::cerr << "ERROR: Could not open debug.log for writing" << std::endl;
            return;
        }
        
        debugLog << "=== " << operation << " - All Cities Debug Info ===" << std::endl;
        debugLog << "Timestamp: " << std::time(nullptr) << std::endl;
        debugLog << std::endl;
        
        // Write debug info for each city
        for (int i = 0; i < CITY_MAX; i++) {
            try {
                debugLog << getCityDebugString(i);
            } catch (const std::exception& e) {
                debugLog << "ERROR: Failed to get debug string for city " << i << ": " << e.what() << std::endl;
            } catch (...) {
                debugLog << "ERROR: Unknown error getting debug string for city " << i << std::endl;
            }
        }
        
        debugLog << "=== " << operation << " completed ===" << std::endl;
        debugLog << std::endl;
        debugLog.close();
        
        std::cout << "Debug log written successfully for operation: " << operation << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ERROR in writeAllCitiesDebugLog: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR: Unknown error in writeAllCitiesDebugLog" << std::endl;
    }
}

// Function to get comprehensive debug information for a person
std::string getPersonDebugString(U8 personId) {
    try {
        if (personId >= PERSON_MAX) {
            return "Invalid person ID";
        }
        
        std::stringstream debug;
        const auto& person = g_Persons[personId];
        
        // Get person name
        std::string personName = getPersonGbkName(personId);
        
        // Person header
        debug << "=== Person " << (int)personId << " (" << personName << ") ===" << std::endl;
        
        // Basic properties
        debug << "ID: " << (int)person.Id << std::endl;
        debug << "Belong: " << (int)person.Belong;
        if (person.Belong > 0) {
            U8 kingId = person.Belong - 1;  // Convert from 1-based to 0-based index
            if (kingId < PERSON_MAX) {
                try {
                    std::string kingName = getPersonGbkName(kingId);
                    debug << " (King: " << kingName << ")";
                } catch (...) {
                    debug << " (King: Error getting king name)";
                }
            } else {
                debug << " (King: Invalid King ID)";
            }
        } else {
            debug << " (Unaffiliated)";
        }
        debug << std::endl;
        
        // Stats
        debug << "Level: " << (int)person.Level << std::endl;
        debug << "Force: " << (int)person.Force << std::endl;
        debug << "IQ: " << (int)person.IQ << std::endl;
        debug << "Devotion: " << (int)person.Devotion << std::endl;
        debug << "Character: " << (int)person.Character << std::endl;
        debug << "Experience: " << (int)person.Experience << std::endl;
        debug << "Thew: " << (int)person.Thew << std::endl;
        debug << "Age: " << (int)person.Age << std::endl;
        
        // Military info
        debug << "ArmsType: " << (int)person.ArmsType << std::endl;
        debug << "Arms: " << person.Arms << std::endl;
        
        // Equipment
        debug << "Equipment: ";
        if (person.Equip[0] || person.Equip[1]) {
            if (person.Equip[0]) {
                std::string equip1Name = getGoodGbkName(person.Equip[0]);
                debug << "Slot1: " << equip1Name << " (ID:" << (int)person.Equip[0] << ")";
            }
            if (person.Equip[1]) {
                if (person.Equip[0]) debug << ", ";
                std::string equip2Name = getGoodGbkName(person.Equip[1]);
                debug << "Slot2: " << equip2Name << " (ID:" << (int)person.Equip[1] << ")";
            }
        } else {
            debug << "None";
        }
        debug << std::endl;
        
        // Location info
        U8 currentCity = GetPersonCity(personId);
        if (currentCity != 0xff) {
            debug << "Current City: " << (int)currentCity << " (" << g_Cities[currentCity].Name << ")" << std::endl;
        } else {
            debug << "Current City: Not in any city (possibly on mission)" << std::endl;
        }
        
        debug << std::endl;
        return debug.str();
    } catch (const std::exception& e) {
        return "ERROR in getPersonDebugString: " + std::string(e.what());
    } catch (...) {
        return "ERROR: Unknown error in getPersonDebugString";
    }
}

// Function to print person debug info to console
// Function to write debug information for all persons to debug.log
void writeAllPersonsDebugLog(const std::string& operation) {
    try {
        // Open debug log file
        std::ofstream debugLog("debug.log", std::ios::app);  // Append mode
        if (!debugLog.is_open()) {
            std::cerr << "ERROR: Could not open debug.log for writing" << std::endl;
            return;
        }
        
        debugLog << "=== " << operation << " - All Persons Debug Info ===" << std::endl;
        debugLog << "Timestamp: " << std::time(nullptr) << std::endl;
        debugLog << std::endl;
        
        // Write debug info for each person
        for (int i = 0; i < PERSON_MAX; i++) {
            try {
                // Only output persons that have valid data (Id != 0)
               
                debugLog << getPersonDebugString(i);
                
            } catch (const std::exception& e) {
                debugLog << "ERROR: Failed to get debug string for person " << i << ": " << e.what() << std::endl;
            } catch (...) {
                debugLog << "ERROR: Unknown error getting debug string for person " << i << std::endl;
            }
        }
        
        debugLog << "=== " << operation << " completed ===" << std::endl;
        debugLog << std::endl;
        debugLog.close();
        
        std::cout << "Debug log written successfully for operation: " << operation << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ERROR in writeAllPersonsDebugLog: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR: Unknown error in writeAllPersonsDebugLog" << std::endl;
    }
}

// Function to print all persons debug info to console
void printAllPersonsDebugInfo() {
    logMessageFromCppFormat("=== All Persons Debug Info ===");
    
    for (int i = 0; i < PERSON_MAX; i++) {
        // Only output persons that have valid data (Id != 0)
		cout<<" person "<<i<<" "<<getPersonDebugString(i)<<endl;
        // if (g_Persons[i].Id != 0) {
        //     printPersonDebugInfo(i);
        // }
    }
    
    logMessageFromCppFormat("=== All Persons Debug Info Completed ===");
}

// ============================================================================
// ACTIVITY LOGGING SYSTEM
// ============================================================================

// Function to get order type name from order ID
std::string getOrderTypeName(U8 orderId) {
    switch (orderId) {
        case ASSART: return "开垦";
        case ACCRACTBUSINESS: return "招商";
        case SEARCH: return "搜寻";
        case FATHER: return "治理";
        case INSPECTION: return "出巡";
        case SURRENDER: return "招降";
        case KILL: return "处斩";
        case BANISH: return "流放";
        case LARGESS: return "赏赐";
        case CONFISCATE: return "没收";
        case EXCHANGE: return "交易";
        case TREAT: return "宴请";
        case TRANSPORTATION: return "输送";
        case MOVE: return "移动";
        case ALIENATE: return "离间";
        case CANVASS: return "招揽";
        case COUNTERESPIONAGE: return "策反";
        case REALIENATE: return "反间";
        case INDUCE: return "劝降";
        case RECONNOITRE: return "侦察";
        case CONSCRIPTION: return "征兵";
        case DISTRIBUTE: return "分配";
        case DEPREDATE: return "掠夺";
        case BATTLE: return "出征";
        default: return "未知命令";
    }
}

// Function to get detailed string for order logging
std::string getOrderDetailedStr(const OrderType* order) {
    if (!order) return "Invalid Order";
    
    try {
        std::string personName = getPersonGbkName(order->Person);
        std::string orderType = getOrderTypeName(order->OrderId);
        std::string cityName = g_Cities[order->City].Name;
        
        std::string details = "Person=" + personName + 
                             " (ID:" + std::to_string(order->Person) + "), " +
                             "Order=" + orderType + 
                             " (ID:" + std::to_string(order->OrderId) + "), " +
                             "City=" + cityName + 
                             " (ID:" + std::to_string(order->City) + ")";
        
        if (order->Object != order->City) {
            std::string objectName = (order->Object < CITY_MAX) ? 
                g_Cities[order->Object].Name : "Person ID " + std::to_string(order->Object);
            details += ", Target=" + objectName + " (ID:" + std::to_string(order->Object) + ")";
        }
        
        return details;
    } catch (const std::exception& e) {
        return "Error generating order details: " + std::string(e.what());
    } catch (...) {
        return "Unknown error generating order details";
    }
}

// Function to log activity to activity.log
void logActivity(const std::string& activity, const std::string& details ) {
    try {
        std::ofstream activityLog("activity.log", std::ios::app);
        if (!activityLog.is_open()) {
            std::cerr << "ERROR: Could not open activity.log for writing" << std::endl;
            return;
        }
        
        // Get current timestamp
        std::time_t now = std::time(nullptr);
        std::string timestamp = std::ctime(&now);
        timestamp.pop_back(); // Remove newline
        
        // Format: [Timestamp] Activity: Details
        activityLog << "[" << timestamp << "] " << activity;
        if (!details.empty()) {
            activityLog << ": " << details;
        }
        activityLog << std::endl;
        
        activityLog.close();
    } catch (const std::exception& e) {
        std::cerr << "ERROR in logActivity: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR: Unknown error in logActivity" << std::endl;
    }
}

// Function to log order creation
void logOrderCreated(const OrderType* order) {
    if (!order) return;
    
    // Check if we should only log user king activities
    if (g_LogOnlyUserKing && !isPersonUserKing(order->Person)) {
        return;
    }
    
    try {
        std::string details = getOrderDetailedStr(order);
        logActivity("ORDER_CREATED", details);
    } catch (const std::exception& e) {
        logActivity("ORDER_CREATED_ERROR", "Failed to log order creation: " + std::string(e.what()));
    } catch (...) {
        logActivity("ORDER_CREATED_ERROR", "Unknown error logging order creation");
    }
}

// Function to log order execution
void logOrderExecuted(const OrderType* order) {
    if (!order) return;
    
    // Check if we should only log user king activities
    if (g_LogOnlyUserKing && !isPersonUserKing(order->Person)) {
        return;
    }
    
    try {
        std::string details = getOrderDetailedStr(order);
        logActivity("ORDER_EXECUTED", details);
    } catch (const std::exception& e) {
        logActivity("ORDER_EXECUTED_ERROR", "Failed to log order execution: " + std::string(e.what()));
    } catch (...) {
        logActivity("ORDER_EXECUTED_ERROR", "Unknown error logging order execution");
    }
}

// Function to log person return to city
void logPersonReturned(U8 personId, U8 cityId, const std::string& reason) {
    // Check if we should only log user king activities
    if (g_LogOnlyUserKing && !isPersonUserKing(personId)) {
        return;
    }
    
    try {
        std::string personName = getPersonGbkName(personId);
        std::string cityName = g_Cities[cityId].Name;
        
        std::string details = "Person=" + personName + 
                             " (ID:" + std::to_string(personId) + "), " +
                             "City=" + cityName + 
                             " (ID:" + std::to_string(cityId) + ")";
        
        if (!reason.empty()) {
            details += ", Reason=" + reason;
        }
        
        logActivity("PERSON_RETURNED", details);
    } catch (const std::exception& e) {
        logActivity("PERSON_RETURNED_ERROR", "Failed to log person return: " + std::string(e.what()));
    } catch (...) {
        logActivity("PERSON_RETURNED_ERROR", "Unknown error logging person return");
    }
}

// Function to log person removal from city
void logPersonRemoved(U8 personId, U8 cityId, const std::string& reason) {
    // Check if we should only log user king activities
    if (g_LogOnlyUserKing && !isPersonUserKing(personId)) {
        return;
    }
    
    try {
        std::string personName = getPersonGbkName(personId);
        std::string cityName = g_Cities[cityId].Name;
        
        std::string details = "Person=" + personName + 
                             " (ID:" + std::to_string(personId) + "), " +
                             "City=" + cityName + 
                             " (ID:" + std::to_string(cityId) + ")";
        
        if (!reason.empty()) {
            details += ", Reason=" + reason;
        }
        
        logActivity("PERSON_REMOVED", details);
    } catch (const std::exception& e) {
        logActivity("PERSON_REMOVED_ERROR", "Failed to log person removal: " + std::string(e.what()));
    } catch (...) {
        logActivity("PERSON_REMOVED_ERROR", "Unknown error logging person removal");
    }
}

// Function to log order deletion
void logOrderDeleted(const OrderType* order) {
    if (!order) return;
    
    // Check if we should only log user king activities
    if (g_LogOnlyUserKing && !isPersonUserKing(order->Person)) {
        return;
    }
    
    try {
        std::string details = getOrderDetailedStr(order);
        logActivity("ORDER_DELETED", details);
    } catch (const std::exception& e) {
        logActivity("ORDER_DELETED_ERROR", "Failed to log order deletion: " + std::string(e.what()));
    } catch (...) {
        logActivity("ORDER_DELETED_ERROR", "Unknown error logging order deletion");
    }
}

// Function to log game turn/cycle
void logGameTurn() {
    try {
        std::string details = "Year=" + std::to_string(g_YearDate) + 
                             ", Month=" + std::to_string(g_MonthDate);
        
        logActivity("GAME_TURN", details);
    } catch (const std::exception& e) {
        logActivity("GAME_TURN_ERROR", "Failed to log game turn: " + std::string(e.what()));
    } catch (...) {
        logActivity("GAME_TURN_ERROR", "Unknown error logging game turn");
    }
}

// Function to log computer tactic generation
void logComputerTactic(U8 cityId, const std::string& tacticType) {
    try {
        std::string cityName = g_Cities[cityId].Name;
        
        std::string details = "City=" + cityName + 
                             " (ID:" + std::to_string(cityId) + "), " +
                             "Tactic=" + tacticType;
        
        logActivity("COMPUTER_TACTIC", details);
    } catch (const std::exception& e) {
        logActivity("COMPUTER_TACTIC_ERROR", "Failed to log computer tactic: " + std::string(e.what()));
    } catch (...) {
        logActivity("COMPUTER_TACTIC_ERROR", "Unknown error logging computer tactic");
    }
}

// C wrapper functions for activity logging

    void logActivityC(const char* activity, const char* details) {
        std::string actStr = activity ? activity : "";
        std::string detStr = details ? details : "";
        logActivity(actStr, detStr);
    }
    
    void logOrderCreatedC(const OrderType* order) {
        logOrderCreated(order);
    }
    
    void logOrderExecutedC(const OrderType* order) {
        logOrderExecuted(order);
    }
    
    void logPersonReturnedC(U8 personId, U8 cityId, const char* reason) {
        std::string reasonStr = reason ? reason : "";
        logPersonReturned(personId, cityId, reasonStr);
    }
    
    void logPersonRemovedC(U8 personId, U8 cityId, const char* reason) {
        std::string reasonStr = reason ? reason : "";
        logPersonRemoved(personId, cityId, reasonStr);
    }
    
    void logOrderDeletedC(const OrderType* order) {
        logOrderDeleted(order);
    }
    
    void logGameTurnC() {
        logGameTurn();
    }
    
    void logComputerTacticC(U8 cityId, const char* tacticType) {
        std::string tacticStr = tacticType ? tacticType : "";
        logComputerTactic(cityId, tacticStr);
    }
    
    // Test function to verify logging system
    void testActivityLogging() {
        logActivity("TEST_START", "Activity logging system test");
        
        // Test with flag enabled (default)
        logActivity("TEST_FLAG_STATUS", "LogOnlyUserKing = " + std::to_string(getLogOnlyUserKing()));
        
        // Test order creation logging
        OrderType testOrder;
        testOrder.OrderId = ASSART;
        testOrder.Person = 0;
        testOrder.City = 0;
        testOrder.Object = 0;
        testOrder.TimeCount = 0;
        logOrderCreatedC(&testOrder);
        
        // Test person removal logging
        logPersonRemovedC(0, 0, "Test Removal");
        
        // Test person return logging
        logPersonReturnedC(0, 0, "Test Return");
        
        // Test order execution logging
        logOrderExecutedC(&testOrder);
        
        // Test order deletion logging
        logOrderDeletedC(&testOrder);
        
        // Test computer tactic logging
        logComputerTacticC(0, "Test Tactic");
        
        // Test game turn logging
        logGameTurnC();
        
        // Test with flag disabled
        logActivity("TEST_DISABLE_FLAG", "Disabling LogOnlyUserKing flag");
        setLogOnlyUserKing(false);
        logActivity("TEST_FLAG_STATUS", "LogOnlyUserKing = " + std::to_string(getLogOnlyUserKing()));
        
        // Test logging again with flag disabled
        logOrderCreatedC(&testOrder);
        logPersonRemovedC(0, 0, "Test Removal (Flag Disabled)");
        logPersonReturnedC(0, 0, "Test Return (Flag Disabled)");
        
        // Re-enable flag
        setLogOnlyUserKing(true);
        logActivity("TEST_ENABLE_FLAG", "Re-enabling LogOnlyUserKing flag");
        
        logActivity("TEST_END", "Activity logging system test completed");
        
        std::cout << "Activity logging test completed. Check activity.log for results." << std::endl;
    }

// C wrapper functions to control logging flag
void setLogOnlyUserKingC(int enabled) {
    setLogOnlyUserKing(enabled != 0);
}

int getLogOnlyUserKingC() {
    return getLogOnlyUserKing() ? 1 : 0;
}

    // Test function to compare old and new order types
    void testOrderTypeComparison() {
        logActivity("ORDER_TYPE_TEST_START", "OrderType comparison test");
        
        // Create array of OrderTypeNew with ORDER_MAX length
        OrderTypeNew newOrders[ORDER_MAX];
        
        // Cast ORDERQUEUE to OrderType array
        OrderType* oldOrders = (OrderType*)ORDERQUEUE;
        
        // Initialize newOrders with content from ORDERQUEUE using memcpy
        memcpy(newOrders, oldOrders, ORDER_MAX * sizeof(OrderType));
        
        // Compare members between old and new order types
        bool allMatch = true;
        int mismatchCount = 0;
        
        for (int i = 0; i < ORDER_MAX; i++) {
            bool orderMatch = true;
            
            if (newOrders[i].OrderId != oldOrders[i].OrderId) {
                logActivity("ORDER_TYPE_MISMATCH", "Order " + std::to_string(i) + " OrderId: old=" + 
                           std::to_string(oldOrders[i].OrderId) + " new=" + std::to_string(newOrders[i].OrderId));
                orderMatch = false;
            }
            
            if (newOrders[i].Person != oldOrders[i].Person) {
                logActivity("ORDER_TYPE_MISMATCH", "Order " + std::to_string(i) + " Person: old=" + 
                           std::to_string(oldOrders[i].Person) + " new=" + std::to_string(newOrders[i].Person));
                orderMatch = false;
            }
            
            if (newOrders[i].City != oldOrders[i].City) {
                logActivity("ORDER_TYPE_MISMATCH", "Order " + std::to_string(i) + " City: old=" + 
                           std::to_string(oldOrders[i].City) + " new=" + std::to_string(newOrders[i].City));
                orderMatch = false;
            }
            
            if (newOrders[i].Object != oldOrders[i].Object) {
                logActivity("ORDER_TYPE_MISMATCH", "Order " + std::to_string(i) + " Object: old=" + 
                           std::to_string(oldOrders[i].Object) + " new=" + std::to_string(newOrders[i].Object));
                orderMatch = false;
            }
            
            if (newOrders[i].Arms != oldOrders[i].Arms) {
                logActivity("ORDER_TYPE_MISMATCH", "Order " + std::to_string(i) + " Arms: old=" + 
                           std::to_string(oldOrders[i].Arms) + " new=" + std::to_string(newOrders[i].Arms));
                orderMatch = false;
            }
            
            if (newOrders[i].Food != oldOrders[i].Food) {
                logActivity("ORDER_TYPE_MISMATCH", "Order " + std::to_string(i) + " Food: old=" + 
                           std::to_string(oldOrders[i].Food) + " new=" + std::to_string(newOrders[i].Food));
                orderMatch = false;
            }
            
            if (newOrders[i].Money != oldOrders[i].Money) {
                logActivity("ORDER_TYPE_MISMATCH", "Order " + std::to_string(i) + " Money: old=" + 
                           std::to_string(oldOrders[i].Money) + " new=" + std::to_string(newOrders[i].Money));
                orderMatch = false;
            }
            
            if (newOrders[i].Consume != oldOrders[i].Consume) {
                logActivity("ORDER_TYPE_MISMATCH", "Order " + std::to_string(i) + " Consume: old=" + 
                           std::to_string(oldOrders[i].Consume) + " new=" + std::to_string(newOrders[i].Consume));
                orderMatch = false;
            }
            
            if (newOrders[i].TimeCount != oldOrders[i].TimeCount) {
                logActivity("ORDER_TYPE_MISMATCH", "Order " + std::to_string(i) + " TimeCount: old=" + 
                           std::to_string(oldOrders[i].TimeCount) + " new=" + std::to_string(newOrders[i].TimeCount));
                orderMatch = false;
            }
            
            if (!orderMatch) {
                mismatchCount++;
                allMatch = false;
            }
        }
        
        // Log size comparison
        logActivity("ORDER_TYPE_SIZE_COMPARISON", "sizeof(OrderType)=" + std::to_string(sizeof(OrderType)) + 
                   " sizeof(OrderTypeNew)=" + std::to_string(sizeof(OrderTypeNew)));
        
        // Log results
        if (allMatch) {
            logActivity("ORDER_TYPE_TEST_RESULT", "SUCCESS: All " + std::to_string(ORDER_MAX) + " orders match perfectly");
        } else {
            logActivity("ORDER_TYPE_TEST_RESULT", "FAILURE: " + std::to_string(mismatchCount) + " orders have mismatches");
        }
        
        logActivity("ORDER_TYPE_TEST_END", "OrderType comparison test completed");
        
        std::cout << "OrderType comparison test completed. Check activity.log for results." << std::endl;
        std::cout << "Size comparison: OrderType=" << sizeof(OrderType) << " bytes, OrderTypeNew=" << sizeof(OrderTypeNew) << " bytes" << std::endl;
        std::cout << "Match result: " << (allMatch ? "SUCCESS" : "FAILURE") << std::endl;
    }

// Function declarations
void testActivityLogging();
void testOrderTypeComparison();

// Uncomment the line below to run the OrderType comparison test
// testOrderTypeComparison();




