#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QString>
#include <QPushButton>
#include <QLayout>
#include <QFileDialog>
#include <QFile>
#include <QThread>
#include <QProgressBar>
#include <QLabel>
#include <QFrame>
#include <QPixmap>
#include <QMainWindow>
#include <QVariant>
#include "mbk04Modules.h"
#include "../../lib/frame_converter/FrameDataIncluder.h"
#include "../buses_imitator/frame_bus_rpc.h"
#include "../buses_imitator/power_bus_rpc.h"

enum REZH_BUF_ZAP
{
	BUF1 = 1,
	BUF2 = 2

};

class MainWidget :
	public QWidget
{
	Q_OBJECT
public:
	MainWidget();
	~MainWidget();
	
private:
	QWidget* widg;
	
	QMainWindow* main_widg;
	
	QPushButton* o_rez_btn;
	
	QPushButton* r_rez_btn;
	
	QPushButton* ik15_btn;
	
	QPushButton* ik8_btn;
	
	QPushButton* vtf_btn;
	QString name = "лай-04";
	int bus = 1;
	bool power_flag;
	static const int MKO = 1;
	static const int adr = 4;
	int flag;
	QMap<CURRENT_DEV, MV_DEV> devices;
	CURRENT_DEV current_dev;
	REZH_FRAME current_rezh;
	QString current_mode;
	QByteArray *buffer1, *buffer2;
	FrameDataIncluder data_includer;
	QByteArray frame;
	QByteArray clean_frame;
	QTimer * timer;
	QTime str_num_timer;
	int frame_str_count;
	int frame_per_msec;
	void imit_on();
	void imit_off();

private:
	void new_SCHBK(QVariantList words);
	void clean_frame_data(QString REZH);
	void new_kvit(int _kvit, int line_num);


	RPC_frame_bus_SLOT_Thread frame_slot_thr;
	RPC_frame_bus_SIGNAL_Thread frame_signal_thr;
	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;

private slots:
	void state_changed();
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_ku(int ku_n, int length, double u);
	void send_frame();
	void get_power(double volt);

signals:
	void new_tm(int nw);
	void state_changed_signal();

};

#endif

