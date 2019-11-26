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
//!!!!!!!!!!!!!!!!!!!!!!!!!!mds1 и 2 наоборот должны быть!!!!!!!!!!!!!!!!!!!!!!!!!!

union MDS2_CHANNELS
{
	quint32 chans;
	struct
	{
		quint32 ONA_chans : 3,
			res1 : 13,
			chans_732 : 5,
			res2 : 11;
	};
};

union MDS1_CHANNELS
{
	quint32 chans;
	struct
	{
		quint32 CBK_chans : 9,
			res1 : 7,
			chans_733 : 5,
			res2 : 11;
	};
};


#include "rpc_vvk4.h"
#include "vvk4_rpc.h"

struct pyro_chan_state
{
	pyro_chan_state(QList<int> _chans, int _state) : chan_num(_chans), state(_state)	{}//to do передать параметр _chans в форме pair
	QList<int> chan_num;
	int state;
};


class KPIUServer : public QWidget
{
	Q_OBJECT
public:
	KPIUServer(QWidget* parent = 0);
public slots:
	//mku_bus_setup
	int KU_NASTROYKA_CELOSTNOSTI_KANALOV(int ku_n, int line);
	int MSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int mshm, int line_m);
	int PSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int pshm, int line_p);

	//unfoi_setup
	int FOI_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan);

	//unols_setup
	int OLS_ZAPIS_DANNIH_FORMIROVANIYA(QVariantList data_buffer, QVariantList mask_buffer);
	int OLS_ZAPIS_DANNIH_REGISTRACII(QVariantList data_buffer);
	int OLS_BISTRIY_START(int devise);
	void OLS_CHTENIE_DANNICH_REGISTRACII(QVariantList& data_buffer);

	//KPRD_setup
	void ANTENNA_USTANOVKA_KOMMUTACII(QString antenna_name, QString connected_antenna_name);

	//OMNIBUS_setup
	int OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan);

	//power_bus_setup
	void USTANOVIT_SOSTOYANIE_SHINI_PITANIYA(int bus, int state);

	//pyro
	void PYRO_USTANOVIT_SOSTOYANIE(QString name, int state);
	QString getXML();
	

	void mfsk_1_impulse(QVariantList channels);

	void mds_1_get_sample(uint& buf, bool& flag);
	void mds_2_get_sample(uint& buf, bool& flag);
	void get_resistance(uint NProcess, int& resistance);
	int get_connection_state(QVariantList& _chans);
private:

	MDS1_CHANNELS mds1_chans;
	MDS2_CHANNELS mds2_chans;

	QMap<QString, pyro_chan_state> pyro_state;
	RpcOmnibusWidget* omnibus_widget;
	InterruptWidget* interrupt_widget;
	KPIWidget* kpi_widget;
	MKUWidget* mku_widget;
	FrameBusWidget* frame_widget;
	PowerWidget* power_widget;
	RpcFoiWidget* foi_widget;
	RpcOlsWidget* ols_widget;
	KPRD_imitator* kprd_widget;

	RPC_vvk4_SLOT_Thread* vvk4_slot_thr;

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