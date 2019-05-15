
#ifndef ControlUnitH
#define ControlUnitH
//================================================================================

#include "Integer.h"
//================================================================================

// маски полей в описателях расположения данных OK
#define OK_ADDR             	0       	// размер поля 5 бит, маска 0x1F
#define OK_ADDR_MASK        	0x1F
#define OK_SUBADDR          	5       	// размер поля 5 бит, маска 0x1F
#define OK_SUBADDR_MASK     	0x1F
#define OK_WORD_NUM         	10      	// размер поля 6 бит, маска 0x3F
#define OK_WORD_NUM_MASK    	0x3F
#define OK_WORD_INDEX       	16      	// размер поля 5 бит, маска 0x1F
#define OK_WORD_INDEX_MASK  	0x1F
#define OK_BIT_INDEX        	21      	// размер поля 4 бит, маска 0x0F
#define OK_BIT_INDEX_MASK   	0x0F
#define OK_FIELD_LEN        	25      	// размер поля 7 бит, маска 0x7F
#define OK_FIELD_LEN_MASK   	0x7F

// макрос для формирования описателя расположения данных OK
#define OK_PLACE(Addr, SubAddr, DataSize, WordIndex, BitIndex, FieldLen) \
  ((((Addr) & OK_ADDR_MASK) << OK_ADDR) | \
   (((SubAddr) & OK_SUBADDR_MASK) << OK_SUBADDR) | \
   (((DataSize) & OK_WORD_NUM_MASK) << OK_WORD_NUM) | \
   (((WordIndex) & OK_WORD_INDEX_MASK) << OK_WORD_INDEX) | \
   (((BitIndex) & OK_BIT_INDEX_MASK) << OK_BIT_INDEX) | \
   (((FieldLen) & OK_FIELD_LEN_MASK) << OK_FIELD_LEN))

// макросы выделения полей расположения OK
#define GET_OK_ADDR(OK)		(((OK) >> OK_ADDR) & OK_ADDR_MASK)
#define GET_OK_SUBADDR(OK)	(((OK) >> OK_SUBADDR) & OK_SUBADDR_MASK)
#define GET_OK_WORD_NUM(OK)	(((OK) >> OK_WORD_NUM) & OK_WORD_NUM_MASK)
#define GET_OK_WORD_INDEX(OK)	(((OK) >> OK_WORD_INDEX) & OK_WORD_INDEX_MASK)
#define GET_OK_BIT_INDEX(OK)	(((OK) >> OK_BIT_INDEX) & OK_BIT_INDEX_MASK)
#define GET_OK_FIELD_LEN(OK)	(((OK) >> OK_FIELD_LEN) & OK_FIELD_LEN_MASK)

#define OK_END			0		// метка конца таблицы описателей

#define DEVICES_COUNT 		32  		// число классов устройств для контроля (максимум)
#define OK_COUNT 		1024     	// число OK для контроля (максимум)
//================================================================================

extern char Control_DeviceList[DEVICES_COUNT];     // список устройств, находящихся на циклическом контроле (1 = контроль включен)
extern unsigned *Control_AddrList[DEVICES_COUNT];  // таблица указателей на данные по расположению OK устройств
extern unsigned *Control_NumList[DEVICES_COUNT];   // таблица указателей на данные по номерам OK устройств
extern WORD Control_StateList[OK_COUNT];           // данные по текущему состоянию контролируемых OK устройств
extern char Control_DataAvail[OK_COUNT];           // данные по наличию реальных данных OK устройств в таблице [Control_StateList]

extern WORD Control_ChangedList[2 * OK_COUNT];     // буфер под изменившиеся данные OK для передачи на КПА
extern int Control_ChangedCount;                   // размер накопленных данных в буфере [Control_ChangedList]
//================================================================================

extern unsigned Control_BitMap[];                  // битовые маски для быстрого расчета размера полей

extern unsigned Control_AddrList00[];              // таблицы данных по расположению OK устройств (указатели заносятся в таблицу [Control_AddrList])
extern unsigned Control_AddrList01[];
extern unsigned Control_AddrList02[];
extern unsigned Control_AddrList03[];
extern unsigned Control_AddrList04[];
extern unsigned Control_AddrList05[];
extern unsigned Control_AddrList06[];
extern unsigned Control_AddrList07[];

extern unsigned Control_AddrList08[];
extern unsigned Control_AddrList09[];
extern unsigned Control_AddrList0A[];
extern unsigned Control_AddrList0B[];
extern unsigned Control_AddrList0C[];
extern unsigned Control_AddrList0D[];
extern unsigned Control_AddrList0E[];
extern unsigned Control_AddrList0F[];

extern unsigned Control_AddrList10[];
extern unsigned Control_AddrList11[];
extern unsigned Control_AddrList12[];
extern unsigned Control_AddrList13[];
extern unsigned Control_AddrList14[];
extern unsigned Control_AddrList15[];
extern unsigned Control_AddrList16[];
extern unsigned Control_AddrList17[];

extern unsigned Control_AddrList18[];
extern unsigned Control_AddrList19[];
extern unsigned Control_AddrList1A[];
extern unsigned Control_AddrList1B[];
extern unsigned Control_AddrList1C[];
extern unsigned Control_AddrList1D[];
extern unsigned Control_AddrList1E[];
extern unsigned Control_AddrList1F[];
//================================================================================

extern unsigned Control_NumList00[];               // таблицы данных по номерам OK устройств (указатели заносятся в таблицу [Control_NumList])
extern unsigned Control_NumList01[];
extern unsigned Control_NumList02[];
extern unsigned Control_NumList03[];
extern unsigned Control_NumList04[];
extern unsigned Control_NumList05[];
extern unsigned Control_NumList06[];
extern unsigned Control_NumList07[];

extern unsigned Control_NumList08[];
extern unsigned Control_NumList09[];
extern unsigned Control_NumList0A[];
extern unsigned Control_NumList0B[];
extern unsigned Control_NumList0C[];
extern unsigned Control_NumList0D[];
extern unsigned Control_NumList0E[];
extern unsigned Control_NumList0F[];

extern unsigned Control_NumList10[];
extern unsigned Control_NumList11[];
extern unsigned Control_NumList12[];
extern unsigned Control_NumList13[];
extern unsigned Control_NumList14[];
extern unsigned Control_NumList15[];
extern unsigned Control_NumList16[];
extern unsigned Control_NumList17[];

extern unsigned Control_NumList18[];
extern unsigned Control_NumList19[];
extern unsigned Control_NumList1A[];
extern unsigned Control_NumList1B[];
extern unsigned Control_NumList1C[];
extern unsigned Control_NumList1D[];
extern unsigned Control_NumList1E[];
extern unsigned Control_NumList1F[];
//================================================================================

void ControlInitTables (void);
void ControlCheckList (void);
void ControlCheckDevice (int ADeviceIndex);
void ControlResetDeviceState (int ADeviceIndex);
int ControlOkNumberToDevice (int ANumber, int *AIndex);
//================================================================================

#endif

