#include "rpc_mn8i.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include "mn8i_socket_rpc.h"


RpcMN8IWidget::RpcMN8IWidget(int slot_port, int signal_port) : QWidget(), auto_scroll(true)
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
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &RpcMN8IWidget::auto_scroll_clicked);
	
	QString val = QString::number(5);

	QGridLayout* gr_layout = new QGridLayout;
	for (int i = 0; i < 2; i++)
	for (int j = 0; j < 4; j++)
	{
		QTextEdit* tmp_edit = new QTextEdit;//!!!
		tmp_edit->setMaximumSize(110, 120);
		checks << tmp_edit;
		gr_layout->addWidget(tmp_edit, i, j);
		for (int k = 0; k < 6; k++)
			tmp_edit->insertPlainText(val + "\n");
	}

	v_lay->addLayout(gr_layout);
	v_lay->addWidget(edit);
	v_lay->addWidget(auto_scroll_box);


	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &RpcMN8IWidget::log_timer_ontimer);
	log_timer.start(200);

	QString ip_str = "127.0.0.1";
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
	setWindowTitle(QString("mn8i %1").arg(slot_port - 30029));
}

int RpcMN8IWidget::unmn8i_input_trigger(bool state)
{
	QString _msg = QString("%1 %2 входные реле").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg((state == true) ? "замыкаю" : "размыкаю");
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");


	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
	return 0;
}

int RpcMN8IWidget::unmn8i_sample_width_q(uint& frame_width, uint&  width_in_bytes)
{
	frame_width = 8;
	width_in_bytes = 64;
	return 0;
}


int RpcMN8IWidget::unmn8i_read_sample(uint& _buf, uint& _firstTime, uint& _thisTime)
{
	_buf = buf_edit->text().toUInt(0, 0);
	_firstTime = 0;
	_thisTime = 0;
	
	QString _msg = QString("%1 Запрос данных").arg(QTime::currentTime().toString("hh:mm:ss.zzz"));
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());

	return 0;
}

int RpcMN8IWidget::unmn8i_start()
{
	QString _msg = QString("%1 Запускаю процесс измерения в текущей конфигурации").arg(QTime::currentTime().toString("hh:mm:ss.zzz"));
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}

	buffer.clear();
	QVariantList tmp_measurment;
	for (int j = 0; j < 6; j++) //!!!!
	for (int i = 0; i < 8; i++)
		
		tmp_measurment << checks[i*8+j]->toPlainText().toDouble();//!!!!!
	buffer << QVariant(tmp_measurment);

	emit packet_ready();

	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
	return 0;
}

int RpcMN8IWidget::unmn8i_read_packet(bool isHot, uint numSamples, QVariantList& buf, uint& realNumSamples)
{
	if (buffer.isEmpty())
	{
		realNumSamples = 0;
	}
	else
	{
		buf = buffer;
		realNumSamples = 1;
		buffer.clear();
	}
	return 0;

}

void RpcMN8IWidget::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}


void RpcMN8IWidget::log_timer_ontimer()
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