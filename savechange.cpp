#include "gameview.h"
#include "global.h"
#include <iostream>
#include <fstream>
#include "attribute.h"
#include "paccount.h"
//#include "sources.h"
//#include "order.h"
//#include "sharefun.h"

using namespace std;

#define HEROMAX_OLD 300
#define CITYMAX_OLD 50
#define GOODMAX_OLD 50
#define TASKLISTMAX_OLD 255

#define HEROMAX_OLD_A 200
#define CITYMAX_OLD_A 50
#define GOODMAX_OLD_A 50
#define TASKLISTMAX_OLD_A 255

#define HEROMAX 200
#define CITYMAX 38
#define GOODMAX 33

extern U8 g_PlayerKing;			/*玩家君主*/
extern U16 g_YearDate;			/*当前日期*/
extern U8 g_MonthDate;			/*当前日期*/
extern U8 g_PIdx;			/*历史时期*/
extern PersonType *g_Persons;		/*存放人才属性指针*/
extern CityType g_Cities[CITYMAX];	/*存放城市属性指针*/
// extern U8 g_PersonsQueue[HEROMAX];	/*人才队列*/
extern U8 g_GoodsQueue[GOODMAX];	/*道具队列*/
extern CitySetType g_CityPos;		/*当前城市地图显示位置结构*/

//extern U8 AddGoods(U8 city,U8 goods);
//extern U8 AddPerson(U8 city,U8 persion);
U8 AddPerson(U8 city,U8 person)
{
   U8 i;
   U8 qnum;

   if (city >= CITY_MAX)
       return(0);

   if (person >= PERSON_MAX)
       return(0);

   g_Cities[city].PersonV.push_back(person);
//    qnum  = g_Cities[city].PersonQueue + g_Cities[city].Persons;

//    for (i = PERSON_MAX - 1;i > qnum;i --)
//    {
//        g_PersonsQueue[i] = g_PersonsQueue[i - 1];
//    }

//    g_PersonsQueue[qnum] = person;
//    g_Cities[city].Persons += 1;

//    for (i = city + 1;i < CITY_MAX;i ++)
//    {
//        g_Cities[i].PersonQueue += 1;
//    }

   return(1);
}

U8 AddGoods(U8 city,U8 goods)
{
   U8 i;
   U8 qnum;

   if (city >= CITY_MAX)
       return(0);

   if (goods >= GOODS_MAX)
       return(0);

   qnum  = g_Cities[city].ToolQueue + g_Cities[city].Tools;

   for (i = GOODS_MAX - 1;i > qnum;i --)
   {
       g_GoodsQueue[i] = g_GoodsQueue[i - 1];
   }

   g_GoodsQueue[qnum] = goods;
   g_Cities[city].Tools += 1;

   for (i = city + 1;i < CITY_MAX;i ++)
   {
       g_Cities[i].ToolQueue += 1;
   }

   return(1);
}

void SetGoods(U8 goods)
{
   U8 qc;
   U8 i;

   qc = g_Cities[CITY_MAX - 1].ToolQueue + g_Cities[CITY_MAX - 1].Tools;
   for (i = 0;i < qc;i ++)
   {
       if ((g_GoodsQueue[i] & 0x7f) == goods)
       {
           g_GoodsQueue[i] |= 0x80;
           break;
       }
   }
}

//需存储的信息
int herosave[HEROMAX_OLD][19];
int citysave[CITYMAX_OLD][20];
int goodsave[GOODMAX_OLD][11];
int tasklistsave[TASKLISTMAX_OLD][10];
int herolistsave[12];
int citylistsave[4];
int goodlistsave[9];
int gamemapsave[9];
int month;
int year1;
int owner1;
int tasknum;
int choice;

char ownername[20];
char ownername1[20];
char ownername2[20];


