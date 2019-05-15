
/*! \file MkoXchgUnit.c
 * \author Гапонов Р.В.
 * \date 2012.10.28
 * \version 1.0.0
 * \brief Модуль содержит функции обмена по МКО
 */

#include "MainUnit.h"
//================================================================================

int CNT_DBG;
int RES_STD_DBG;
UINT XchgBTrack;

/*! \fn int MkoXchgAn (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
 * \brief Функция передает одно сообщение по МКО, используется пакет A. Используется в функциях основного потока СПО
 * \param [in] MkoLine - номер линии МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] MkoChan - номер канала МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] MkoCmd - командное слово МКО для обмена
 * \param [in] F4Mode - признак передачи команды в режиме Ф4 (0 = нет, 1 = да)
 * \return Функция возвращает результат обмена: <br>
 * 0 = успешно <br>
 * >0 = ошибка в функциях обмена (для детальной информации см. таблицы констант) <br>
 * <0 = нет связи/сбой передачи (для детальной информации см. таблицы констант) <br>
 * \details Обмен по МКО может производиться в обычном и расширенном режимах (если разрешено в конфигурации СПО),
 * при этом возможна передача расширенных данных об ошибках обмена
 */

int MkoXchgAn (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode)
{
int BaseMkoLine, BaseMkoChan;
int TotalResult, Pass, MkoMode, ErrorCode, ErrorBuffer;
STATUS Result;

// проверка сообщения на допустимость
if ((!F4Mode) && (IsProhibitedMkoMsg(MkoCmd, &MkoBufA[0][0]))) return -5;

// устанавливаем тип обмена
MkoMode = (MkoCmd & 0x0400) ? (mcTM_RTtoBC) : (mcTM_BCtoRT);
if (F4Mode) MkoMode = mcTM_ModeNoData;

TotalResult = 0; // общий результат выполнения
Pass = 0; // номер прохода

while (1)
  {
  // инициализация описателя пакета МКО
  MkoPackA.nLast = 0;		// номер последнего сообщения
  memset(&MkoPackResA, 0, sizeof(MkoPackResA));  // очистка флагов ошибок пакета

  // инициализация описателя сообщения #0 в пакете
  MkoMsgA[0].TM   = MkoMode;	// тип сообщения
  MkoMsgA[0].Chan = MkoChan;	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgA[0].CW   = MkoCmd;	// первое командное слово

  MkoMsgA[0].MissSW = 1;	// прервать передачу при ошибках
  MkoMsgA[0].FlagSW = 1;
  MkoMsgA[0].FmtError = 1;

  // ставим флаг прерывания по концу передачи сообщения
  MkoMsgA[0].Eom = (SpoConfig.EnableEomMkoFlag) ? (1) : (0);

  MkoMsgA[0].GapTime = 0;	// передача без пауз
  // конец инициализации сообщения #0

  // сброс флагов для сообщения
  memset(&MkoMsgResA[0], 0, sizeof(mkoMsgRes_t));

  if (SpoConfig.EnableCompMkoMsg)
    {
    WORD Data = 0xDB01;
    Output((MkoLine) ? (0) : (1), KPA_ADDR, KPA_SUBADDR_DEBUG, 1, &Data);
    }

  // проведение обмена по МКО
  Result = mkoPackIniStart(MkoLine, &MkoPackA, MkoMsgPtrA, 0);  // запуск обмена по МКО
  if (Result != OK) { TotalResult = 1; break; }

  if (SpoConfig.EnableCompMkoMsg)
    {
    WORD Data = 0xDB02;
    Output((MkoLine) ? (0) : (1), KPA_ADDR, KPA_SUBADDR_DEBUG, 1, &Data);
    }

  Result = mkoNumPackWait(MkoLine, &MkoPackA);  // ожидание завершения обмена по МКО
  if (Result != OK) { TotalResult = 2; break; }

  if (SpoConfig.EnableCompMkoMsg)
    {
    WORD Data = 0xDB03;
    Output((MkoLine) ? (0) : (1), KPA_ADDR, KPA_SUBADDR_DEBUG, 1, &Data);
    }

  // проверка результата обмена
  TotalResult = CheckMkoXchg(&MkoPackResA, &MkoMsgA[0], &MkoMsgResA[0], 1);
  if (TotalResult == -3)
    {
    // повтор при занятости абонента
    if (++Pass < 2) { SleepMSec(10); continue; }
    }

  break;
  }

// передача расширенных данных об ошибках обмена на МКО
if ((TotalResult) && (SpoConfig.EnableMkoErrorExtendedInfo))
  {
  ErrorCode = ErrCodGet();
  ErrorBuffer = ErrMKIGet((void*) &DebugMki, sizeof(DebugMki) / sizeof(DWORD));

  BaseMkoLine = SpoConfig.KpaMkoLine & 1;
  BaseMkoChan = SpoConfig.KpaMkoChan & 1;

  ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9001, KPA_SUBADDR_DEBUG, &TotalResult, sizeof(TotalResult));
  ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9002, KPA_SUBADDR_DEBUG, &MkoPackResA, sizeof(MkoPackResA));
  ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9003, KPA_SUBADDR_DEBUG, &MkoMsgResA[0], sizeof(MkoMsgResA[0]));

  if (ErrorCode) ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9004, KPA_SUBADDR_DEBUG, &ErrorCode, sizeof(ErrorCode));
  if (ErrorBuffer) ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9005, KPA_SUBADDR_DEBUG, &DebugMki, 64);
  }

// обмен завершен
return TotalResult;
}
//================================================================================

/*! \fn int MkoXchgBn (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
 * \brief Функция передает одно сообщение по МКО, используется пакет B. Используется в функциях, вызываемых из прерываний СПО
 * \param [in] MkoLine - номер линии МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] MkoChan - номер канала МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] MkoCmd - командное слово МКО для обмена
 * \param [in] F4Mode - признак передачи команды в режиме Ф4 (0 = нет, 1 = да)
 * \return Функция возвращает результат обмена: <br>
 * 0 = успешно <br>
 * >0 = ошибка в функциях обмена (для детальной информации см. таблицы констант) <br>
 * <0 = нет связи/сбой передачи (для детальной информации см. таблицы констант) <br>
 * \details Обмен по МКО может производиться в обычном и расширенном режимах (если разрешено в конфигурации СПО),
 * при этом обмен с некоторыми устройствами производится по особому алгоритму, и возможна передача расширенных данных об ошибках обмена
 */

// static int PrevFsvuCode = 0;

