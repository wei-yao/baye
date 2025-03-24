/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	showface.c
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
*	1.0    2005-6-10  陈泽伟     基本的功能完成
***********************************************************************/

#include "stdsys.h"
#include "comm.h"
#undef	SHOWFACE_C
#define	SHOWFACE_C
#include "enghead.h"
/*void GetCityProStr(U8 city,U8 pro,U8 *str);
void GetPersonProStr(U8 person,U8 pro,U8 *str);
void GetPersonName(U8 person,U8 *str);*/

//void printCityDebugInfo(U8 cityId);

extern int g_stat;
extern int ngthird1;
extern int g_noUseStrMem;

/******************************************************************************
* 函数名:ShowGoodsPro
* 说  明:显示道具属性
*
* 入口参数：person -- 道具编号，pro -- 起始属性编号，x,y -- 显示坐标，wid -- 显示宽度
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005/5/18 11:26AM	基本功能完成
******************************************************************************/
void ShowGoodsPro(U8 goods,U8 pro,U8 x,U8 y,U8 wid)
{
	U8 sx,sy;
	U8 str[20];
	U8 i;
	U8 ptr[7];
	PosItemType positem;
	
	InitItem(x,y,x + wid - 1,y + ASC_HGT,&positem);
	
	if (AddItem(ASC_WID * 10 + 1,ASC_HGT,&positem,&sx,&sy))
	{
		GetGoodsName(goods,str);
		PlcMidShowStr(sx + ASC_WID * 10 / 2,sy,str);
		/*GamStrShowS(sx,sy,str);*/
	}
	else
	{
		return;
	}
    //g_tlen = GOODS_PRO_WIDLen;
    ResLoadToMem(IFACE_CONID,GOODS_PRO_WID,ptr);
	for (i = pro;i < 5;i ++)
	{
		if (AddItem(ASC_WID * ptr[i] + 1,ASC_HGT,&positem,&sx,&sy))
		{
			GetGoodsProStr(goods,i,str);
			PlcMidShowStr(sx + ASC_WID * ptr[i] / 2,sy,str);
			/*GamStrShowS(sx,sy,str);*/
		}
		else
		{
			break;
		}
	}
}


/******************************************************************************
* 函数名:GetGoodsProStr
* 说  明:获取道具属性值字符串
*
* 入口参数：goods -- 道具编号，pro -- 属性编号，str -- 属性字符串
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-7-28 14:57	基本功能完成
******************************************************************************/
void GetGoodsProStr(U8 goods,U8 pro,U8 *str)
{
	U8 idx;
	GOODS *gptr;
	
	
	gptr = (GOODS *) ResLoadToCon(GOODS_RESID,1,g_CBnkPtr);
	gptr = &gptr[goods];
	switch (pro)
	{
	case 0:		/*用法*/
		switch (gptr->useflag)
		{
		case 0:
			idx = GOODS_USEFLAG0;
			break;
		case 1:
			idx = GOODS_USEFLAG1;
			break;
		}
		ResLoadToMem(STRING_CONST,idx,str);
		break;
	case 1:		/*加武力*/
		gam_itoa(gptr->at,str,10);
		break;
	case 2:		/*加智力*/
		gam_itoa(gptr->iq,str,10);
		break;
	case 3:		/*加移动*/
		gam_itoa(gptr->move,str,10);
		break;
	case 4:		/*改兵种*/
        if(g_useMOD == 1)
        {
            switch (gptr->arm)
            {
            case 255:		/*无*/
                idx = ATRR_STR70;
                break;
            case 0:
                idx = ATRR_STR11;
                break;
            case 1:
                idx = ATRR_STR12;
                break;
            case 2:
                idx = ATRR_STR13;
                break;
            case 3:
                idx = ATRR_STR14;
                break;
            case 4:
                idx = ATRR_STR15;
                break;
            case 5:
                idx = ATRR_STR16;
                break;
            }
        }
        else
        {
		switch (gptr->arm)
		{
		case 0:		/*无*/
			idx = ATRR_STR70;
			break;
		case 1:		/*水兵*/
			idx = GOODS_ARM1;
			break;
		case 2:		/*玄兵*/
			idx = GOODS_ARM2;
			break;
		case 3:		/*极兵*/
			idx = GOODS_ARM3;
			break;
        }
        }
		ResLoadToMem(STRING_CONST,idx,str);
		break;
	}
}

