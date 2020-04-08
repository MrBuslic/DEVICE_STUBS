#ifndef BAU_H
#define BAU_H

#include <QDialog>
#include <QMap>
#include <QXmlStreamReader>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>
#include <QLineEdit>
#include <QTimer>
#include <QTime>
#include <qplaintextedit.h>
#include <qsettings.h>
#include <mku_bus.h>

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
#include "../buses_imitator/power_bus_rpc.h"


enum MK
{
	MK_OFF = 0,
	MK_1 = 1,
	MK_2 = 2,
	MK_3 = 4
};

enum MT
{
	MT_OFF = 0,
	MT_1 = 1,
	MT_2 = 2,
	MT_3 = 4
};

enum MU
{
	MU_OFF = 0,
	MU_1 = 1,
	MU_2 = 2,
	MU_3 = 3
};

enum UM
{
	UM_OFF = 0,
	UM_1 = 4,
	UM_2 = 2,
	UM_3 = 1
};

enum LBV
{
	LBV_OFF = 0,
	LBV_1 = 4,
	LBV_2 = 2,
	LBV_3 = 1
};

enum PRM
{
	PRM_Ant_OFF = 0,
	PRM_Ant_1 = 2,
	PRM_Ant_2 = 1
};

enum SGTS
{
	SGTS_Ant_OFF = 0,
	SGTS_Ant_1 = 2,
	SGTS_Ant_2 = 1
};

enum BOCH
{
	BOCH_OFF = 0,
	BOCH_1 = 4,
	BOCH_2 = 2,
	BOCH_3 = 1
};

enum FOS
{
	FOS_OFF = 0,
	FOS_1 = 4,
	FOS_2 = 2,
	FOS_3 = 1
};

enum AOS
{
	AOS_OFF = 0,
	AOS_1 = 4,
	AOS_2 = 2,
	AOS_3 = 1
};

enum UPCH
{
	UPCH_OFF = 0,
	UPCH_1 = 2,
	UPCH_2 = 1
};

enum ALPS
{
	ALPS_OFF = 0,
	ALPS_1 = 4,
	ALPS_2 = 2,
	ALPS_3 = 1
};

enum MFS
{
	MFS_OFF = 0,
	MFS_1 = 4,
	MFS_2 = 2,
	MFS_3 = 1
};

enum LPch
{
	LPch_OFF = 0,
	LPch_1 = 4,
	LPch_2 = 2,
	LPch_3 = 1
};

enum BUFAR
{
	BUFAR_OFF = 0,
	BUFAR_1 = 4,
	BUFAR_2 = 2,
	BUFAR_3 = 1,
};

union Serv_Union
{
	unsigned char bits;
	struct
	{
		quint16

			first_bit : 1,
			second_bit : 1,
			third_bit : 1,
			fourth_bit : 1,
			rez : 12;
	};
};

