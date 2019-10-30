#ifndef R733_MODULES_H
#define R733_MODULES_H

#include <QMap>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>


enum class REGIME
{
	ERR = -1,
	PI15 = 0,
	PI8 = 1,
	VTF = 2,
	RBK = 5,
	
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

//enum STATE_UPI
//{
//	MAIN = 1,
//	OFF = 2
//};

//class UPI_MODULE : public R733_MODULE
//{
//public:
//	UPI_MODULE();
//	virtual unsigned short get_tm();
//	void set_working(R733_CURRENT_DEV _dev, bool _flag)
//	{
//		if (_dev == OFF)
//			return;
//
//		working[_dev] = _flag;
//	}
//	void set_ab_working(R733_CURRENT_DEV _dev, bool _flag)
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
//	void switch_cur_dev(R733_CURRENT_DEV _dev)
//	{
//		if (_dev == OFF)
//			return;
//		current_dev = _dev;
//	}
//	R733_CURRENT_DEV get_current_dev()
//	{
//		return current_dev;
//	}
//private:
//	QMap<R733_CURRENT_DEV, bool> working;
//	QMap<R733_CURRENT_DEV, bool> ab_working;
//	REGIME regime_UPI;
//};


class UPI_MODULE 
{
public:
	UPI_MODULE();
	
	enum UPI_CHANEL
	{
		UPI_0 = 0,
		UPI_1,
		UPI_2,
		UPI_3
	};
	void set_working_channels(QList<int> chanels_state, bool can_on = false);

	bool get_working(int channel)
	{
		return working[UPI_CHANEL(channel)];
	}
	void set_work_channel(UPI_CHANEL _dev)
	{
		work_channel = _dev;
	}
	void set_control_channel(UPI_CHANEL _dev)
	{
		work_channel = _dev;
	}

	void set_working(UPI_CHANEL _dev, bool _flag)
	{
		working[_dev] = _flag;
	}

	UPI_CHANEL get_work_channel()
	{
		return work_channel;
	}
	UPI_CHANEL get_control_channel()
	{
		return control_channel;
	}

private:
	UPI_CHANEL work_channel;
	UPI_CHANEL control_channel;
	QMap<UPI_CHANEL, bool> working;

};

#endif // R733_MODULES_H