int MkoXchgBn (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode)
{
int Result, Addr, SubAddr, Dir, DataSize;
WORD Data, Buffer[32];
// int FsvuXchg, FsvuDataCode;
int BupXchg;

// проверка сообщения на допустимость
if ((!F4Mode) && (IsProhibitedMkoMsg(MkoCmd, &MkoBufB[0][0]))) return -5;

// проверяем, не требуется ли расширеный алгоритм обмена
Addr     = MKO_CMD_ADDR(MkoCmd);
SubAddr  = MKO_CMD_SUBADDR(MkoCmd);
Dir      = MKO_CMD_DIR(MkoCmd);
DataSize = MKO_CMD_SIZE(MkoCmd);

// FsvuXchg = FsvuDataCode = 0;
BupXchg = 0;

if ((Addr == 4) && (SubAddr == 28) && (Dir == MKO_TX))
  {
  if (DataSize == 1)
    {
    Data = MkoBufB[0][0];

    // контроль МБК-07
    #if (0)
    if ((Data == 0x8004) || (Data == 0x9004) || (Data == 0xA004))
      {
      FsvuXchg = 1;
      FsvuDataCode = Data;
      }
    #endif

    // контроль вкл/откл БУП-Е
    if (SpoConfig.CheckBupState)
      {
      // контроль БУП-Е-09
      if ((Data == 0x1020) || (Data == 0x1040) || (Data == 0x1080))
        {
        BupXchg = Data;
        if (Data == 0x1020) { if ((SpoConfig.Bup_09_State == 0) || (SpoConfig.Bup_09_State == 1)) { } else { return 0x83; } }
        if (Data == 0x1040) { if ((SpoConfig.Bup_09_State == 0) || (SpoConfig.Bup_09_State == 2)) { } else { return 0x83; } }
        }
      // контроль БУП-Е-19
      if ((Data == 0x5040) || (Data == 0x6040) || (Data == 0x3040))
        {
        BupXchg = Data;
        if (Data == 0x5040) { if ((SpoConfig.Bup_19_State == 0) || (SpoConfig.Bup_19_State == 1)) { } else { return 0x83; } }
        if (Data == 0x6040) { if ((SpoConfig.Bup_19_State == 0) || (SpoConfig.Bup_19_State == 2)) { } else { return 0x83; } }
        }
      // контроль БУП-Е-20
      if ((Data == 0x5080) || (Data == 0x6080) || (Data == 0x3080))
        {
        BupXchg = Data;
        if (Data == 0x5080) { if ((SpoConfig.Bup_20_State == 0) || (SpoConfig.Bup_20_State == 1)) { } else { return 0x83; } }
        if (Data == 0x6080) { if ((SpoConfig.Bup_20_State == 0) || (SpoConfig.Bup_20_State == 2)) { } else { return 0x83; } }
        }
      }
    }
  else
    {
    // контроль размера команды для ЛКА-05
    if (SpoConfig.CheckLkaCmdSize) return 0x83;
    }
  }

// если проверка команд вкл/выкл аппаратуры передатчика включена, смотрим параметры передачи
if (SpoConfig.EnableMBK07CmdCheck)
  {
  // если данная команда - для управления аппаратурой передатчика, проверяем данные
  if ((Addr == 4) && (SubAddr == 28) && (Dir == MKO_TX) && (DataSize == 1))
    {
    Data = MkoBufB[0][0];

    // проверка команд для АНТ
    if ((Data == 0x8008) || (Data == 0x9008) || (Data == 0xA008))
      {
      memmove(&Buffer, &MkoBufB[0][0], sizeof(Buffer)); // сохраняем данные команды
      Result = CheckFsmuPower(MkoLine, MkoChan);
      if (Result == 0)
        {
        // норма: ФСМУ отключено

        // повторное отключение ФСМУ
        MkoBufB[0][0] = 0xB001;
        Result = MkoXchgBn(MkoLine, MkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
        if (Result) return Result;
        SleepMSec(MBK07_CMD_CTRL_TIME); // пауза на отключение ФСМУ

        // повторное отключение ФСВУ
        MkoBufB[0][0] = 0xB004;
        Result = MkoXchgBn(MkoLine, MkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
        if (Result) return Result;
        SleepMSec(MBK07_CMD_CTRL_TIME); // пауза на отключение ФСВУ
        }
      else
        {
        // ошибка: ФСМУ включено или неисправно
        return 0x80;
        }
      memmove(&MkoBufB[0][0], &Buffer, sizeof(Buffer)); // восстанавливаем данные команды
      }
    // проверка команд для ФСВУ
    else if ((Data == 0x8004) || (Data == 0x9004) || (Data == 0xA004))
      {
      memmove(&Buffer, &MkoBufB[0][0], sizeof(Buffer)); // сохраняем данные команды
      Result = CheckFsmuPower(MkoLine, MkoChan);
      if (Result == 0)
        {
        // норма: ФСМУ отключено

        // отключение ФСМУ
        MkoBufB[0][0] = 0xB001;
        Result = MkoXchgBn(MkoLine, MkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
        if (Result) return Result;
        SleepMSec(MBK07_CMD_CTRL_TIME); // пауза на отключение ФСМУ

        // отключение ФСВУ
        MkoBufB[0][0] = 0xB004;
        Result = MkoXchgBn(MkoLine, MkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
        if (Result) return Result;
        SleepMSec(MBK07_CMD_CTRL_TIME); // пауза на отключение ФСВУ
        }
      else
        {
        // ошибка: ФСМУ включено или неисправно
        return 0x80;
        }
      memmove(&MkoBufB[0][0], &Buffer, sizeof(Buffer)); // восстанавливаем данные команды
      }
    }
  }

if ((SpoConfig.EnableMkoExtXchg) &&
    (((Addr == 2) || (Addr == 4)) && ((SubAddr == 28) || (SubAddr == 29))) &&
    (Dir == MKO_TX) &&
    (DataSize == 1))
  {
  // обмен с адресами 2/4 - расширенный алгоритм обмена
  Result = MkoXchgBnExt(MkoLine, MkoChan, MkoCmd, F4Mode);
  }
else
  {
  // обмен с прочими адресами - стандартный алгоритм обмена
  Result = MkoXchgBnStd(MkoLine, MkoChan, MkoCmd, F4Mode);
  }

// контроль МБК-07
// if ((!Result) && (FsvuXchg)) PrevFsvuCode = FsvuDataCode;

// контроль вкл/откл БУП-Е
if ((!Result) && (BupXchg))
  {
  Data = BupXchg;

  // контроль БУП-Е-09
  if (Data == 0x1020) SpoConfig.Bup_09_State = 1;
  if (Data == 0x1040) SpoConfig.Bup_09_State = 2;
  if (Data == 0x1080) SpoConfig.Bup_09_State = 0;

  // контроль БУП-Е-19
  if (Data == 0x5040) SpoConfig.Bup_19_State = 1;
  if (Data == 0x6040) SpoConfig.Bup_19_State = 2;
  if (Data == 0x3040) SpoConfig.Bup_19_State = 0;

  // контроль БУП-Е-20
  if (Data == 0x5080) SpoConfig.Bup_20_State = 1;
  if (Data == 0x6080) SpoConfig.Bup_20_State = 2;
  if (Data == 0x3080) SpoConfig.Bup_20_State = 0;
  }

return Result;
}
//================================================================================

/*! \fn int MkoXchgBnStd (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
 * \brief Служебная функция, производит обмен с устройствами на МКО в обычном режиме, для обмена используется пакет B.
 * \param [in] MkoLine - номер линии МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] MkoChan - номер канала МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] MkoCmd - командное слово МКО для обмена
 * \param [in] F4Mode - признак передачи команды в режиме Ф4 (0 = нет, 1 = да)
 * \return Функция возвращает результат обмена: <br>
 * 0 = успешно <br>
 * >0 = ошибка в функциях обмена (для детальной информации см. таблицы констант) <br>
 * <0 = нет связи/сбой передачи (для детальной информации см. таблицы констант) <br>
 * \details Возможна передача расширенных данных об ошибках обмена (если разрешено в конфигурации СПО).
 */

int MkoXchgBnStd (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode)
{
int BaseMkoLine, BaseMkoChan;
int TotalResult, Pass, MkoMode, ErrorCode, ErrorBuffer;
STATUS Result;

XchgBTrack = 0x00000000;

// устанавливаем тип обмена
MkoMode = (MkoCmd & 0x0400) ? (mcTM_RTtoBC) : (mcTM_BCtoRT);
if (F4Mode) MkoMode = mcTM_ModeNoData;

TotalResult = 0; // общий результат выполнения
Pass = 0; // номер прохода

CNT_DBG = 0x1111;
while (1)
  {
  CNT_DBG++;
  // инициализация описателя пакета МКО
  MkoPackB.nLast = 0;		// номер последнего сообщения
  memset(&MkoPackResB, 0, sizeof(MkoPackResB));  // очистка флагов ошибок пакета

  // инициализация описателя сообщения #0 в пакете
  MkoMsgB[0].TM   = MkoMode;	// тип сообщения
  MkoMsgB[0].Chan = MkoChan;	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgB[0].CW   = MkoCmd;	// первое командное слово

  MkoMsgB[0].MissSW = 1;	// прервать передачу при ошибках
  MkoMsgB[0].FlagSW = 1;
  MkoMsgB[0].FmtError = 1;

  // ставим флаг прерывания по концу передачи сообщения
  MkoMsgB[0].Eom = (SpoConfig.EnableEomMkoFlag) ? (1) : (0);

  MkoMsgB[0].GapTime = 0;	// передача без пауз
  // конец инициализации сообщения #0

  // сброс флагов для сообщения
  memset(&MkoMsgResB[0], 0, sizeof(mkoMsgRes_t));

  if (SpoConfig.EnableCompMkoMsg)
    {
    WORD Data = 0xDB04;
    Output((MkoLine) ? (0) : (1), KPA_ADDR, KPA_SUBADDR_DEBUG, 1, &Data);
    }

  // проведение обмена по МКО
  Result = mkoPackIniStart(MkoLine, &MkoPackB, MkoMsgPtrB, 0);  // запуск обмена по МКО
  if (Result != OK) { XchgBTrack |= 0x00000001; TotalResult = 1; RES_STD_DBG = 0x2222; break; }
  XchgBTrack |= 0x00000002;

  if (SpoConfig.EnableCompMkoMsg)
    {
    WORD Data = 0xDB05;
    Output((MkoLine) ? (0) : (1), KPA_ADDR, KPA_SUBADDR_DEBUG, 1, &Data);
    }

  Result = mkoNumPackWait(MkoLine, &MkoPackB);  // ожидание завершения обмена по МКО
  if (Result != OK) { XchgBTrack |= 0x00000030; TotalResult = 2; RES_STD_DBG = 0x4444; break; }
  XchgBTrack |= 0x00000040;

  if (SpoConfig.EnableCompMkoMsg)
    {
    WORD Data = 0xDB06;
    Output((MkoLine) ? (0) : (1), KPA_ADDR, KPA_SUBADDR_DEBUG, 1, &Data);
    }

  // проверка результата обмена
  TotalResult = CheckMkoXchg(&MkoPackResB, &MkoMsgB[0], &MkoMsgResB[0], 1);
  if (TotalResult == -3)
    {
    // повтор при занятости абонента
    if (++Pass < 2) { SleepMSec(10); continue; }
    }

  break;
  }

// передача расширенных данных об ошибках обмена на МКО
if ((TotalResult) && (SpoConfig.EnableMkoErrorExtendedInfo))
  {
  ErrorCode = ErrCodGet();
  ErrorBuffer = ErrMKIGet((void*) &DebugMki, sizeof(DebugMki) / sizeof(DWORD));

  BaseMkoLine = SpoConfig.KpaMkoLine & 1;
  BaseMkoChan = SpoConfig.KpaMkoChan & 1;

  ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9001, KPA_SUBADDR_DEBUG, &TotalResult, sizeof(TotalResult));
  ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9002, KPA_SUBADDR_DEBUG, &MkoPackResB, sizeof(MkoPackResB));
  ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9003, KPA_SUBADDR_DEBUG, &MkoMsgResB[0], sizeof(MkoMsgResB[0]));

  if (ErrorCode) ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9004, KPA_SUBADDR_DEBUG, &ErrorCode, sizeof(ErrorCode));
  if (ErrorBuffer) ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9005, KPA_SUBADDR_DEBUG, &DebugMki, 64);
  }

