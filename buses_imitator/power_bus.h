#ifndef POWER_BUS_H
#define POWER_BUS_H

#include <QWidget>
#include <qmap.h>
#include <QPushButton>
#include "rpc_loger.h"

enum POWER_BUSES
{
	NK = 1,
	K1,
	K2
};

class PowerWidget : public QWidget
{
	Q_OBJECT

public:
	PowerWidget(QWidget *parent = 0);
public slots:
	void set_u(int bus, double volt);
	void get_i(int bus, double& curr);
	void set_i(int bus, QString name, double curr);
	void set_bus_state(int bus, int state);
	void set_bus_u(QString name, double volt);
private slots:
	void set_on();
	void set_off();
signals:
	void u_on_bus(QString name, double volt);
	void u_on_nk(double volt);
	void u_on_k1(double volt);
	void u_on_k2(double volt);

protected:
	void closeEvent(QCloseEvent *event);

private:
	double nk_volt;
	double k1_volt;
	double k2_volt;
	double nk_curr;
	double k1_curr;
	double k2_curr;

	QMap<QString, double> nk_curr_map;
	QMap<QString, double> k1_curr_map;
	QMap<QString, double> k2_curr_map;

	QMap<int, int> power_bus_state_map;//1 - work; 0 - break

	QPushButton* on_btn;
	QPushButton* off_btn;

	LogWidget* log_widget;
};

#endif // POWER_BUS_H

