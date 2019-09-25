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

RpcOmnibusWidget::RpcOmnibusWidget(QWidget* parent) : QWidget(parent)
{
	for (int i = 0; i <= 2; i++) map_channels[i] = 3;//инициализация мап исправных каналов

	log_widget = new LogWidget(this, "omnibusbox");
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	v_lay->addWidget(log_widget);

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
	connect(this, &RpcOmnibusWidget::message_to_log, log_widget, &LogWidget::log_append, Qt::DirectConnection);
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

void RpcOmnibusWidget::switch_ab_os(int mko, int addr, int _os)
{
	abonents[mko][addr].os = _os;
	QString _msg = QString("Абонент с адресом %1 на МКО %2 с ответным словом %3").arg(addr).arg(mko).arg(_os);
	emit message_to_log(_msg);
}


void RpcOmnibusWidget::switch_ab(int mko, int addr, bool _on)
{
	abonents[mko][addr].on = _on;
	QString _msg = QString("Абонент с адресом %1 на МКО %2 %3").arg(addr).arg(mko).arg(_on ? "включен" : "выключен");
	emit message_to_log(_msg);
}

void RpcOmnibusWidget::set_new_data(int mko, int addr, int saddr, QVariantList words)
{
	for (int i = 0; i < words.count(); i++)
	{
		abonents[mko][addr].words[saddr][i] = words[i].toInt();
	}
	QString _msg = QString("Абоненту с адресом %1 на МКО %2 заданы новые данные в подадрес %4").arg(addr).arg(mko).arg(saddr);
	emit message_to_log(_msg);
}

void RpcOmnibusWidget::send_msg(int mko, int line, int cwd, QVariantList& words, int& os)
{
	MkoWord tmp_cwd;	
	tmp_cwd.cw = cwd;
	int work_line = line + 1;//для совпадения значений работающей линией с мапой каналов (1;2) вместо (0;1)

	if (!(map_channels[mko] & work_line))
	{
		QString _msg = QString("МКО %1 канал %2 не работает").arg(mko).arg(line);
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
	QString _msg = QString("Обмен на МКО %1 КС 0x%2").arg(mko).arg(cwd, 4, 16, QChar('0'));
	emit message_to_log(_msg);
	emit new_message(QDateTime::currentMSecsSinceEpoch() * 1000, mko, line, cwd, words, os);
}


QVariant RpcOmnibusWidget::get_dt()
{
	return QDateTime::currentMSecsSinceEpoch()*1000;
}

int RpcOmnibusWidget::unomnibus_map_channels_setup(int _n, int _chan)
{
	map_channels[_n] = _chan;
	emit message_to_log(QString("Установлено состояние МКО%1 в %2").arg(_n).arg(_chan));
	return 0;
}