#ifndef BUFAR_H
#define BUFAR_H

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
#include "../buses_imitator/interrupt_bus_rpc.h"
#include "../buses_imitator/power_bus_rpc.h"

enum BUFAR_MODEs
{
	MODE_OFF = 0,
	MODE_1 = 1,
	MODE_2 = 2,
	MODE_3 = 3,
	MODE_4 = 4
};

union I_3_DataWords
{
	quint16 data_words;
	struct
	{
		quint16
			G1 : 1,
			G2 : 1,
			G3 : 1,
			G4 : 1,
			G5 : 1,
			//Gn : 5,
			V1 : 1,
			V2 : 1,
			V3 : 1,
			V4 : 1,
			V5 : 1,
			//Vn : 5,
			rez : 6;
	};
};

class BUM_current
{
public:
	bool chan_1 = false;
	bool chan_2 = false;
	bool chan_3 = false;
	bool chan_4 = false;
	bool power = false;
	int faze = 0;
};

class BUM_cl
{
public:
	QGroupBox* BUMs_gbox;
	QGridLayout* BUMs_glay;
	QList<QPushButton*> BUMs_pbut;
	void create_BUMs();
};

class BUFAR_widg : public QWidget
{
	Q_OBJECT

public:
	BUFAR_widg(QWidget *parent = 0);
	~BUFAR_widg();

private:
	QWidget* widg;
	/// -- Главное окно;
	QMainWindow* main_widg;
	//Хранимые текущие

	QList<BUM_cl> BUM_cl_list;

	QVBoxLayout *All_vlay;
	QVBoxLayout *BUFAR_cm_vlay;
	QHBoxLayout *BUFAR_compl_hlay;
	QHBoxLayout *BUFAR_mode_hlay;
	QGridLayout *BUMs_glay;

	QLabel *BUFAR_comp_lb;
	QLabel *BUFAR_mode_lb;
	QLineEdit *BUFAR_comp_le;
	QLineEdit *BUFAR_mode_le;

	int BUFAR_current = 0;
	BUFAR_MODEs BUFAR_mode = MODE_1;
	int BUFAR_sub_mode = 0;
	QList<BUM_current> BUM_current_list;
	int OY_faze = 0;
	int OZ_faze = 0;

	QPlainTextEdit* Logs;
	QLineEdit* Mode_le;
	const int MKO = 1;
	const int adr = 18;
	QPushButton * okBut;
	QDialog *dlg;
	bool ready_og;
	bool flag_on;

	void update_graphics();//обновление графики
	void set_new_tm();//составление ОК-ов
	void imit_off();//включение имитатора
	void imit_on();//выключение имитатора
	void set_power_back();//возврат силы тока в зависимости от мощности
	void I_1(QVariantList words);
	void I_3(QVariantList words);
	void I_4(QVariantList words);
	void I_5();
	void I_6(QVariantList words);
	void I_7();
	void I_8(QVariantList words);
	void I_9(int BUM_numb);
	void I_3_coord(int x, I_3_DataWords words);
	void faze_calc(int BUM_numb, int ind_faze, int chan);
	//void set_new_I_2();
	void change_BUMs(int num, bool ch_1, bool ch_2, bool ch_3, bool ch_4);
	void change_BUMs(int num, bool all_ch);
protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	void get_power(double volt);
	void new_mt_at_state(int dev_name, int state);
private:
	void msg_to_log(const QString& _msg);

	int volt;
	int power = 0;
	int bus = 0; //Узнать точно
	QString name = "БУФАР";

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

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;


};

#endif // BECH_H

