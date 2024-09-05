#pragma pack(1)

#ifndef ATTRIBUT_H
#define ATTRIBUT_H

#define PERSON_MAX		200	/*最大武将数*/
#define CITY_MAX		38	/*最大城市数*/
#define GOODS_MAX		33	/*最大道具数*/
#define PERSON_DEATH_AGE	90	/*武将寿命*/
#define PERSON_APPEAR_AGE	16	/*武将出现年龄*/

/*------------------------
  君主性格定义
  ------------------------*/
#define CHARACTER_PEACE		4	/*和平*/
#define CHARACTER_JUSTICE	3	/*大义*/
#define CHARACTER_DUPLICITY	2	/*奸诈*/
#define CHARACTER_CRAZY		1	/*狂人*/
#define CHARACTER_RASH		0	/*冒进*/

/*------------------------
  君主性格劝降机率
  ------------------------*/
#define PERSUADE_PEACE		15	/*和平*/
#define PERSUADE_JUSTICE	5	/*大义*/
#define PERSUADE_DUPLICITY	20	/*奸诈*/
#define PERSUADE_CRAZY		1	/*狂人*/
#define PERSUADE_RASH		10	/*冒进*/

/*------------------------
  性格定义
  ------------------------*/
#define CHARACTER_LOYALISM	4	/*忠义*/
#define CHARACTER_IDEAL		3	/*大志*/
#define CHARACTER_AVARICE	2	/*贪财*/
#define CHARACTER_DREAD		1	/*怕死*/
#define CHARACTER_TEMERITY	0	/*卤莽*/

/*------------------------
  性格离间机率
  ------------------------*/
#define ALIENATE_LOYALISM	5	/*忠义*/
#define ALIENATE_IDEAL		30	/*大志*/
#define ALIENATE_AVARICE	40	/*贪财*/
#define ALIENATE_DREAD		30	/*怕死*/
#define ALIENATE_TEMERITY	50	/*卤莽*/

/*------------------------
  性格招揽机率
  ------------------------*/
#define CANVASS_LOYALISM	5	/*忠义*/
#define CANVASS_IDEAL		20	/*大志*/
#define CANVASS_AVARICE		30	/*贪财*/
#define CANVASS_DREAD		40	/*怕死*/
#define CANVASS_TEMERITY	15	/*卤莽*/

/*------------------------
  性格策反机率
  ------------------------*/
#define COUNTERESPIONGE_LOYALISM	5	/*忠义*/
#define COUNTERESPIONGE_IDEAL		60	/*大志*/
#define COUNTERESPIONGE_AVARICE		20	/*贪财*/
#define COUNTERESPIONGE_DREAD		10	/*怕死*/
#define COUNTERESPIONGE_TEMERITY	30	/*卤莽*/

/*------------------------
  性格招降机率
  ------------------------*/
#define SURRENDER_LOYALISM	5	/*忠义*/
#define SURRENDER_IDEAL		20	/*大志*/
#define SURRENDER_AVARICE	30	/*贪财*/
#define SURRENDER_DREAD		60	/*怕死*/
#define SURRENDER_TEMERITY	15	/*卤莽*/

/*------------------------
  性格招降机率系数
  ------------------------*/
#define SURRENDER_LOYALISM_MOD	1	/*忠义*/
#define SURRENDER_IDEAL_MOD	3	/*大志*/
#define SURRENDER_AVARICE_MOD	4	/*贪财*/
#define SURRENDER_DREAD_MOD	5	/*怕死*/
#define SURRENDER_TEMERITY_MOD	2	/*卤莽*/

/*------------------------
  方向定义
  ------------------------*/
#define	DIRECT_N	0		/* 方向——北 */
#define	DIRECT_EN	1		/* 方向——东北 */
#define	DIRECT_E	2		/* 方向——东 */
#define	DIRECT_ES	3		/* 方向——东南 */
#define	DIRECT_S	4		/* 方向——南 */
#define	DIRECT_WS	5		/* 方向——西南 */
#define	DIRECT_W	6		/* 方向——西 */
#define	DIRECT_WN	7		/* 方向——西北 */


/*------------------------
  城市状态定义
  ------------------------*/
#define	STATE_NORMAL		0	/*正常*/
#define	STATE_FAMINE		1	/*饥荒*/
#define	STATE_DROUGHT		2	/*旱灾*/
#define	STATE_FLOOD		3	/*水灾*/
#define	STATE_REBELLION		4	/*暴动*/


#define THREW_RENEW		4	/*每月恢复体力*/
#define THREW_TREAT		50	/*宴请恢复体力*/

