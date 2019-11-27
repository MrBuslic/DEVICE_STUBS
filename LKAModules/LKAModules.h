#ifndef LKAMODULES_H
#define LKAMODULES_H

#include <QObject>
#include <QDialog>
#include <QMap>
#include <QTimer>
#include <QXmlStreamReader>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>
#include <QVariantList>

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
#include "../buses_imitator/interrupt_bus_rpc.h"

#define VCHM_START_TIME 30000

class LKA_MODULE
{
public:
	virtual unsigned short get_tm() = 0;
};

enum class LKA_CURRENT_DEV
{
	MAIN = 1,
	RESERVE = 2,
	OFF = 3
};

class MU_MODULE : public LKA_MODULE
{
public:
	MU_MODULE();
	virtual unsigned short get_tm();
	void set_working(LKA_CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == LKA_CURRENT_DEV::OFF)
			return;

		working[_dev] = _flag;
	}
	void set_ab_working(LKA_CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == LKA_CURRENT_DEV::OFF)
			return;

		ab_working[_dev] = _flag;
	}
	bool get_working()
	{
		return working[current_dev];
	}
	bool get_ab_working()
	{
		return ab_working[current_dev];
	}
	void switch_cur_dev(LKA_CURRENT_DEV _dev)
	{
		//if (_dev == LKA05_CURRENT_DEV::OFF)
		//	return;
		current_dev = _dev;
	}
	LKA_CURRENT_DEV get_current_dev()
	{
		return current_dev;
	}
private:
	QMap<LKA_CURRENT_DEV, bool> working;
	QMap<LKA_CURRENT_DEV, bool> ab_working;
	LKA_CURRENT_DEV current_dev;
};


struct LKA_MV_DEV
{
	double u_kom = 27.0;
	int dt_kom = 1;
	int length_kom = 200;
	bool working = true;
};

class LKA_MV_MODULE : public LKA_MODULE
{
public:
	LKA_MV_MODULE(int _com, int _nim);
	virtual unsigned short get_tm();
	virtual unsigned short get_data_mvku();
	virtual unsigned short get_data_mvmk();
	bool get_working()
	{
		return devices[current_dev].working;
	}
	void set_working(LKA_CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == LKA_CURRENT_DEV::OFF)
			return;

		devices[_dev].working = _flag;
	}
	void switch_cur_dev(LKA_CURRENT_DEV _dev)
	{
		current_dev = _dev;
	}
	LKA_CURRENT_DEV get_current_dev()
	{
		return current_dev;
	}
	LKA_MV_DEV& get_settings()
	{
		return devices[current_dev];
	}
//	void set_settings()
//	{	
//	}
	void set_ku_m(int n)
	{
		ku_m = n;
	}
	void set_ku_p(int n)
	{
		ku_p = n;
	}

private:
	QMap<LKA_CURRENT_DEV, LKA_MV_DEV> devices;
	LKA_CURRENT_DEV current_dev;
	int com;
	int nim;

	int ku_m = -1;
	int ku_p = -1;
};



class LKA06_MODULE : public QWidget
{
	Q_OBJECT
public:

	LKA06_MODULE(int _adr);

	MU_MODULE mu_module;
	QList<LKA_MV_MODULE> mpvn_modules;
	QList<LKA_MV_MODULE> mvku_modules;

	unsigned short mko_counter;

	QVariantList get_mko_counter_word();
	QVariantList get_pups_words_list();
	QVariantList get_vchm_word();

	QVariantList get_tm();

	void new_message_mu(QVariantList words);

	void restart_vchm_proc(int chanel);
	bool need_mvku_renew;
	QList<int> vchm_chanels_init;

	virtual void paint_buttons() = 0;
	virtual void set_new_tm() = 0;

	enum VCHM_CHANEL
	{
		VCHM0 = 0,
		VCHM1,
		VCHM2,
		VCHM3
	};

	enum VCHM_PO
	{
		PO_732 = 0,
		PO_733
	};

	void set_working_chanels(QList<int> chanels_state, bool can_on = false);
	void set_working_proc(int chanel, int state);
	bool get_working(int chanel)
	{
		return working[VCHM_CHANEL(chanel)];
	}
	bool get_proc_working(int chanel)
	{
		return proc_working[VCHM_CHANEL(chanel)];
	}
	QTimer po_start;
	QTimer mu_on_timer;
	int PUPS;


	void lka_imit_on();
	void lka_imit_off();

	int MKO;
	int adr;


	RPC_omnibus_SLOT_Thread omni_slot_thr;
	RPC_omnibus_SIGNAL_Thread omni_signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_interrupt_bus_SLOT_Thread interrupt_slot_thr;
	RPC_interrupt_bus_SIGNAL_Thread interrupt_signal_thr;

	VCHM_PO cur_po;


	LKA_CURRENT_DEV mu_dev_to_set;

public slots:
	void po_start_timeout();
	void set_mu_on();

private:
	QMap<VCHM_CHANEL, bool> working;
	QMap<VCHM_CHANEL, bool> proc_working;
};


#endif // LKAModules

