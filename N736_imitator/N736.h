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
//#include "LKAModules.h"

#include "../OMNIBUSBOX/omnibus_rpc.h"
//#include "../mbk04_imitator/mbk04_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
//#include "../buses_imitator/power_bus_rpc.h"

class N736_widg : public QWidget
{
    Q_OBJECT

public:
//	explicit LKA05_widg(QWidget *parent = 0);
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

	int MKO;
	int adr;
	int num_ku;
	QVariantList words;
	bool flag_on;

	//QCheckBox* add_set(QString name, QString data, bool is_main = true);

	//void paint_buttons();

	QString name = "14Н736"; // русская н

	int bus = 2;
	int power = 0;
	double volt;
	QTimer *AbOn_tmr;

protected:
	void closeEvent(QCloseEvent *event);
public slots:
	//void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os); 

signals:
	void new_ku(int ku_n, int length, double u, int line);

private:

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;
};

#endif // N736_H

