#ifndef KPRD_IMITATOR_H
#define KPRD_IMITATOR_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QTextEdit>
#include <QScrollBar>
#include <QList>
#include <QMessageBox>

#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>

#include "../UNOLS/ols_rpc.h"
#include "../buses_imitator/kpi_bus_rpc.h"

class KPRD_imitator :
	public QMainWindow
{
	Q_OBJECT
public:
	KPRD_imitator();
	~KPRD_imitator();
	
private:
	QWidget* widg;
	
	QPushButton *pause_btn;
	QPushButton *zero_btn;
	QPushButton *one_btn;

	QLabel *generetors_label;
	QLabel *pause_kod_label;
	QLabel *zero_kod_label;
	QLabel *one_kod_label;
	QLabel *antenna_label;
	QLabel *attenuation_label;

	QTextEdit *log_edit;
	QScrollBar* _scroll_bar;

	QGridLayout *gridLayout;

	void set_antenna_label(qulonglong val);

	bool FREQ_P_CLOCK = false;
	bool FREQ_P_DATA = false;
	bool FREQ_P_ENABLE = true;
	bool FREQ_0_CLOCK = false;
	bool FREQ_0_DATA = false;
	bool FREQ_0_ENABLE = true;
	bool FREQ_1_CLOCK = false;
	bool FREQ_1_DATA = false;
	bool FREQ_1_ENABLE = true;

	bool KPI_P = false;
	bool KPI_0 = false;
	bool KPI_1 = false;
	bool KPI_R = false;

	int FREQ_P_code = 0;
	int FREQ_0_code = 0;
	int FREQ_1_code = 0;

	QString KPIString;

	bool isset(qulonglong x, qulonglong n);

	RPC_ols_SLOT_Thread ols_slot_thr;
	RPC_ols_SIGNAL_Thread ols_signal_thr;

	RPC_kpi_bus_SLOT_Thread kpi_slot_thr;
	RPC_kpi_bus_SIGNAL_Thread kpi_signal_thr;

signals:
	void test(QVariantList maskList, QVariantList dataList);
	void sendKPI(QString kpiList);

public slots:
	void dataIn(QVariantList maskList, QVariantList dataList);


};

#endif

