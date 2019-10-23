#ifndef R732_MODULES_H
#define R732_MODULES_H

#include <QMap>
#include <QTimer>

class R732_MODULE
{
public:
	virtual unsigned short get_tm() = 0;
};

enum class R732_CURRENT_DEV
{
	MAIN = 1,
	RESERVE = 2,
	OFF = 3
};

class R732_MU_MODULE : public R732_MODULE
{
public:
	R732_MU_MODULE();
	virtual unsigned short get_tm();
	void set_working(R732_CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == R732_CURRENT_DEV::OFF)
			return;

		working[_dev] = _flag;
	}
	void set_ab_working(R732_CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == R732_CURRENT_DEV::OFF)
			return;

		ab_working[_dev] = _flag;
	}
	bool get_working()
	{
		return working[R732_CURRENT_DEV];
	}
	bool get_ab_working()
	{
		return ab_working[R732_CURRENT_DEV];
	}
	void switch_cur_dev(R732_CURRENT_DEV _dev)
	{
		//if (_dev == OFF)
		//	return;
		R732_CURRENT_DEV = _dev;
	}
	R732_CURRENT_DEV get_R732_CURRENT_DEV()
	{
		return R732_CURRENT_DEV;
	}
private:
	QMap<R732_CURRENT_DEV, bool> working;
	QMap<R732_CURRENT_DEV, bool> ab_working;
	R732_CURRENT_DEV R732_CURRENT_DEV;
};


struct R732_MV_DEV
{
	double u_kom = 27.0;
	int dt_kom = 1;
	int length_kom = 200;
	bool working = true;
};

class R732_MV_MODULE : public R732_MODULE
{
public:
	R732_MV_MODULE(int _com, int _nim);
	virtual unsigned short get_tm();
	virtual unsigned short get_data_mvku();
	bool get_working()
	{
		return devices[R732_CURRENT_DEV].working;
	}
	void set_working(R732_CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == R732_CURRENT_DEV::OFF)
			return;

		devices[_dev].working = _flag;
	}
	void switch_cur_dev(R732_CURRENT_DEV _dev)
	{
		R732_CURRENT_DEV = _dev;
	}
	R732_CURRENT_DEV get_R732_CURRENT_DEV()
	{
		return R732_CURRENT_DEV;
	}
	R732_MV_DEV& get_settings()
	{
		return devices[R732_CURRENT_DEV];
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
	QMap<R732_CURRENT_DEV, R732_MV_DEV> devices;
	R732_CURRENT_DEV R732_CURRENT_DEV;
	int com;
	int nim;

	int ku_m = -1;
	int ku_p = -1;
};

class R732_VCHM_MODULE
{
public:
	R732_VCHM_MODULE();
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
		return working[VCHM_CHANEL(chanel)];
	}
private:
	QMap<VCHM_CHANEL, bool> working;

};

#endif // R732_MODULES_H

