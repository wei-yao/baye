#include "pconst.h"
/* 程序常量数据文件 */
/* 兵种攻击范围(按兵种排序)
U8 dFgtAtRange[]={
    0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,1,1,0,0,1,0,1,0,0,1,1,1,0,0,0,0,0,0,
    0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,
    0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,1,1,0,0,1,0,1,0,0,1,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0};

int dFgtAtRangeLen = 150;
/* 兵种策略(按兵种排序)
U8 dFgtJNArray[]={
    1,2,3,0,0,0,0,0,0,0,
    5,6,7,8,0,0,0,0,0,0,
    9,10,11,0,0,0,0,0,0,0,
    12,13,11,0,0,0,0,0,0,0,
    1,2,3,4,0,0,0,0,0,0,
	14,15,16,17,18,19,20,29,27,0};

int dFgtJNArrayLen = 60;

/* 各种地形对不同兵种的移动力影响
U8 dFgtLandR[]={
    0,129,129,130,130,131,
    131,131,131,131,131,131,131,131,131,131,
    131,131,131,131,131,131,131,131,131,131,
    131,131,131,131,131,131,131,131,131,131,
    131,131,131,131,131,
    130,131,131,131,131,
    0,129,129,130,130,129,
    129,129,129,129,129,129,129,129,129,129,
    131,131,131,131,131,131,131,131,131,131,
    131,131,131,131,131,131,131,131,131,130,
    131,131,131,131,131,
    130,131,131,131,131,
    0,129,129,130,130,129,
    129,129,129,129,129,129,129,129,129,129,
    131,131,131,131,131,131,131,131,131,131,
    131,131,131,131,131,131,131,131,131,130,
    131,131,131,131,131,
    130,131,131,131,131,
    0,130,130,130,130,131,
    131,131,131,131,131,131,131,131,131,131,
    129,129,129,129,129,129,129,129,129,129,
    129,129,129,129,129,129,129,129,129,129,
    129,129,129,129,129,
    130,129,129,129,129,
    0,129,129,130,130,131,
    130,130,130,130,130,130,130,130,130,130,
    131,131,131,131,131,131,131,131,131,131,
    131,131,131,131,131,131,131,131,131,131,
    131,131,131,131,131,
    130,131,131,131,131,
    0,129,129,130,130,129,
    129,129,129,129,129,129,129,129,129,129,
    130,130,130,130,130,130,130,130,130,130,
    130,130,130,130,130,130,130,130,130,130,
    130,130,130,130,130,
    130,131,131,131,131};

int dFgtLandRLen = 46*6;

/* 各种地形对不同兵种的战力量影响
   草地 平原 山地 森林 村庄 城池 营寨 河流
U8 dFgtLandF[]={
    0,0,2,1,0,0,0,3,
    0,0,0,0,0,0,0,2,
    0,0,0,0,0,0,0,2,
    1,1,3,2,0,0,0,0,
    0,0,1,1,0,0,0,1,
	0,0,0,0,0,0,0,1};

int dFgtLandFLen = 48;

/* 战斗初始化坐标 攻击方*8+防守方
U8 dFgtIntPos[]={
    2,2,2,3,1,2,3,2,2,1,0,4,4,4,1,1,3,1,2,0,
    2,2,2,3,1,2,3,2,2,1,1,3,0,1,3,4,3,1,4,0,
    2,2,2,3,1,2,3,2,2,1,0,0,0,4,3,1,3,3,4,2,
    2,2,2,3,1,2,3,2,2,1,1,1,0,3,3,0,3,3,4,4,
    2,2,2,3,1,2,3,2,2,1,0,0,4,0,1,3,3,3,2,4,
    2,2,2,3,1,2,3,2,2,1,3,1,1,0,4,3,1,3,0,4,
    2,2,2,3,1,2,3,2,2,1,4,0,4,4,1,1,1,3,0,2,
    2,2,2,3,1,2,3,2,2,1,3,3,1,4,4,1,1,1,0,0,
	2,2,2,3,1,2,3,2,2,1,1,1,3,3,1,3,3,1,2,0};

int dFgtIntPosLen = 180;

/* 城市数组
U8 dCityPos[] = {
    1,0,9,0,11,0,
    2,1,5,1,7,1,8,1,10,1,
    3,2,4,2,5,2,7,2,8,2,9,2,
    3,3,7,3,8,3,9,3,10,3,
    2,4,6,4,7,4,9,4,10,4,
    2,5,3,5,6,5,7,5,8,5,10,5,
    0,6,3,6,4,6,7,6,9,6,
	5,7,7,7,9,7};

int dCityPosLen = 76;

/*
U8 GOODS_PRO_WID[]={4,6,6,6,6};
int GOODS_PRO_WIDLen = 5;

U8 PERSON_PRO_WID[]={8,10,4,4,4,4,4,4,4,5,4,10,10};
int PERSON_PRO_WIDLen = 13;
/*
U8 DirectP[]={7,0,1,6,0,2,5,4,3};
int DirectPLen = 9;

/* 指令消耗体力		任 开 招 搜 治 出 招 处 流 赏 没 交 宴 输 移 离 招 策 反 劝 无 无 无 侦 征 分 掠 出
U8 ConsumeThew[]={ 0, 8, 8, 8, 8, 8,15, 0, 0, 0, 0,12, 0, 8, 0,20,20,20, 0,10, 0, 0, 0,10,12, 0,20, 0};
int ConsumeThewLen = 28;
/* 指令消耗金钱		 任  开  招  搜  治  出  招  处  流  赏  没  交  宴  输  移  离  招  策  反  劝  无  无  无  侦  征  分  掠  出
U8 ConsumeMoney[]={ 0, 50, 50,  0, 50, 50,100,  0,  0,  0,  0,  0,100,  0,  0, 50, 50, 50, 50, 50,  0,  0,  0, 20,  1,  0,  0,  0};
int ConsumeMoneyLen = 28;
U8 KingTacticOddsIH[]={50,40,30,20,10};
int KingTacticOddsIHLen = 5;
U8 KingTacticOddsD[]={80,70,70,40,20};
int KingTacticOddsDLen = 5;
U8 C_MAP[]={0,1,0,0,0,0,0,0,0,2,0,3,0,0,4,0,0,5,0,6,7,0,8,0,0,0,0,9,10,11,0,12,13,14,0,0,0,0,0,15,0,0,0,16,17,18,19,0,0,0,20,0,0,0,21,22,0,23,24,0,0,0,25,26,0,0,27,28,29,0,30,0,31,0,0,32,33,0,0,34,0,35,0,0,0,0,0,0,0,36,0,37,0,38,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int C_MAPLen = 108;
U8 dCityMapId[]={6,0,1,6,3,4,2,5,6,3,1,0,2,3,4,0,2,1,4,6,1,1,3,5,6,6,3,3,4,2,6,6,1,2,3,4,5,1};
int dCityMapIdLen = 38;*/
