#ifndef N736_H
#define N736_H

#include <QDialog>
#include <QMap>
#include <QXmlStreamReader>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>
#include <qlist.h>



#include "../buses_imitator/kpi_bus_rpc.h"
#include "../OMNIBUSBOX/omnibus_rpc.h"
//#include "../mbk04_imitator/mbk04_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
//#include "../buses_imitator/power_bus_rpc.h"

#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>

//enum class CURRENT_CONNECT
//{
//	MAIN = 1,
//
//	OFF = 3
//};

enum class CURRENT_MKO
{
	MAIN = 1,
	RESERVE = 2,
	OFF = 3
};


enum class CURRENT_COMP
{
	MAIN = 1,
	RESERVE = 2,
	OFF = 3
};

enum chnl {
	CHAN_BLOCK_1 = 4,
	CHAN_INCLUDE_1 = 2,
	CHAN_CONNECT_1 = 3,
	CHAN_WORK_1 = 1,

	CHAN_BLOCK_2 = 36,
	CHAN_INCLUDE_2 =34,
	CHAN_CONNECT_2 = 35,
	CHAN_WORK_2 = 33,

	CHAN_READY_CU_1 = 9,
	CHAN_SET_BLOCK_1 = 14, 
	CHAN_SET_INCLUDE_1 = 12,
	CHAN_SET_OK_1 = 11,
	CHAN_SET_CONNECT_1 = 13, 
	CHAN_SET_WORK_1 = 10, 

	CHAN_READY_CU_2 = 41,
	CHAN_SET_BLOCK_2 = 46,
	CHAN_SET_INCLUDE_2 = 44,
	CHAN_SET_OK_2 = 43,
	CHAN_SET_CONNECT_2 = 45,
	CHAN_SET_WORK_2 = 42

};
union ZaprSinch {
	WORD  Zapr_word[3];
	struct
	{
		WORD  pSP : 1,     // признак синхронизации
			pRegim : 2,  // режим синхронизации
			pSKPI : 1,   // признак передачи КПИ
			pKPI : 2,    // признак КПИ
			NumKA : 4,   // номер КА
			NumSI : 4,   // номер СИ
			Rezerve : 2;

		WORD pRegim2 : 2, // режим синхронизации
			NumKP : 4,   // номер КП
			NumSI2 : 4,  // номер СИ
			NumVxW1 : 6; // номер вхождения в синхронизацию

		WORD NumVxW2;   // номер вхождения в синхронизацию
	
	};
};

struct SYNCHR_STRUCT
{

	unsigned char schzk[10];
	unsigned char pr;
	unsigned char nkp;
	unsigned char niis;
	unsigned int nsync;
	unsigned char sync[16];



};

//---------------------------------------------------------------------------

#define PR_PSP						0x0001    // признак готовности синхропосылки (01)
#define PR_RSSP						0x0002    // признак готовности состояния СП  (02) (синхронизации)
#define PR_RSSCHBK					0x0004    // признак готовности слова состояния СЧБК (04)
#define PR_RSendSHKPI				0x0008    // признак готовности посылки ШКПИ (08)
#define PR_RSSKPI					0x0010    // признак готовности состояния передачи ШКПИ (10)
#define PR_REndSeanceConn			0x0020	  // признак готовности завершения сеанса связи (20)
#define PR_RS14H736					0x0040    // признак готовности получения состояния МПШ (40)
#define PR_REndSync					0x0080    // признак готовности состояния завершения синхронизации(80)
#define PR_RSPrKPI					0x0100    // признак готовности преобраз.КПИ (100)
#define PR_MinRezHandlerInf			0x0200	  // призн.отриц.результат обработки информации(200)

//---------------------------------------------------------------------------
// Определение структуры слова состояния 14H736
//
typedef struct _STATEMPSH
{
	union
	{
		struct
		{
			WORD pRSP : 1,           // признак готовности синхропосылки (01)
				pRSSP : 1,            // признак готовности состояния СП  (02) (синхронизации)
				pRSSCHBK : 1,         // признак готовности слова состояния СЧБК (04)
				pRSendSHKPI : 1,      // признак готовности посылки ШКПИ (08)
				pRSSKPI : 1,          // признак готовности состояния передачи ШКПИ (10)
				pREndSeanceConn : 1,  // признак готовности завершения сеанса связи (20)
				pRS14H736 : 1,        // признак готовности получения состояния МПШ (40)
				pREndSync : 1,        // признак готовности состояния завершения синхронизации(80)
				pRSPrKPI : 1,         // признак готовности преобраз.КПИ (100)
				pMinRezHandlerInf : 1,// призн.отриц.результат обработки информации(200)
				Rezerve : 6;          // резерв
		};
		WORD state;
	};
}STATEMPSH, *PSTATEMPSH;

//struct {};
union word_SP
{
	WORD SP[10];
};

union format_SCHBK{
	WORD  SCHBK_word[2];
	struct
	{
		WORD scbk_1;

		WORD scbk_2;
	};
};

class N736_widg : public QWidget
{
    Q_OBJECT

public:

	N736_widg();
	~N736_widg();
	//void set_new_tm();
	//void new_data(int mko, int addr, int saddr, QVariantList words);
private:
	QWidget* widg;
	/// -- Главное окно;
	QMainWindow* main_widg;

	QPushButton *main_comp;
	QPushButton *reserve_comp;
	QPushButton *line_mko_0;
	QPushButton *line_mko_1;

	QList<QPushButton*> main_btn;


	QGroupBox *on_chan_gb;
	QGroupBox *on_sgnl_gb;
	QGroupBox *on_kit_gb;
	QGroupBox *on_line_gb;
	QGridLayout *inf_glayout;
	QGridLayout *inf_glayout_2;
	QGridLayout *kit_glayout;
	QGridLayout *line_glayout;

	//QMap<QList<int>, 

	bool isset(qulonglong x, qulonglong n);

	qulonglong num_chnl;

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	//void onDevice();
	void set_tm_state();
	void paint_buttons();
	CURRENT_COMP current_dev;
	CURRENT_MKO current_mko;


	int MKO;
	//int adr;
	int num_ku;
	QVariantList words;
	bool flag_on;
	bool kr;
	bool off_device;
	bool active_device;
	//QCheckBox* add_set(QString name, QString data, bool is_main = true);
	QList<int> name_lst_0;
	
	int adr_0;
	int adr_1;
	int adr_device;
	_STATEMPSH _state;

	void omni_connect();
	void new_tm(int tm);
	QVariantList synchr_w;
	void set_state();

	QString name = "14Н736"; // русская н
	QTimer *AbOn_tmr;
	void imit_on();
	SYNCHR_STRUCT synchr_strct;
	void example_but();

	void msg_syn(unsigned char* msg);
protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void dataIn(QVariantList dataList, QVariantList maskList);
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
signals:
	void new_data();
};

#endif // N736_H

