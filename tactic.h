#ifndef TAC_TIC_HEADER
#define TAC_TIC_HEADER
#include<vector>
#ifdef __cplusplus
extern "C" {
#endif

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
 std::vector<U8> getAllCityPersonsBelonging(U8 city);
 int SmartAssign(U8 city, int k);
 U8 GetEnemyPersons(U8 king, U8* pqueue);
 U8 AddPerson(U8 city, U8 person);
 U8 DelPerson(U8 city, U8 person);
 U8 GetPersonCity(U8 person);
 U8 GetCityPersons(U8 city, U8* pqueue);
U8 GetCityOutPersons(U8 city, U8* pqueue);
U8 GetCityOutPersons(U8 city, U8* pqueue);
U8 GetCityCaptives(U8 city, U8* pqueue);
U8 GetWeekCity(U8 count, U8* cqueue);

#ifdef __cplusplus
}
#endif
#endif