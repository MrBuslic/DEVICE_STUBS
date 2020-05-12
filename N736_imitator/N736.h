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
//#include "LKAModules.h"

#include "../buses_imitator/kpi_bus_rpc.h"
#include "../OMNIBUSBOX/omnibus_rpc.h"
//#include "../mbk04_imitator/mbk04_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
//#include "../buses_imitator/power_bus_rpc.h"

#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>

enum class CURRENT_CONNECT
{
	MAIN = 1,

	OFF = 3
};

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
	CURRENT_CONNECT current_connect;

	QVariantList maskListTest;
	QVariantList dataListTest;

	int MKO;
	int adr;
	int num_ku;
	QVariantList words;
	bool flag_on;
	bool kr;
	int j;
	bool off_device;
	//QCheckBox* add_set(QString name, QString data, bool is_main = true);

	

	QString name = "14Н736"; // русская н
	QTimer *AbOn_tmr;

	void example_but();


protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void dataIn(QVariantList dataList, QVariantList maskList);
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
signals:
	void new_data();
	void test(QVariantList dataList, QVariantList maskList);
};

#endif // N736_H

