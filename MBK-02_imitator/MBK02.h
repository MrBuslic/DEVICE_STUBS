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

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../LKA-05_imitator/lka05_rpc.h"

enum CHANEL
{
	CHANEL_1 = 0,
	CHANEL_2 = 1,
	CHANEL_ERR = 2,
	CHANEL_OFF = 3
};
//enum ANTENNA_CH1
//{
//	MHA1PY = 0,
//	MHA1MY = 1,
//	MHA1OFF = 3
//};
//enum ANTENNA_CH2
//{
//	MHA2PY = 0,
//	MHA2MY = 1,
//	MHA2OFF = 3
//};
enum ANTENNA
{
	MHA1PY = 0,
	MHA1MY = 1,
	MHA2PY = 2,
	MHA2MY = 3,
	MHAOFF = 4
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

	int current_lit = 0;
	CHANEL current_chan = CHANEL_OFF;
//	ANTENNA_CH1 current_ant_ch1 = MHA1OFF;
//	ANTENNA_CH2 current_ant_ch2 = MHA2OFF;
	ANTENNA current_ant = MHAOFF;

	const int MKO = 1;
	const int adr = 2;
	QVariantList words;
	bool flag;
	QPushButton * okBut;
	QDialog *dlg;

	QVBoxLayout *All_vblayout;
	QGridLayout *All_glayout;
	QVBoxLayout *Chan1_vblayout;
	QVBoxLayout *Chan2_vblayout;
	QVBoxLayout *Ant_vblayout;
	QVBoxLayout *Lit_vblayout;

	QPushButton *Chan1_pbut;
	QPushButton *Chan2_pbut;
	QPushButton *Ant_pbut;
	QLineEdit* Lit_le;

	void paint_buttons();
	void write_words();
	void set_new_tm();
protected:

	public slots :
		void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt);
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
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
	void msg_to_log(const QString& _msg);

	QMap<int, QString> ant_names;

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_lka05_SLOT_Thread lka05_slot_thr;
	RPC_lka05_SIGNAL_Thread lka05_signal_thr;

};

#endif // MBK02_H