union ZTM_DataWords
{
	quint16 data_words[25];
	struct
	{
		quint16 //1
			TM_MFS_OZU : 1, //Норма ОЗУ
			TM_LPch_k : 1, //Норма норма контроля частоты ЛПЧ
			OK_LPch : 3, //Выходные сигналы ЛПЧ 1-3 комплектов(далее "к")
			TM_SGS_k : 1, //Сигнал контрольного СГС
			TM_SGS_o : 1, //Сигнал основного СГС
			TM_STS_k : 1, //Сигнал контрольного СТС
			TM_STS_o : 1, //Сигнал основного СТС
			TM_og : 1, //Сигнал ОГ
			OK_MFS : 3, //Питание МФС 1-3к
			OK_ALPS : 3; //Питание АЛПС 1-3к
		quint16//2
			TM_SGTS_ant_A : 2, //Включен СТС/СГС антена 2
			TM_PRM_ant_A : 2, // Включен ПРМ антенна 2
			TM_PRM_ant_1 : 2, // Включен ПРМ антенна 1
			OK_BUFAR : 3, // Питание БУФАР 1-3к
			TM_BFS : 1, // Норма стыковки разъёмов в БФС
			TM_MFS_STCH : 1, // Норма синтезатора частот МФС
			TM_MFS_I : 1, // Норма информации МФС
			TM_MFS_KS : 1, // Норма контрольного сигнала МФС
			TM_MFS_BS : 1, // Норма выходного сигнала
			TM_MFS_OB : 1, // Норма обмена с МФС
			TM_MFS_PZU : 1; // Норма ПЗУ
		quint16//3
			VNP_APD_MT : 3, // Вторичное напряжение питания АПД-МТ
			PNP_APD_AT : 1, // Первичное напряжение питания АПД-МТ
			PNP_APD_MT : 1, // Первичное напряжение питания АПД-АТ
			TM_PRM_ant_a_AP2 : 2, // ПРМ антенна А подключена через АП 2
			TM_PRM_ant_1_AP1 : 2, // ПРМ антенна 1 подключена через АП 1
			TM_FOS : 3, // Включен ФОС
			TM_UPCH : 2, // Включен УПЧ
			TM_SGTS_ant_1 : 2; //Включен СТС/СГС антена 1 
		quint16//4
			TM_LBV_FP_1 : 1, // Подключен ЛБВ 1к через ФП
			TM_BOCH : 3, // Включен БОЧ
			GOT_CBA : 3, // Готов канал А,Б и С БМВ
			VP_CBA : 3, // Вторичное напряжение питания канала А,Б и С БВМ
			PP_CBA : 3, // Первичное напряжение питания канала А,Б и С БВМ
			VNP_APD_AT : 3; // Вторичное напряжение питания АПД-АТ
		quint16//5
			TAK : 1, // Функция контроля информационного обмена между ЦБК и БАУ. 0 - выкл, 1 - вкл. 
			SAK : 1, // Сработал автомат контроля информационного обмена между ЦБК и БАУ с последующис выключениес устройств БАУ
			rez5 : 7, // Резерв
			TM_RAOS : 1, // Исправен АОС
			TM_RBU_AOS : 1, // Исправен УУ
			TM_AOS : 3, // Включен АОС 1-3к
			TM_LBV_FP_3 : 1, // Подключен ЛБВ 3к через ФП
			TM_LBV_FP_2 : 1; // Подключен ЛБВ 2к через Фп
		quint16//6
			TM_ARU_UPch_2 : 8, // Уровень АРУ УПЧ 2к
			TM_ARU_UPch_1 : 8; // Уровень АРУ УПЧ 1к
		quint16//7
			TM_Rout_FOS_2 : 8, // Мощность на выходе ФОС 2к
			TM_Rout_FOS_1 : 8; // Мощность на выходе ФОС 1к
		quint16//8
			TM_Rout_LBV : 8, // Мощность на выходе ЛБВ 1-3к
			TM_Rout_FOS_3 : 8; // Мощность на выходе ФОС 3к
		quint16//9
			TM_Uc_LBV_1 : 8, // Ток сетки ЛБВ
			TM_Ic_LBV : 8; // Напряжение сетки ЛБВ 1к 
		quint16//10
			TM_Uc_LBV_3 : 8, // Напряжение сетки ЛБВ 3к
			TM_Uc_LBV_2 : 8; // Напряжение сетки ЛБВ 2к
		quint16//11
			T_rez : 8, //Резервный датчик температуры
			T_VIP_BAU : 8; //Температура ВИП БАУ
		quint16//12
			TM_T2_LBV2 : 8, //Температура ЛБВ2
			TM_T1_LBV1 : 8; //Температура ЛБВ1
		quint16//13
			TM_TD : 8, //Температура в БАЛОИ
			TM_T3_LBV3 : 8; //Температура ЛБВ3
		quint16//14
			MT : 2, //Включенный комплект МТ
			MK : 2, //Включенный комплект МК
			rez14 : 6,
			KMT_123 : 3, // Контроль МТ 1-3
			KMK_123 : 3; // Контроль МК 1-3
		quint16//15
			rez15 : 6,
			K15 : 10; //Контроль К1-5 компататора 1-5 МТМ
		quint16//16
			KAD_17 : 12,//Контрольный аналоговый датчик БАУ
			KAD_A_17 : 1,//Авария датчика 1-7КАД
			rez16 : 3;
		quint16//17
			KAD_814 : 12, //Контрольный аналоговый датчик БАУ
			KAD_A_814 : 1, //Авария датчика 8-14КАД
			rez17 : 3;
		quint16//18
			KTD46 : 12, //Контрольный температурный датчик датчик БАУ
			KTD_A_46 : 1, //Авария датчика 4-6КТД
			rez18 : 3;
		quint16//19
			KTD13 : 12, //Контрольный температурный датчик датчик БАУ
			KTD_A_13 : 1, //Авария датчика 1-3КТД
			rez19 : 3;
		quint16//20
			AD_rez_2 : 8, //Аналоговый датчик 2
			AD_rez_1 : 8; //Аналоговый датчик 1
		quint16//21
			AD_rez_4 : 8, //Аналоговый датчик 4
			AD_rez_3 : 8; //Аналоговый датчик 3
		quint16//22
			Sch_TM : 8, //Счётчик телеметрии
			rez22 : 4,
			DD_rez_7 : 1, //Дискретный резервный датчик 7
			DD_rez_5 : 1, //Дискретный резервный датчик 5
			DD_rez_3 : 1, //Дискретный резервный датчик 3
			DD_rez_1 : 1; //Дискретный резервный датчик 1
		quint16//23
			Sch_10ms : 16; //Счётчик 10мс меток
		quint16//24
			REsch : 1, //Режим рабочий или технологический
			rezR : 1,
			SBANK : 1, // Номер стартового банка ЭППЗУ
			rez2S : 1,
			COPY : 2, //Номер копии прикладной программы
			rezC : 4,
			KP12 : 1,//Соответствие контрольной суммы прикладной программы банк 1, копия 2
			KP11 : 1,//Соответствие контрольной суммы прикладной программы банк 1, копия 1
			KP02 : 1,//Соответствие контрольной суммы прикладной программы банк 0, копия 2
			KP01 : 1,//Соответствие контрольной суммы прикладной программы банк 0, копия 1
			KS1 : 1,//Соответствие контрольной суммы СПО банк 0
			KS0 : 1;//Соответствие контрольной суммы СПО банк 1
		quint16//25
			ABANK : 1, //Активный банк ПЗУ
			PITB2 : 1,//Питание ПЗУ банка 0
			PITB1 : 1,//Питание ПЗУ банка 1
			Poff : 1,//Рестарт по включению питания БС 27
			Ptok : 1,//Рестарт по превышению тока потребления процессора
			Pnapr : 1,//Рестарт по снижению напряжения питания процессора
			Ptime : 1,//Рестарт по сторожевому таймеру
			PRon : 1,//Рестарт по включению питания БС 27 - предыдущее состояние 
			PRtok : 1,//Рестарт по снижению напряжения питания процессора - предыдущее состояние
			PRnapr : 1,//Рестарт по превышению тока потребления процессора - предыдущее состояние
			PRtime : 1,//Рестарт по сторожевому таймеру - предыдущее состояние
			rez25 : 5;
	};
};

