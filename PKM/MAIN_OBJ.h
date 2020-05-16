/********************************************************************/
//
// PKM.h
//
// Header-file for all parts of PKM + TSRP
// Target platform: WINNT 4.0 
//
// Author:  Melkonyan
//         NII SM, 06.09.2001
//

/******************************************************************/

#include <dos.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <conio.h>

//#include <afxmt.h>

#include"socket_rpc.h"
#include "omnibus_rpc.h"
#include "interrupt_bus_rpc.h"

//#include <windows.h>
//#include <windowsx.h>

/******************************************************************/

//---------register addresses - Model Call Reasons ----------------
#define  REASON_INIT			0x00
#define  REASON_TACT			0x01
#define  REASON_TIMER			0x02
#define  REASON_TIMER_CYCLE		0x03
#define  REASON_TICK			0x04
#define  REASON_INTERRUPT_I		0x05
#define  REASON_INTERRUPT_P		0x06
#define  REASON_INTERRUPT_UP	0x07
#define  REASON_TACT_CVM		0x08
#define  REASON_TICK_CVM		0x09
#define  REASON_TM_S_CVM		0x0a
#define  REASON_TM_H_CVM		0x0b
#define  REASON_RELAY_P_CVM		0x0c
#define  REASON_RELAY_UP_CVM	0x0d
#define  REASON_MKO0			0x10
#define  REASON_MKO1			0x11
#define  REASON_MKO2			0x12
#define  REASON_MKO3			0x13
#define  REASON_MKO4			0x14
#define  REASON_MKO5			0x15
#define  REASON_OU0             0x20 
#define  REASON_OU1             0x21
#define  REASON_OU2             0x22
#define  REASON_OU3             0x23
#define  REASON_OU4             0x24
#define  REASON_OU5             0x25
#define  REASON_TACT_CVM00      0x40  
#define  REASON_TACT_CVM01      0x41
#define  REASON_TACT_CVM10      0x42
#define  REASON_TACT_CVM11      0x43
#define  REASON_TACT_CVM20      0x44
#define  REASON_TACT_CVM21      0x45
#define  REASON_TACT_CVM30      0x46
#define  REASON_TACT_CVM31      0x47
#define  REASON_TICK1           0x48 
#define  REASON_HIM             0x30

//---------data record codes --------------------------------------

#define CODE_BLNK    0xFFFF

#define SIZE_ZAG_BLOCK    5     // Header size for In/Out information
				//  (DOS <> NT)
//--------- Response Codes-----------------------------------------

#define W_END_DBG          01 	// End of Work from Debugger
#define W_END_UAI          02	// End of UAI Input File
#define W_END_USER         04	// End of UAI Input File

#define F_ERROR_SYS       -02	// Error - can't Load DLL and so on
#define F_ERROR_UAI       -04	// Error of UAI
#define F_ERROR_USER      -010   // Error of User Models


//***********************************************************************
//--------------------------------------- table of models ----------------
struct mod_table {
	// Model ID 
	unsigned long int ID;			
	// the Reason
	unsigned long int Reason;		
	// the Address on MKO 
	unsigned long int Address;		
	// basic enter to model
	void (*Subrut)(void);			
	// pointer to Input Buffer
	unsigned long int *pInputBuffer;	
	// pointer to Output Buffer
	unsigned long int *pOutputBuffer;	
	//who is the first in Line?
	unsigned long int Prioritet;		
		 };


//--------------------------------------- table of param---------------
//тип для функции печати
typedef void (*_printkl)();

struct reg_table
             {

	//причина вызова модели
	unsigned long int Reason;
	//вывод в файл
	_printkl	pTextToFile;
	//вывод на экран
	_printkl	pTextToWindow;
	//входной буфер
	unsigned long int *pInputBuffer;
	//выходной буфер
	unsigned long int *pOutputBuffer;
	//строка для вывода
			 char BufferForText[256];

              };

//*************************************************************************

class MAIN_OBJECT : public QThread
{
	Q_OBJECT

public:
	MAIN_OBJECT();
	void run();
	//главная функция обработки МКО
	void main_func();

public slots:
	//вывод в файл
	void addLog(QString _msg);
	//обработка сигнала прерываний
	void get_new_interrupt(int _n, short _chan, double _u, double _t);

private:
	QString log_filename;

};