void OldSaveFileRead(char *name)
{

    ifstream infile(name,ios::in);

    infile>>owner1;
    g_PlayerKing = owner1-1;

    infile>>year1;
    g_YearDate = year1;

    infile>>choice;
    g_PIdx = choice+1;
    int i;
    int j;

    for(i=0;i<HEROMAX_OLD;i++)//HEROMAX=200
    {
        for(j=0;j<18;j++)
        {
            infile>>herosave[i][j];
        }

        if(i>=HEROMAX)continue;

        g_Persons[i].Id = herosave[i][18];//0];
        g_Persons[i].Belong = herosave[i][1];//>HEROMAX?0:herosave[i][1];
        g_Persons[i].Level = herosave[i][2];
        g_Persons[i].Force = herosave[i][3];
        g_Persons[i].IQ = herosave[i][4];
        g_Persons[i].Devotion = herosave[i][5];
        g_Persons[i].Character = herosave[i][6];
        g_Persons[i].Experience = herosave[i][7];
        g_Persons[i].Thew = herosave[i][8];
        g_Persons[i].ArmsType = herosave[i][9];
        g_Persons[i].Arms = herosave[i][10];
        g_Persons[i].Equip[0] = herosave[i][11];
        g_Persons[i].Equip[1] = herosave[i][12];
        g_Persons[i].Age = herosave[i][13];


    }
    /*save.herosave[i][0]=herolist.hero[i].no;
        save.herosave[i][1]=herolist.hero[i].owner;
        save.herosave[i][2]=herolist.hero[i].degree;
        save.herosave[i][3]=herolist.hero[i].wuli;
        save.herosave[i][4]=herolist.hero[i].zili;
        save.herosave[i][5]=herolist.hero[i].heart;
        save.herosave[i][6]=herolist.hero[i].charcter;
        save.herosave[i][7]=herolist.hero[i].experience;
        save.herosave[i][8]=herolist.hero[i].tili;
        save.herosave[i][9]=herolist.hero[i].style;
        save.herosave[i][10]=herolist.hero[i].bingli;
        save.herosave[i][11]=herolist.hero[i].goods1;
        save.herosave[i][12]=herolist.hero[i].goods2;
        save.herosave[i][13]=herolist.hero[i].age;
        save.herosave[i][14]=herolist.hero[i].hiscity;
        save.herosave[i][15]=herolist.hero[i].inlistflag;
        save.herosave[i][16]=herolist.hero[i].speedup;
        save.herosave[i][17]=herolist.hero[i].hisfinder;*/



    for(i=0;i<CITYMAX_OLD;i++)//citymax 38
    {
        for(j=0;j<19;j++)
        {
            infile>>citysave[i][j];
        }

        if(i>=CITYMAX)continue;
        g_Cities[i].Id = citysave[i][18];//0];//20180512
        g_Cities[i].Belong = citysave[i][1];
        g_Cities[i].SatrapId = citysave[i][2];
        g_Cities[i].FarmingLimit = citysave[i][3];
        g_Cities[i].Farming = citysave[i][4];
        g_Cities[i].CommerceLimit = citysave[i][5];
        g_Cities[i].Commerce = citysave[i][6];
        g_Cities[i].PeopleDevotion = citysave[i][7];
        g_Cities[i].AvoidCalamity = citysave[i][8];
        g_Cities[i].PopulationLimit = citysave[i][9];
        g_Cities[i].Population = citysave[i][10];
        g_Cities[i].Money = citysave[i][11];
        g_Cities[i].Food = citysave[i][12];
        g_Cities[i].MothballArms = citysave[i][13];
        g_Cities[i].PersonQueue = 0;//citysave[i][14];
        g_Cities[i].Persons = 0;//citysave[i][15];
        g_Cities[i].ToolQueue = 0;//citysave[i][16];
        g_Cities[i].Tools = 0;//citysave[i][17];
    }

    /*save.citysave[i][0]=citylist.city[i].no;
        save.citysave[i][1]=citylist.city[i].owner;
        save.citysave[i][2]=citylist.city[i].ownto;
        save.citysave[i][3]=citylist.city[i].agriculturemax;
        save.citysave[i][4]=citylist.city[i].agriculture;
        save.citysave[i][5]=citylist.city[i].businessmax;
        save.citysave[i][6]=citylist.city[i].business;
        save.citysave[i][7]=citylist.city[i].heart;
        save.citysave[i][8]=citylist.city[i].disease;
        save.citysave[i][9]=citylist.city[i].peoplemax;
        save.citysave[i][10]=citylist.city[i].people;
        save.citysave[i][11]=citylist.city[i].money;
        save.citysave[i][12]=citylist.city[i].foods;
        save.citysave[i][13]=citylist.city[i].soldierspace;
        save.citysave[i][14]=citylist.city[i].employers;
        save.citysave[i][15]=citylist.city[i].employnum;
        save.citysave[i][16]=citylist.city[i].goods;
        save.citysave[i][17]=citylist.city[i].goodnum;
        save.citysave[i][18]=citylist.city[i].statue;*/

    for(i=0;i<GOODMAX_OLD;i++)//goodmax 33
    {
        for(j=0;j<10;j++)
        {
            infile>>goodsave[i][j];
        }

        //g_GoodsQueue[i].Id = citysave[i][0];

    }

    //初始化g_GoodsQueue
    for(i=0;i<GOODMAX;i++)
    {
        AddGoods(goodsave[i][7],i);
        if(goodsave[i][9] ==1)//已经发现
        {
            SetGoods(i);
        }
    }


    //初始化g_PersonsQueue
    for(i=0;i<HEROMAX;i++)
    {
        AddPerson(herosave[i][14]-1,i);//20180512修正 -1
    }

    /*save.goodsave[i][0]=goodlist.good[i].no;
        save.goodsave[i][1]=goodlist.good[i].owner;
        save.goodsave[i][2]=goodlist.good[i].style;
        save.goodsave[i][3]=goodlist.good[i].wuliup;
        save.goodsave[i][4]=goodlist.good[i].ziliup;
        save.goodsave[i][5]=goodlist.good[i].speedup;
        save.goodsave[i][6]=goodlist.good[i].bingzhong;
        save.goodsave[i][7]=goodlist.good[i].hiscity;
        save.goodsave[i][8]=goodlist.good[i].hisfinder;
        save.goodsave[i][9]=goodlist.good[i].finded;*/

    //存档转换不考虑执行的操作
    for(i=0;i<TASKLISTMAX_OLD;i++)
    {
        for(j=0;j<10;j++)
        {
            infile>>tasklistsave[i][j];
        }
    }

    for(i=0;i<12;i++)
    {
        infile>>herolistsave[i];
    }

    for(i=0;i<4;i++)
    {
        infile>>citylistsave[i];
    }

    for(i=0;i<9;i++)
    {
        infile>>goodlistsave[i];
    }

    for(i=0;i<9;i++)
    {
        infile>>gamemapsave[i];
    }

    //g_CityPos.setx = citylistsave[0];//.gamemapsave[0];
    //g_CityPos.sety = citylistsave[1];//.gamemapsave[1];

    infile>>month;
    g_MonthDate = month;


    infile.close();
    //infile>>version_save;


    //infile>>tasknum;





    return;

}


