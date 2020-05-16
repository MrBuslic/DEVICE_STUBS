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

#define UNICODE
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QMutex>
#include <QThread>
//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
//#include <afxdisp.h>        // MFC OLE automation classes


//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC support for Windows Common Controls
////#include "Inc\mrcext.h" 
//#include <afxtempl.h>
//#include <afxpriv.h> 
//#include <afxole.h>
//#endif

#include <dos.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <conio.h>

#include <windows.h>
#include <windowsx.h>

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

//КОНСТАНТЫ, ЗАДАЮЩИЕ РАЗМЕРЫ МАССИВОВ:

#define rost    400          //Для остановов.
#define rwzw    100          //Для возвратов из подпрограмм.
#define rsource 100          //Для вложенных файлов.
#define rimper  1000          //Для имен внутренних переменных.
#define rmetka  100          //Для меток переходов.
#define rrom    100          //Для группы романовских чисел.
#define relse   1000          //Для стека if-else .
#define rpar 100             //Для массива передаваемых парам. 

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
	unsigned long int ID;			// Model ID 
	unsigned long int Reason;		// the Reason 
	unsigned long int Address;		// the Address on MKO 
	void (*Subrut)(void);			// basic enter to model
	unsigned long int *pInputBuffer;	// pointer to Input Buffer
	unsigned long int *pOutputBuffer;	// pointer to Output Buffer
	unsigned long int Prioritet;		//who is the first in Line?
		 };


//--------------------------------------- table of param---------------
typedef void (*_printkl)();

struct reg_table
             {

	unsigned long int Reason;
	_printkl	pTextToFile;
	_printkl	pTextToWindow;
	unsigned long int *pInputBuffer;
	unsigned long int *pOutputBuffer;
			 char BufferForText[256];

              };

//------------------------------ function declarations ---------------
void TextToFile(QString string);
void TextToFile(char* string);
void TextToWindow(QString string);
//*************************************************************************

typedef struct _m_bp {
	UINT pr;	//признаки
	UINT adr;
	UINT save;
	UINT count;
}M_BP;

typedef struct {
	UINT	st_name;		/* Symbol name, index in string tbl */
	UINT	st_value;		/* Value of the symbol */
	UINT	st_size;		/* Associated symbol size */
	UCHAR	st_info;		/* Type and binding attributes */
	UCHAR	st_other;		/* No defined meaning, 0 */
	USHORT	st_shndx;		/* Associated section index */
} Elf32_External_Sym;

typedef struct {

	UINT	n_strx;	// смещение в stabst
	UCHAR	n_type;	// тип
	UCHAR	n_other;	// =0
	USHORT	n_desc;	// обычно - номер строки
	UINT	n_value;	//  обычно - адрес
}Elf32_stab;


typedef struct _lrom {
	QString rom;
	UINT start;
	UINT ftext;
	UINT etext;
	UINT fdata;
	UINT edata;
	UINT fbss;
	UINT end;
	Elf32_stab* pstab;
	UINT lstab;
	UCHAR* pstabst;
	UINT lstabst;
	Elf32_External_Sym* psym;
	UINT lsym;
	UCHAR* pstrtab;
	UINT lstrtab;


}Srom;