union ZTK_DataWords
{
	quint16 data_words[4];
	struct
	{
		quint16
			rez_1 : 1,
			SGTS_ant_A_com : 2,
			SGTS_ant_one_com : 2,
			PRM_ant_A_com : 2,
			PRM_ant_one_com : 2,
			LBV : 3,
			rez_2 : 1,
			UM : 3;
		quint16
			LPch_first_bit : 1,
			MFS : 3,
			ALPS : 3,
			AOS : 3,
			FOS : 3,
			BOCH : 3;
		quint16
			FOS_att2_D3 : 1,
			BOCH_CH : 8,
			UPCH : 2,
			BUFAR : 3,
			LPch_third_b : 1,
			LPch_sec_b : 1;
		quint16
			tout_AT : 1,
			tout_MT : 1,
			rez : 1,
			UPCH_att : 5,
			FOS_PS : 1,
			FOS_att1 : 4,
			FOS_att2_D0 : 1,
			FOS_att2_D1 : 1,
			FOS_att2_D2 : 1;
	};
};

union BOCH_ch_com
{
	quint16 data_words;

	struct
	{
		quint16
			BOCH_ch_D0 : 1,
			BOCH_ch_D1 : 1,
			BOCH_ch_D2 : 1,
			BOCH_ch_D3 : 1,
			BOCH_ch_D4 : 1,
			BOCH_ch_D5 : 1,
			BOCH_ch_D6 : 1,
			BOCH_ch_D7 : 1;
	};
};

