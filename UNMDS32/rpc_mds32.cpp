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
RpcMDS32Widget::RpcMDS32Widget(int mds_num) : QWidget(), auto_scroll(true)
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
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &RpcMDS32Widget::auto_scroll_clicked);
	
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
	v_lay->addWidget(buf_edit);
	v_lay->addWidget(edit);
	v_lay->addWidget(auto_scroll_box);

	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &RpcMDS32Widget::log_timer_ontimer);
	log_timer.start(200);

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
	QString _msg = QString("%1 %2  входные реле").arg(QTime::currentTime().toString("hh:mm : ss.zzz")).arg((state == true) ? "замыкаю" : "размыкаю");
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");


	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
	return 0; 
}

int RpcMDS32Widget::unmds32_read_sample(uint& _buf, uint& _firstTime, uint& _lasteTime)
{
	std::bitset<32> channels(0xFFFFFFFF);
	std::bitset<32> edit_channels(buf_edit->text().toUInt(0, 0));
	int flag = 0;
	for (int i = 0; i < 32; i++)
	{
		uint tmp = 0;
		emit mds32_get_sample(i+1, tmp, flag);
		if (flag)
		{
			channels.set(i, tmp);
		}
		else
		{
			channels.set(i, edit_channels[i]);
		}
	}

	_buf = channels.to_ulong();
	//_buf = buf_edit->text().toUInt(0, 0);
	_firstTime = 0;
	_lasteTime = 0;
	
	//QString _msg = QString("%1 Изменение данных: %2").arg(QTime::currentTime().toString("hh:mm : ss.zzz")).arg(_buf);
	//{
	//	QMutexLocker lock(&log_mutex);
	//	log_buffer << _msg;
	//}
	//_cursor->insertText(_msg + "\n");


	//if (auto_scroll)
	//	_scroll_bar->setValue(_scroll_bar->maximum());
	return 0;
}

int RpcMDS32Widget::unmds32_start()
{
	QString _msg = QString("%1 Запускаю процесс измерения в текущей конфигурации").arg(QTime::currentTime().toString("hh:mm:ss.zzz"));
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
	return 0;
}


void RpcMDS32Widget::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}


void RpcMDS32Widget::log_timer_ontimer()
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