void OldSaveFileRead_A(char *name)
{

    ifstream infile(name,ios::in);

    infile>>owner1;
    g_PlayerKing = owner1-1;

    infile>>year1;
    g_YearDate = year1;

    infile>>choice;
    g_PIdx = choice+1;
    int i;
    int j;

    for(i=0;i<HEROMAX_OLD_A;i++)//HEROMAX=200
    {
        for(j=0;j<18;j++)
        {
            infile>>herosave[i][j];
        }

        if(i>=HEROMAX)continue;

        g_Persons[i].Id = herosave[i][0];
        g_Persons[i].Belong = herosave[i][1];//>HEROMAX?0:herosave[i][1];
        g_Persons[i].Level = herosave[i][2];
        g_Persons[i].Force = herosave[i][3];
        g_Persons[i].IQ = herosave[i][4];
        g_Persons[i].Devotion = herosave[i][5];
        g_Persons[i].Character = herosave[i][6];
        g_Persons[i].Experience = herosave[i][7];
        g_Persons[i].Thew = herosave[i][8];
        g_Persons[i].ArmsType = herosave[i][9];
        g_Persons[i].Arms = herosave[i][10];
        g_Persons[i].Equip[0] = herosave[i][11];
        g_Persons[i].Equip[1] = herosave[i][12];
        g_Persons[i].Age = herosave[i][13];


    }
    /*save.herosave[i][0]=herolist.hero[i].no;
        save.herosave[i][1]=herolist.hero[i].owner;
        save.herosave[i][2]=herolist.hero[i].degree;
        save.herosave[i][3]=herolist.hero[i].wuli;
        save.herosave[i][4]=herolist.hero[i].zili;
        save.herosave[i][5]=herolist.hero[i].heart;
        save.herosave[i][6]=herolist.hero[i].charcter;
        save.herosave[i][7]=herolist.hero[i].experience;
        save.herosave[i][8]=herolist.hero[i].tili;
        save.herosave[i][9]=herolist.hero[i].style;
        save.herosave[i][10]=herolist.hero[i].bingli;
        save.herosave[i][11]=herolist.hero[i].goods1;
        save.herosave[i][12]=herolist.hero[i].goods2;
        save.herosave[i][13]=herolist.hero[i].age;
        save.herosave[i][14]=herolist.hero[i].hiscity;
        save.herosave[i][15]=herolist.hero[i].inlistflag;
        save.herosave[i][16]=herolist.hero[i].speedup;
        save.herosave[i][17]=herolist.hero[i].hisfinder;*/



    for(i=0;i<CITYMAX_OLD_A;i++)//citymax 38
    {
        for(j=0;j<19;j++)
        {
            infile>>citysave[i][j];
        }

        if(i>=CITYMAX)continue;
        g_Cities[i].Id = citysave[i][18];//0];//20180512
        g_Cities[i].Belong = citysave[i][1];
        g_Cities[i].SatrapId = citysave[i][2];
        g_Cities[i].FarmingLimit = citysave[i][3];
        g_Cities[i].Farming = citysave[i][4];
        g_Cities[i].CommerceLimit = citysave[i][5];
        g_Cities[i].Commerce = citysave[i][6];
        g_Cities[i].PeopleDevotion = citysave[i][7];
        g_Cities[i].AvoidCalamity = citysave[i][8];
        g_Cities[i].PopulationLimit = citysave[i][9];
        g_Cities[i].Population = citysave[i][10];
        g_Cities[i].Money = citysave[i][11];
        g_Cities[i].Food = citysave[i][12];
        g_Cities[i].MothballArms = citysave[i][13];
        g_Cities[i].PersonQueue = 0;//citysave[i][14];
        g_Cities[i].Persons = 0;//citysave[i][15];
        g_Cities[i].ToolQueue = 0;//citysave[i][16];
        g_Cities[i].Tools = 0;//citysave[i][17];
    }

    /*save.citysave[i][0]=citylist.city[i].no;
        save.citysave[i][1]=citylist.city[i].owner;
        save.citysave[i][2]=citylist.city[i].ownto;
        save.citysave[i][3]=citylist.city[i].agriculturemax;
        save.citysave[i][4]=citylist.city[i].agriculture;
        save.citysave[i][5]=citylist.city[i].businessmax;
        save.citysave[i][6]=citylist.city[i].business;
        save.citysave[i][7]=citylist.city[i].heart;
        save.citysave[i][8]=citylist.city[i].disease;
        save.citysave[i][9]=citylist.city[i].peoplemax;
        save.citysave[i][10]=citylist.city[i].people;
        save.citysave[i][11]=citylist.city[i].money;
        save.citysave[i][12]=citylist.city[i].foods;
        save.citysave[i][13]=citylist.city[i].soldierspace;
        save.citysave[i][14]=citylist.city[i].employers;
        save.citysave[i][15]=citylist.city[i].employnum;
        save.citysave[i][16]=citylist.city[i].goods;
        save.citysave[i][17]=citylist.city[i].goodnum;
        save.citysave[i][18]=citylist.city[i].statue;*/

    for(i=0;i<GOODMAX_OLD_A;i++)//goodmax 33
    {
        for(j=0;j<10;j++)
        {
            infile>>goodsave[i][j];
        }

        //g_GoodsQueue[i].Id = citysave[i][0];

    }

    //初始化g_GoodsQueue
    for(i=0;i<GOODMAX;i++)
    {
        AddGoods(goodsave[i][7],i);
        if(goodsave[i][9] ==1)//已经发现
        {
            SetGoods(i);
        }
    }


    //初始化g_PersonsQueue
    for(i=0;i<HEROMAX;i++)
    {
        AddPerson(herosave[i][14]-1,i);//20180512修正 -1
    }

    /*save.goodsave[i][0]=goodlist.good[i].no;
        save.goodsave[i][1]=goodlist.good[i].owner;
        save.goodsave[i][2]=goodlist.good[i].style;
        save.goodsave[i][3]=goodlist.good[i].wuliup;
        save.goodsave[i][4]=goodlist.good[i].ziliup;
        save.goodsave[i][5]=goodlist.good[i].speedup;
        save.goodsave[i][6]=goodlist.good[i].bingzhong;
        save.goodsave[i][7]=goodlist.good[i].hiscity;
        save.goodsave[i][8]=goodlist.good[i].hisfinder;
        save.goodsave[i][9]=goodlist.good[i].finded;*/

    //存档转换不考虑执行的操作
    for(i=0;i<TASKLISTMAX_OLD_A;i++)
    {
        for(j=0;j<10;j++)
        {
            infile>>tasklistsave[i][j];
        }
    }

    for(i=0;i<12;i++)
    {
        infile>>herolistsave[i];
    }

    for(i=0;i<4;i++)
    {
        infile>>citylistsave[i];
    }

    for(i=0;i<9;i++)
    {
        infile>>goodlistsave[i];
    }

    for(i=0;i<9;i++)
    {
        infile>>gamemapsave[i];
    }

    //g_CityPos.setx = citylistsave[0];//.gamemapsave[0];
    //g_CityPos.sety = citylistsave[1];//.gamemapsave[1];

    infile>>month;
    g_MonthDate = month;


    infile.close();
    //infile>>version_save;


    //infile>>tasknum;





    return;

}

