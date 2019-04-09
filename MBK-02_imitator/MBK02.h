#ifndef MBK02_H
#define MBK02_H

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
#include <qplaintextedit.h>
#include <QTime>
#include <QTimer>

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
#include "../buses_imitator/kpi_bus_rpc.h"

enum CHANEL
{
	CHANEL_1 = 0,
	CHANEL_2 = 1,
	CHANEL_ERR = 2,
	CHANEL_OFF = 3
};
enum ANTENNA
{
	MHA1PY = 0,
	MHA1MY = 1,
	MHA2PY = 2,
	MHA2MY = 3,
	MHAOFF = 4
};
enum KPI_STATE
{
	LITER_PAUSE = 0x138CE20, 
	ZERO = 0xBB8,
	ONE = 0xFA0,
	STEP = 0x27100
};
class MBK02_widg : public QWidget
{
	Q_OBJECT

public:
	//	explicit LKA05_widg(QWidget *parent = 0);
	MBK02_widg(QWidget *parent = 0);
	//~MBK02_widg();


private:
	QWidget* widg;
	/// -- Главное окно;
	QMainWindow* main_widg;
	//Хранимые текущие

	int current_lit;
	CHANEL current_chan = CHANEL_OFF;
	ANTENNA current_ant = MHAOFF;

	const int MKO = 1;
	const int adr = 2;
	QVariantList words;
	bool flag;
	QPushButton * okBut;
	QDialog *dlg;

	QVBoxLayout *All_vblay;
	QGridLayout *All_glay;
	QHBoxLayout *Chan_hblay;
	QHBoxLayout *Sig_hblay;
	QVBoxLayout *Ant_vblay;
	QVBoxLayout *Lit_vblay;
	QGroupBox *Channels_gbox;
	QGroupBox *Signal_gbox;
	QGroupBox *Sup_gbox;

	QPushButton *Chan1_pbut;
	QPushButton *Chan2_pbut;
	QPushButton *Sig_pbut;
	QPushButton *Ant_pbut;
	QLineEdit* Lit_le;

	

public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void new_KPI(QVariantList KPI_list);
	void auto_scroll_clicked(int _state);
	void update_tm(int sadr);
	void log_timer_ontimer();
	void reverse_ant();
	void lose_cont();
	void update_graphics();
signals:
	void msg_to_14R732(QVariantList data);
	void set_new_tm(int sadr, int word);
	void emit_update_graphics();
private:
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
	QTimer *t_ant_ch;
	QTimer *t_sleep;
	QTimer *t_err_kpi;
	bool signal_con = false;
	QVariantList list_to_R14732;

	void msg_to_log(const QString& _msg);

	QMap<int, QString> ant_names;
	QMap<int, KPI_STATE> check_KPI;
	QVariantList tmp_list;

	//RPC_omnibus_SLOT_Thread slot_thr;
	//RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_kpi_bus_SLOT_Thread kpi_slot_thr;
	RPC_kpi_bus_SIGNAL_Thread kpi_signal_thr;

};

#endif // MBK02_H