/******************************************************************************
* 函数名:ShowGoodsProStr
* 说  明:显示道具属性字符串
*
* 入口参数：pro -- 起始属性编号，x,y -- 显示坐标，wid -- 显示宽度
*
* 出口参数：下一个显示属性编号
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005/5/18 11:26AM	基本功能完成
******************************************************************************/
U8 ShowGoodsProStr(U8 pro,U8 x,U8 y,U8 wid)
{
	U8 sx,sy;
	U8 str[12];
	U8 ptr[7];
	U8 i;
	PosItemType positem;
	
	InitItem(x,y,x + wid - 1,y + ASC_HGT,&positem);
	
	if (AddItem(ASC_WID * 10 + 1,ASC_HGT,&positem,&sx,&sy))
	{
		ResLoadToMem(STRING_CONST,GOODS_ATRR_STR0,str);
		PlcMidShowStr(sx + ASC_WID * 10 / 2,sy,str);
		/*GamStrShowS(sx,sy,str);*/
	}
	else
	{
		return(0);
	}
    //g_tlen = GOODS_PRO_WIDLen;
    ResLoadToMem(IFACE_CONID,GOODS_PRO_WID,ptr);
	for (i = pro;i < 5;i ++)
	{
		if (AddItem(ASC_WID * ptr[i] + 1,ASC_HGT,&positem,&sx,&sy))
		{
			ResLoadToMem(STRING_CONST,GOODS_ATRR_STR1 + i,str);
			PlcMidShowStr(sx + ASC_WID * ptr[i] / 2,sy,str);
			/*GamStrShowS(sx,sy,str);*/
		}
		else
		{
			return(i);
		}
	}
	return(i);

}

/******************************************************************************
* 函数名:ShowGoodsControl
* 说  明:显示、选择武将
*
* 入口参数：goods -- 道具队列，gcount -- 道具总数，x0 y0 x1 y1 -- 显示范围
*
* 出口参数：0xff -- 退出，其他值 -- 选中武将序号
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005/5/18 11:26AM	基本功能完成
******************************************************************************/
 U8 ShowGoodsControl(U8 *goods,U8 gcount,U8 x0,U8 y0,U8 x1,U8 y1)
{
	U8 i,showflag,count,top,set;
	U8 spc,spcv[6];
	U8 wid;
	GMType Msg;
	
	if (!gcount)
		return(0xff);
		
	/*gam_clrlcd(x0,y0,x1,y1);*/
	
	wid = x1 - x0 + 1;
	count = (y1 - y0 + 1) / ASC_HGT - 1;
	if (count > gcount)
	{
		count = gcount;
	}
	y1 = y0 + count * ASC_HGT + ASC_HGT;
	gam_rect(x0,y0,x1,y1 + 1);
	x0 += 1;
	y0 += 1;
	x1 -= 1;
	
	top = 0;
	set = 0;
	spc = 0;
	spcv[0] = 0;
	showflag = 1;
	while (1)
	{
		if (showflag)
		{
			gam_clrlcd(x0,y0,x1,y0 + count * ASC_HGT + ASC_HGT - 1);
			spcv[spc + 1] = ShowGoodsProStr(spcv[spc],x0,y0,wid);
			for (i = 0;i < count;i ++)
			{
				if (i >= gcount)
					break;
				
				ShowGoodsPro(goods[top + i],spcv[spc],x0,y0 + ASC_HGT * i + ASC_HGT,wid);
			}
			gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 10 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
			showflag = 0;
		}
		
		GamGetMsg(&Msg);
		
		if (VM_CHAR_FUN == Msg.type)
		{
			switch (Msg.param)
			{
			case VK_UP:
				if (set)
				{
					gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 10 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
					set -= 1;
					if (set < top)
					{
						top = set;
						showflag = 1;
					}
					else
					{
						gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 10 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
					}
				}
				break;
			case VK_DOWN:
				if (set < gcount - 1)
				{
					gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 10 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
					set += 1;
					if (set >= top + count)
					{
						top = set + 1 - count;
						showflag = 1;
					}
					else
					{
						gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 10 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
					}
				}
				break;
			case VK_LEFT:
				if (spcv[spc])
				{
					spc -= 1;
					showflag = 1;
				}
				break;
			case VK_RIGHT:
				if (spcv[spc + 1] < 5)
				{
					spc += 1;
					showflag = 1;
				}
				break;
			case VK_ENTER:
				return(set);
			case VK_EXIT:
				return(0xff);
			}
		}
	}
	
	return(0xff);
}


