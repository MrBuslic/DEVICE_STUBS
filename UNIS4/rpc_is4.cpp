#include "rpc_is4.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <qmessagebox.h>
#include "is4_socket_rpc.h"
#include "rpc_ports.h"

RpcIS4Widget::RpcIS4Widget(int is4_num) : QWidget(), auto_scroll(true), is4_line(false)
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
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &RpcIS4Widget::auto_scroll_clicked);
	
	QHBoxLayout* hb_layout = new QHBoxLayout();
	edit_number = new QLineEdit();
	n_box = new QCheckBox();
	n_box->setText("Проверка");
	n_box->setChecked(false);
	hb_layout->addWidget(edit_number);
	hb_layout->addWidget(n_box);
	connect(n_box, &QCheckBox::stateChanged, this, &RpcIS4Widget::is4_clicked);

	v_lay->addLayout(hb_layout);
	v_lay->addWidget(edit);
	v_lay->addWidget(auto_scroll_box);

	 log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &RpcIS4Widget::log_timer_ontimer);
	log_timer.start(200);
 
/*	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); // вот тут падает

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); // вот тут падает

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku");
		this->deleteLater();
		return;
	}
	*/

	//connect(mku_signal_thr.get_obj().get(), SIGNAL(new_ku(int, int, double, int)), this, SLOT(new_ku(int, int, double, int)));
	
	QString ip_str = "127.0.0.1";
	is4_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new is4_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, IS4_SLOT+is4_num);
	rpc_slot_srv->start();
	is4_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new is4_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, IS4_SIGNAL+is4_num);
	rpc_signal_srv->start();
	setWindowTitle(QString("is4 %1").arg(is4_num));

	_process = 0;
	res_meas = 0;

}

int RpcIS4Widget::unis4_SetTypeProcess(uint EProcess)
{
	_process = EProcess;
	return 0;
}

int  RpcIS4Widget::unis4_RangeMeas(uint range)
{
	_range = range;
	return 0;
}
int RpcIS4Widget::unis4_ResultMeas(double& ResMeas, uint& NumRes)
{
	QString _msg = QString("%1 Запрос данных").arg(QTime::currentTime().toString("hh:mm:ss.zzz"));
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	ResMeas = res_meas;
	NumRes = 1;

	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());

	return 0;
}

int RpcIS4Widget::unis4_StartCalibr()
{
	return 0;
}

int RpcIS4Widget::unis4_StartACP()
{		
	
	QVariant tmp_res;
	if (is4_line)
	{
		tmp_res = edit_number->text();
	}
	else
	{
		emit is4_measure(_process, tmp_res);
	}
	res_meas = tmp_res.toDouble();	

	if (_process)
	{
		QString _msg;
		if (_process == 6)//IS4_MeasRes2W
			_msg = QString("%1 Запускаю процесс измерения сопротивления. Результат: %2Ом").arg(QTime::currentTime().toString("hh:mm:ss.zzz").arg(res_meas));
		//Пределы: 
		//3-1кОм 
		//4-10кОм 
		//7-10МОм 
		//8-100МОм 
		if (_process == 9)//IS4_MeasResInsul
			_msg = QString("%1 Запускаю процесс измерения сопротивления изоляции").arg(QTime::currentTime().toString("hh:mm:ss.zzz"));
		//вообще все по дефолту 
		if (_process == 1)//IS4_MeasVoltDC
			_msg = QString("%1 Запускаю процесс измерения напряжения постоянного тока").arg(QTime::currentTime().toString("hh:mm:ss.zzz"));
		//два диапазона 1=10В и 2=100В 
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;

		_cursor->insertText(_msg + "\n");
		if (auto_scroll)
			_scroll_bar->setValue(_scroll_bar->maximum());
		return 0;
	}

}

void RpcIS4Widget::is4_clicked(int state_is4)
{
	is4_line = (state_is4 != 0);
}

void RpcIS4Widget::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}


void RpcIS4Widget::log_timer_ontimer()
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


