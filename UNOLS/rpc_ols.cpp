#include "rpc_ols.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include "ols_socket_rpc.h"
#include "rpc_ports.h"

//rpc_buffer_class kprd_list;

RpcOlsWidget::RpcOlsWidget(QWidget* parent, int ols_num) : QWidget(parent)
{

	log_widget = new LogWidget(this, QString("ols_%1").arg(ols_num));
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	v_lay->addWidget(log_widget);

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
	log_widget->log_append(QString("Записаны новые данные"));
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

	if (devise & 2){
		log_widget->log_append(QString("Запуск выдачи"));
		emit new_ols_data(rpc_ols_kf_buffer, rpc_mask_buffer);
	}

	if (devise & 1)//возможно обойтись без ветвления?
	{
		log_widget->log_append(QString("Запуск чтения"));
		emit receive_data(rpc_ols_kr_buffer);//добавить связь сигнала rpc_ols с КПИУ
	}
	return 0;
}

void RpcOlsWidget::unols_read_data_kr(QVariantList& data_buffer)
{
	//rpc_ols_kr_buffer << 10;
	data_buffer = rpc_ols_kr_buffer;
}

int RpcOlsWidget::unols_mStart()
{

	return 0;
}