#ifndef BECH_H
#define BECH_H

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

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
#include "../buses_imitator/interrupt_bus_rpc.h"
#include "../buses_imitator/power_bus_rpc.h"

enum LKA
{
	LKA_1 = 0,
	LKA_2 = 1,
	LKA_OFF = 3
};
enum OG
{
	OG_1 = 0,
	OG_2 = 1,
	OG_3 = 2,
	OG_ERR = 3
};
enum FINIK
{
	FINIK_1 = 0,
	FINIK_2 = 1,
	FINIK_3 = 2,
	FINIK_ERR = 3
};
enum FINIK_REZH
{
	FINIK_REZH_PI15 = 0,
	FINIK_REZH_PI8 = 1,
	FINIK_REZH_ERR = 2
};
enum KP
{
	KP_1 = 0,
	KP_2 = 1,
	KP_OFF = 3
};
class BECH_widg : public QWidget
{
    Q_OBJECT

public:
	BECH_widg(QWidget *parent = 0);
	~BECH_widg();
	
private:
	QWidget* widg;
	/// -- Главное окно;
	QMainWindow* main_widg;
	//Хранимые текущие

	OG current_OG = OG_ERR;
	FINIK current_FINIK = FINIK_ERR;
	FINIK_REZH current_FINIK_REZH = FINIK_REZH_ERR;
	KP current_KP = KP_OFF;
	LKA current_LKA = LKA_OFF;

	QList<QPushButton*> LKA_pbut;
	QList<QPushButton*> OG_pbut;
	QList<QPushButton*> FINIK_pbut;
	QList<QPushButton*> KP_pbut;

	QVBoxLayout *All_vblay;
	QHBoxLayout *LKA_hlay;
	QHBoxLayout *FINIK_hlay;
	QHBoxLayout *OG_hlay;
	QHBoxLayout *KP_hlay;
	QHBoxLayout *FINIK_REZH_hlay;

	QGroupBox *LKA_gb;
	QGroupBox *FINIK_gb;
	QGroupBox *KP_gb;
	QGroupBox *OG_gb;

	QPlainTextEdit* Logs;
	QLineEdit* Mode_le;
	const int MKO = 1;
	const int adr = 5;
	QPushButton * okBut;
	QDialog *dlg;
	bool ready_og;
	bool flag_on;

	void update_graphics();//обновление графики
	void set_new_tm();//составление ОК-ов
	void omni_connect();//почключение к omnibus спустя n-ое количество времени
	void update_time();//Расчет времени прогрева ОГ в зависимости от времени включения и окончания прогрева
	void set_warm_og();//сохранение окончания прогревания и обьявление о прогретости/непрогретости
	void imit_off();//включение имитатора
	void imit_on();//выключение имитатора
	void change_power(bool switch_og);//изменение мощности
	void set_change_power();//передает в change_power параметр false, означающий, что функция вызвана таймером
	void set_power_back();//возврат силы тока в зависимости от мощности
protected:
	
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	void BECH_interrupt_setup();
	void BECH_interrupt_run();
	void get_power(double volt);
private:
	void msg_to_log(const QString& _msg);

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
	QTimer *inter_tmr;
	QTimer *warm_og_tmr;
	QTimer *AbOn_tmr;
	QTimer *Power_tmr;
	QSettings LKA_sett;

	QMap<int, QString> mode_names;
	QMap<OG, qint64> OG_start_warm;//Мап старта прогрева
	QMap<OG, qint64> OG_finish_warm;//Мап окончания прогрева

	int standart_tm = 120000;//стандартное время прогрева
	int tm_towarm;//текущее время прогрева, с растчетом прогретости
	int cooling_cof = 4;//коэфициент охлаждения (во сколько раз ОГ быстрее нагревается чем охлаждается)
	int power_vt = 0;//мощность
	int warm_er = 500;//погрешность нагрева
	int n;
	short chan;
	double u;
	double t;
	QString name = "БЭЧ";//Имя устройства
	int bus  = 2;//номер шины для шины питания(power_bus)
	int volt;//Принятое напряжение

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_interrupt_bus_SLOT_Thread interrupt_slot_thr;
	RPC_interrupt_bus_SIGNAL_Thread interrupt_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;

protected:
	void closeEvent(QCloseEvent *event);
};

#endif // BECH_H

