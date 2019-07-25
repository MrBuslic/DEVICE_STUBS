#ifndef KPIU_SERVER_H
#define KPIU_SERVER_H
#ifdef WIN32
#include "winsock2.h"
#endif
#include <QObject>
#include "mku_bus.h"
#include "OMNIBUSBOX.h"
#include "kpi_bus.h"
#include "mku_bus.h"
#include "interrupt_bus.h"
#include "frame_bus.h"
#include "power_bus.h"
#include "LKA05.h"
#include "rpc_foi.h"
#include "rpc_ols.h"
#include "KPRD_imitator.h"

class KPIUServer : public QWidget
{
	Q_OBJECT
public:
	KPIUServer(QWidget* parent = 0);
public slots:
	//mku_bus_setup
	int ku_map_channels_setup(int ku_n, short line);
	int mshm_map_channels_setup(int mshm, short line_m);
	int pshm_map_channels_setup(int pshm, short line_p);

	//unfoi_setup
	int unfoi_map_channels_setup(int _n, short _chan);

	//unols_setup
	int unols_write_data_kf(QVariantList data_buffer, QVariantList mask_buffer);
	int unols_trigger_imm(int devise);
	void unols_read_data_kr(QVariantList& data_buffer);

	//KPRD_setup
	void set_antenna_connection(QString antenna_name, QString connected_antenna_name);

	//OMNIBUS_setup
	int unomnibus_map_channels_setup(int _n, short _chan);

	//power_bus_setup
	void set_bus_state(int bus, int state);

private:
	RpcOmnibusWidget* omnibus_widget;
	InterruptWidget* interrupt_widget;
	KPIWidget* kpi_widget;
	MKUWidget* mku_widget;
	FrameBusWidget* frame_widget;
	PowerWidget* power_widget;
	RpcFoiWidget* foi_widget;
	RpcOlsWidget* ols_widget;
	KPRD_imitator* kprd_widget;

signals:
	void string_msg(QString _msg);
	void int_msg(int _msg);
	void int_return_signal(int& ret_int);
	/**
	*	\brief Асинхронный сигнал, передающий клиенту сообщение в журнал
	*	\param message - текст сообщения
	*/
	void toLog(QString message);
	void toProtocol(QString message);

};


#endif //KPIU_SERVER_H