/******************************************************************************
* 函数名:GetGoodsName
* 说  明:获取道具名称
*
* 入口参数：goods -- 道具编号，str -- 武将名称
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-7-28 14:45	基本功能完成
******************************************************************************/
 void GetGoodsName(U8 goods,U8 *str)
{
	/*GOODS *gptr;
	
	gptr = (GOODS *) ResLoadToCon(GOODS_RESID,1,g_CBnkPtr);
	gam_memcpy(str,gptr[goods].name,10);
	str[10] = 0;*/
	ResLoadToMem(GOODS_NAME,goods + 1,str);
}

/******************************************************************************
* 函数名:GetPersonName
* 说  明:获取武将名称
*
* 入口参数：person -- 武将编号，str -- 武将名称
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-10 10:16	基本功能完成
******************************************************************************/
 void GetPersonName(U8 person,U8 *str)
{
	U8 l;
	
	switch (g_PIdx)
	{
	case 1:
		l = GENERAL_NAME;
		break;
	case 2:
		l = GENERAL_NAME2;
		break;
	case 3:
		l = GENERAL_NAME3;
		break;
	case 4:
		l = GENERAL_NAME4;
		break;
	}
	ResLoadToMem(l,person + 1,str);
/***********************************************************/
	/*dptr = ResLoadToCon(GENERAL_NAME,g_PIdx,g_CBnkPtr);
	pnt = person;
	pnt *= 8;
	gam_memcpy(str,&dptr[pnt],8);
	str[8] = 0;*/
/***********************************************************/
}

/******************************************************************************
* 函数名:ShowPersonPro
* 说  明:显示武将属性
*
* 入口参数：person -- 武将编号，pro -- 起始属性编号，x,y -- 显示坐标，wid -- 显示宽度
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005/5/18 11:26AM	基本功能完成
******************************************************************************/
void ShowPersonPro(U8 person,U8 pro,U8 x,U8 y,U8 wid)
{
	U8 sx,sy;
	U8 str[20];
	U8 ptr[14];
	U8 i;
	PosItemType positem;
	
	InitItem(x,y,x + wid - 1,y + ASC_HGT,&positem);
	
	if (AddItem(ASC_WID * 8 + 1,ASC_HGT,&positem,&sx,&sy))
	{
		GetPersonName(person,str);
		PlcMidShowStr(sx + ASC_WID * 8 / 2,sy,str);
		/*GamStrShowS(sx,sy,str);*/
	}
	else
	{
		return;
	}
    //g_tlen = PERSON_PRO_WIDLen;
    ResLoadToMem(IFACE_CONID,PERSON_PRO_WID,ptr);
	for (i = pro;i < 13;i ++)
	{
		if (AddItem(ASC_WID * ptr[i] + 1,ASC_HGT,&positem,&sx,&sy))
		{
			GetPersonProStr(person,i,str);
			PlcMidShowStr(sx + ASC_WID * ptr[i] / 2,sy,str);
			/*GamStrShowS(sx,sy,str);*/
		}
		else
		{
			break;
		}
	}
}

