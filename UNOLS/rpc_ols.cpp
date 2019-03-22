#include "rpc_ols.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include "ols_socket_rpc.h"


//rpc_buffer_class kprd_list;

RpcOlsWidget::RpcOlsWidget() : QWidget(), auto_scroll(true)
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
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &RpcOlsWidget::auto_scroll_clicked);
	v_lay->addWidget(edit);
	v_lay->addWidget(auto_scroll_box);

	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &RpcOlsWidget::log_timer_ontimer);
	log_timer.start(200);

	QString ip_str = "127.0.0.1";
	int slot_port = 30001;
	int signal_port = 30002;
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();

	//buffer_class = new rpc_buffer_class;
	//connect(buffer_class, &rpc_buffer_class::send_data, this, &new_data);
}

int RpcOlsWidget::unols_write_data_kf(QVariantList data_buffer, QVariantList mask_buffer)
{
	rpc_ols_buffer = data_buffer;
	rpc_mask_buffer = mask_buffer;
	return 0;
}
int RpcOlsWidget::unols_trigger_imm()
{
	emit new_ols_data(rpc_ols_buffer, rpc_mask_buffer);
	return 0;
}

void RpcOlsWidget::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}


void RpcOlsWidget::log_timer_ontimer()
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