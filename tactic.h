#ifndef TAC_TIC_HEADER
#define TAC_TIC_HEADER
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
 U8 FunctionMenu(void);
 U16 NumOperate(U16 min,U16 max);
 void ShowTacticNote(void);
#ifdef __cplusplus
}
#endif
#endif