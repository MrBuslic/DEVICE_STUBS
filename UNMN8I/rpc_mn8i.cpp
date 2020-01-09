#include "rpc_mn8i.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <qmessagebox.h>
#include "mn8i_socket_rpc.h"
#include "rpc_ports.h"

RpcMN8IWidget::RpcMN8IWidget(int mn8i_num) : QWidget(), measuring(false), state(false)
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	log_widget = new LogWidget(this, QString("mn8i_%1").arg(mn8i_num));

	setMinimumSize(490, 300);

	  QGridLayout* gr_layout = new QGridLayout;
	for (int i = 0; i < 2; i++)
	 for (int j = 0; j < 4; j++)
	{
		QLineEdit* tmp_edit = new QLineEdit;
	 	checks << tmp_edit;
	 	gr_layout->addWidget(tmp_edit, i*2, j);
		QPushButton* push_b = new QPushButton(QString::number(j+1+i*4));
		connect(push_b, &QPushButton::clicked, this, &RpcMN8IWidget::button_clicked);
		buttons.insert(push_b, i * 2 + j);
 		gr_layout->addWidget(push_b,i*2+1, j);
	} 
	  
	v_lay->addLayout(gr_layout);
	v_lay->addWidget(log_widget);

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); // вот тут падает

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); // вот тут падает

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku");
		this->deleteLater();
		return;
	}

	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));
	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_ku(int, int, double, int)), this, SLOT(new_ku(int, int, double, int)));

	QString ip_str = "127.0.0.1";
	mn8i_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new mn8i_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, MN8I_SLOT+mn8i_num);
	rpc_slot_srv->start();
	mn8i_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new mn8i_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, MN8I_SIGNAL+mn8i_num);
	rpc_signal_srv->start();
	setWindowTitle(QString("mn8i %1").arg(mn8i_num));

}

int RpcMN8IWidget::unmn8i_input_trigger(bool state)
{
	QString _msg = QString("%1 входные реле").arg((state == true) ? "замыкаю" : "размыкаю");
	log_widget->log_append(_msg);
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
	 
	QString _msg = QString("Запрос данных");
	log_widget->log_append(_msg);

	return 0;
}
int RpcMN8IWidget::unmn8i_num_ready_data (uint&_num)
{
	_num = buffer.size() ; 
	return 0;
}
int RpcMN8IWidget::unmn8i_mode_cycle(uint _size)
{
	samples = _size;
	return 0;
}

int RpcMN8IWidget::unmn8i_sample_period(double _periodS)
{
	periodS = _periodS;
	return 0;
}

int RpcMN8IWidget::unmn8i_start()
{
	QString _msg;
	if (samples == 1)
		_msg = QString("Запускаю процесс однократного измерения");
	else
		if (samples == 0)
			_msg = QString("Запускаю процесс непрерывного измерения");
		else 
			_msg = QString("Запускаю процесс измерения %1 семплов").arg(samples);
	 
	log_widget->log_append(_msg);


	if (samples == 1)
	{ 
		buffer.clear();
		QVariantList tmp_measurment; 
		for (int i = 0; i < 8; i++)
			tmp_measurment << checks[i]->text().toDouble();

		buffer << QVariant(tmp_measurment);

		emit packet_ready();
	}
	else
	{
		if (samples == 0)
		{
			buffer.clear(); 
			infin_timer = std::unique_ptr<QTimer>(new QTimer);
			connect(infin_timer.get(), &QTimer::timeout, this, &RpcMN8IWidget::infin_timer_ontimer);
			infin_timer->start(periodS*1000);
			infinit = true;

		}

		else
		{
			buffer.clear();
			buffer.reserve(samples);
			for (int i = 0; i < samples; i++)
			{
				QVariantList tmp_measurment;
				for (int j = 0; j < 8; j++)
					tmp_measurment << 0;
				buffer << QVariant(tmp_measurment);
			}
			measuring = true;
			begin_time = QTime::currentTime();
			QTimer::singleShot(samples * 1000 * periodS, this, SLOT(measurement_timer_ontimer()));
		}
	}
	 

	return 0;
}

int RpcMN8IWidget::unmn8i_stop()
{
	if (infin_timer != std::unique_ptr<QTimer>())
		infin_timer->stop();
	infinit = false;

	return 0;
}

int RpcMN8IWidget::unmn8i_read_packet(bool isHot, uint numSamples, QVariantList& buf, uint& realNumSamples)
{
	if (buffer.isEmpty())

		realNumSamples = 0;

	else
	{
		realNumSamples = numSamples;
		if (realNumSamples > buffer.size())
			realNumSamples = buffer.size();
		buf = buffer.mid(0, realNumSamples);
		buffer.erase(buffer.begin(), buffer.begin()+realNumSamples);
	}
	return 0;

}

void RpcMN8IWidget::measurement_timer_ontimer()
{
	measuring = false;
	emit packet_ready();
}

void RpcMN8IWidget::infin_timer_ontimer()
{ 
	QVariantList tmp_measurment;
	for (int i = 0; i < 8; i++)
		tmp_measurment << checks[i]->text().toDouble();

	buffer << QVariant(tmp_measurment);


}  

void RpcMN8IWidget::button_clicked()
{

	if (!measuring)
		return;

	QMap<QObject*, int>::iterator itr = buttons.find(sender());
	if (itr == buttons.end())
		return;

	form_impulse(itr.value(), checks[itr.value()]->text().toDouble(), 26.5);
}

void RpcMN8IWidget::form_impulse(int chan, double length, double u)
{
	int cur_msecs = begin_time.msecsTo(QTime::currentTime());

	double msecs_d = (double)(double(cur_msecs) / (double)(1000));

	int start_ind = msecs_d / periodS;

	int samples_length = length / periodS;

	for (int i = start_ind; i < start_ind + samples_length; i++)
	{
		QVariantList tmp_meas = buffer[i].toList();
		tmp_meas[chan] = u;
		buffer[i] = tmp_meas;
	}
}

void RpcMN8IWidget::new_ku(int ku_n, int length, double u, int line)
{
	if (!measuring)
		return;

	if (line & 1)
		form_impulse(4, double(length) / 1000.0, u);
	if (line & 2)
		form_impulse(5, double(length) / 1000.0, u);
}

void RpcMN8IWidget::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	if (!measuring)
		return;

	if (line_m & 1)
		form_impulse(0, double(length_m) / 1000.0, u_m);
	if (line_m & 2)
		form_impulse(1, double(length_m) / 1000.0, u_m);

	if (line_p & 1)
		form_impulse(2, double(length_p) / 1000.0, u_p);
	if (line_p & 2)
		form_impulse(3, double(length_p) / 1000.0, u_p);

}