XchgBTrack |= 0x80000000;
CNT_DBG = 0xFEDC;


// обмен завершен
return TotalResult;
}
//================================================================================

/*! \fn int MkoXchgBnMini (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
 * \brief Служебная функция, производит обмен с устройствами на МКО в обычном режиме, для обмена используется пакет B.
 *        Проверка результата производится только для пакета, повторные обмены при сбоях не производятся
 * \param [in] MkoLine - номер линии МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] MkoChan - номер канала МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] MkoCmd - командное слово МКО для обмена
 * \param [in] F4Mode - признак передачи команды в режиме Ф4 (0 = нет, 1 = да)
 * \return Функция возвращает результат обмена: <br>
 * 0 = успешно <br>
 * >0 = ошибка в функциях обмена (для детальной информации см. таблицы констант) <br>
 * <0 = ошибки в процессе обмена (для детальной информации см. таблицы констант) <br>
 */

/*
MkoLine, MkoChan
Addr, SubAddr
RxTxMode, DataSize
*/

int MkoXchgBnMini (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode)
{
int Result, MkoMode;
STATUS Status;

// проверка сообщения на допустимость
if ((!F4Mode) && (IsProhibitedMkoMsg(MkoCmd, &MkoBufB[0][0]))) return -5;

// устанавливаем тип обмена
MkoMode = (MkoCmd & 0x0400) ? (mcTM_RTtoBC) : (mcTM_BCtoRT);
if (F4Mode) MkoMode = mcTM_ModeNoData;

do
  {
  // инициализация описателя пакета МКО
  MkoPackB.nLast = 0;		// номер последнего сообщения
  memset(&MkoPackResB, 0, sizeof(MkoPackResB));  // очистка флагов ошибок пакета

  // инициализация описателя сообщения #0 в пакете
  MkoMsgB[0].TM   = MkoMode;	// тип сообщения
  MkoMsgB[0].Chan = MkoChan;	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgB[0].CW   = MkoCmd;	// первое командное слово

  MkoMsgB[0].MissSW = 1;	// прервать передачу при ошибках
  MkoMsgB[0].FlagSW = 1;
  MkoMsgB[0].FmtError = 1;

  MkoMsgB[0].GapTime = 0;	// передача без пауз
  // конец инициализации сообщения #0

  // сброс флагов для сообщения
  memset(&MkoMsgResB[0], 0, sizeof(mkoMsgRes_t));

  // проведение обмена по МКО
  Status = mkoPackIniStart(MkoLine, &MkoPackB, MkoMsgPtrB, 0);  // запуск обмена по МКО
  if (Status != OK) { Result = 1; break; }

  Status = mkoNumPackWait(MkoLine, &MkoPackB);  // ожидание завершения обмена по МКО
  if (Status != OK) { Result = 2; break; }

  // проверка результата обмена
  Result = CheckMkoXchg(&MkoPackResB, &MkoMsgB[0], &MkoMsgResB[0], 1);
  }
while (0);

// обмен завершен
return Result;
}
//================================================================================

