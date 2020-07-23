#ifndef SORENSEN_H
#define SORENSEN_H
#ifdef WIN32
#include "winsock2.h"
#endif
#include <QObject>
#include <QString>
#include <QWidget>
#include <QThread>
#include <QTextEdit>
#include <QScrollBar>
#include <QCheckBox>
#include <QTimer>
#include <QMutex>
#include <QLineEdit>
#include <QLabel>
#include <QTcpSocket>
#include <QTcpServer>
#include <memory>
#include <qlayout.h>
#include "../UNKP50/kp50_rpc.h"

class SORENSENWidget : public QWidget
{
	Q_OBJECT
public:
	SORENSENWidget();
public slots:
	void tcp_slot();
	void read_data();
	void update_graphics();
private:
	QTcpServer* server;
	QTcpSocket* socket;

	QLabel* u_label;
	QLabel* i_label;
	QLabel* state_label;
	QLabel* u_meas_label;
	QLabel* i_meas_label;

	double u;
	double i;
	bool state;

	double u_meas;
	double i_meas;
	void calc_meas();
	
	RPC_kp50_SLOT_Thread kp50_slot_thr;
	RPC_kp50_SIGNAL_Thread kp50_signal_thr;
signals:
	void update_graphics_signal();
	void power_out(int n, double _u);
protected:
	void closeEvent(QCloseEvent *event);
};

#endif //SORENSEN_H