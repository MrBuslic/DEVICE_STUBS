#ifndef KPRD_IMITATOR_H
#define KPRD_IMITATOR_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QTextEdit>
#include <QScrollBar>
#include <QList>

#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>


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
	bool FREQ_P_ENABLE = false;
	bool FREQ_0_CLOCK = false;
	bool FREQ_0_DATA = false;
	bool FREQ_0_ENABLE = false;
	bool FREQ_1_CLOCK = false;
	bool FREQ_1_DATA = false;
	bool FREQ_1_ENABLE = false;

	bool KPI_P = false;
	bool KPI_0 = false;
	bool KPI_1 = false;
	bool KPI_R = false;

	int FREQ_P_code = 0;
	int FREQ_0_code = 0;
	int FREQ_1_code = 0;

	QString KPIString;

	bool isset(qulonglong x, qulonglong n);

signals:
	void test(QVariantList maskList, QVariantList dataList);
	void sendKPI(QString kpiList);

public slots:
	void dataIn(QVariantList maskList, QVariantList dataList);


};

#endif

