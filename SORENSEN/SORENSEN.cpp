#include "SORENSEN.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <qmessagebox.h>
#include "rpc_ports.h"

SORENSENWidget::SORENSENWidget() : QWidget(), u(0.0), i(0.0)
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);

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
	connect(this, &SORENSENWidget::update_graphics_signal, this, &SORENSENWidget::update_graphics);
	server->listen(QHostAddress::Any, SORENSEN_PORT);

	kp50_slot_thr.set_connection_params("127.0.0.1", KP50_SLOT);
	kp50_slot_thr.start();

	kp50_signal_thr.set_connection_params("127.0.0.1", KP50_SIGNAL);
	kp50_signal_thr.start();

	if (!kp50_slot_thr.wait_connected(3) || !kp50_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с kp50 от sorensen");
		this->deleteLater();
		return;
	}

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("sorensen_geometry").toByteArray());
}

void SORENSENWidget::tcp_slot()
{
	socket = server->nextPendingConnection(); 
	connect(socket, SIGNAL(readyRead()), this, SLOT(read_data()), Qt::DirectConnection);
}

void SORENSENWidget::update_graphics()
{
	calc_meas();
	u_label->setText(QString("Установленное напряжение : %1В").arg(u));
	i_label->setText(QString("Установленный ток ограничения : %1А").arg(i));
	state_label->setText(QString("Состояние выхода: %1").arg(state ? "включено" : "выключено"));
	u_meas_label->setText(QString("Измеренное напряжение : %1В").arg(u_meas));
	i_meas_label->setText(QString("Измеренный ток : %1А").arg(i_meas));
}

void SORENSENWidget::read_data()
{
	QByteArray read_data;
	QString ret_str;

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
				{
					state = true;
					kp50_slot_thr.get_kp50_obj()->set_u_in(u);
				}
				else
				{
					state = false;
					kp50_slot_thr.get_kp50_obj()->set_u_in(0);
				}

			}
			if (command_string == "MEAS:VOLT?")
			{
				ret_str = QString::number(state ? u : 0);
			}
			if (command_string == "*IDN?")
			{
				ret_str = "SORENSEN IMITATOR POWER SOURCE";
			}
			if (command_string == "VOLT?")
			{
				/* вывести переменную u*/
				ret_str = QString::number(u);
			}
			if (command_string == "CURR?")
			{
				/*выводить curr, переменную создал*/
				ret_str = QString::number(i);
			}

			if (command_string == "MEAS:CURR?")
			{
				/*выводить измеренное curr, переменную создал*/
				i_meas = 0;
				i_meas += kp50_slot_thr.get_kp50_obj()->unkp50_meas_I(1);
				i_meas += kp50_slot_thr.get_kp50_obj()->unkp50_meas_I(2);
				i_meas += kp50_slot_thr.get_kp50_obj()->unkp50_meas_I(3);

				ret_str = QString::number(i_meas);
			}

			if (command_string == "*TST?")
			{
				/* должен выводить 0*/
				ret_str = "0"; /*как я понял - не таким образом*/
			}


			if ((command_string == ":CURR %1") || (command_string == "CURR:LIM %1"))
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

void SORENSENWidget::calc_meas()
{
	if (!state)
	{
		u_meas = 0;
		i_meas = 0;
	}
	else
	{
		u_meas = u;
		i_meas = 0;
		i_meas += kp50_slot_thr.get_kp50_obj()->unkp50_meas_I(1);
		i_meas += kp50_slot_thr.get_kp50_obj()->unkp50_meas_I(2);
		i_meas += kp50_slot_thr.get_kp50_obj()->unkp50_meas_I(3);
	}
}

void SORENSENWidget::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("sorensen_geometry", saveGeometry());
	QWidget::closeEvent(event);
}