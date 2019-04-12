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
	/// -- √лавное окно;
	QMainWindow* main_widg;
	//’ранимые текущие

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
	bool flag;
	bool ready_og;

	void update_graphics();
	void set_new_tm();
	void omni_connect();
	void update_time();
	void set_warm_og();
	void imit_off();
	void imit_on();
	void change_power(bool switch_og);
	void set_change_power();
	void set_power_back();
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

	QMap<int, QString> mode_names;
	QMap<OG, qint64> OG_start_warm;
	QMap<OG, qint64> OG_finish_warm;

	int standart_tm = 120000;
	int tm_towarm;
	int cooling_cof = 4;
	int power = 0;
	int warm_er = 500;//погрешность нагрева
	int n;
	short chan;
	double u;
	double t;
	QString name = "ЅЁ„";
	int bus  = 1;
	int _volt;

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_interrupt_bus_SLOT_Thread interrupt_slot_thr;
	RPC_interrupt_bus_SIGNAL_Thread interrupt_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;
};

#endif // BECH_H