union ZOBWords
{
	quint16 zob_word;
	struct
	{
		quint16
			rez : 4,
			NZP : 1,
			FOS_OON : 1,
			BOCH_OON : 1,
			BOCH_OOFF : 1,
			AOS_OON : 1,
			UM_LBV : 1,
			LPch_ALPS : 1,
			PRK : 1,
			KAT : 1,
			KMT : 1,
			NKK : 1,
			ZAN : 1;

	};
};

union TM_BAU
{
	quint16 tm_data;
	struct
	{
		quint16
			VBAU_1 : 1,
			VBAU_2 : 1,
			VBAU_3 : 1,
			VPBAU_1 : 1,
			VPBAU_2 : 1,
			VPBAU_3 : 1,
			rez : 9;
	};
};

enum FOS_PS
{
	FOS_PS_ON = 0, // > -30 дБ
	FOS_PS_OFF = 1 // < -30 дБ
};

class AT_widg : public QWidget
{
	Q_OBJECT

public:
	AT_widg(QWidget *parent = 0);
	~AT_widg();
private:
	QMainWindow* main_widg;

	QList<QPushButton*> ALPS_pbut;
	QList<QPushButton*> MFS_pbut;
	QList<QPushButton*> LPch_pbut;
	QList<QPushButton*> BUFAR_pbut;

	QGridLayout *App_glay;
	QHBoxLayout *ALPS_hlay;
	QHBoxLayout *MFS_hlay;
	QHBoxLayout *LPch_hlay;
	QHBoxLayout *BUFAR_hlay;

	QGroupBox *ALPS_gb;
	QGroupBox *MFS_gb;
	QGroupBox *LPch_gb;
	QGroupBox *BUFAR_gb;
public:
	ALPS current_ALPS = ALPS_OFF;
	MFS current_MFS = MFS_OFF;
	LPch current_LPch = LPch_OFF;
	BUFAR current_BUFAR = BUFAR_OFF;

	void update_graphics_AT();
};

class MT_widg : public QWidget
{
	Q_OBJECT

public:
	MT_widg(QWidget *parent = 0);
	~MT_widg();

private:
	QMainWindow* main_widg;

	QList<QPushButton*> UM_pbut;
	QList<QPushButton*> LBV_pbut;
	QList<QPushButton*> BOCH_pbut;
	QList<QPushButton*> FOS_pbut;
	QList<QPushButton*> AOS_pbut;
	QList<QPushButton*> UPCH_pbut;
	QList<QPushButton*> PRM_Ant_1_pbut;
	QList<QPushButton*> PRM_Ant_A_pbut;
	QList<QPushButton*> SGTS_Ant_1_pbut;
	QList<QPushButton*> SGTS_Ant_A_pbut;

	QVBoxLayout *All_vlay;
	QVBoxLayout *All_BAU_vlay;
	QVBoxLayout *App_all_vlay;
	QGridLayout *App_glay;
	QHBoxLayout *App_PRM_SGTS_hlay;
	QHBoxLayout *UM_hlay;
	QHBoxLayout *LBV_hlay;
	QHBoxLayout *BOCH_hlay;
	QHBoxLayout *FOS_hlay;
	QHBoxLayout *AOS_hlay;
	QHBoxLayout *UPCH_hlay;
	QHBoxLayout *PRM_Ant_hlay;
	QHBoxLayout *PRM_Ant_1_hlay;
	QHBoxLayout *PRM_Ant_2_hlay;
	QHBoxLayout *SGTS_Ant_hlay;
	QHBoxLayout *SGTS_Ant_1_hlay;
	QHBoxLayout *SGTS_Ant_2_hlay;

