#include "rpc_mfsk24.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QLabel>
#include "mfsk24_socket_rpc.h"
#include <QVector>
#include "rpc_ports.h"

RpcMFSK24Widget::RpcMFSK24Widget(int mfsk_num) : QWidget(), auto_scroll(true)
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
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &RpcMFSK24Widget::auto_scroll_clicked);

	QGridLayout* gr_layout = new QGridLayout;
		for (int i = 0; i < 4; i++)
		for (int j = 0; j < 6; j++)
		{
			QPushButton* tmp_label = new QPushButton;
			//tmp_label->setText("t");
			tmp_label->setIcon(QIcon(":/icons/dark.png"));
			tmp_label->setIconSize(QSize(32, 32));
			tmp_label->setFixedSize(32, 32);
			labels << tmp_label;
			state << false;
			gr_layout->addWidget(tmp_label, i, j);
		}

	v_lay->addLayout(gr_layout);
	v_lay->addWidget(edit);
	v_lay->addWidget(auto_scroll_box);

	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &RpcMFSK24Widget::log_timer_ontimer);
	log_timer.start(200);

	QString ip_str = "127.0.0.1";
	mfsk24_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new mfsk24_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, MFSK_SLOT+ mfsk_num);
	rpc_slot_srv->start();
	mfsk24_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new mfsk24_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, MFSK_SIGNAL+ mfsk_num);
	rpc_signal_srv->start();
	setWindowTitle(QString("mfsk24 %1").arg(mfsk_num));
}

int RpcMFSK24Widget::unmfsk24_manual_group_cmd(int _state_chan, QVariantList _vec)
{
	QString tmp_channels;
	for (int i = 0; i < 24; i++)
	{
		if (_vec.at(i).toInt()==1)
		{
			tmp_channels += QString(" %1,").arg(i+1);
			if (_state_chan)
			{
				state[i] = true;
				labels[i]->setIcon(QIcon(":/icons/light.png"));
			}
			else
			{
				state[i] = false;
				labels[i]->setIcon(QIcon("d:/icons/dark.png"));
			}
		}
	}

	tmp_channels.chop(1);
	QString _msg = QString("%1 %2 каналы: %3 ").arg(QTime::currentTime().toString("hh:mm : ss.zzz")).arg((_state_chan == 1) ? "замыкаю" : "размыкаю").arg(tmp_channels);
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	
	
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());

	return 0;
}


int RpcMFSK24Widget::unmfsk24_state(QVariantList& _state)
{
	_state = state;

	return 0;
}

int RpcMFSK24Widget::unmfsk24_manual_cmd(int _chan, int _state_chan)
{
	if (_state_chan)
	{
		emit mfsk24_state_change(_chan, _state_chan);
		state[_chan-1] = true;
		labels[_chan-1]->setIcon(QIcon(":/icons/light.png"));
	}
	else
	{
		emit mfsk24_state_change(_chan, _state_chan);
		state[_chan-1] = false;
		labels[_chan-1]->setIcon(QIcon(":/icons/dark.png"));
	}

	QString _msg = QString("%1 %2 канал: %3 ").arg(QTime::currentTime().toString("hh:mm : ss.zzz")).arg((_state_chan == 1) ? "замыкаю" : "размыкаю").arg(_chan);
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");


	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());


	return 0;


}

int RpcMFSK24Widget::unmfsk24_set_cmd_time(int _chan, int _time)
{
	impulses.insert(_chan, _time);

	return 0;
}

QVariantList RpcMFSK24Widget::map_convert()
{
	QVariantList tmp_list;
	for (QMap<int, int>::iterator itr = impulses.begin(); itr != impulses.end(); ++itr)
	{
		tmp_list << itr.key();
		tmp_list << itr.value();
	}
	return tmp_list;
}

int RpcMFSK24Widget::unmfsk24_start(QVariantList _state)
{
	QString tmp_channels;
	QTime t = QTime::currentTime();
	QString _msg = QString("%1 Одновременная выдача команд:\n").arg(t.toString("hh:mm:ss.zzz"));
	

	
	for (int i = 0; i <= 24; i++)
	{
		if (impulses.contains(i))
		{
			_msg += QString("\tканал: %1 время: %2").arg(i).arg(impulses[i]);
			
		}
			
	}
	emit mfsk24_impulse_change(map_convert());
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");

	if (auto_scroll)
			_scroll_bar->setValue(_scroll_bar->maximum());

	impulses.clear();

	return 0;
}


void RpcMFSK24Widget::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}


void RpcMFSK24Widget::log_timer_ontimer()
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