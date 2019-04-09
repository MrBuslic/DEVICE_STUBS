#ifndef BOOP_H
#define BOOP_H

#include <qwidget.h>
#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qgroupbox.h>

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"

class BOOP : public QWidget
{
	Q_OBJECT
public:
	BOOP();
	~BOOP();
	void new_data(int mko, int addr, int saddr, QVariantList words);

private:
	// Main Window
	QMainWindow* mainWindow;

	// Main widget
	QWidget* widget;

	// Temperature indicator
	QLineEdit *temperatureEdit;

	// Hals-set buttons
	QPushButton *mainSetButton;
	QPushButton *reserveSetButton;

	// Self-control checkboxes
	QCheckBox *bit19;
	QCheckBox *bit18;
	QCheckBox *bit17;
	QCheckBox *bit16;
	QCheckBox *bit15;
	QCheckBox *bit14;
	QCheckBox *bit13;
	QCheckBox *bit12;
	QCheckBox *bit4;

	// Uplilon channel servo controls
	QLabel *upsilonAngleValue;
	QCheckBox *upsilonAngleSensorPower;
	QCheckBox *upsilonAngleServoPower;
	QPushButton *increaseUpsilonAngle;
	QPushButton *decreaseUpsilonAngle;

	// Phi channel servo controls
	QLabel *phiAngleValue;
	QCheckBox *phiAngleSensorPower;
	QCheckBox *phiAngleServoPower;
	QPushButton *increasePhiAngle;
	QPushButton *decreasePhiAngle;

	// Log area
	QTextEdit *logArea;

public slots:
	 void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
private:
	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SLOT_Thread mku_signal_thr;
};

#endif
