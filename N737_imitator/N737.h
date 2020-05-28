#ifndef N737_H
#define N737_H

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


#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>

enum class CURRENT_MKO_7
{
	MAIN = 1,
	RESERVE = 2,
	OFF = 3
};


enum class CURRENT_COMP_7
{
	MAIN = 1,
	RESERVE = 2,
	OFF = 3
};

enum chnl_7 {
	CHAN_R_BLOCK_1 = 21,
	CHAN_R_INCLUDE_1 = 19,
	CHAN_R_CONNECT_1 = 20,
	CHAN_R_WORK_FK_1 = 17,
	CHAN_R_WORK_1 = 18,

	CHAN_R_BLOCK_2 = 53,
	CHAN_R_INCLUDE_2 = 51,
	CHAN_R_CONNECT_2 = 52,
	CHAN_R_WORK_FK_2 = 49,
	CHAN_R_WORK_2 = 50,

	CHAN_R_READY_CU_1 = 26,
	CHAN_R_SET_BLOCK_1 = 32,
	CHAN_R_SET_INCLUDE_1 = 30,
	CHAN_R_SET_OK_1 = 28,
	CHAN_R_SET_CONNECT_1 = 31,
	CHAN_R_SET_WORK_FK_1 = 25,
	CHAN_R_SET_WORK_1 = 27,
	CHAN_R_PSP_1 = 29,

	CHAN_R_READY_CU_2 = 58,
	CHAN_R_SET_BLOCK_2 = 64,
	CHAN_R_SET_INCLUDE_2 = 62,
	CHAN_R_SET_OK_2 = 60,
	CHAN_R_SET_CONNECT_2 = 63,
	CHAN_R_SET_WORK_FK_2 = 57,
	CHAN_R_SET_WORK_2 = 59,
	CHAN_R_PSP_2 = 61
};


class N737_widg : public QWidget
{
    Q_OBJECT

public:
//	explicit LKA05_widg(QWidget *parent = 0);
	N737_widg();
	~N737_widg();
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

	bool isset(qulonglong x, qulonglong n);

	qulonglong num_chnl;

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	void set_tm_state();
	void paint_buttons();

	CURRENT_COMP_7 current_dev_7;
	CURRENT_MKO_7 current_mko_7;
	QList<int> in_lst, out_lst;
	bool kr;
	bool off_device;



	int MKO;
	int adr;
	int num_ku;
	QVariantList words;
	bool flag_on;

	//QCheckBox* add_set(QString name, QString data, bool is_main = true);

	QString name = "14Н737"; // русская н
	
	QTimer *AbOn_tmr;

protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os); 
	void dataIn(QVariantList dataList, QVariantList maskList);
signals:
	void new_ku(int ku_n, int length, double u, int line);


};

#endif // N737_H