/*! \fn int MkoXchgBnExt (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
 * \brief Служебная функция, производит обмен с устройствами на МКО в расширенном режиме (по адресам 2/4), для обмена используется пакет B.
 * \param [in] MkoLine - номер линии МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] MkoChan - номер канала МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] MkoCmd - командное слово МКО для обмена
 * \param [in] F4Mode - признак передачи команды в режиме Ф4 (0 = нет, 1 = да)
 * \return Функция возвращает результат обмена: <br>
 * 0 = успешно <br>
 * >0 = ошибка в функциях обмена (для детальной информации см. таблицы констант) <br>
 * <0 = нет связи/сбой передачи (для детальной информации см. таблицы констант) <br>
 * \details Возможна передача расширенных данных об ошибках обмена (если разрешено в конфигурации СПО).
 */

int MkoXchgBnExt (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode)
{
int Result, Count, State1, State2, Index;
WORD Addr, SubAddr, Command;
WORD Buf[32];

memmove(&Buf, &MkoBufB[0], sizeof(Buf));

Addr = MKO_CMD_ADDR(MkoCmd);
SubAddr = MKO_CMD_SUBADDR(MkoCmd);
Command = Buf[0];

// сброс состояния
if ((Addr == 4) && (SubAddr == 28))
  {
  MkoBufB[0][0] = 0x300D;
  MkoBufB[0][1] = 0x310D;
  MkoBufB[0][2] = 0x320D;
  MkoBufB[0][3] = 0x300E;
  MkoBufB[0][4] = 0x310E;
  MkoBufB[0][5] = 0x320E;
  Result = MkoXchgBnStd(MkoLine, MkoChan, MKO_CMD(4, 17, MKO_TX, 6), 0);
  if (Result) return Result;
  SleepMSec(100);
  }
if ((Addr == 2) && (SubAddr == 29))
  {
  MkoBufB[0][0] = 0x200D;
  MkoBufB[0][1] = 0x210D;
  MkoBufB[0][2] = 0x220D;
  MkoBufB[0][3] = 0x200E;
  MkoBufB[0][4] = 0x210E;
  MkoBufB[0][5] = 0x220E;
  Result = MkoXchgBnStd(MkoLine, MkoChan, MKO_CMD(2, 17, MKO_TX, 6), 0);
  if (Result) return Result;
  SleepMSec(100);
  }
if ((Addr == 4) && (SubAddr == 29))
  {
  MkoBufB[0][0] = 0x200D;
  MkoBufB[0][1] = 0x210D;
  MkoBufB[0][2] = 0x220D;
  MkoBufB[0][3] = 0x200E;
  MkoBufB[0][4] = 0x210E;
  MkoBufB[0][5] = 0x220E;
  Result = MkoXchgBnStd(MkoLine, MkoChan, MKO_CMD(4, 17, MKO_TX, 6), 0);
  if (Result) return Result;
  SleepMSec(100);
  }

// считывание состояния
if ((Addr != 2) || (SubAddr != 28))
  {
  Count = (Addr == 2) ? (9) : (7);
  Result = MkoXchgBnStd(MkoLine, MkoChan, MKO_CMD(Addr, 17, MKO_RX, Count), 0);
  if (Result) return Result;
  SleepMSec(10);
  }

// проверка состояния
for (Index = 1; Index <= 6; Index++) MkoBufB[0][Index] |= 0x0003;

if ((Addr == 4) && (SubAddr == 28))
  {
  State1 = ((MkoBufB[0][4] != 0x3063) || (MkoBufB[0][5] != 0x3163) || (MkoBufB[0][6] != 0x3263)) ? (1) : (0);
  State2 = ((MkoBufB[0][4] != 0x3093) || (MkoBufB[0][5] != 0x3193) || (MkoBufB[0][6] != 0x3293)) ? (1) : (0);
  if ((State1) && (State2)) { return 0x80; }
  }
if ((Addr == 2) && (SubAddr == 29))
  {
  State1 = ((MkoBufB[0][1] != 0x2063) || (MkoBufB[0][2] != 0x2163) || (MkoBufB[0][3] != 0x2263)) ? (1) : (0);
  State2 = ((MkoBufB[0][1] != 0x2093) || (MkoBufB[0][2] != 0x2193) || (MkoBufB[0][3] != 0x2293)) ? (1) : (0);
  if ((State1) && (State2)) { return 0x80; }
  }
if ((Addr == 4) && (SubAddr == 29))
  {
  State1 = ((MkoBufB[0][1] != 0x2063) || (MkoBufB[0][2] != 0x2163) || (MkoBufB[0][3] != 0x2263)) ? (1) : (0);
  State2 = ((MkoBufB[0][1] != 0x2093) || (MkoBufB[0][2] != 0x2193) || (MkoBufB[0][3] != 0x2293)) ? (1) : (0);
  if ((State1) && (State2)) { return 0x80; }
  }

// проверка состояния МВКУ/МПВН
if ((Addr == 4) && (SubAddr == 28))
  {
  Result = MkoXchgBnStd(MkoLine, MkoChan, MKO_CMD(Addr, SubAddr, MKO_RX, 6), 0);
  if (Result) return Result;
  if ((MkoBufB[0][0] != MkoBufB[0][1]) || (MkoBufB[0][0] != 0x0000)) { return 0x81; }
  if ((MkoBufB[0][2] != MkoBufB[0][3]) || (MkoBufB[0][2] != 0x0100)) { return 0x81; }
  if ((MkoBufB[0][4] != MkoBufB[0][5]) || (MkoBufB[0][4] != 0x0200)) { return 0x81; }
  }
if ((Addr == 2) && (SubAddr == 29))
  {
  Result = MkoXchgBnStd(MkoLine, MkoChan, MKO_CMD(Addr, SubAddr, MKO_RX, 6), 0);
  if (Result) return Result;
  if ((MkoBufB[0][0] != MkoBufB[0][1]) || (MkoBufB[0][0] != 0x0000)) { return 0x81; }
  if ((MkoBufB[0][2] != MkoBufB[0][3]) || (MkoBufB[0][2] != 0x0100)) { return 0x81; }
  if ((MkoBufB[0][4] != MkoBufB[0][5]) || (MkoBufB[0][4] != 0x0200)) { return 0x81; }
  }
if ((Addr == 4) && (SubAddr == 29))
  {
  Result = MkoXchgBnStd(MkoLine, MkoChan, MKO_CMD(Addr, SubAddr, MKO_RX, 6), 0);
  if (Result) return Result;
  if ((MkoBufB[0][0] != MkoBufB[0][1]) || (MkoBufB[0][0] != 0x0000)) { return 0x81; }
  if ((MkoBufB[0][2] != MkoBufB[0][3]) || (MkoBufB[0][2] != 0x0100)) { return 0x81; }
  if ((MkoBufB[0][4] != MkoBufB[0][5]) || (MkoBufB[0][4] != 0x0200)) { return 0x81; }
  }
SleepMSec(10);

// передача оригинальной команды на устройство
memmove(&MkoBufB[0], &Buf, sizeof(Buf));
Result = MkoXchgBnStd(MkoLine, MkoChan, MkoCmd, F4Mode);
if (Result) return Result;

// проверка состояния после подачи команды
if ((Addr == 4) && (SubAddr == 28))
  {
  SleepMSec(300);
  Result = MkoXchgBnStd(MkoLine, MkoChan, MKO_CMD(Addr, SubAddr, MKO_RX, 6), 0);
  if (Result) return Result;
  memmove(&Buf, &MkoBufB[0], sizeof(MkoBufB[0]));

  switch (Command)
    {
    case 0x0002: { if ((Buf[0] != 0x0012) || (Buf[1] != 0x0012) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x0004: { if ((Buf[0] != 0x0014) || (Buf[1] != 0x0014) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x0008: { if ((Buf[0] != 0x0018) || (Buf[1] != 0x0018) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x0010: { if ((Buf[0] != 0x0010) || (Buf[1] != 0x0010) || (Buf[2] != 0x0101) || (Buf[3] != 0x0101) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x0020: { if ((Buf[0] != 0x0010) || (Buf[1] != 0x0010) || (Buf[2] != 0x0102) || (Buf[3] != 0x0102) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x0040: { if ((Buf[0] != 0x0010) || (Buf[1] != 0x0010) || (Buf[2] != 0x0104) || (Buf[3] != 0x0104) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x1001: { if ((Buf[0] != 0x0021) || (Buf[1] != 0x0021) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x1002: { if ((Buf[0] != 0x0022) || (Buf[1] != 0x0022) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x1004: { if ((Buf[0] != 0x0024) || (Buf[1] != 0x0024) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x1020: { if ((Buf[0] != 0x0020) || (Buf[1] != 0x0020) || (Buf[2] != 0x0102) || (Buf[3] != 0x0102) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x1040: { if ((Buf[0] != 0x0020) || (Buf[1] != 0x0020) || (Buf[2] != 0x0104) || (Buf[3] != 0x0104) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x1080: { if ((Buf[0] != 0x0020) || (Buf[1] != 0x0020) || (Buf[2] != 0x0108) || (Buf[3] != 0x0108) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x8001: { if ((Buf[0] != 0x0001) || (Buf[1] != 0x0001) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0210) || (Buf[5] != 0x0210)) { return 0x82; } break; }
    case 0x8002: { if ((Buf[0] != 0x0002) || (Buf[1] != 0x0002) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0210) || (Buf[5] != 0x0210)) { return 0x82; } break; }
    case 0x8004: { if ((Buf[0] != 0x0004) || (Buf[1] != 0x0004) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0210) || (Buf[5] != 0x0210)) { return 0x82; } break; }
    case 0x8008: { if ((Buf[0] != 0x0008) || (Buf[1] != 0x0008) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0210) || (Buf[5] != 0x0210)) { return 0x82; } break; }
    case 0x9001: { if ((Buf[0] != 0x0001) || (Buf[1] != 0x0001) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0220) || (Buf[5] != 0x0220)) { return 0x82; } break; }
    case 0x9002: { if ((Buf[0] != 0x0002) || (Buf[1] != 0x0002) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0220) || (Buf[5] != 0x0220)) { return 0x82; } break; }
    case 0x9004: { if ((Buf[0] != 0x0004) || (Buf[1] != 0x0004) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0220) || (Buf[5] != 0x0220)) { return 0x82; } break; }
    case 0x9008: { if ((Buf[0] != 0x0008) || (Buf[1] != 0x0008) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0220) || (Buf[5] != 0x0220)) { return 0x82; } break; }
    case 0xA001: { if ((Buf[0] != 0x0001) || (Buf[1] != 0x0001) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0240) || (Buf[5] != 0x0240)) { return 0x82; } break; }
    case 0xA002: { if ((Buf[0] != 0x0002) || (Buf[1] != 0x0002) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0240) || (Buf[5] != 0x0240)) { return 0x82; } break; }
    case 0xA004: { if ((Buf[0] != 0x0004) || (Buf[1] != 0x0004) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0240) || (Buf[5] != 0x0240)) { return 0x82; } break; }
    case 0xA008: { if ((Buf[0] != 0x0008) || (Buf[1] != 0x0008) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0240) || (Buf[5] != 0x0240)) { return 0x82; } break; }
    case 0xB001: { if ((Buf[0] != 0x0001) || (Buf[1] != 0x0001) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0280) || (Buf[5] != 0x0280)) { return 0x82; } break; }
    case 0xB002: { if ((Buf[0] != 0x0002) || (Buf[1] != 0x0002) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0280) || (Buf[5] != 0x0280)) { return 0x82; } break; }
    case 0xB004: { if ((Buf[0] != 0x0004) || (Buf[1] != 0x0004) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0280) || (Buf[5] != 0x0280)) { return 0x82; } break; }
    }
  }
if ((Addr == 2) && (SubAddr == 29))
  {
  SleepMSec(300);
  Result = MkoXchgBnStd(MkoLine, MkoChan, MKO_CMD(Addr, SubAddr, MKO_RX, 6), 0);
  if (Result) return Result;
  memmove(&Buf, &MkoBufB[0], sizeof(MkoBufB[0]));

  switch (Command)
    {
    case 0x0001: { if ((Buf[0] != 0x0001) || (Buf[1] != 0x0001) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x0002: { if ((Buf[0] != 0x0002) || (Buf[1] != 0x0002) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x0004: { if ((Buf[0] != 0x0004) || (Buf[1] != 0x0004) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x0008: { if ((Buf[0] != 0x0008) || (Buf[1] != 0x0008) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    case 0x0010: { if ((Buf[0] != 0x0010) || (Buf[1] != 0x0010) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0200) || (Buf[5] != 0x0200)) { return 0x82; } break; }
    }
  }
if ((Addr == 4) && (SubAddr == 29))
  {
  SleepMSec(300);
  Result = MkoXchgBnStd(MkoLine, MkoChan, MKO_CMD(Addr, SubAddr, MKO_RX, 6), 0);
  if (Result) return Result;
  memmove(&Buf, &MkoBufB[0], sizeof(MkoBufB[0]));

  switch (Command)
    {
    case 0x0201: { if ((Buf[0] != 0x0000) || (Buf[1] != 0x0000) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0201) || (Buf[5] != 0x0201)) { return 0x82; } break; }
    case 0x0202: { if ((Buf[0] != 0x0000) || (Buf[1] != 0x0000) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0202) || (Buf[5] != 0x0202)) { return 0x82; } break; }
    case 0x0204: { if ((Buf[0] != 0x0000) || (Buf[1] != 0x0000) || (Buf[2] != 0x0100) || (Buf[3] != 0x0100) || (Buf[4] != 0x0204) || (Buf[5] != 0x0204)) { return 0x82; } break; }
    }
  }

// успешно завершено
return 0;
}
//================================================================================

/*! \fn int MkoXchgBnBlock (int AMkoLine, int AMkoChan, WORD AMkoCmd1, WORD AMkoCmd2, int AMsgCount, int AFlagMode);
 * \brief Функция передает набор сообщений по МКО, используется пакет B. Используется в функциях, вызываемых из прерываний СПО (при обменах с МБК04)
 * \param [in] AMkoLine - номер линии МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] AMkoChan - номер канала МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] AMkoCmd1 - командное слово МКО для передачи данных на МБК04
 * \param [in] AMkoCmd2 - командное слово МКО для передачи флага на МБК04
 * \param [in] AMsgCount - число передаваемых сообщений в пакете (1..32)
 * \param [in] AFlagMode - режим передачи флага данных (перед данными/после данных/не передавать)
 * \return Функция возвращает результат обмена: <br>
 * 0 = успешно <br>
 * >0 = ошибка в функциях обмена (для детальной информации см. таблицы констант) <br>
 * <0 = нет связи/сбой передачи (для детальной информации см. таблицы констант) <br>
 * \details Обмен по МКО может производиться в обычном и расширенном режимах (если разрешено в конфигурации СПО),
 * при этом обмен возможна передача расширенных данных об ошибках обмена
 */

int MkoXchgBnBlock (int AMkoLine, int AMkoChan, WORD AMkoCmd1, WORD AMkoCmd2, int AMsgCount, int AFlagMode)
{
int BaseMkoLine, BaseMkoChan;
int TotalResult, MkoMode, ErrorCode, ErrorBuffer, Index, MsgIndex, MsgExtCount;
STATUS Result;

// направления передачи команд должны совпадать
if ((AMkoCmd1 & 0x0400) != (AMkoCmd2 & 0x0400)) return -4;

// устанавливаем тип обмена
MkoMode = (AMkoCmd1 & 0x0400) ? (mcTM_RTtoBC) : (mcTM_BCtoRT);

TotalResult = 0;
MsgExtCount = (AFlagMode != MBK04_FLAG_NONE) ? (1) : (0);

do
  {
  // инициализация описателя пакета МКО
  MkoPackB.nLast = AMsgCount + MsgExtCount - 1;  // номер последнего сообщения
  memset(&MkoPackResB, 0, sizeof(MkoPackResB));  // очистка флагов ошибок пакета

  // индекс первого сообщения по МКО
  MsgIndex = 0;

  // если флаг идет перед данными
  if (AFlagMode == MBK04_FLAG_BEFORE_DATA)
    {
    MkoMsgPtrB[MsgIndex]->TM = MkoMode;
    MkoMsgPtrB[MsgIndex]->Chan = AMkoChan;
    MkoMsgPtrB[MsgIndex]->CW = AMkoCmd2;

    MkoMsgPtrB[MsgIndex]->MissSW = 1;  // прервать передачу при ошибках
    MkoMsgPtrB[MsgIndex]->FlagSW = 1;
    MkoMsgPtrB[MsgIndex]->FmtError = 1;

    MkoMsgPtrB[MsgIndex]->GapTime = 1200; // передача с паузой между сообщениями >= 400 мкс
    
    // сброс флагов для сообщения
    memset(&MkoMsgResB[MsgIndex], 0, sizeof(mkoMsgRes_t));
    MsgIndex++;
    }

  // инициализация сообщений МКО с данными
  for (Index = 0; Index < AMsgCount; Index++)
    {
    MkoMsgPtrB[MsgIndex]->TM = MkoMode;
    MkoMsgPtrB[MsgIndex]->Chan = AMkoChan;
    MkoMsgPtrB[MsgIndex]->CW = AMkoCmd1;

    MkoMsgPtrB[MsgIndex]->MissSW = 1;  // прервать передачу при ошибках
    MkoMsgPtrB[MsgIndex]->FlagSW = 1;
    MkoMsgPtrB[MsgIndex]->FmtError = 1;

    MkoMsgPtrB[MsgIndex]->GapTime = 1200; // передача с паузой между сообщениями >= 400 мкс
    
    // сброс флагов для сообщения
    memset(&MkoMsgResB[MsgIndex], 0, sizeof(mkoMsgRes_t));
    MsgIndex++;
    }

  // если флаг идет после данных
  if (AFlagMode == MBK04_FLAG_AFTER_DATA)
    {
    MkoMsgPtrB[MsgIndex]->TM = MkoMode;
    MkoMsgPtrB[MsgIndex]->Chan = AMkoChan;
    MkoMsgPtrB[MsgIndex]->CW = AMkoCmd2;

    MkoMsgPtrB[MsgIndex]->MissSW = 1;  // прервать передачу при ошибках
    MkoMsgPtrB[MsgIndex]->FlagSW = 1;
    MkoMsgPtrB[MsgIndex]->FmtError = 1;

    MkoMsgPtrB[MsgIndex]->GapTime = 1200; // передача с паузой между сообщениями >= 400 мкс
    
    // сброс флагов для сообщения
    memset(&MkoMsgResB[MsgIndex], 0, sizeof(mkoMsgRes_t)); 
    MsgIndex++;
    }

  // проведение обмена по МКО
  Result = mkoPackIniStart(AMkoLine, &MkoPackB, MkoMsgPtrB, 0);  // запуск обмена по МКО
  if (Result != OK) { TotalResult = 1; break; }

  Result = mkoNumPackWait(AMkoLine, &MkoPackB);  // ожидание завершения обмена по МКО
  if (Result != OK) { TotalResult = 2; break; }

  // проверка результата обмена
  TotalResult = CheckMkoXchg(&MkoPackResB, &MkoMsgB[0], &MkoMsgResB[0], MsgIndex);
  }
while (0);

// передача расширенных данных об ошибках обмена на МКО
if ((TotalResult) && (SpoConfig.EnableMkoErrorExtendedInfo))
  {
  ErrorCode = ErrCodGet();
  ErrorBuffer = ErrMKIGet((void*) &DebugMki, sizeof(DebugMki) / sizeof(DWORD));

  BaseMkoLine = SpoConfig.KpaMkoLine & 1;
  BaseMkoChan = SpoConfig.KpaMkoChan & 1;

  ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9001, KPA_SUBADDR_DEBUG, &TotalResult, sizeof(TotalResult));
  ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9002, KPA_SUBADDR_DEBUG, &MkoPackResB, sizeof(MkoPackResB));
  ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9003, KPA_SUBADDR_DEBUG, &MkoMsgResB[0], sizeof(MkoMsgResB[0]));

  if (ErrorCode) ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9004, KPA_SUBADDR_DEBUG, &ErrorCode, sizeof(ErrorCode));
  if (ErrorBuffer) ReplyToKpaWithDataX(BaseMkoLine, BaseMkoChan, 0x9005, KPA_SUBADDR_DEBUG, &DebugMki, 64);
  }

// обмен успешно завершен
return TotalResult;
}
//================================================================================

/*! \fn int MkoXchgXn (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
 * \brief Функция передает одно сообщение по МКО, используется пакет X. Используется в функциях, вызываемых из задачи обработки ошибок СПО.
 * \param [in] MkoLine - номер линии МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] MkoChan - номер канала МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] MkoCmd - командное слово МКО для обмена
 * \param [in] F4Mode - признак передачи команды в режиме Ф4 (0 = нет, 1 = да)
 * \return Функция возвращает результат обмена: <br>
 * 0 = успешно <br>
 * >0 = ошибка в функциях обмена (для детальной информации см. таблицы констант) <br>
 * <0 = нет связи/сбой передачи (для детальной информации см. таблицы констант) <br>
 */

int MkoXchgXn (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode)
{
int TotalResult, Pass, MkoMode;
STATUS Result;

// устанавливаем тип обмена
MkoMode = (MkoCmd & 0x0400) ? (mcTM_RTtoBC) : (mcTM_BCtoRT);

TotalResult = 0; // общий результат выполнения
Pass = 0; // номер прохода

while (1)
  {
  // инициализация описателя пакета МКО
  MkoPackX.nLast = 0;		// номер последнего сообщения
  memset(&MkoPackResX, 0, sizeof(MkoPackResX));  // очистка флагов ошибок пакета

  // инициализация описателя сообщения #0 в пакете
  MkoMsgX.TM   = MkoMode;	// тип сообщения
  MkoMsgX.Chan = MkoChan;	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgX.CW   = MkoCmd;	// первое командное слово

  MkoMsgX.MissSW = 1;		// прервать передачу при ошибках
  MkoMsgX.FlagSW = 1;
  MkoMsgX.FmtError = 1;

  MkoMsgX.GapTime = 0;	// передача без пауз
  // конец инициализации сообщения #00

  // сброс флагов для сообщения
  memset(&MkoMsgResX, 0, sizeof(mkoMsgRes_t));

  // проведение обмена по МКО
  Result = mkoPackIniStart(MkoLine, &MkoPackX, MkoMsgPtrX, 0);  // запуск обмена по МКО
  if (Result != OK) { TotalResult = 1; break; }

  Result = mkoNumPackWait(MkoLine, &MkoPackX);  // ожидание завершения обмена по МКО
  if (Result != OK) { TotalResult = 2; break; }

  // проверка результата обмена
  TotalResult = CheckMkoXchg(&MkoPackResX, &MkoMsgX, &MkoMsgResX, 1);
  if (TotalResult == -3)
    {
    // повтор при занятости абонента
    if (++Pass < 2) { SleepMSec(10); continue; }
    }

  break;
  }

// обмен успешно завершен
return TotalResult;
}
//================================================================================

/*! \fn int MkoXchgStub (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
 * \brief Функция-заглушка, используется в случаях, не предусматривающих выдачу сообщений по МКО
 * \param [in] MkoLine - номер линии МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] MkoChan - номер канала МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] MkoCmd - командное слово МКО для обмена
 * \param [in] F4Mode - признак передачи команды в режиме Ф4 (0 = нет, 1 = да)
 * \return Функция всегда возвращает 0 (успешно завершено)
 */

int MkoXchgStub (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode)
{
return 0;
}
//================================================================================

/*! \fn int CheckMkoXchg (mkoPackRes_t *PackRes, mkoPackMsg_t *MsgList, mkoMsgRes_t *MsgRes, int MsgCount);
 * \brief Функция проверяет результат обмена по МКО и возвращает код результата (для ф-ии DECODE_XCHG_ERROR())
 * \brief Возврат из функции - по первой ошибке в пакете или сообщении (если сообщений больше 1)
 * \param [in] PackRes - указатель на результат обмена для пакета
 * \param [in] MsgList - указатель на список сообщений пакета
 * \param [in] MsgRes - указатель на список результатов обмена для сообщений пакета
 * \param [in] MsgCount - число сообщений в пакете
 * \return Возвращает код результата обмена (0 = норма, не-0 = ошибки)
 */

int CheckMkoXchg (mkoPackRes_t *PackRes, mkoPackMsg_t *MsgList, mkoMsgRes_t *MsgRes, int MsgCount)
{
int Result = 0, Index, Cmd, Reply;

do
  {
  // [проверка результата для пакета]
  if (PackRes->MissSW) { Result = -1; break; } // нет ответного слова

  // если есть флаги в ответном слове
  if ((PackRes->FlagSW) || (PackRes->ErrSW))
    {
    // проверка сообщений пакета
    for (Index = 0; Index < MsgCount; Index++)
      {
      // команда/ответ от абонента
      Cmd = MsgList[Index].CW;
      Reply = MsgRes[Index].SW;

      // обмен закончен, адрес подтвержден
      if ((MsgRes[Index].Eom) && (MKO_CMD_ADDR(Cmd) == MKO_OS_ADDR(Reply)))
        {
        if (MKO_OS_OU_ERROR(Reply))  { Result = -6; break; } // неисправность ОУ
        if (MKO_OS_AB_ERROR(Reply))  { Result = -6; break; } // неисправность абонента
        if (MKO_OS_MSG_ERROR(Reply)) { Result = -7; break; } // ошибка в сообщении
        if (MKO_OS_BUSY(Reply))      { Result = -3; break; } // абонент занят
        }
      }
    if (Result) break;

    // прочие ошибки в ОС
    Result = -8; break;
    }

  // ошибки формата, длительности и т.д. при передаче
  if (PackRes->Error) { Result = -2; break; }

  // обмен успешно завершен
  Result = 0;
  }
while (0);

return Result;
}
//================================================================================

/*! \fn int DECODE_XCHG_ERROR (int Result);
 * \brief Функция распаковывает код результата обмена по МКО в коды ошибок СПО
 * \param [in] Result - код результата обмена по МКО
 * \return Возвращает код ошибки, соответствующий данному коду (для детальной информации см. таблицы констант)
 */

int DECODE_XCHG_ERROR (int Result)
{
if (Result > 0)
  {
  if (Result == 1) return REPLY_ERROR_POINT_1;
  if (Result == 2) return REPLY_ERROR_POINT_2;
  if (Result == 3) return REPLY_ERROR_POINT_3;
  if (Result == 0x80) return REPLY_DEVICE_STATE1_ERROR;
  if (Result == 0x81) return REPLY_DEVICE_STATE2_ERROR;
  if (Result == 0x82) return REPLY_DEVICE_STATE3_ERROR;
  if (Result == 0x83) return REPLY_CMD_DATA_ERROR;
  return REPLY_MKO_INT_ERROR;
  }

if (Result < 0)
  {
  if (Result == -1) return REPLY_MKO_NO_OS_ERROR;
  if (Result == -2) return REPLY_MKO_LINK_ERROR;
  if (Result == -3) return REPLY_DEVICE_BUSY_ERROR;
  if (Result == -4) return REPLY_CMD_DATA_ERROR;
  if (Result == -5) return REPLY_PROHIB_MSG_ERROR;
  if (Result == -6) return REPLY_MKO_OU_ERROR;
  if (Result == -7) return REPLY_MKO_OS_MSG_ERROR; // new
  if (Result == -8) return REPLY_MKO_OS_STATE_ERROR; // new
  return REPLY_UNK_ERROR;
  }

return REPLY_CMD_DONE;
}
//================================================================================

/*! \fn void ReplyToKpaNoData (int ABaseMkoLine, int ABaseMkoChan, int AReply, int ASubAddr);
 * \brief Функция передачи сообщения без данных на КПА, используется в функциях, вызываемых из прерываний СПО.
 * \param [in] ABaseMkoLine - линия МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] ABaseMkoChan - канал МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] AReply - передаваемый код сообщения
 * \param [in] ASubAddr - подадрес КПА, на который передается сообщение
 */

void ReplyToKpaNoData (int ABaseMkoLine, int ABaseMkoChan, int AReply, int ASubAddr)
{
MkoBufB[0][0] = AReply;
MkoXchgBn(ABaseMkoLine, ABaseMkoChan, MKO_CMD(KPA_ADDR, ASubAddr, MKO_TX, 1), 0);
}
//================================================================================

/*! \fn void ReplyToKpaWithData (int ABaseMkoLine, int ABaseMkoChan, int AReply, int ASubAddr, void *ABuffer, int ADataSize);
 * \brief Функция передачи сообщения и данных на КПА, используется в функциях, вызываемых из прерываний СПО.
 * \param [in] ABaseMkoLine - линия МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] ABaseMkoChan - канал МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] AReply - передаваемый код сообщения
 * \param [in] ASubAddr - подадрес КПА, на который передается сообщение
 * \param [in] ABuffer - указатель на буфер с данными для передачи, данные передаются как
 * один или несколько пакетов на подадрес [ASubAddr+1] КПА перед передачей сообщения.
 * \param [in] ADataSize - размер передаваемых данных в байтах
 */

void ReplyToKpaWithData (int ABaseMkoLine, int ABaseMkoChan, int AReply, int ASubAddr, void *ABuffer, int ADataSize)
{
int Result, RemainSize, BlockSize, WordCount;
BYTE *DataSrc;

RemainSize = ADataSize;
DataSrc = (BYTE*) ABuffer;

while (RemainSize)
  {
  BlockSize = (RemainSize >= 64) ? (64) : (RemainSize);
  memmove(&MkoBufB[0][0], DataSrc, BlockSize);

  RemainSize -= BlockSize;
  DataSrc += BlockSize;

  WordCount = (BlockSize + 1) / 2;
  Result = MkoXchgBn(ABaseMkoLine, ABaseMkoChan, MKO_CMD(KPA_ADDR, ASubAddr + 1, MKO_TX, WordCount), 0);
  if (Result)
    {
    MkoBufB[0][0] = DECODE_XCHG_ERROR(Result);
    MkoXchgBn(ABaseMkoLine, ABaseMkoChan, MKO_CMD(KPA_ADDR, ASubAddr, MKO_TX, 1), 0);
    return;
    }
  }

MkoBufB[0][0] = AReply;
MkoBufB[0][1] = ADataSize;
MkoBufB[0][2] = CalcCrc16(ABuffer, ADataSize);
MkoXchgBn(ABaseMkoLine, ABaseMkoChan, MKO_CMD(KPA_ADDR, ASubAddr, MKO_TX, 3), 0);
}
//================================================================================

/*! \fn void ReplyToKpaWithDataX (int ABaseMkoLine, int ABaseMkoChan, int AReply, int ASubAddr, void *ABuffer, int ADataSize);
 * \brief Функция передачи сообщения и данных на КПА (данные об ошибках), используется в функциях, вызываемых из задачи обработки ошибок.
 * \param [in] ABaseMkoLine - линия МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] ABaseMkoChan - канал МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] AReply - передаваемый код сообщения
 * \param [in] ASubAddr - подадрес КПА, на который передается сообщение
 * \param [in] ABuffer - указатель на буфер с данными для передачи, данные передаются как
 * один или несколько пакетов на подадрес [ASubAddr+1] КПА перед передачей сообщения.
 * \param [in] ADataSize - размер передаваемых данных в байтах
 */

void ReplyToKpaWithDataX (int ABaseMkoLine, int ABaseMkoChan, int AReply, int ASubAddr, void *ABuffer, int ADataSize)
{
int Result, RemainSize, BlockSize, WordCount;
BYTE *DataSrc;

RemainSize = ADataSize;
DataSrc = (BYTE*) ABuffer;

while (RemainSize)
  {
  BlockSize = (RemainSize >= 64) ? (64) : (RemainSize);
  memmove(&MkoBufX[0][0], DataSrc, BlockSize);

  RemainSize -= BlockSize;
  DataSrc += BlockSize;

  WordCount = (BlockSize + 1) / 2;
  Result = MkoXchgXn(ABaseMkoLine, ABaseMkoChan, MKO_CMD(KPA_ADDR, ASubAddr + 1, MKO_TX, WordCount), 0);
  if (Result)
    {
    MkoBufX[0][0] = DECODE_XCHG_ERROR(Result);
    MkoXchgXn(ABaseMkoLine, ABaseMkoChan, MKO_CMD(KPA_ADDR, ASubAddr, MKO_TX, 1), 0);
    return;
    }
  }

MkoBufX[0][0] = AReply;
MkoBufX[0][1] = ADataSize;
MkoBufX[0][2] = CalcCrc16(ABuffer, ADataSize);
MkoXchgXn(ABaseMkoLine, ABaseMkoChan, MKO_CMD(KPA_ADDR, ASubAddr, MKO_TX, 3), 0);
}
//================================================================================

// функция приема одиночного сообщения через вызов mkoSendPri()

void Input (unsigned nMKO, unsigned ADR, unsigned pADR, unsigned nCD, unsigned short *INF)
{
mkoMsgRes_t R_OBM;
mkoMSG_t DS_OBM;

DS_OBM.TM = 2;
DS_OBM.CWrd = (ADR << 11) | (0x0400) | (pADR << 5) | (nCD & 0x1F);
DS_OBM.CWrd2 = 0;
DS_OBM.adrData = INF;
DS_OBM.DescMsgRes = &R_OBM;

// принимаем по основной линии
DS_OBM.Chan = 0;

mkoSendPri(nMKO, (mkoMSG_t*) &DS_OBM, 5);
mkoWait(nMKO, (mkoMSG_t*) &DS_OBM);
}
//================================================================================

// функция передачи одиночного сообщения через вызов mkoSendPri()

void Output (unsigned nMKO, unsigned ADR, unsigned pADR, unsigned nCD, unsigned short *INF)
{
mkoMsgRes_t R_OBM;
mkoMSG_t DS_OBM;

DS_OBM.TM = 1;
DS_OBM.CWrd = (ADR << 11) | (pADR << 5) | (nCD & 0x1F);
DS_OBM.CWrd2 = 0;
DS_OBM.adrData = INF;
DS_OBM.DescMsgRes = &R_OBM;

// передаем по основной линии
DS_OBM.Chan = 0;

mkoSendPri(nMKO, (mkoMSG_t*) &DS_OBM, 5);
mkoWait(nMKO, (mkoMSG_t*) &DS_OBM);
}
//================================================================================

