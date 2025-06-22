#ifndef TAC_TIC_HEADER
#define TAC_TIC_HEADER
#include<vector>
#include "order.h"

// Global flag to control order logging - only log when person belongs to user king
extern bool g_LogOnlyUserKing;

// Functions to control the logging flag
void setLogOnlyUserKing(bool enabled);
bool getLogOnlyUserKing();

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
bool SaveGameJson(U8 idx);
bool LoadGameJson(U8 idx);
std::string gbk_to_utf8(const char* gbk_str, size_t gbk_len);
std::string getGoodGbkName(U8 toolId);
std::string getPersonGbkName(U8 personId);
std::string getCityDebugString(U8 cityId);
void writeAllCitiesDebugLog(const std::string& operation = "Debug");
void printCityDebugInfo(U8 cityId);
void printCityDebugInfoCout(U8 cityId);

// Person debug functions
std::string getPersonDebugString(U8 personId);
void printPersonDebugInfo(U8 personId);
void printPersonDebugInfoCout(U8 personId);
void printAllPersonsDebugInfo();
void printAllPersonsDebugInfoCout();
void writeAllPersonsDebugLog(const std::string& operation = "Debug");

// C wrapper functions for person debug functions
void writeAllPersonsDebugLogC(const char* operation);
void printAllPersonsDebugInfoC();
void printAllPersonsDebugInfoCoutC();
void printPersonDebugInfoC(U8 personId);
void printPersonDebugInfoCoutC(U8 personId);

// Activity logging functions
void logActivity(const std::string& activity, const std::string& details = "");
void logOrderCreated(const OrderType* order);
void logOrderExecuted(const OrderType* order);
void logPersonReturned(U8 personId, U8 cityId, const std::string& reason = "");
void logPersonRemoved(U8 personId, U8 cityId, const std::string& reason = "");
void logOrderDeleted(const OrderType* order);
void logGameTurn();
void logComputerTactic(U8 cityId, const std::string& tacticType);

// Helper function for order logging
std::string getOrderDetailedStr(const OrderType* order);

// C wrapper functions for activity logging
void logActivityC(const char* activity, const char* details);
void logOrderCreatedC(const OrderType* order);
void logOrderExecutedC(const OrderType* order);
void logPersonReturnedC(U8 personId, U8 cityId, const char* reason);
void logPersonRemovedC(U8 personId, U8 cityId, const char* reason);
void logOrderDeletedC(const OrderType* order);
void logGameTurnC();
void logComputerTacticC(U8 cityId, const char* tacticType);

// C wrapper functions to control logging flag
void setLogOnlyUserKingC(int enabled);
int getLogOnlyUserKingC();

// Test function for activity logging
void testActivityLogging();
void testOrderTypeComparison();

void clearPersonNameCache();

#endif