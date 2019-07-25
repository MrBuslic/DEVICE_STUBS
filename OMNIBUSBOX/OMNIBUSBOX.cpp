#include "OMNIBUSBOX.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include "omnibus_socket_rpc.h"
#include <qsettings.h>
#include "instruments.h"


//структура командного слова сообщения МКО
//
union MkoWord
{
	quint16 cw;				 // командное слово целиком
	struct
	{
		quint16 count : 5,    // число сл.данных / команда
	subadr : 5,   // подадрес
		 tr : 1,       // направление передачи(1-чт.ОУ)
		  adr : 5;
	};
};

RpcOmnibusWidget::RpcOmnibusWidget() : QWidget(), auto_scroll(true)
{
	for (int i = 1; i <= 2; i++) map_channels[i] = 3;//инициализация мап исправных каналов
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	edit = new QTextEdit(this);
	_scroll_bar = edit->verticalScrollBar();
	_doc = new QTextDocument();
	_cursor = new QTextCursor(_doc);
	edit->setDocument(_doc);
	edit->setReadOnly(true);
	_doc->setMaximumBlockCount(1000);
	setMinimumSize(490, 300);
	setMaximumSize(500, 500);
	auto_scroll_box = new QCheckBox(this);
	auto_scroll_box->setText("Автопрокрутка");
	auto_scroll_box->setChecked(true);
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &RpcOmnibusWidget::auto_scroll_clicked);
	v_lay->addWidget(edit);
	v_lay->addWidget(auto_scroll_box);

	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &RpcOmnibusWidget::log_timer_ontimer);
	log_timer.start(200);


	for (int i = 0; i < 2; i++)
	{
		QMap<short, RpcAbonent> tmp_map;
		for (int j = 1; j < 30; j++)
		{
			RpcAbonent tmp_ab(j);
			tmp_map.insert(j, tmp_ab);
		}
		abonents.insert(i, tmp_map);
	}

	
	QString ip_str = instr::GetIpFromSettings("rpc_omnibus");
	
		
	//QString ip_str = "127.0.0.1";
	int slot_port = OMNIBUS_SLOT;
	int signal_port = OMNIBUS_SIGNAL;
	omnibus_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new omnibus_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	omnibus_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new omnibus_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
	connect(this, &RpcOmnibusWidget::message_to_log, this, &RpcOmnibusWidget::message_to_log_slot, Qt::BlockingQueuedConnection);
}

RpcAbonent::RpcAbonent(int addr)
{
	on = false;
	os = addr << 11;
	for (int i = 1; i < 30; i++)
	{
		QList<unsigned short> tmp_words;
		for (int j = 0; j < 32; j++)
		{
			tmp_words << 0;
		}

		words.insert(i, tmp_words);
	}
}

void RpcOmnibusWidget::message_to_log_slot(QString _msg)
{
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
}

void RpcOmnibusWidget::switch_ab_os(int mko, int addr, int _os)
{
	abonents[mko][addr].os = _os;
	QString _msg = QString("%1 абонент с адресом %2 на МКО %3 с ответным словом %4").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(addr).arg(mko).arg(_os);
	emit message_to_log(_msg);
}


void RpcOmnibusWidget::switch_ab(int mko, int addr, bool _on)
{
	abonents[mko][addr].on = _on;
	QString _msg = QString("%1 абонент с адресом %2 на МКО %3 %4").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(addr).arg(mko).arg(_on ? "включен" : "выключен");
	emit message_to_log(_msg);
}

void RpcOmnibusWidget::set_new_data(int mko, int addr, int saddr, QVariantList words)
{
	for (int i = 0; i < words.count(); i++)
	{
		abonents[mko][addr].words[saddr][i] = words[i].toInt();
	}
	QString _msg = QString("%1 абоненту с адресом %2 на МКО %3 заданы новые данные в подадрес %4").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(addr).arg(mko).arg(saddr);
	emit message_to_log(_msg);
}

void RpcOmnibusWidget::send_msg(int mko, int line, int cwd, QVariantList& words, int& os)
{
	MkoWord tmp_cwd;	
	tmp_cwd.cw = cwd;
	int work_line = line + 1;//для совпадения значений работающей линией с мапой каналов (1;2) вместо (0;1)

	if (!(map_channels[mko] & work_line))
	{
		QString _msg = QString("%1 МКО %2 канал %3 не работает").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(mko).arg(line);
		emit message_to_log(_msg);
		return;
	}
	if (abonents[mko][tmp_cwd.adr].on)
	{
		os = abonents[mko][tmp_cwd.adr].os;
		if (tmp_cwd.tr)
		{
			words.clear();
			words.reserve(tmp_cwd.count);
			for (int i = 0; i < tmp_cwd.count; i++)
				words << abonents[mko][tmp_cwd.adr].words[tmp_cwd.subadr][i];
		}
	}
	else
	{
		os = -1;
	}
	QString _msg = QString("%1 обмен на МКО %2 КС 0x%3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(mko).arg(cwd, 4, 16, QChar('0'));
	emit message_to_log(_msg);
	emit new_message(QDateTime::currentMSecsSinceEpoch() * 1000, mko, line, cwd, words, os);
}


QVariant RpcOmnibusWidget::get_dt()
{
	return QDateTime::currentMSecsSinceEpoch()*1000;
}

void RpcOmnibusWidget::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}


void RpcOmnibusWidget::log_timer_ontimer()
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

int RpcOmnibusWidget::unomnibus_map_channels_setup(int _n, short _chan)
{
	map_channels[_n] = _chan;
	return 0;
}