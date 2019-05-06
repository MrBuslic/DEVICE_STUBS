#ifndef R732_MODULES_H
#define R732_MODULES_H

#include <QMap>
#include <QTimer>

class R732_MODULE
{
public:
	virtual unsigned short get_tm() = 0;
};

enum CURRENT_DEV
{
	MAIN = 1,
	RESERVE = 2,
	OFF = 3
};

class MU_MODULE : public R732_MODULE
{
public:
	MU_MODULE();
	virtual unsigned short get_tm();
	void set_working(CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == OFF)
			return;

		working[_dev] = _flag;
	}
	void set_ab_working(CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == OFF)
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
	void switch_cur_dev(CURRENT_DEV _dev)
	{
		//if (_dev == OFF)
		//	return;
		current_dev = _dev;
	}
	CURRENT_DEV get_current_dev()
	{
		return current_dev;
	}
private:
	QMap<CURRENT_DEV, bool> working;
	QMap<CURRENT_DEV, bool> ab_working;
	CURRENT_DEV current_dev;
};


struct MV_DEV
{
	double u_kom = 27.0;
	int dt_kom = 1;
	int length_kom = 200;
	bool working = true;
};

class MV_MODULE : public R732_MODULE
{
public:
	MV_MODULE(int _com, int _nim);
	virtual unsigned short get_tm();
	virtual unsigned short get_data_mvku();
	bool get_working()
	{
		return devices[current_dev].working;
	}
	void set_working(CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == OFF)
			return;

		devices[_dev].working = _flag;
	}
	void switch_cur_dev(CURRENT_DEV _dev)
	{
		current_dev = _dev;
	}
	CURRENT_DEV get_current_dev()
	{
		return current_dev;
	}
	MV_DEV& get_settings()
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
	QMap<CURRENT_DEV, MV_DEV> devices;
	CURRENT_DEV current_dev;
	int com;
	int nim;

	int ku_m = -1;
	int ku_p = -1;
};

class VCHM_MODULE
{
public:
	VCHM_MODULE();
	enum VCHM_CHANEL
	{
		VCHM0 = 0,
		VCHM1,
		VCHM2,
		VCHM3
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
private:
	QMap<VCHM_CHANEL, bool> working;
	QMap<VCHM_CHANEL, bool> proc_working;
};

#endif // R732_MODULES_H