/******************************************************************************
* 函数名:GetPersonProStr
* 说  明:获取武将属性值字符串
*
* 入口参数：person -- 武将编号，pro -- 属性编号，str -- 属性字符串
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005/5/18 11:26AM	基本功能完成
******************************************************************************/
void GetPersonProStr(U8 person,U8 pro,U8 *str)
{
	U8 idx;
	U8 b;
	
	b = g_Persons[person].Belong;
	switch (pro)
	{
	case 0:		/*归属*/
		if (0xff == b)
		{
			ResLoadToMem(STRING_CONST,ATRR_STR69,str);
		}
		else if (b == person + 1)
		{
			ResLoadToMem(STRING_CONST,ATRR_STR68,str);
		}
		else if (b)
		{
			GetPersonName(b - 1,str);
		}
		else
		{
			ResLoadToMem(STRING_CONST,ATRR_STR67,str);
		}
		break;
	case 1:		/*所在城市*/
		GetCityName(GetPersonCity(person),str);
		break;
	case 2:		/*等级*/
		gam_itoa(g_Persons[person].Level,str,10);
		break;
	case 3:		/*武力*/
		gam_itoa(g_Persons[person].Force,str,10);
		break;
	case 4:		/*智力*/
		gam_itoa(g_Persons[person].IQ,str,10);
		break;
	case 5:		/*忠诚*/
		if (b == (person + 1))
		{
			ResLoadToMem(STRING_CONST,ATRR_STR71,str);
		}
		else if (b)
		{
			gam_itoa(g_Persons[person].Devotion,str,10);
		}
		else
		{
			ResLoadToMem(STRING_CONST,ATRR_STR71,str);
		}
		
		break;
	case 6:		/*经验*/
		gam_itoa(g_Persons[person].Experience,str,10);
		break;
	case 7:		/*体力*/
		gam_itoa(g_Persons[person].Thew,str,10);
		break;
	case 8:		/*兵种*/
		switch (g_Persons[person].ArmsType)
		{
		case 0:
			idx = ATRR_STR11;
			break;
		case 1:
			idx = ATRR_STR12;
			break;
		case 2:
			idx = ATRR_STR13;
			break;
		case 3:
			idx = ATRR_STR14;
			break;
		case 4:
			idx = ATRR_STR15;
			break;
		case 5:
			idx = ATRR_STR16;
			break;
		}
		ResLoadToMem(STRING_CONST,idx,str);
		break;
	case 9:		/*兵力*/
		gam_itoa(g_Persons[person].Arms,str,10);
		break;
	case 10:		/*年龄*/
		gam_itoa(g_Persons[person].Age,str,10);
		break;
	case 11:		/*道具一*/
		str[0] = 0;
		if (g_Persons[person].Equip[0])
			GetGoodsName(g_Persons[person].Equip[0] - 1,str);
		break;
	case 12:		/*道具二*/
		str[0] = 0;
		if (g_Persons[person].Equip[1])
			GetGoodsName(g_Persons[person].Equip[1] - 1,str);
		break;
	}
}

/******************************************************************************
* 函数名:ShowPersonProStr
* 说  明:显示武将属性字符串
*
* 入口参数：pro -- 起始属性编号，x,y -- 显示坐标，wid -- 显示宽度
*
* 出口参数：下一个显示属性编号
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005/5/18 11:26AM	基本功能完成
******************************************************************************/
U8 ShowPersonProStr(U8 pro,U8 x,U8 y,U8 wid)
{
	U8 sx,sy;
	U8 str[12];
	U8 ptr[14];
	U8 i;
	PosItemType positem;

	InitItem(x,y,x + wid - 1,y + ASC_HGT,&positem);
	
	if (AddItem(ASC_WID * 8 + 1,ASC_HGT,&positem,&sx,&sy))
	{
		ResLoadToMem(STRING_CONST,ATRR_STR17,str);
		PlcMidShowStr(sx + ASC_WID * 8 / 2,sy,str);
		/*GamStrShowS(sx,sy,str);*/
	}
	else
	{

		return(0);
	}
    //g_tlen = PERSON_PRO_WIDLen;
    ResLoadToMem(IFACE_CONID,PERSON_PRO_WID,ptr);
	for (i = pro;i < 13;i ++)
	{
		if (AddItem(ASC_WID * ptr[i] + 1,ASC_HGT,&positem,&sx,&sy))
		{
			ResLoadToMem(STRING_CONST,ATRR_STR18 + i,str);
			PlcMidShowStr(sx + ASC_WID * ptr[i] / 2,sy,str);
			/*GamStrShowS(sx,sy,str);*/
		}
		else
		{

			return(i);
		}
	}

	return(i);

}

