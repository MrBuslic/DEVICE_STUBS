#include "rpc_ads128.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "ads128_socket_rpc.h"
#include "rpc_ports.h"
#include <qmessagebox.h>

RpcADS128Widget::RpcADS128Widget(int _ads_num) : QWidget(), auto_scroll(true),  state(false), ads_num(_ads_num)
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	edit = new QTextEdit(this);
	_scroll_bar = edit->verticalScrollBar();
	_doc = new QTextDocument();
	_cursor = new QTextCursor(_doc);
	edit->setDocument(_doc);
	edit->setReadOnly(true);
	_doc->setMaximumBlockCount(1000);
	setMinimumSize(490, 300);
	auto_scroll_box = new QCheckBox(this);
	auto_scroll_box->setText("Автопрокрутка");
	auto_scroll_box->setChecked(true);
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &RpcADS128Widget::auto_scroll_clicked);
	v_lay->addWidget(edit);
	v_lay->addWidget(auto_scroll_box);

	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &RpcADS128Widget::log_timer_ontimer);
	ads_timer = std::unique_ptr<QTimer>(new QTimer);
	connect(ads_timer.get(), &QTimer::timeout, this, &RpcADS128Widget::ads_timer_ontimer);

	log_timer.start(200);

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); 

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); 

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku");
		this->deleteLater();
		return;
	}
	if (ads_num == 0)
		connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));
	else
		connect(mku_signal_thr.get_obj().get(), SIGNAL(new_ku(int, int, double, int)), this, SLOT(new_ku(int, int, double, int)));

	QString ip_str = "127.0.0.1";
	ads128_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new ads128_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, ADS_SLOT+ads_num);
	rpc_slot_srv->start();
	ads128_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new ads128_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, ADS_SIGNAL + ads_num);
	rpc_signal_srv->start();
	setWindowTitle(QString("ADS128 %1").arg(ads_num));
	for (int i = 0; i < 16; i++)
		state_buffer << 0;
	running = false;
}



int RpcADS128Widget::ads128_read_data(QVariantList& thisbuf, QVariantList& firstbuf)
{   
	QString _msg = QString("%1 Чтение данных").arg(QTime::currentTime().toString("hh:mm:ss.zzz"));
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());


	QMutexLocker lock(&ads_mutex);

	thisbuf = state_buffer;
	return 0;
}

int RpcADS128Widget::ads128_stop()
{
	running = false;
	return 0;
}
int RpcADS128Widget::ads128_start()
{
	QString _msg;
		_msg = QString("%1 Запускаю процесс измерения").arg(QTime::currentTime().toString("hh:mm:ss.zzz"));
		{
		QMutexLocker lock(&log_mutex);
			log_buffer << _msg;
		}
		
	running = true;

	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
	return 0;
}

int RpcADS128Widget::ads128_conf_analog(uint group, double level_0, double level_1)
{
	step_1 = level_0;
	step_2 = level_1;
	return 0;
}

int RpcADS128Widget::ads128_analog_q(uint group_, double& lev0, double& lev1)
{
	lev0 = 5;
	lev1 = 15;
	return 0;
}

void RpcADS128Widget::ads_timer_ontimer()
{
	ads_timer->stop();
	QMutexLocker lock(&ads_mutex);
	state_buffer.clear();
	for (int i = 0; i < 16; i++)
		state_buffer << 0;
}

void RpcADS128Widget::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}

void RpcADS128Widget::log_timer_ontimer()
{
	QStringList tmp_buffer;
	{
		QMutexLocker lock(&log_mutex);
		tmp_buffer = log_buffer;
		log_buffer.clear();
	}
	if (tmp_buffer.isEmpty())
		return;
	QFile log_file(log_filename);
	QTextStream log_stream(&log_file);
	log_file.open(QIODevice::Append);
	for (QStringList::iterator itr = tmp_buffer.begin(); itr != tmp_buffer.end(); itr++)
		log_stream << *itr << "\n";
	log_file.close();
}
void RpcADS128Widget::add_signal(int ads_chan, double _u)
{
	unsigned char new_state = 0;
	if (_u > step_1)
		new_state = 1;
	if (_u > step_2)
		new_state = 3;

	int ads_group_n = ads_chan / 8;
	int ads_chan_group = ads_chan % 8;

	unsigned short old_group = state_buffer[ads_group_n].toInt();

	old_group = ( old_group & ( ~ (3 << (ads_chan_group * 2)))) | (new_state << (ads_chan_group*2));
	state_buffer[ads_group_n] = old_group;
}

void RpcADS128Widget::new_ku(int ku_n, int length, double u, int line)
{
	int ads_chan_n = ku_n;
	if (line & 1)
		add_signal(ads_chan_n, u);
	if (line & 2)
		add_signal(ads_chan_n+8, u);

	ads_timer->start(length);
}

void RpcADS128Widget::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	int ads_chan_m = mshm + 32;
	int ads_chan_p = pshm;
	if (line_m & 1)
		add_signal(ads_chan_m, u_m);
	if (line_m & 2)
		add_signal(mshm+48, u_m);
	if (line_p & 1)
		add_signal(ads_chan_p, u_p);
	if (line_p & 2)
		add_signal(pshm+16, u_p);
	ads_timer->start(qMax(length_m, length_p));
}