	QWidget *App_gb;
	QGroupBox *UM_gb;
	QGroupBox *LBV_gb;
	QGroupBox *BOCH_gb;
	QGroupBox *FOS_gb;
	QGroupBox *AOS_gb;
	QGroupBox *UPCH_gb;
	QGroupBox *PRM_Ant_1_gb;
	QGroupBox *PRM_Ant_2_gb;
	QGroupBox *SGTS_Ant_1_gb;
	QGroupBox *SGTS_Ant_2_gb;
	QGroupBox *PRM_gb;
	QGroupBox *SGTS_gb;

public:
	UM current_UM = UM_OFF;
	LBV current_LBV = LBV_OFF;
	PRM current_PRM_Ant_1 = PRM_Ant_OFF;
	PRM current_PRM_Ant_A = PRM_Ant_OFF;
	SGTS current_SGTS_Ant_1 = SGTS_Ant_OFF;
	SGTS current_SGTS_Ant_A = SGTS_Ant_OFF;
	BOCH current_BOCH = BOCH_OFF;
	FOS current_FOS = FOS_OFF;
	AOS current_AOS = AOS_OFF;
	UPCH current_UPCH = UPCH_OFF;
	FOS_PS current_PS = FOS_PS_OFF;

	qint16 current_FOS_att1;
	qint16 current_FOS_att2;
	qint16 current_UPCH_att;
	BOCH_ch_com current_BOCH_ch;

	void update_graphics_MT();
};

class BAU_widg : public QWidget
{
	Q_OBJECT

public:
	BAU_widg(QWidget *parent = 0);
	~BAU_widg();

private:
	/// -- Главное окно;
	QMainWindow* main_widg;

	MK current_MK = MK_OFF;
	MT current_MT = MT_OFF;
	MU current_MU = MU_OFF;

	ZOBWords ZOB;
	ZTK_DataWords ZTK;
	ZTM_DataWords ZTM;

	QList<QPushButton*> MK_pbut;
	QList<QPushButton*> MT_pbut;
	QList<QPushButton*> MU_pbut;

	QVBoxLayout *All_vlay;
	QVBoxLayout *All_BAU_vlay;
	QHBoxLayout *MU_hlay;
	QHBoxLayout *MT_hlay;
	QHBoxLayout *MK_hlay;
	QHBoxLayout *MTMK_hlay;

	QGroupBox *MU_gb;
	QGroupBox *MT_gb;
	QGroupBox *MK_gb;
	QWidget *BAU_gb;

	const int MKO = 1;
	const int adr = 12;

	void imit_on();
	void imit_off();
	void update_graphics_BAU();
	void K_BRTK_M(QVariantList words);
	void K_BRTK_A(QVariantList words);
	void K_BAU(QVariantList words);
	void K_CBK();
	void ZTM_create();
	void ZTK_create();
	void set_tm_state();
	void set_power_back();
	void change_power();

	bool kcbk = false;
	bool ruk = false;
signals:
	void make_mt_at_state(int dev_name, int state);
protected:
	public slots :
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void get_power(double volt);

private:
	//void msg_to_log(const QString& _msg);

	quint16 sum_bits(Serv_Union& bits);

	TM_BAU tm_data;
	QTextEdit* edit;
	QScrollBar* _scroll_bar;
	QTextDocument* _doc;
	QTextCursor* _cursor;
	QString log_filename;
	QCheckBox* auto_scroll_box;
	bool auto_scroll;
	QTimer log_timer;
	QStringList log_buffer;
	QMutex log_mutex;
	QTimer *K_CBK_timer;
	QTimer *AbOn_tmr;
	QTimer *Power_tmr;
	QSettings LKA_sett;

	int volt;
	int power = 0;
	int bus = 0; //Узнать точно
	QString name = "БАУ";

	RPC_omnibus_SLOT_Thread omnibus_slot_thr;
	RPC_omnibus_SIGNAL_Thread omnibus_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	MT_widg MT_widget;
	AT_widg AT_widget;
};

#endif // BAU_H