/******************************************************************************
* 函数名:ShowPersonControl
* 说  明:显示、选择武将
*
* 入口参数：person -- 武将队列，pcount -- 武将总数，x0 y0 x1 y1 -- 显示范围
*
* 出口参数：0xff -- 退出，其他值 -- 选中武将序号
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005/5/18 11:26AM	基本功能完成
******************************************************************************/
 U8 ShowPersonControl(U8 *person,U8 pcount,U8 x0,U8 y0,U8 x1,U8 y1)
{
	U8 i,showflag,count,top,set;
	U8 spc,spcv[7];
	U8 wid;
	GMType Msg;
	
	if (!pcount)
		return(0xff);
		
	/*gam_clrlcd(x0,y0,x1,y1);*/

	wid = x1 - x0 + 1 - 2;
	count = (y1 - y0 + 1 - 2) / ASC_HGT - 1;
	if (count > pcount)
	{
		count = pcount;
	}
	y1 = y0 + count * ASC_HGT + ASC_HGT;
	gam_rect(x0,y0,x1,y1 + 1);
	x0 += 1;
	y0 += 1;
	x1 -= 1;
	
	top = 0;
	set = 0;
	spc = 0;
	spcv[0] = 0;
    showflag = 1;
    ngthird1 = 1;
	while (1)
	{
		if (showflag)
		{

            gam_clrlcd(x0,y0,x1,y1);

            g_noUseStrMem = 1;
            SysCopyFromDcToStr();

			spcv[spc + 1] = ShowPersonProStr(spcv[spc],x0,y0,wid);
			for (i = 0;i < count;i ++)
			{
				if (i >= pcount)
					break;
				
				ShowPersonPro(person[top + i],spcv[spc],x0,y0 + ASC_HGT * i + ASC_HGT,wid);
			}

            g_noUseStrMem = 0;
            SysCopyToDcStr();

			gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 8 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
			showflag = 0;
		}
		
		GamGetMsg(&Msg);
		
		if (VM_CHAR_FUN == Msg.type)
		{
			switch (Msg.param)
			{
			case VK_PGUP:
				if (set)
				{
					if (set < count)
						set = 1;
					else
						set -= (count - 1);
					showflag = 1;
				}
			case VK_UP:
				if (set)
				{
					gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 8 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
					set -= 1;
					if (set < top)
					{
						top = set;
						showflag = 1;
					}
					else
					{
						gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 8 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
					}
				}
				break;
			case VK_PGDN:
				if (set < pcount - 2)
				{
					set += (count - 1);
					if (set >= pcount - 1)
						set = pcount - 2;
					showflag = 1;
				}
			case VK_DOWN:
				if (set < pcount - 1)
				{
					gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 8 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
					set += 1;
					if (set >= top + count)
					{
						top = set + 1 - count;
						showflag = 1;
					}
					else
					{
						gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 8 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
					}
				}
				break;
			case VK_LEFT:
				if (spcv[spc])
				{
					spc -= 1;
					showflag = 1;
				}
				break;
			case VK_RIGHT:
				if (spcv[spc + 1] < 13)
				{
					spc += 1;
					showflag = 1;
				}
				break;
			case VK_ENTER:
                g_stat = set;
                ngthird1 = 0;
				return(set);
			case VK_EXIT:
                ngthird1 = 0;
				return(0xff);
			}
            g_stat = set;

		}
	}
	
	return(0xff);
}


/******************************************************************************
* 函数名:GetCityName
* 说  明:获取武将名称
*
* 入口参数：city -- 城市编号，str -- 城市名称
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-10 10:18	基本功能完成
******************************************************************************/
 void GetCityName(U8 city,U8 *str)
{
	/*U8 *dptr;
	U16 cnt;
	
	dptr = ResLoadToCon(CITY_NAME,g_PIdx,g_CBnkPtr);
	cnt = city;
	cnt *= 10;
	gam_memcpy(str,&dptr[cnt],10);
	str[10] = 0;*/
	
	ResLoadToMem(CITY_NAME,city + 1,str);
}

