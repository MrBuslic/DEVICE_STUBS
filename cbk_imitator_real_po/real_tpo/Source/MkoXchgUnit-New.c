
/*! \file MkoXchgUnit.c
 * \author Гапонов Р.В.
 * \date 2012.10.28
 * \version 1.0.0
 * \brief Модуль содержит функции обмена по МКО
 */

#include "MainUnit.h"
//================================================================================

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

// устанавливаем тип обмена
MkoMode = (MkoCmd & 0x0400) ? (mcTM_RTtoBC) : (mcTM_BCtoRT);
if (F4Mode) MkoMode = mcTM_ModeNoData;

TotalResult = 0; // общий результат выполнения
Pass = 1; // номер прохода

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

  MkoMsgA[0].GapTime = 0;	// передача без пауз
  // конец инициализации сообщения #0

  // проведение обмена по МКО
  MkoMsgResA[0].SW = 0;		// обнуление ответных слов

  Result = mkoPackIniStart(MkoLine, &MkoPackA, MkoMsgPtrA, 0);  // запуск обмена по МКО
  if (Result != OK) { TotalResult = 1; break; }
  
  Result = mkoNumPackWait(MkoLine, &MkoPackA);  // ожидание завершения обмена по МКО
  if (Result != OK) { TotalResult = 2; break; }

  // [проверка результата для пакета]

  // нет ответного слова
  if (MkoPackResA.MissSW) { TotalResult = -1; break; }

  // флаги ошибок в ответном слове
  if ((MkoPackResA.FlagSW) || (MkoPackResA.ErrSW)) 
    { 
    // устройство выставило бит занятости
    if ((MKO_OS_ADDR(MkoMsgResA[0].SW) == MKO_CMD_ADDR(MkoCmd)) && (MKO_OS_BUSY(MkoMsgResA[0].SW)))
      {
      if (++Pass > 2) { TotalResult = -3; break; }
      SleepMSec(10);
      continue;
      }

    // сбой при передаче команды
    if ((MKO_OS_ADDR(MkoMsgResA[0].SW) == MKO_CMD_ADDR(MkoCmd)) && (MKO_OS_MSG_ERROR(MkoMsgResA[0].SW)))
      {
      if (++Pass > 2) { TotalResult = -2; break; }
      SleepMSec(10);
      continue;
      }

    // прочие ошибки
    TotalResult = -2; break; 
    }

  // ошибки формата, длительности и т.д. при передаче
  if (MkoPackResA.Error) { TotalResult = -4; break; }

  // [проверка результата для сообщений]

  // если нет ответного слова
  if (MkoMsgResA[0].MissSW) { TotalResult = -1; break; }

  // флаги ошибок в ответном слове
  if (MkoMsgResA[0].FlagSW)
    {
    // устройство выставило бит занятости
    if ((MKO_OS_ADDR(MkoMsgResA[0].SW) == MKO_CMD_ADDR(MkoCmd)) && (MKO_OS_BUSY(MkoMsgResA[0].SW)))
      {
      if (++Pass > 2) { TotalResult = -3; break; }
      SleepMSec(10);
      continue;
      }

    // сбой при передаче команды
    if ((MKO_OS_ADDR(MkoMsgResA[0].SW) == MKO_CMD_ADDR(MkoCmd)) && (MKO_OS_MSG_ERROR(MkoMsgResA[0].SW)))
      {
      if (++Pass > 2) { TotalResult = -2; break; }
      SleepMSec(10);
      continue;
      }

    // прочие ошибки
    TotalResult = -2; break; 
    }
  
  // если нет правильного адреса в ответном слове
  if (MKO_OS_ADDR(MkoMsgResA[0].SW) != MKO_CMD_ADDR(MkoCmd)) { TotalResult = -2; break; }

  // устройство выставило бит занятости
  if (MKO_OS_BUSY(MkoMsgResA[0].SW))
    {
    if (++Pass > 2) { TotalResult = -3; break; }
    SleepMSec(10);
    continue;
    }

  // сбой при передаче команды
  if ((MKO_OS_ADDR(MkoMsgResA[0].SW) == MKO_CMD_ADDR(MkoCmd)) && (MKO_OS_MSG_ERROR(MkoMsgResA[0].SW)))
    {
    if (++Pass > 2) { TotalResult = -2; break; }
    SleepMSec(10);
    continue;
    }

  // ошибки формата, длительности и т.д. при передаче
  if (MkoMsgResA[0].FmtError) { TotalResult = -4; break; }

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

