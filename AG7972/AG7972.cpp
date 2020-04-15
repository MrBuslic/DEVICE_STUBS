#include "AG7972.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QMessageBox>

#include "rpc_ports.h"


AG7972Widget::AG7972Widget() : QWidget(), u(0.0), i(0.0)
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	v_lay->addWidget(new QLabel("Сопротивление нагрузки:"));
	R_edit = new QLineEdit();
	v_lay->addWidget(R_edit);
	R_edit->setText("10");
	u_label = new QLabel("Установленное напряжение: 0.0В");
	v_lay->addWidget(u_label);
	i_label = new QLabel("Установленный ток ограничения: 0.0А");
	v_lay->addWidget(i_label);
	state_label = new QLabel("Состояние выхода: выключено");
	v_lay->addWidget(state_label);
	u_meas_label = new QLabel("Измеренное напряжение: 0.0В");
	v_lay->addWidget(u_meas_label);
	i_meas_label = new QLabel("Измеренный ток: 0.0А");
	v_lay->addWidget(i_meas_label);

	server = new QTcpServer();
	connect(server, SIGNAL(newConnection()), this, SLOT(tcp_slot()));
	connect(R_edit, SIGNAL(editingFinished()), this, SLOT(update_graphics()));

	connect(this, &AG7972Widget::update_graphics_signal, this, &AG7972Widget::update_graphics);

	server->listen(QHostAddress::Any, AG7972_PORT);

	power_slot_thr.set_connection_params("127.0.0.1", POWER_SLOT);
	power_slot_thr.start();

	power_signal_thr.set_connection_params("127.0.0.1", POWER_SIGNAL);
	power_signal_thr.start();
	
	if (!power_slot_thr.wait_connected(3) || !power_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с power_bus");
		this->deleteLater();
		return;
	}

}

void AG7972Widget::tcp_slot()
{
	socket = server->nextPendingConnection(); 
	connect(socket, SIGNAL(readyRead()), this, SLOT(read_data()));
}

void AG7972Widget::update_graphics()
{
	calc_meas();
	u_label->setText(QString("Установленное напряжение : %1В").arg(u));
	i_label->setText(QString("Установленный ток ограничения : %1А").arg(i));
	state_label->setText(QString("Состояние выхода: %1").arg(state ? "включено" : "выключено"));
	u_meas_label->setText(QString("Измеренное напряжение : %1В").arg(u_meas));
	i_meas_label->setText(QString("Измеренный ток : %1А").arg(i_meas));
}

void AG7972Widget::read_data()
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

			if ((command_string == ":VOLT") || (command_string == "VOLT"))
			{
				u = params.at(0).toDouble();
			}
			if (command_string == "OUTP")
			{
				if (params.at(0) == "ON")
					state = true;
				else
					state = false;

			}
			if (command_string == "MEAS:VOLT?")
			{
				QByteArray tmp_arr;
				QDataStream tmp_stream(tmp_arr);
				tmp_stream << u_meas;
				socket->write(tmp_arr);
				socket->waitForBytesWritten();
			}
			if (command_string == "*IDN?")
			{
				QByteArray tmp_arr;
				QDataStream tmp_stream(tmp_arr);
				tmp_stream << "IMITATOR POWER SOURCE";
				socket->write(tmp_arr);
				socket->waitForBytesWritten();
			}
			if (command_string == "VOLT?")
			{
				/* вывести переменную u*/
				QByteArray tmp_arr;
				QDataStream tmp_stream(tmp_arr);
				tmp_stream << u;
				socket->write(tmp_arr);
				socket->waitForBytesWritten();
			}
			if (command_string == "CURR?")
			{
				/*выводить curr, переменную создал*/
				QByteArray tmp_arr;
				QDataStream tmp_stream(tmp_arr);
				tmp_stream << i;
				socket->write(tmp_arr);
				socket->waitForBytesWritten();
			}

			if (command_string == "MEAS:CURR?")
			{
				/*выводить измеренное curr, переменную создал*/
				QByteArray tmp_arr;
				QDataStream tmp_stream(tmp_arr);
				tmp_stream << i_meas;
				socket->write(tmp_arr);
				socket->waitForBytesWritten();
			}

			if (command_string == "*TST?")
			{
				/* должен выводить 0*/
				QByteArray tmp_arr;
				QDataStream tmp_stream(tmp_arr);
				tmp_stream << "0"; /*как я понял - не таким образом*/
				socket->write(tmp_arr);
				socket->waitForBytesWritten();
			}


			if ((command_string == ":CURR") || (command_string == "CURR:LIM"))
			{
				i = params.at(0).toDouble();
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

void AG7972Widget::calc_meas()
{
	if (!state)
		u_meas = 0;
	else
	{
		double tmp_r = R_edit->text().toDouble();
		i_meas = u / tmp_r;
		if (i_meas > i)
			u_meas = tmp_r * i;
		else
			u_meas = u;
	}
	power_slot_thr.get_power_bus_obj()->set_bus_u("AG7972", u_meas);
}

