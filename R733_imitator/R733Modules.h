#ifndef R733_MODULES_H
#define R733_MODULES_H

#include <QMap>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>


class R733_MODULE
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

enum REGIME
{
	PI15 = 0,
	PI8 = 1,
	VTF = 2,
	RBK = 5
};
enum NUM_CHANNEL
{
	CHANNEL_1 = 0,
	CHANNEL_2,
	CHANNEL_3,
	CHANNEL_4
};
//enum NUM_CHANNEL
//{
//	WORK_CHANNEL = 0,
//	CONTROL_CHANNEL = 1,
//	FSCH_CHANNEL = 2,
//	MBK07_CHANNEL = 3
//};

enum MAJORITAR
{
	C_1_MAJOR =1,
	C_2_MAJOR = 1,
	C_3_MAJOR = 1,
	B_1_GSCH = 1,
	B_2_GSCH = 1,
	B_3_GSCH = 1,
	B_4_GSCH = 1
};
enum STATE_UPI
{
	VKL = 1,
	VIKL = 2
};


//class UPI_MODULE : public R733_MODULE
//{
//public:
//	UPI_MODULE();
//	virtual unsigned short get_tm();
//	void set_working(CURRENT_DEV _dev, bool _flag)
//	{
//		if (_dev == OFF)
//			return;
//
//		working[_dev] = _flag;
//	}
//	void set_ab_working(CURRENT_DEV _dev, bool _flag)
//	{
//		if (_dev == OFF)
//			return;
//
//		ab_working[_dev] = _flag;
//	}
//	bool get_working()
//	{
//		return working[current_dev];
//	}
//	bool get_ab_working()
//	{
//		return ab_working[current_dev];
//	}
//	void switch_cur_dev(CURRENT_DEV _dev)
//	{
//		if (_dev == OFF)
//			return;
//		current_dev = _dev;
//	}
//	CURRENT_DEV get_current_dev()
//	{
//		return current_dev;
//	}
//private:
//	QMap<CURRENT_DEV, bool> working;
//	QMap<CURRENT_DEV, bool> ab_working;
//	REGIME regime_UPI;
//};


class MU_MODULE : public R733_MODULE
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
	//	if (_dev == OFF)
	//		return;
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

class MV_MODULE : public R733_MODULE
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
	bool get_working(int chanel)
	{
		return working[VCHM_CHANEL(chanel)];
	}
private:
	QMap<VCHM_CHANEL, bool> working;
};



class UPI_MODULE 
{
public:
	UPI_MODULE();
	
	/*enum STATE_UPI
	{
		MAIN = 1,
		OFF = 2
	};*/
	void set_working_channels(QList<int> chanels_state, bool can_on = false);
	bool get_working(int channel)
	{
		return working[num_channel];
	}
	void switch_num_chan(STATE_UPI _dev)
	{
		num_channel = _dev;
	}

	void set_working(STATE_UPI _dev, bool _flag)
	{
		working[_dev] = _flag;
	}

	STATE_UPI get_current_dev()
	{
		return num_channel;
	}

private:
	STATE_UPI num_channel;
	QMap<STATE_UPI, bool> working;
};

#endif // R733_MODULES_H

