#include "rpc_mds32.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include "mds32_socket_rpc.h"
#include <bitset>
#include "rpc_ports.h"

union mds_chan
{
	uint channel;
	struct
	{
		uint ch_1 : 1,
			 ch_2 : 1,
			 ch_3 : 1,
			 ch_4 : 1,
			 ch_5 : 1,
			 ch_6 : 1,
			 ch_7 : 1,
			 ch_8 : 1;
	};
};
RpcMDS32Widget::RpcMDS32Widget(int mds_num) : QWidget()
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	
	buf_edit = new QLineEdit;
	connect(buf_edit, &QLineEdit::textEdited, this, &RpcMDS32Widget::line_edit_changed);
	QGridLayout* gr_layout = new QGridLayout;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 8; j++)
		{
		
			QCheckBox* tmp_check = new QCheckBox;
			connect(tmp_check, &QCheckBox::clicked, this, &RpcMDS32Widget::check_box_clicked);
			checks << tmp_check;
			gr_layout->addWidget(tmp_check, i, j);
		}

	v_lay->addLayout(gr_layout);
	log_widget = new LogWidget(this, QString("mds32_%1").arg(mds_num));
	v_lay->addWidget(log_widget);


	QString ip_str = "127.0.0.1";
	mds32_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new mds32_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, MDS_SLOT+mds_num);
	rpc_slot_srv->start();
	mds32_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new mds32_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, MDS_SIGNAL+mds_num);
	rpc_signal_srv->start();
	setWindowTitle(QString("mds32 %1").arg(mds_num));
}

void RpcMDS32Widget::check_box_clicked()
{
	unsigned int tmp_buf = 0;
	for (int i = 0; i < 32; i++)
	{
		if (checks[i]->isChecked())
			tmp_buf += 1 << i;
	}

	buf_edit->setText(QString("0x%1").arg(tmp_buf, 8, 16, QChar('0')));
}

void RpcMDS32Widget::line_edit_changed(const QString& _text)
{
	unsigned int tmp_buf = _text.toUInt(0, 0);
	for (int i = 0; i < 32; i++)
	{
		if (tmp_buf & (1 << i))
			checks[i]->setChecked(true);
		else
			checks[i]->setChecked(false);
	}
}

int RpcMDS32Widget::unmds32_input_trigger(bool state)
{
	QString _msg = QString("%1 входные реле").arg((state == true) ? "замыкаю" : "размыкаю");
	log_widget->log_append(_msg);
	return 0; 
}

int RpcMDS32Widget::unmds32_read_sample(uint& _buf, uint& _firstTime, uint& _lasteTime)
{
	bool flag = false;

	uint tmp = 0;
	emit mds32_get_sample(_buf, flag);
	if (flag)
	{
		buf_edit->setText(QString("0x%1").arg(_buf, 8, 16, QChar('0')));
	}
	else
	{
		_buf = buf_edit->text().toUInt(0, 0);
	}

	_firstTime = 0;
	_lasteTime = 0;
	
	QString _msg = QString("считывание данных: %1").arg(_buf);
	log_widget->log_append(_msg);
	return 0;
}

int RpcMDS32Widget::unmds32_start()
{
	QString _msg = QString("Запускаю процесс измерения в текущей конфигурации");
	log_widget->log_append(_msg);
	return 0;
}