/******************************************************************************
* 函数名:GetCityProStr
* 说  明:获取城市属性字符串
*
* 入口参数：city -- 城市编号，pro -- 属性编号，str -- 属性字符串
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-10 9:47	基本功能完成
******************************************************************************/
void GetCityProStr(U8 city,U8 pro,U8 *str)
{
	U8 strbuf[20];
	
	str[0] = '\0';
	switch (pro)
	{
	case 0:		/*归属*/
		ResLoadToMem(STRING_CONST,ATRR_STR1,str);
		// printf("city: %d, Belong: %d\n", city, g_Cities[city].Belong);
		if (g_Cities[city].Belong)
		{
			GetPersonName(g_Cities[city].Belong - 1,&str[5]);
		}
		else
		{
			ResLoadToMem(STRING_CONST,ATRR_STR70,&str[5]);
		}
		break;
	case 1:		/*太守*/
		ResLoadToMem(STRING_CONST,ATRR_STR2,str);
		if (g_Cities[city].SatrapId)
		{
			GetPersonName(g_Cities[city].SatrapId - 1,&str[5]);
		}
		else
		{
			ResLoadToMem(STRING_CONST,ATRR_STR70,&str[5]);
		}
		break;
	case 2:		/*农业*/
		ResLoadToMem(STRING_CONST,ATRR_STR3,str);
		gam_itoa(g_Cities[city].Farming,strbuf,10);
		gam_strcat(str,strbuf);
		str[gam_strlen(str) + 1] = '\0';
		str[gam_strlen(str)] = '/';
		gam_itoa(g_Cities[city].FarmingLimit,strbuf,10);
		gam_strcat(str,strbuf);
		break;
	case 3:		/*商业*/
		ResLoadToMem(STRING_CONST,ATRR_STR4,str);
		gam_itoa(g_Cities[city].Commerce,strbuf,10);
		gam_strcat(str,strbuf);
		str[gam_strlen(str) + 1] = '\0';
		str[gam_strlen(str)] = '/';
		gam_itoa(g_Cities[city].CommerceLimit,strbuf,10);
		gam_strcat(str,strbuf);/**/
		break;
	case 4:		/*民忠*/
		ResLoadToMem(STRING_CONST,ATRR_STR5,str);
		gam_itoa(g_Cities[city].PeopleDevotion,strbuf,10);
		gam_strcat(str,strbuf);
		break;
	case 5:		/*防灾*/
		ResLoadToMem(STRING_CONST,ATRR_STR6,str);
		gam_itoa(g_Cities[city].AvoidCalamity,strbuf,10);
		gam_strcat(str,strbuf);
		break;
	case 6:		/*人口*/
		ResLoadToMem(STRING_CONST,ATRR_STR7,str);
		gam_ltoa(g_Cities[city].Population,strbuf,10);
		gam_strcat(str,strbuf);
		break;
	case 7:		/*金钱*/
		ResLoadToMem(STRING_CONST,ATRR_STR8,str);
		gam_itoa(g_Cities[city].Money,strbuf,10);
		gam_strcat(str,strbuf);
		break;
	case 8:		/*粮食*/
		ResLoadToMem(STRING_CONST,ATRR_STR9,str);
		gam_itoa(g_Cities[city].Food,strbuf,10);
		gam_strcat(str,strbuf);
		break;
	case 9:		/*后备兵力*/
		ResLoadToMem(STRING_CONST,ATRR_STR10,str);
		gam_itoa(g_Cities[city].MothballArms,strbuf,10);
		gam_strcat(str,strbuf);
		break;
	case 10:	/*城市状态*/
		ResLoadToMem(STRING_CONST,ATRR_STR0,str);
		GetCityState(city,strbuf);
		gam_strcat(str,strbuf);
		break;
	//todo : to show the auto status of city
	}
}

 void GetCityState(U8 city,U8 *str)
{
	U8 state;
	
	switch (g_Cities[city].Id)
	{
	case STATE_NORMAL:	/*正常*/
		state = CITY_STATE_STR1;
		break;
	case STATE_FAMINE:	/*饥荒*/
		state = CITY_STATE_STR2;
		break;
	case STATE_DROUGHT:	/*旱灾*/
		state = CITY_STATE_STR3;
		break;
	case STATE_FLOOD:	/*水灾*/
		state = CITY_STATE_STR4;
		break;
	case STATE_REBELLION:	/*暴动*/
		state = CITY_STATE_STR5;
		break;
	}
	ResLoadToMem(STRING_CONST,state,str);
}