int MkoXchgBn (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode)
{
int Result, Addr, SubAddr, Dir, DataSize;

// проверяем, не требуется ли расширеный алгоритм обмена
Addr     = MKO_CMD_ADDR(MkoCmd);
SubAddr  = MKO_CMD_SUBADDR(MkoCmd);
Dir      = MKO_CMD_DIR(MkoCmd);
DataSize = MKO_CMD_DATASIZE(MkoCmd);

if ((SpoConfig.EnableMkoExtXchg) &&
    (Dir == 0) && 
    (DataSize == 1) && 
    (((Addr == 2) || (Addr == 4)) && ((SubAddr == 28) || (SubAddr == 29))))
  {
  // обмен с адресами 2/4 - расширенный алгоритм обмена
  Result = MkoXchgBnExt(MkoLine, MkoChan, MkoCmd, F4Mode);
  }
else
  {
  // обмен с прочими адресами - стандартный алгоритм обмена
  Result = MkoXchgBnStd(MkoLine, MkoChan, MkoCmd, F4Mode);
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

// устанавливаем тип обмена
MkoMode = (MkoCmd & 0x0400) ? (mcTM_RTtoBC) : (mcTM_BCtoRT);
if (F4Mode) MkoMode = mcTM_ModeNoData;

TotalResult = 0; // общий результат выполнения
Pass = 1; // номер прохода

while (1)
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

  // проведение обмена по МКО
  MkoMsgResB[0].SW = 0;		// обнуление ответных слов

  Result = mkoPackIniStart(MkoLine, &MkoPackB, MkoMsgPtrB, 0);  // запуск обмена по МКО
  if (Result != OK) { TotalResult = 1; break; }
  
  Result = mkoNumPackWait(MkoLine, &MkoPackB);  // ожидание завершения обмена по МКО
  if (Result != OK) { TotalResult = 2; break; }

  // [проверка результата для пакета]

  // нет ответного слова
  if (MkoPackResB.MissSW) { TotalResult = -1; break; } 

  // флаги ошибок в ответном слове
  if ((MkoPackResB.FlagSW) || (MkoPackResB.ErrSW)) 
    {  
    // устройство выставило бит занятости
    if ((MKO_OS_ADDR(MkoMsgResB[0].SW) == MKO_CMD_ADDR(MkoCmd)) && (MKO_OS_BUSY(MkoMsgResB[0].SW)))
      {
      if (++Pass > 2) { TotalResult = -3; break; }
      SleepMSec(10);
      continue;
      }

    // сбой при передаче команды
    if ((MKO_OS_ADDR(MkoMsgResB[0].SW) == MKO_CMD_ADDR(MkoCmd)) && (MKO_OS_MSG_ERROR(MkoMsgResB[0].SW)))
      {
      if (++Pass > 2) { TotalResult = -2; break; }
      SleepMSec(10);
      continue;
      }

    // прочие ошибки 
    TotalResult = -2; break; 
    }

  // ошибки формата, длительности и т.д. при передаче
  if (MkoPackResB.Error) { TotalResult = -4; break; }

  // [проверка результата для сообщений]

  // если нет ответного слова
  if (MkoMsgResB[0].MissSW) { TotalResult = -1; break; }

  // флаги ошибок в ответном слове
  if (MkoMsgResB[0].FlagSW)
    {
    // устройство выставило бит занятости
    if ((MKO_OS_ADDR(MkoMsgResB[0].SW) == MKO_CMD_ADDR(MkoCmd)) && (MKO_OS_BUSY(MkoMsgResB[0].SW)))
      {
      if (++Pass > 2) { TotalResult = -3; break; }
      SleepMSec(10);
      continue;
      }

    // сбой при передаче команды
    if ((MKO_OS_ADDR(MkoMsgResB[0].SW) == MKO_CMD_ADDR(MkoCmd)) && (MKO_OS_MSG_ERROR(MkoMsgResB[0].SW)))
      {
      if (++Pass > 2) { TotalResult = -2; break; }
      SleepMSec(10);
      continue;
      }

    // прочие ошибки
    TotalResult = -2; break; 
    }

  // если нет правильного адреса в ответном слове
  if (MKO_OS_ADDR(MkoMsgResB[0].SW) != MKO_CMD_ADDR(MkoCmd)) { TotalResult = -2; break; }

  // устройство выставило бит занятости
  if (MKO_OS_BUSY(MkoMsgResB[0].SW))
    {
    if (++Pass > 2) { TotalResult = -3; break; }
    SleepMSec(10); 
    continue;
    }

  // сбой при передаче команды
  if ((MKO_OS_ADDR(MkoMsgResB[0].SW) == MKO_CMD_ADDR(MkoCmd)) && (MKO_OS_MSG_ERROR(MkoMsgResB[0].SW)))
    {
    if (++Pass > 2) { TotalResult = -2; break; }
    SleepMSec(10);
    continue;
    }

  // ошибки формата, длительности и т.д. при передаче
  if (MkoMsgResB[0].FmtError) { TotalResult = -4; break; }

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

// обмен завершен
return TotalResult;
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

/*! \fn int MkoXchgBnBlock (int MkoLine, int MkoChan, WORD MkoCmd1, WORD MkoCmd2, int MsgCount, int SendCmdMsg);
 * \brief Функция передает набор сообщений по МКО, используется пакет B. Используется в функциях, вызываемых из прерываний СПО (при обменах с МБК04)
 * \param [in] MkoLine - номер линии МКО для обмена (0 = 1 линия, 1 = 2 линия)
 * \param [in] MkoChan - номер канала МКО для обмена (0 = основной канал, 1 = резервный канал)
 * \param [in] MkoCmd - командное слово МКО для обмена
 * \param [in] F4Mode - признак передачи команды в режиме Ф4 (0 = нет, 1 = да)
 * \param [in] MsgCount - число передаваемых сообщений в пакете (1..32)
 * \param [in] SendCmdMsg - передавать или нет командное сообщение на 8 подадрес (0 = нет, 1 = да)
 * \return Функция возвращает результат обмена: <br>
 * 0 = успешно <br>
 * >0 = ошибка в функциях обмена (для детальной информации см. таблицы констант) <br>
 * <0 = нет связи/сбой передачи (для детальной информации см. таблицы констант) <br>
 * \details Обмен по МКО может производиться в обычном и расширенном режимах (если разрешено в конфигурации СПО), 
 * при этом обмен возможна передача расширенных данных об ошибках обмена
 */

int MkoXchgBnBlock (int MkoLine, int MkoChan, WORD MkoCmd1, WORD MkoCmd2, int MsgCount, int SendCmdMsg)
{
int BaseMkoLine, BaseMkoChan;
int TotalResult, MkoMode, ErrorCode, ErrorBuffer, Index;
STATUS Result;
WORD MkoCmd;
mkoMsgRes_t *MsgRes;

// направления передачи команд должны совпадать
if ((MkoCmd1 & 0x0400) != (MkoCmd2 & 0x0400)) return -4;

// устанавливаем тип обмена
MkoMode = (MkoCmd1 & 0x0400) ? (mcTM_RTtoBC) : (mcTM_BCtoRT);

TotalResult = 0;

while (1)
  {
  // инициализация описателя пакета МКО
  MkoPackB.nLast = MsgCount + SendCmdMsg - 1;  // номер последнего сообщения
  memset(&MkoPackResB, 0, sizeof(MkoPackResB));  // очистка флагов ошибок пакета

  // инициализация сообщений
  for (Index = 0; Index < MsgCount; Index++)
    {
    MkoMsgPtrB[Index]->TM = MkoMode;
    MkoMsgPtrB[Index]->Chan = MkoChan;
    MkoMsgPtrB[Index]->CW = MkoCmd1;

    MkoMsgPtrB[Index]->MissSW = 1;  // прервать передачу при ошибках
    MkoMsgPtrB[Index]->FlagSW = 1;
    MkoMsgPtrB[Index]->FmtError = 1;

    MkoMsgPtrB[Index]->GapTime = 1200; // передача с паузой между сообщениями >= 400 мкс

    MsgRes = MkoMsgPtrB[Index]->DescMsgRes;
    MsgRes->SW = 0; // обнуление ответных слов
    }

  // добавление в пакет командного сообщения
  if (SendCmdMsg)
    {
    MkoMsgPtrB[Index]->TM = MkoMode;
    MkoMsgPtrB[Index]->Chan = MkoChan;
    MkoMsgPtrB[Index]->CW = MkoCmd2;

    MkoMsgPtrB[Index]->MissSW = 1;  // прервать передачу при ошибках
    MkoMsgPtrB[Index]->FlagSW = 1;
    MkoMsgPtrB[Index]->FmtError = 1;

    MkoMsgPtrB[Index]->GapTime = 1200; // передача с паузой между сообщениями >= 400 мкс

    MsgRes = MkoMsgPtrB[Index]->DescMsgRes;
    MsgRes->SW = 0; // обнуление ответных слов
    }

  // проведение обмена по МКО
  Result = mkoPackIniStart(MkoLine, &MkoPackB, MkoMsgPtrB, 0);  // запуск обмена по МКО
  if (Result != OK) { TotalResult = 1; break; }

  Result = mkoNumPackWait(MkoLine, &MkoPackB);  // ожидание завершения обмена по МКО
  if (Result != OK) { TotalResult = 2; break; }

  // [проверка результата для пакета]

  // нет ответного слова
  if (MkoPackResB.MissSW) { TotalResult = -1; break; }

  // флаги ошибок в ответном слове
  if ((MkoPackResB.FlagSW) || (MkoPackResB.ErrSW)) { TotalResult = -2; break; }

  // ошибки формата, длительности и т.д. при передаче
  if (MkoPackResB.Error) { TotalResult = -4; break; }

  // [проверка результата для сообщений]

  for (Index = 0; Index <= (MsgCount + SendCmdMsg - 1); Index++)
    {
    MsgRes = MkoMsgPtrB[Index]->DescMsgRes;

    // если нет ответного слова
    if (MsgRes->MissSW) { TotalResult = -1; break; }

    // флаги ошибок в ответном слове
    if (MsgRes->FlagSW) { TotalResult = -2; break; }

    // если нет правильного адреса в ответном слове
    MkoCmd = (Index != (MsgCount + SendCmdMsg - 1)) ? (MkoCmd1) : (MkoCmd2);
    if (MKO_OS_ADDR(MsgRes->SW) != MKO_CMD_ADDR(MkoCmd)) { TotalResult = -2; break; }

    // устройство выставило бит занятости 
    if (MKO_OS_BUSY(MsgRes->SW)) { TotalResult = -3; break; }

    // сбой при передаче команды
    if (MKO_OS_MSG_ERROR(MsgRes->SW)) { TotalResult = -2; break; }

    // ошибки формата, длительности и т.д. при передаче
    if (MkoMsgResB[0].FmtError) { TotalResult = -4; break; }
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
Pass = 1; // номер прохода

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

  // проведение обмена по МКО
  MkoMsgResX.SW = 0;		// обнуление ответных слов

  Result = mkoPackIniStart(MkoLine, &MkoPackX, MkoMsgPtrX, 0);  // запуск обмена по МКО
  if (Result != OK) { TotalResult = 1; break; }
  
  Result = mkoNumPackWait(MkoLine, &MkoPackX);  // ожидание завершения обмена по МКО
  if (Result != OK) { TotalResult = 2; break; }

  // [проверка результата для пакета]
  // нет ответного слова
  if (MkoPackResX.MissSW) { TotalResult = -1; break; }

  // ошибки в ответном слове
  if (MkoPackResX.ErrSW) { TotalResult = -2; break; }

  // ошибки при передаче
  if (MkoPackResX.Error) { TotalResult = 3; break; }

  // [проверка результата для сообщений]
  // если нет ответного слова
  if (MkoMsgResX.MissSW) { TotalResult = -1; break; }

  // если нет правильного адреса в ответном слове
  if (MKO_OS_ADDR(MkoMsgResX.SW) != MKO_CMD_ADDR(MkoCmd)) { TotalResult = -2; break; }

  if (MKO_OS_BUSY(MkoMsgResX.SW))
    {
    if (Pass > 1) { TotalResult = -3; break; }
    Pass++;
    SleepMSec(10);
    continue;
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

/*! \fn int DECODE_XCHG_ERROR (int Result);
 * \brief Функция распаковывает код результата обмена по МКО в коды ошибок СПО
 * \param [in] Result - код результата обмена по МКО
 * \return Возвращает код ошибки, соответствующий данному коду (для детальной информации см. таблицы констант)
 */

int DECODE_XCHG_ERROR (int Result)
{
if (Result == 0x80) return REPLY_DEVICE_STATE1_ERROR;
if (Result == 0x81) return REPLY_DEVICE_STATE2_ERROR;
if (Result == 0x82) return REPLY_DEVICE_STATE3_ERROR;

if (Result > 0) return REPLY_MKO_INT_ERROR;

if (Result == -1) return REPLY_MKO_NO_OS_ERROR;
if (Result == -2) return REPLY_MKO_OU_ERROR;
if (Result == -3) return REPLY_DEVICE_BUSY_ERROR;
if (Result == -4) return REPLY_MKO_LINK_ERROR;

return REPLY_UNK_ERROR;
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

