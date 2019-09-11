#include "rpc_ols.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include "ols_socket_rpc.h"
#include "rpc_ports.h"

//rpc_buffer_class kprd_list;

RpcOlsWidget::RpcOlsWidget(int ols_num) : QWidget(), auto_scroll(true)
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
	setMaximumSize(500, 300);
	auto_scroll_box = new QCheckBox(this);
	auto_scroll_box->setText("Автопрокрутка");
	auto_scroll_box->setChecked(true);
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &RpcOlsWidget::auto_scroll_clicked);
	
	v_lay->addWidget(edit);
	v_lay->addWidget(auto_scroll_box);

	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &RpcOlsWidget::log_timer_ontimer);
	log_timer.start(200);

	QString ip_str = "127.0.0.1";
	int slot_port = OLS_SLOT;
	int signal_port = OLS_SIGNAL;
	ols_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new ols_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port + ols_num);
	rpc_slot_srv->start();
	ols_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new ols_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port + ols_num);
	rpc_signal_srv->start();
	setWindowTitle(QString("ols %1").arg(ols_num));

	//buffer_class = new rpc_buffer_class;
	//connect(buffer_class, &rpc_buffer_class::send_data, this, &new_data);
}

int RpcOlsWidget::unols_write_data_kf(QVariantList data_buffer, QVariantList mask_buffer)
{
	rpc_ols_kf_buffer = data_buffer;
	rpc_mask_buffer = mask_buffer;
//	SRPCSignalClass::Instance().toLog(QString("Записываю данные %1 с маской %2").arg(data_buffer).arg(mask_buffer));
	return 0;
}
/*
int RpcOlsWidget::unols_read_data_kr(QVariantList data_buffer)
{
	rpc_ols_kr_buffer = data_buffer;
}
*/
int RpcOlsWidget::unols_trigger_imm(int devise)//добавить параметр (выдача, приём)
{
	if (devise == 1)//возможно обойтись без ветвления?
	{
		emit send_data(rpc_ols_kr_buffer);//добавить связь сигнала rpc_ols с КПИУ
	}
	else if (devise == 2){
		emit new_ols_data(rpc_ols_kf_buffer, rpc_mask_buffer);
	}
	return 0;
}

void RpcOlsWidget::unols_read_data_kr(QVariantList& data_buffer)
{
	//rpc_ols_kr_buffer << 10;
	data_buffer = rpc_ols_kr_buffer;
}

void RpcOlsWidget::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}


void RpcOlsWidget::log_timer_ontimer()
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

int RpcOlsWidget::unols_mStart()
{

	return 0;
}