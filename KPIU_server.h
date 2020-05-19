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
#include "rpc_foi.h"
#include "rpc_ols.h"
#include "KPRD_imitator.h"
#include "rpc_kp50.h"
#include "rpc_mfsk24.h"
#include "rpc_mds32.h"
#include "rpc_ads128.h"
#include "rpc_mn8i.h"
#include "rpc_vvk4.h"
#include "rpc_is4.h"
#include "RM_MBK07_imitator.h"
#include "rpc_mkprm.h"
#include "sorensen.h"
#include "BECH.h"
#include "N6705.h"
#include "mbk04Widget.h"
#include "MBK02.h"
#include "R732.h"
#include "R733.h"
#include "BOOP.h"
#include "MBK07.h"
#include "ASN.h"
#include "LKA05.h"
#include "LKA-05_imitator\lka05_rpc.h"

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
	KPIUServer(QWidget* parent = 0, QString platform = "mca");
public slots:
	//mku_bus_setup
	int KU_NASTROYKA_CELOSTNOSTI_KANALOV(int ku_n, int line);
	int MSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int mshm, int line_m);
	int PSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int pshm, int line_p);

	//unfoi_setup
	int FOI_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan);

	//KPRD_setup
	int ANTENNA_USTANOVKA_KOMMUTACII(QString antenna_name, QString connected_antenna_name);

	//OMNIBUS_setup
	int OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan);

	//power_bus_setup
	int USTANOVIT_SOSTOYANIE_SHINI_PITANIYA(int bus, int state);

	//pyro
	int PYRO_USTANOVIT_SOSTOYANIE(QString name, int state);

	//LKA05
	int LKA05_KU_USTANOVIT_SOSTOYANIE(int module_num, int _dev, bool _flag);
	int LKA05_MK_USTANOVIT_SOSTOYANIE(int module_num, int _dev, bool _flag);
	int LKA05_MPVN_USTANOVIT_SOSTOYANIE(int _dev, bool _flag);
	int LKA05_MU_USTANOVIT_SOSTOYANIE(int _dev, bool _flag);

	QString getXML();
	

	void mfsk_1_impulse(QVariantList channels);

	void mds_1_get_sample(uint& buf, bool& flag);
	void mds_2_get_sample(uint& buf, bool& flag);
	void get_resistance(uint NProcess, QVariant& resistance);
	int get_connection_state(QVariantList& _chans);

private:
	MDS1_CHANNELS mds1_chans;
	MDS2_CHANNELS mds2_chans;

	RPC_lka05_SLOT_Thread lka05_slot_thr;
	RPC_lka05_SIGNAL_Thread lka05_signal_thr;

	QMap<QString, pyro_chan_state> pyro_state;

	QList<int> bau_chans;
	int bau_ground = 147;

	QTabWidget* tab_widget;
	
	RpcOmnibusWidget* omnibus_widget;
	InterruptWidget* interrupt_widget;
	KPIWidget* kpi_widget;
	MKUWidget* mku_widget;
	FrameBusWidget* frame_widget;
	PowerWidget* power_widget;
	RpcFoiWidget* foi_widget;
	KPRD_imitator* kprd_widget;
	RpcKP50Widget* kp50_widget;
	RpcOlsWidget* ols_0_widget;
	RpcOlsWidget* ols_1_widget;
	RpcMDS32Widget* mds32_0_widget;
	RpcMDS32Widget* mds32_1_widget;
	RpcMFSK24Widget* mfsk24_0_widget;
	RpcMFSK24Widget* mfsk24_1_widget;
	RpcMFSK24Widget* mfsk24_2_widget;
	RpcMFSK24Widget* mfsk24_3_widget;
	RpcMFSK24Widget* mfsk24_4_widget;
	RpcMFSK24Widget* mfsk24_5_widget;
	RpcMFSK24Widget* mfsk24_6_widget;
	RpcMFSK24Widget* mfsk24_7_widget;
	RpcMFSK24Widget* mfsk24_8_widget;
	RpcADS128Widget* ads128_0_widget;
	RpcADS128Widget* ads128_1_widget;
	RpcMN8IWidget* mn8i_widget;
	RpcVvk4Widget* vvk4_widget;
	RpcIS4Widget* is4_widget;
	RM_MBK07_imitator* rm_mbk07_widget;
	RpcMKPRMWidget* mkprm_widget;
	SORENSENWidget* sorensen_widget;
	N6705Widget* n6705_widget;

	RPC_vvk4_SLOT_Thread* vvk4_slot_thr;

signals:
	
	void string_msg(QString _msg);
	void int_msg(int _msg);
	void int_return_signal(int& ret_int);
	/**
	*	\brief Асинхронный сигнал, передающий клиенту сообщение в журнал
	*	\param message - текст сообщения
	*/
	void toLogs(QString message);
	void toProtocols(QString message);
};


#endif //KPIU_SERVER_H