/******************************************************************************
* 函数名:ShowCityPro
* 说  明:显示城市属性
*
* 入口参数：city -- 城市编号
*
* 出口参数：0 -- 退出，1 -- 确认
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-9 17:19	基本功能完成
******************************************************************************/
 U8 ShowCityPro(U8 city)
{
	U8 showflag,showtop,i;
	U8 str[30];
	GMType Msg;
	
	showtop = 0;
	showflag = 1;
	// printCityDebugInfo(city);
	while (1)
	{
		if (showflag)
		{
			gam_clrlcd((WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX - 5,WK_SY + 2,(WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX + ASC_WID * 15 + 5,WK_SY + 2 + (ASC_HGT + 1) * ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)));
			gam_rect((WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX - 5 - 1,WK_SY + 2 - 1,(WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX + ASC_WID * 15 + 5 + 1,WK_SY + 2 + (ASC_HGT + 1) * ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)) + 1);
			GetCityName(city,str);
            g_noUseStrMem = 1;
            SysCopyFromDcToStr();

			GamStrShowS((WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX,WK_SY + 2,(const char*)str);
			for (i = 0;i < ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)) - 1;i ++)
			{
				GetCityProStr(city,showtop + i,str);
				GamStrShowS((WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX, WK_SY + 2 + ASC_HGT + (ASC_HGT + 1) * i,(const char*)str);
			}
            g_noUseStrMem = 0;
            SysCopyToDcStr();

			showflag = 0;
		}
		
		GamGetMsg(&Msg);
		
		if (VM_CHAR_FUN == Msg.type)
		{
			switch (Msg.param)
			{
			case VK_UP:
			case VK_LEFT:
				if (showtop)
				{
					showtop -= ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)) - 1;
					showflag = 1;
				}
				break;
			case VK_DOWN:
			case VK_RIGHT:
				if (showtop + ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)) - 1 < 11)
				{
					showtop += ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)) - 1;
					showflag = 1;
				}
				break;
			case VK_ENTER:
				return(1);
			case VK_EXIT:
				return(0);
			}
		}
	}
	
	return(0);
}


/******************************************************************************
* 函数名:MenuComm
* 说  明:菜单程序
*
* 入口参数：menu -- 菜单结构
*
* 出口参数：0xff -- 退出，其他值 -- 选中项
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-13 9:41	基本功能完成
******************************************************************************/
/* U8 MenuComm(MenuType *menu)
{
	U8 scount,i;
	U8 showflag;
	GMType Msg;
	
	
	showflag = 1;
	scount = (menu->ey - menu->sy + 1) / ASC_HGT;
	if (scount > menu->count)
	{
		scount = menu->count;
	}
	gam_rect(menu->sx - 1,menu->sy - 1,menu->ex + 1,menu->sy + scount * ASC_HGT - 1 + 1);
	while (1)
	{
		if (showflag)
		{
			gam_clrlcd(menu->sx,menu->sy,menu->ex,menu->sy + scount * ASC_HGT - 1);
			for (i = 0;i < scount;i ++)
			{
				GamStrShowS(menu->sx,menu->sy + i * ASC_HGT,menu->menustr[i + menu->top]);
			}
			gam_revlcd(menu->sx,menu->sy + (menu->set - menu->top) * ASC_HGT,menu->ex,menu->sy + (menu->set - menu->top) * ASC_HGT + ASC_HGT - 1);
			showflag = 0;
		}
		
		GamGetMsg(&Msg);
		
		if (VM_CHAR_FUN == Msg.type)
		{
			switch (Msg.param)
			{
			case VK_UP:
			case VK_LEFT:
				
				if (!menu->set)
				{
					menu->set = menu->count - 1;
					if (scount < menu->count)
					{
						menu->top = menu->set - scount + 1;
					}
				}
				else
				{
					if (menu->top == menu->set)
					{
						menu->top -= 1;
					}
					menu->set -= 1;
				}
				showflag = 1;
				break;
			case VK_DOWN:
			case VK_RIGHT:
				
				if (menu->set >= menu->count - 1)
				{
					menu->set = 0;
					menu->top = 0;
				}
				else
				{
					menu->set += 1;
					if (menu->set - menu->top >= scount)
					{
						menu->top += 1;
					}
				}
				showflag = 1;
				break;
			case VK_ENTER:
				return(menu->set);
			case VK_EXIT:
				return(0xff);
			}
		}
	}
	return(0xff);
}*/
