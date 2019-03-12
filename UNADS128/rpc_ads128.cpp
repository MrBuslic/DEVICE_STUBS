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
#include <qmessagebox.h>

RpcADS128Widget::RpcADS128Widget(int slot_port, int signal_port) : QWidget(), auto_scroll(true),  state(false)
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
	log_timer.start(200);

	lka05_slot_thr.set_connection_params("127.0.0.1", 50061);
	lka05_slot_thr.start(); // вот тут падает

	lka05_signal_thr.set_connection_params("127.0.0.1", 50062);
	lka05_signal_thr.start(); // вот тут падает

	if (!lka05_slot_thr.wait_connected(3) || !lka05_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с lka05");
		this->deleteLater();
		return;
	}

	connect(lka05_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int)), this, SLOT(new_mk(int, int, int, int, double, double, int)));
	connect(lka05_signal_thr.get_obj().get(), SIGNAL(new_ku(int, int, double)), this, SLOT(new_ku(int, int, double)));

	QString ip_str = "127.0.0.1";
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
	setWindowTitle(QString("ADS128 %1").arg(slot_port - 30059));

	running = false;
}

int RpcADS128Widget::ads128_read_data(QVariantList thisbuf, QVariantList firstbuf)
{
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
	
	QMutexLocker lock(&log_mutex);
	log_buffer << _msg;
	
	running = true;

	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
	return 0;
}

void RpcADS128Widget::ads_timer_ontimer()
{
	
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

void RpcADS128Widget::new_ku(int ku_n, int length, double u)
{
	  

}

void RpcADS128Widget::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt)
{
 

}