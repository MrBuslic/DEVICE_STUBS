#ifndef RPC_ADS128_H
#define RPC_ADS128_H
#ifdef WIN32
#include "winsock2.h"
#endif
#include <QObject>
#include <QString>
#include <QWidget>
#include <QThread>
#include <QTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QCheckBox>
#include <QTimer>
#include <QMutex>
#include <QLineEdit>
#include <QTime>
#include <memory>
#include <qlayout.h>
#include <loki/Singleton.h>

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class RpcADS128Widget : public QWidget
{
	Q_OBJECT
public:
	RpcADS128Widget(int slot_port, int signal_port);
public slots:

	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	void measurement_timer_ontimer();
	void infin_timer_ontimer();

	int unads128_start();
	int unads128_input_trigger(bool state);
	int unads128_sample_width_q(uint& frame_width, uint&  width_in_bytes);
	int unads128_read_sample(uint& _buf, uint& _firstTime, uint& _thisTime);
	int unads128_read_packet(bool isHot, uint numSamples, QVariantList& buf, uint& realNumSamples);
	int unads128_sample_period(double _periodS);
	int unads128_mode_cycle(uint _size);
	int unads128_num_ready_data(uint& _num);
	

	void button_clicked();
private:
	QTextEdit* edit;
	QScrollBar* _scroll_bar;
	QTextDocument* _doc;
	QTextCursor* _cursor;
	QPushButton* push_b; 
	QCheckBox* auto_scroll_box;
	bool auto_scroll;
	QString log_filename;
	QTimer log_timer;
	std::unique_ptr<QTimer> infin_timer;
	QTime begin_time;

	QStringList log_buffer;
	QMutex log_mutex;
	bool state;

	double periodS;
	uint samples;
	bool measuring;
	bool infinit;

	QList <QLineEdit*>  checks; 
	QLineEdit* buf_edit;
	QVariantList buffer;
	QMap<QObject*, int> buttons;
	double impulse_length;
signals:
	void packet_ready();
};

#endif