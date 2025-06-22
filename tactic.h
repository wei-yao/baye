#ifndef TAC_TIC_HEADER
#define TAC_TIC_HEADER
#include<vector>

void AITacticDiplomatism(U8 city);
 U8 PlayerTactic();
 void ComputerTactic(void);
 void ComputerTacticInterior(U8 city);
 void ComputerTacticDiplomatism(U8 city);
 void ComputerTacticHarmonize(U8 city,  bool isAuto);
 void ComputerTacticArmament(U8 city, bool isAuto);
 void ConditionUpdate(void);
 U8 GameDevDrv(void);
 U8 IsWin(void);
 U8 IsLoss(void);
 void SetCitySatrap(void);
 U8 GetPeriodKings(U8 period,U8 *kings);
 U8 GetAllKing(U8 *kings);
 void LoadPeriod(U8 period);
 void OldCityToNewCity(OldCityType old_cities[38], U8 o_PersonsQueue[200]);
 void NewCityToOldPersonsQueue(U8* personsQueue);
 U8 FunctionMenu(void);
 U16 NumOperate(U16 min,U16 max);
 void ShowTacticNote(void);
 U8 GetEnemyPersons(U8 king, U8* pqueue);
 U8 AddPerson(U8 city, U8 person);
 U8 DelPerson(U8 city, U8 person);
 U8 AddGoods(U8 city, U8 goods);
 U8 DelGoods(U8 city, U8 goods);
 U8 GetCityGoods(U8 city, U8* gqueue);
 U8 GetCityDiscoveredGoods(U8 city, U8* gqueue);
 U8 GetCityUndiscoveredGoods(U8 city, U8* gqueue);
 U8 GetPersonCity(U8 person);
 U8 GetCityPersons(U8 city, U8* pqueue);
U8 GetCityOutPersons(U8 city, U8* pqueue);
U8 GetCityOutPersons(U8 city, U8* pqueue);
U8 GetCityCaptives(U8 city, U8* pqueue);
U8 GetWeekCity(U8 count, U8* cqueue);

// C wrapper functions for C++ functions
U8 SaveCityJsonC(U8 idx);
U8 LoadCityJsonC(U8 idx);

// Function to initialize ToolsV from old system
void InitializeToolsVFromOldSystem(void);

// C wrapper for debug logging (visible to both C and C++)
void writeAllCitiesDebugLogC(const char* operation);

// C++ specific functions (cannot have C linkage)
std::vector<U8> getAllCityPersonsBelonging(U8 city);
int SmartAssign(U8 city, int k);
bool SaveCityJson(U8 idx);
bool LoadCityJson(U8 idx);
std::string gbk_to_utf8(const char* gbk_str, size_t gbk_len);
std::string getGoodGbkName(U8 toolId);
std::string getPersonGbkName(U8 personId);
std::string getCityDebugString(U8 cityId);
void writeAllCitiesDebugLog(const std::string& operation = "Debug");
void printCityDebugInfo(U8 cityId);
void printCityDebugInfoCout(U8 cityId);



#endif