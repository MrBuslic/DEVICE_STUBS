#ifndef LKA05_MODULES_H
#define LKA05_MODULES_H

#include <QDialog>
#include <QMap>
#include <QXmlStreamReader>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>


class LKA05_MODULE
{
public:
	virtual unsigned short get_tm() = 0;
};

enum class LKA05_CURRENT_DEV
{
	MAIN = 1,
	RESERVE = 2,
	OFF = 3
};

class LKA05_MU_MODULE : public LKA05_MODULE
{
public:
	LKA05_MU_MODULE();
	virtual unsigned short get_tm();
	void set_working(LKA05_CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == LKA05_CURRENT_DEV::OFF)
			return;

		working[_dev] = _flag;
	}
	void set_ab_working(LKA05_CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == LKA05_CURRENT_DEV::OFF)
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
	void switch_cur_dev(LKA05_CURRENT_DEV _dev)
	{
		//if (_dev == LKA05_CURRENT_DEV::OFF)
		//	return;
		current_dev = _dev;
	}
	LKA05_CURRENT_DEV get_current_dev()
	{
		return current_dev;
	}
private:
	QMap<LKA05_CURRENT_DEV, bool> working;
	QMap<LKA05_CURRENT_DEV, bool> ab_working;
	LKA05_CURRENT_DEV current_dev;
};


struct LKA05_MV_DEV
{
	double u_kom = 27.0;
	int dt_kom = 1;
	int length_kom = 200;
	bool working = true;
};

class LKA05_MV_MODULE : public LKA05_MODULE
{
public:
	LKA05_MV_MODULE(int _com, int _nim);
	virtual unsigned short get_tm();
	virtual unsigned short get_data_mvku();
	virtual unsigned short get_data_mvmk();
	bool get_working()
	{
		return devices[current_dev].working;
	}
	void set_working(LKA05_CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == LKA05_CURRENT_DEV::OFF)
			return;

		devices[_dev].working = _flag;
	}
	void switch_cur_dev(LKA05_CURRENT_DEV _dev)
	{
		current_dev = _dev;
	}
	LKA05_CURRENT_DEV get_current_dev()
	{
		return current_dev;
	}
	LKA05_MV_DEV& get_settings()
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
	QMap<LKA05_CURRENT_DEV, LKA05_MV_DEV> devices;
	LKA05_CURRENT_DEV current_dev;
	int com;
	int nim;

	int ku_m = -1;
	int ku_p = -1;
};

#endif // LKA05_H