typedef struct {
	U8	idx;			/* 道具序号 */
	U8	useflag;		/* 使用标志：是被使用还是被装备*/
	U8	name[10];		/* 道具名字 */
	U8	inf[50];		/* 道具说明 */
	U8	at;			/* 对武力的加层 */
	U8	iq;			/* 对智力的加层 */
	U8	move;			/* 对移动力的加层 */
	U8	arm;			/* 对兵种的改变 */
}GOODS;

typedef struct PersonRes			/*人才属性*/
{
	U8 Id;				/*编号*/
	U8 Belong;			/*归属*/
	U8 Level;			/*等级*/
	U8 Force;			/*武力*/
	U8 IQ;				/*智力*/
	U8 Devotion;			/*忠诚*/
	U8 Character;			/*性格*/
	U8 Experience;			/*经验*/
	U8 Thew;			/*体力*/
	U8 ArmsType;			/*兵种*/
	U16 Arms;			/*兵力*/
	U8 Equip[2];			/*装备*/
	U8 Age;				/*年龄*/
	U8 name[10];		/*名称*/
	U8 Birth;			/*出生年*/
	U8 SearchId;		/*搜索者编号*/
	U8 AppearCityId;	/*出现城市编号*/
}PersonResType;

typedef struct Person				/*人才属性 (12 Bytes) */
{
	U8 Id;				/*编号*/
	U8 Belong;			/*归属*/
	U8 Level;			/*等级*/
	U8 Force;			/*武力*/
	U8 IQ;				/*智力*/
	U8 Devotion;			/*忠诚*/
	U8 Character;			/*性格*/
	U8 Experience;			/*经验*/
	U8 Thew;			/*体力*/
	U8 ArmsType;			/*兵种*/
	U16 Arms;			/*兵力*/
	U8 Equip[2];			/*装备*/
	U8 Age;				/*年龄*/
}PersonType;

typedef struct CityRes				/*城市属性*/
{
	U8 Id;				/*编号*/
	U8 Belong;			/*归属*/
	U8 SatrapId;			/*太守编号*/
	U16 FarmingLimit;		/*农业上限*/
	U16 Farming;			/*农业开发度*/
	U16 CommerceLimit;		/*商业上限*/
	U16 Commerce;			/*商业开发度*/
	U8 PeopleDevotion;		/*民忠*/
	U8 AvoidCalamity;		/*防灾*/
	U32 PopulationLimit;		/*人口上限*/
	U32 Population;			/*人口*/
	U16 Money;			/*金钱*/
	U16 Food;			/*粮食*/
	U16 MothballArms;		/*后备兵力*/
	U8 PersonQueue;			/*人才队列*/
	U8 Persons;			/*人才数*/
	U8 ToolQueue;			/*道具队列*/
	U8 Tools;			/*道具数*/
	U8 name[10];		/*名称*/
	U8 Links[8];			/*城市连接（北、东北、东、东南、南、西南、西、西北）*/
	U8 Distances[8];	/*城市间距离*/
}CityResType;

typedef struct OldCity				/*城市属性(30 Bytes)*/
{
    U8 Id;
    U8 Belong;			/*归属*/
    U8 SatrapId;			/*太守编号*/
    U16 FarmingLimit;		/*农业上限*/
    U16 Farming;			/*农业开发度*/
    U16 CommerceLimit;		/*商业上限*/
    U16 Commerce;			/*商业开发度*/
    U8 PeopleDevotion;		/*民忠*/
    U8 AvoidCalamity;		/*防灾*/
    U32 PopulationLimit;		/*人口上限*/
    U32 Population;			/*人口*/
    U16 Money;			/*金钱*/
    U16 Food;			/*粮食*/
    U16 MothballArms;		/*后备兵力*/
    U8 PersonQueue;			/*人才队列*/
    U8 Persons;			/*人才数*/
    U8 ToolQueue;			/*道具队列*/
    U8 Tools;			/*道具数*/

} OldCityType;

typedef struct City				/*城市属性(30 Bytes)*/
{
    U8 Id;
    U8 Belong;			/*归属*/
    U8 SatrapId;			/*太守编号*/
    U16 FarmingLimit;		/*农业上限*/
    U16 Farming;			/*农业开发度*/
    U16 CommerceLimit;		/*商业上限*/
    U16 Commerce;			/*商业开发度*/
    U8 PeopleDevotion;		/*民忠*/
    U8 AvoidCalamity;		/*防灾*/
    U32 PopulationLimit;		/*人口上限*/
    U32 Population;			/*人口*/
    U16 Money;			/*金钱*/
    U16 Food;			/*粮食*/
    U16 MothballArms;		/*后备兵力*/
    U8 PersonQueue;			/*人才队列*/
    U8 Persons;			/*人才数*/
    U8 ToolQueue;			/*道具队列*/
    U8 Tools;			/*道具数*/
	bool autoManage;           // if auto manage

}CityType;

#endif

