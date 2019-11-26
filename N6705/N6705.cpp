#include "N6705.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <qmessagebox.h>
#include "rpc_ports.h"
#include <QSettings>
#include "rpc_loger.h"

N6705Widget::N6705Widget() : QWidget()
{
	QVBoxLayout* h_lay = new QVBoxLayout(this);
	log_widget = new LogWidget(0, "n6705");

	for (int i = 0; i < 3; i++)
	{
		chan_state tmp_chan_state;
		tmp_chan_state.u = 0.0;
		tmp_chan_state.i = 0.0;
		tmp_chan_state.state = false;


		chan_states << tmp_chan_state;
		QVBoxLayout* v_lay = new QVBoxLayout(this);

		QLabel* u_label = new QLabel("Установленное напряжение: 0.0В");
		v_lay->addWidget(u_label);
		u_labels << u_label;
		QLabel* i_label = new QLabel("Установленный ток ограничения: 0.0А");
		v_lay->addWidget(i_label);
		i_labels << i_label;
		QLabel* state_label = new QLabel("Состояние выхода: выключено");
		v_lay->addWidget(state_label);
		state_labels << state_label;
		QLabel* u_meas_label = new QLabel("Измеренное напряжение: 0.0В");
		v_lay->addWidget(u_meas_label);
		u_meas_labels << u_meas_label;
		QLabel* i_meas_label = new QLabel("Измеренный ток: 0.0А");
		v_lay->addWidget(i_meas_label);
		i_meas_labels << i_meas_label;

		h_lay->addLayout(v_lay);
	}
	calc_meas();
	server = new QTcpServer();
	connect(server, SIGNAL(newConnection()), this, SLOT(tcp_slot()));
	connect(this, &N6705Widget::update_graphics_signal, this, &N6705Widget::update_graphics);
	server->listen(QHostAddress::Any, N6705_PORT);

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("n6705_geometry").toByteArray());
}

void N6705Widget::tcp_slot()
{
	SRPCSignalClass::Instance().toLog("connected");
	QTcpSocket* socket = server->nextPendingConnection(); 
	connect(socket, SIGNAL(readyRead()), this, SLOT(read_data()), Qt::DirectConnection);
}

void N6705Widget::update_graphics()
{
	calc_meas();
	for (int i = 0; i < 3; i++)
	{
		u_labels[i]->setText(QString("Установленное напряжение : %1В").arg(chan_states[i].u));
		i_labels[i]->setText(QString("Установленный ток ограничения : %1А").arg(chan_states[i].i));
		state_labels[i]->setText(QString("Состояние выхода: %1").arg(chan_states[i].state ? "включено" : "выключено"));
		u_meas_labels[i]->setText(QString("Измеренное напряжение : %1В").arg(chan_states[i].u_meas));
		i_meas_labels[i]->setText(QString("Измеренный ток : %1А").arg(chan_states[i].i_meas));
	}
}

void N6705Widget::read_data()
{
	SRPCSignalClass::Instance().toLog("new data");
	QByteArray read_data;
	QString ret_str;
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

	while (socket->bytesAvailable())
	{
		while (socket->bytesAvailable())
		{
			read_data += socket->read(socket->bytesAvailable());
			Sleep(10);
		}
		QString command(read_data.toStdString().c_str());
		SRPCSignalClass::Instance().toLog(command);
		QStringList command_list = command.split("\r\n");
		command_list.pop_back();
		for (int ii = 0; ii < command_list.count(); ++ii)
		{
			command = command_list.at(ii);
			QStringList params;
			QString command_string;
			if (command.contains(" "))
			{
				QString param_string = command.right(command.size() - command.indexOf(" ") - 1);
				params = param_string.split(", ");
				command_string = command.left(command.indexOf(" "));
			}
			else
				command_string = command;


			qDebug() << command;
			SRPCSignalClass::Instance().toLog(command);
			SRPCSignalClass::Instance().toLog(command_string);
			SRPCSignalClass::Instance().toLog(RPCSignalClass::QVariantToString(params));

			int chan_n;
			if (!params.empty())
			{
				QString last_param = params.last();
				if (last_param.contains("@"));
					chan_n = last_param.mid(2, 1).toInt() - 1;
			}

			if ((command_string == ":VOLT") || (command_string == "VOLT"))
			{
				chan_states[chan_n].u = params.at(0).toDouble();
			}
			if (command_string == "OUTP")
			{
				if (params.at(0) == "ON")
				{
					chan_states[chan_n].state = true;
				}
				else
				{
					chan_states[chan_n].state = false;
				}

			}
			if (command_string == "MEAS:VOLT?")
			{
				ret_str = QString::number(chan_states[chan_n].state ? chan_states[chan_n].u : 0);
			}
			if (command_string == "*IDN?")
			{
				ret_str = "AGILENT 6705 IMITATOR POWER SOURCE";
			}
			if (command_string == "VOLT?")
			{
				/* вывести переменную u*/
				ret_str = QString::number(chan_states[chan_n].u);
			}
			if (command_string == "CURR?")
			{
				/*выводить curr, переменную создал*/
				ret_str = QString::number(chan_states[chan_n].i);
			}

			if (command_string == "MEAS:CURR?")
			{
				calc_meas();
				/*выводить измеренное curr, переменную создал*/

				ret_str = QString::number(chan_states[chan_n].i_meas);
			}

			if (command_string == "*TST?")
			{
				/* должен выводить 0*/
				ret_str = "0"; /*как я понял - не таким образом*/
			}


			if ((command_string == ":CURR %1") || (command_string == "CURR:LIM %1"))
			{
				chan_states[chan_n].i = params.at(0).toDouble();
			}

			if (command_string.contains("?"))
			{
				qDebug() << "4" << ret_str;
				socket->write(ret_str.toStdString().c_str());
				if (!socket->waitForBytesWritten(1000))
					QMessageBox::critical(0, "Sorensen", QString("Ошибка отправки данных: %1 (%2)").arg(socket->errorString()).arg(socket->error()));
			}
		}
		emit update_graphics_signal();

	}
}

void N6705Widget::calc_meas()
{

	for (int i = 0; i < 3; i++)
	{

		if (!chan_states[i].state)
		{
			chan_states[i].u_meas = 0;
			chan_states[i].i_meas = 0;
		}
		else
		{
			chan_states[i].u_meas = chan_states[i].u;
			chan_states[i].i_meas = 0;
		}
	}
}

void N6705Widget::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("n6705_geometry", saveGeometry());
	QWidget::closeEvent(event);
}