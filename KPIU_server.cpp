#include "KPIU_server.h"
#include "socket_rpc.h"
#include "rpc_ports.h"
#include "kpiu_socket_rpc.h"

KPIUServer::KPIUServer(QWidget* parent) : QWidget(parent)
{
	QString ip_str = "127.0.0.1";
	QGridLayout* bus_grd_layout = new QGridLayout(this);
	bus_grd_layout->setVerticalSpacing(10);
	bus_grd_layout->setHorizontalSpacing(10);
	omnibus_widget = new RpcOmnibusWidget(this);
	interrupt_widget = new InterruptWidget(this);
	kpi_widget = new KPIWidget(this);
	mku_widget = new MKUWidget(this);
	frame_widget = new FrameBusWidget(this);
	power_widget = new PowerWidget(this);
	ols_widget = new RpcOlsWidget(this, 0);
	kprd_widget = new KPRD_imitator(this);
	foi_widget = new RpcFoiWidget(this);
	QLabel* omnibus_label = new QLabel("OMNIBUS");
	QLabel* interrupt_label = new QLabel("Interrupt");
	QLabel* kpi_label = new QLabel("KPI");
	QLabel* mku_label = new QLabel("MKU");
	QLabel* frame_label = new QLabel("Frame bus");
	QLabel* power_label = new QLabel("Power bus");
	QLabel* ols_label = new QLabel("Ols");
	QLabel* kprd_label = new QLabel("KPRD");
	QLabel* foi_label = new QLabel("Foi");
	//connect(ols_widget, &RpcOlsWidget::new_ols_data, kprd_widget, &KPRD_imitator::dataIn);
	bus_grd_layout->addWidget(omnibus_label, 0, 0, Qt::AlignCenter);
	bus_grd_layout->addWidget(interrupt_label, 0, 1, Qt::AlignCenter);
	bus_grd_layout->addWidget(kpi_label, 0, 2, Qt::AlignCenter);
	bus_grd_layout->addWidget(omnibus_widget, 1, 0);//отладить 
	bus_grd_layout->addWidget(interrupt_widget, 1, 1);
	bus_grd_layout->addWidget(kpi_widget, 1, 2);
	bus_grd_layout->addWidget(mku_label, 2, 0, Qt::AlignCenter);
	bus_grd_layout->addWidget(frame_label, 2, 1, Qt::AlignCenter);
	bus_grd_layout->addWidget(power_label, 2, 2, Qt::AlignCenter);
	bus_grd_layout->addWidget(mku_widget, 3, 0);
	bus_grd_layout->addWidget(frame_widget, 3, 1);
	bus_grd_layout->addWidget(power_widget, 3, 2, Qt::AlignTop);
	bus_grd_layout->addWidget(ols_label, 4, 0, Qt::AlignCenter);
	bus_grd_layout->addWidget(kprd_label, 4, 1, Qt::AlignCenter);
	bus_grd_layout->addWidget(foi_label, 4, 2, Qt::AlignCenter);
	bus_grd_layout->addWidget(ols_widget, 5, 0, Qt::AlignTop);	
	bus_grd_layout->addWidget(kprd_widget, 5, 1, Qt::AlignTop);
	bus_grd_layout->addWidget(foi_widget, 5, 2);
	
	kpiu_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new kpiu_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);

	rpc_slot_srv->set_params(ip_str, KPIU_SERVER_SLOT);
	rpc_slot_srv->start();
	kpiu_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new kpiu_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, KPIU_SERVER_SIGNAL);
	rpc_signal_srv->start();
}
//mku_bus_setup
int KPIUServer::KU_NASTROYKA_CELOSTNOSTI_KANALOV(int ku_n, int line)
{
	mku_widget->ku_map_channels_setup(ku_n, line);
	return 0;
}
int KPIUServer::MSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int mshm, int line_m)
{
	mku_widget->mshm_map_channels_setup(mshm, line_m);
	return 0;
}
int KPIUServer::PSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int pshm, int line_p)
{
	mku_widget->pshm_map_channels_setup(pshm, line_p);
	return 0;
}

//unfoi_setup
int KPIUServer::FOI_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan)
{
	foi_widget->unfoi_map_channels_setup(_n, _chan);
	return 0;
}

//unols_setup
int KPIUServer::OLS_ZAPIS_DANNIH_FORMIROVANIYA(QVariantList data_buffer, QVariantList mask_buffer)
{
	ols_widget->unols_write_data_kf(data_buffer, mask_buffer);
	return 0;
}

int KPIUServer::OLS_ZAPIS_DANNIH_REGISTRACII(QVariantList data_buffer)
{
	ols_widget->unols_read_data_kr(data_buffer);
	return 0;
}

int KPIUServer::OLS_BISTRIY_START(int devise)
{
	ols_widget->unols_trigger_imm(devise);
	return 0;
}

void KPIUServer::OLS_CHTENIE_DANNICH_REGISTRACII(QVariantList& data_buffer)
{
	ols_widget->unols_read_data_kr(data_buffer);
}

//KPRD_setup
void KPIUServer::ANTENNA_USTANOVKA_KOMMUTACII(QString antenna_name, QString connected_antenna_name)
{
	kprd_widget->set_antenna_connection(antenna_name, connected_antenna_name);
}

//OMNIBUS_setup
int KPIUServer::OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan)
{
	omnibus_widget->unomnibus_map_channels_setup(_n, _chan);
	SRPCSignalClass::Instance().toLog(QString("ѕрисваиваю каналу %1 значение %2").arg(_n).arg(_chan));
	return 0;
}

//power_bus_setup
void KPIUServer::USTANOVIT_SOSTOYANIE_SHINI_PITANIYA(int bus, int state)
{
	power_widget->set_bus_state(bus, state);
}

QString KPIUServer::getXML()
{
	QFile file(QCoreApplication::applicationDirPath() + "/OPERATORI_NASTROYKI_IMITATOROV_SHIN.xml");

	if (file.open(QFile::ReadOnly | QFile::Text))
		return file.readAll();
	else
		return "";
}