
#ifndef CrcUnitH
#define CrcUnitH
//================================================================================

#include "Integer.h"
//================================================================================

extern const DWORD Crc32Table[256];
extern const WORD Crc16Table[256];
extern const BYTE Crc8Table[256];
//================================================================================

/**
 * \brief расчет CRC32 для блока данных, CRC-32 IEEE (Polynom = 0x104C11DB7)
 * \param Buffer - адрес буфера
 * \param Size - размер данных в байтах
 * \return CRC32
 */
DWORD CalcCrc32 (void *Buffer, int Size);

/**
 * \brief расчет CRC16 для блока данных, CRC-16 CCITT (Polynom = 0x11021)
 * \param Buffer - адрес буфера
 * \param Size - размер данных в байтах
 * \return CRC16
 */
WORD CalcCrc16 (void *Buffer, int Size);

/**
 * \brief расчет CRC8 для блока данных, CRC-8 CCITT (Polynom = 0x18D)
 * \param Buffer - адрес буфера
 * \param Size - размер данных в байтах
 * \return CRC8
 */
BYTE CalcCrc8 (void *Buffer, int Size);
//================================================================================

#endif

