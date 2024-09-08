#ifndef	_EXTERN_H
#define	_EXTERN_H

#ifndef	_VAR_MAL_			/* 屏蔽在变量定义文件中对本头文件的包含 */

/*======================================
  程序中所有常量和所有变量的引用
======================================*/

/*------------------------
  常量的引用
  ------------------------*/
#include "pstring.h"		/*程序字符串常量定义头文件*/
#include "pconst.h"		/*程序数据常量定义头文件*/

/*------------------------
  变量的引用
  ------------------------*/
/*in comIn.c*/
extern U8	g_GamKeySound;		/*按键声音*/
extern U16	g_GamKBState;		/*键盘状态*/
extern U8	*g_CBnkPtr;		/*常量页起始地址指针*/
extern U8	*g_CBnkPtr2;
//extern RandEnvType	g_RandEnv;	/*随机变量*/
extern gam_FILE	*g_LibFp;		/*资源库文件指针*/
extern gam_FILE	*g_FontFp;		/*字库文件指针*/
extern gam_FILE	*g_RlbFp;		/*字库文件指针*/
extern gam_FILE	*g_ModFp;		/*MOD文件指针*/
/* in comOut.c */
extern U8	*c_VisScr;		/*虚拟屏幕指针*/
extern U8	c_ReFlag;		/* 恢复标志 */
extern U8	c_Sx,c_Sy,c_Ex,c_Ey;	/*字符串显示区域*/
extern U16	c_FontBnkS;		/*汉字库存放的起始页号，由调用者初始化*/

/*引擎变量*/
extern U8	*g_VisScr,*g_BVisScr;	/*虚拟屏幕指针 二级虚拟屏幕指针*/
extern U8	g_RunErr;		/* 运行错误 */

/*变量定义*/
/*------------------------------------------*/
extern U8	*g_FightMap;		/* 战斗地图缓存 */
extern U8	*g_FightPath;		/* 战斗行军计算 */
extern U8	*g_FgtAtkRng;		/* 攻击范围 */
extern U16	g_TileId;		/* 当前地图所用tile的ID */
extern U16	g_EneTmpProv;		/* 战斗模块，敌人粮草临时变量 */
extern U8	g_MapWid;		/* 战斗地图宽度 */
extern U8	g_MapHgt;		/* 战斗地图高度 */
extern U8	g_FgtOver;		/* 战斗是否结束 */
extern U8	g_FgtWeather;		/* 战场天气情况 */
extern U8	g_FgtBoutCnt;		/* 战斗回合计数 */
extern U8	g_MainGenIdx;		/* 主将序号 */
extern U8	g_LookEnemy;		/* 是否观看敌人移动 */
extern U8	g_LookMovie;		/* 是否观看战斗动画 */
extern U8	g_MoveSpeed;		/* 战斗中移动速度 */
extern U8	g_FoucsX,g_FoucsY;	/* 光标的地图坐标 */
extern U8	g_MapSX,g_MapSY;	/* 当前地图显示的起始坐标 */
extern U8	g_PathSX,g_PathSY;	/* 行军范围地图起始坐标 */
extern U8	g_PUseSX,g_PUseSY;	/* 行军范围使用起始坐标 */
extern U8	g_BakUpX,g_BakUpY;	/* 将领坐标备份 */
extern U8	g_CityX,g_CityY;	/* 城市坐标 */
extern FGTJK	g_FgtParam;		/* 战斗模块接口参数 */
extern JLPOS	g_GenPos[FGTA_MAX];	/* 将领地图位置及基本属性 */
extern JLATT	g_GenAtt[2];		/* 攻击状态下的两个将领属性 */

/*变量定义*/
/*------------------------------------------*/
extern U8 g_PlayerKing;			/*玩家君主*/
extern U16 g_YearDate;			/*当前日期*/
extern U8 g_MonthDate;			/*当前日期*/
extern U8 g_PIdx;			/*历史时期*/
extern PersonType g_Persons[PERSON_MAX];		/*存放人才属性指针*/
extern CityType g_Cities[CITY_MAX];	/*存放城市属性指针*/
extern U8 g_PersonsQueue[PERSON_MAX];	/*人才队列*/
extern U8 g_GoodsQueue[GOODS_MAX];	/*道具队列*/
extern OrderQueueType *g_OrderHead;	/*命令队列头指针*/
extern OrderQueueType *g_OrderEnd;	/*命令队列末指针*/
extern CitySetType g_CityPos;		/*当前城市地图显示位置结构*/
/*变量定义*/
/*------------------------------------------*/
extern U8 citymap[SHOWMAP_HS][SHOWMAP_WS];	/*当前显示城市地图*/
extern U8 cavpdb,cavps;				/*战争俘虏临时变量*/

extern int g_tlen;
extern int g_useMOD;

#endif	/* _VAR_MAL_ */
#endif	/* _EXTERN_H */
