#pragma once

#include <qdialog.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qobject.h>
#include <qgridlayout.h>
#include <qtextcodec.h>
#include <qtimer.h>
#include <QTime>
#include <qsettings.h>
#include <QMessageBox>
#include <qcoreapplication.h>
#include <qfile.h>


#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
#include "../buses_imitator/interrupt_bus_rpc.h"
#include "../buses_imitator/power_bus_rpc.h"


enum SUBSET
{
	SUBSET_1 = 0,
	SUBSET_2 = 1,
	SUBSET_OFF = 3
};

enum ASN_MODE
{
	MAIN_WORK = 1,
	VECTOR_MODE = 2,
	WAIT_MODE = 5,
	TEST_MODE = 6
};

enum SYS_MODE
{
	EXPANDED_ASN = 0,
	SYS_INF = 1
};

enum NAV_SYS
{
	NAV_SYS_GL = 0,
	NAV_SYS_GPS = 1,
	NAV_SYS_GG = 2,
};

struct KU_DateDATA
{
	int		y;          /* год */
	int		m;          /* месяц */
	int		d;          /* день */
	int		h;          /* часы */
	int		min;        /* минуты */
	double	sec;        /* секунды */
};


class ASN_widg :public QWidget
{
	Q_OBJECT

public:
	ASN_widg(QWidget *parent = 0);
	~ASN_widg();

private:
	//QPushButton *io; //In/Out (Включить/Выключить)
	QPushButton *first_half;
	QPushButton *second_half;
	QPushButton *nw; //Normal work (Штатная работа)
	QPushButton *ts; //Test switch (Поверочное включение)
	QPushButton *jm_glonass; //Job mode GLONASS (Задание режима работы по ГЛОНАСС)
	QPushButton *jm_gps; //Job mode GPS (Задание режима работы по GPS)
	QPushButton *jm_gg; //Job mode GLONASS and GPS (Задание режима работы по ГЛОНАСС и GPS)
	QPushButton *n_hrg; //No heating RG (Команда "Отказ от прогрева ОГ")
	QPushButton *zacep; //Зацеп к спутнику

	QTextEdit *text; // Информация

	QString name = "ASN";//Имя устройства

	void EnableButton();

	private slots:
	void io_Clicked();
	void nw_Clicked();
	void ts_Clicked();
	void jm_glonass_Clicked();
	void jm_gps_Clicked();
	void jm_gg_Clicked();
	void n_hrg_Clicked();
	void zacep_Clicked();
	void set_new_tm_2();

	void heating_OG();//сохранение окончания прогревания и обьявление о прогретости/непрогретости

	void get_power(double volt);
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);

private:

	QSettings sett;
	SUBSET current_SUBSET = SUBSET_OFF;
	NAV_SYS nav_SYS = NAV_SYS_GG;
	ASN_MODE _mode = WAIT_MODE;
	SYS_MODE sys_mode = EXPANDED_ASN;
	bool test; // Тест пройден/нет
	bool OG; // Прогрет/не прогрет
	bool radio; // Есть радиоконтакт
	bool dost;

	bool flag_on; // Вкл/выкл имитатор
	int volt; // Принятое напряжение
	QDateTime time; // Начало работы устройства

	int standart_tm = 10000;//2400000; // Стандартное время прогрева
	int tm_towarm;// Текущее время прогрева
	int cooling_cof = 80;//коэфициент охлаждения (во сколько раз ОГ быстрее нагревается чем охлаждается)
	int warm_er = 500;//погрешность нагрева
	int OG_start_warm = 0;//Старта прогрева
	int OG_finish_warm = 0;//Окончания прогрева
	void update_time();//Расчет времени прогрева ОГ в зависимости от времени включения и окончания прогрева
	QTimer *heat_og_tmr; // Прогрев

	const int MKO = 1;
	const int adr = 3; // Адрес ОУ
	void set_new_tm(int subadr);//составление ОК-ов

	QTimer *prer; // Прерывание

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_interrupt_bus_SLOT_Thread interrupt_slot_thr;
	RPC_interrupt_bus_SIGNAL_Thread interrupt_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;
	bool priznak_dost;

	double KA[4000][3];
	double VKA[4000][3];
	double M[3][3];
	void load_fact();
	int cycle_i = 0;

	int count = 1;
	bool a = true;

	QTimer *one;
	QTimer *two;

	KU_DateDATA SEV_time[4000];

	unsigned short sokr_time(QDateTime t, bool a);
	unsigned short od_plav(float f, int a);
protected:
	void closeEvent(QCloseEvent *event);
};
