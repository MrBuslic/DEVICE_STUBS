#ifndef RPC_IS4_H
#define RPC_IS4_H
#ifdef WIN32
#include "winsock2.h"
#endif
#include <QObject>
#include <QString>
#include <QWidget>
#include <QThread>
#include <QTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QCheckBox>
#include <QTimer>
#include <QMutex>
#include <QLineEdit>
#include <QTime>
#include <memory>
#include <qlayout.h>
#include "../buses_imitator/mku_bus_rpc.h"
#include "rpc_loger.h"


class RpcIS4Widget : public QWidget
{
	Q_OBJECT
public:
	RpcIS4Widget(int is4_num);
public slots:

	void is4_clicked(int state_is4);

	int unis4_SetTypeProcess(uint EProcess);
	int unis4_StartACP();
	int unis4_ResultMeas(double& ResMeas, uint& NumRes);
	int unis4_RangeMeas(uint range);
	int unis4_StartCalibr();

private:
	QLineEdit* edit_number;
	QCheckBox* n_box;
	LogWidget* log_widget;
	int _process;
	double res_meas;
	int _range;
	bool is4_line;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;
signals:
	void is4_measure(uint NProcess, QVariant& value);

};

#endif