
#ifndef BkoiUnitH
#define BkoiUnitH
//================================================================================

#include "MainUnit.h"
//================================================================================

#ifdef SPO_BKOI
//================================================================================

typedef WORD TBkoi_I01_Array[17];
typedef WORD TBkoi_I12_Array[2];

typedef WORD TBkoi_I03_Array[26];
typedef WORD TBkoi_I04_Array[28];
typedef WORD TBkoi_I05_Array[21];
typedef WORD TBkoi_I06_Array[8][32];

typedef WORD TBkoi_I17_Array[56];
typedef WORD TBkoi_I18_Array[31];

typedef WORD TBkoi_I13_Array[5];
typedef WORD TBkoi_I14_Array[5];
//================================================================================

extern unsigned Bkoi_I01_Imit_Enabled;
extern unsigned Bkoi_I01_Index;
extern unsigned Bkoi_I01_Count;
extern unsigned Bkoi_I01_Interval;
extern sysTime_t Bkoi_I01_Time;
extern const TBkoi_I01_Array Bkoi_I01_Buffer[205];
//================================================================================

extern unsigned Bkoi_I12_Imit_Enabled;
extern unsigned Bkoi_I12_Index;
extern unsigned Bkoi_I12_Count;
extern unsigned Bkoi_I12_Interval;
extern sysTime_t Bkoi_I12_Time;
extern const TBkoi_I12_Array Bkoi_I12_Buffer[205];
//================================================================================

extern unsigned Bkoi_I03_Imit_Enabled;
extern unsigned Bkoi_I03_Index;
extern unsigned Bkoi_I03_Count;
extern unsigned Bkoi_I03_Interval;
extern sysTime_t Bkoi_I03_Time;
extern const TBkoi_I03_Array Bkoi_I03_Buffer[205];
//================================================================================

extern unsigned Bkoi_I04_Imit_Enabled;
extern unsigned Bkoi_I04_Index;
extern unsigned Bkoi_I04_Count;
extern unsigned Bkoi_I04_Interval;
extern sysTime_t Bkoi_I04_Time;
extern const TBkoi_I04_Array Bkoi_I04_Buffer[205];
//================================================================================

extern unsigned Bkoi_I05_Imit_Enabled;
extern unsigned Bkoi_I05_Index;
extern unsigned Bkoi_I05_Count;
extern unsigned Bkoi_I05_Interval;
extern sysTime_t Bkoi_I05_Time;
extern const TBkoi_I05_Array Bkoi_I05_Buffer[205];
//================================================================================

extern unsigned Bkoi_I06_Imit_Enabled;
extern unsigned Bkoi_I06_Index;
extern unsigned Bkoi_I06_Count;
extern unsigned Bkoi_I06_Interval;
extern sysTime_t Bkoi_I06_Time;
extern const TBkoi_I06_Array Bkoi_I06_Buffer[820];
//================================================================================

extern unsigned Bkoi_I17_Imit_Enabled;
extern unsigned Bkoi_I17_Index, Bkoi_I17_Overflow;
extern unsigned Bkoi_I17_Count;
extern unsigned Bkoi_I17_Interval;
extern sysTime_t Bkoi_I17_Time;
extern TBkoi_I17_Array Bkoi_I17_Buffer[450];
//================================================================================

extern unsigned Bkoi_I18_Imit_Enabled;
extern unsigned Bkoi_I18_Index, Bkoi_I18_Overflow;
extern unsigned Bkoi_I18_Count;
extern unsigned Bkoi_I18_Interval;
extern sysTime_t Bkoi_I18_Time;
extern TBkoi_I18_Array Bkoi_I18_Buffer[450];
//================================================================================

void BkoiInit (void);
void KpaCommand66 (int BaseMkoLine, int BaseMkoChan);

void Bkoi_I01_DoImit (sysTime_t *Time);
void Bkoi_I12_DoImit (sysTime_t *Time);

void Bkoi_I03_DoImit (sysTime_t *Time);
void Bkoi_I04_DoImit (sysTime_t *Time);
void Bkoi_I05_DoImit (sysTime_t *Time);
void Bkoi_I06_DoImit (sysTime_t *Time);

void Bkoi_I17_DoImit (sysTime_t *Time);
void Bkoi_I18_DoImit (sysTime_t *Time);
//================================================================================

#endif
//================================================================================

#endif

