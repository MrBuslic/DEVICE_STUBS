#include "MAIN_OBJ.h"
#include "rpc_ports.h"

union MKOWord
{
	quint16 com_word;				 // командное слово целиком
	struct
	{
		quint16 count_word : 5,    // число сл.данных / команда
			subadr : 5,   // подадрес
			trans_dir : 1,       // направление передачи(1-чт.ќ”)
			adr : 5;
	};
};
MAIN_OBJECT main_obj;

int bus_n = 0;

bool int_needed;
int int_chan;

//буферы для МКО
unsigned long int *pInputBuffer = new unsigned long int[128];	// pointer to Input Buffer
unsigned long int *pOutputBuffer = new unsigned long int[128];	// pointer to Output Buffer

//буферы для инициализации
unsigned long int *pIntInputBuffer = new unsigned long int[128];	// pointer to Input Buffer
unsigned long int *pIntOutputBuffer = new unsigned long int[128];	// pointer to Output Buffer

//буферы для прерываний
unsigned long int *pInitInputBuffer = new unsigned long int[128];	// pointer to Input Buffer
unsigned long int *pInitOutputBuffer = new unsigned long int[128];	// pointer to Output Buffer

//потоки связи с шинами
RPC_omnibus_SLOT_Thread* omnibus_slot_thr;
RPC_interrupt_bus_SIGNAL_Thread* interrupt_signal_thr;
reg_table * reg_tab;

void MAIN_OBJECT::addLog(QString _msg)
{
	//вывод в файл
	QFile log_file(log_filename);
	log_file.open(QIODevice::Append);
	QTextStream str(&log_file);
	str << QString("%1 %2\r\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")).arg(_msg);
	log_file.close();

	//вывод сообщения в тсрп
	strcpy(reg_tab->BufferForText, _msg.toStdString().c_str());
	(*reg_tab->pTextToFile);
	(*reg_tab->pTextToWindow);
}

void Subrut();
void SubrutInt()
{
	main_obj.addLog("=============SubrutINT=================");
	pIntOutputBuffer[0] = 0;

	if (int_needed)
	{
		//заказываем прерывание
		pIntOutputBuffer[0] = 6;
		pIntOutputBuffer[1] = REASON_INTERRUPT_I;
		pIntOutputBuffer[2] = 0;
		pIntOutputBuffer[3] = 0;
		pIntOutputBuffer[4] = 0;
		//прерывания передаются побитово
		pIntOutputBuffer[5] = 1 << int_chan;
		pIntOutputBuffer[6] = 0;
		int_needed = false;
		main_obj.addLog("=======================add int=============================");
	}
}
void SubrutInit()
{
	//заказ опроса модели по тактам для реакции на прерывания
	pInitOutputBuffer[0] = 5;
	pInitOutputBuffer[1] = REASON_TACT;
	pInitOutputBuffer[2] = 0;
	pInitOutputBuffer[3] = 99999;
	pInitOutputBuffer[4] = 0;
	pInitOutputBuffer[5] = 0;
}
mod_table base_mod_table[] =
{
	//Id,	Reason,		Address,	S,	pInp,		pOut        Prioritet

	{ 1,REASON_MKO0,	1,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 1,REASON_MKO1,	1,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 2,REASON_MKO1,	2,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 3,REASON_MKO1,	3,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 4,REASON_MKO1,	4,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 5,REASON_MKO1,	5,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 6,REASON_MKO1,	6,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 7,REASON_MKO1,	7,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 9,REASON_MKO1,	9,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 12,REASON_MKO1,	12,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 13,REASON_MKO1,	13,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 18,REASON_MKO1,	18,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 19,REASON_MKO1,	19,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 20,REASON_MKO1,	20,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 21,REASON_MKO1,	21,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 10,REASON_MKO0,	10,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 11,REASON_MKO0,	11,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 23,REASON_MKO0,	23,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 24,REASON_MKO0,	24,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 14,REASON_MKO0,	14,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 15,REASON_MKO0,	15,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 16,REASON_MKO0,	16,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 17,REASON_MKO0,	17,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 27,REASON_MKO0,	27,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 22,REASON_MKO0,	22,	Subrut,	pInputBuffer, pOutputBuffer, 1 },
	{ 8,REASON_INIT,	1,	SubrutInit,	pInitInputBuffer, pInitOutputBuffer, 1 },
	{ 8,REASON_TACT,	1,	SubrutInt,	pIntInputBuffer, pIntOutputBuffer, 1 },
	{ 8,REASON_INTERRUPT_I,	1,	SubrutInt,	pIntInputBuffer, pIntOutputBuffer, 1 },
	{ 0,		0  ,	0,	0,		0,	0, 0 }
};

//функция для обработки модели МКО
void Subrut()
{
	main_obj.addLog("Subrut");
	main_obj.main_func();
	//SEND_THREAD send_thr;
	//send_thr.start();
	//send_thr.wait();
}

//void SEND_THREAD::run()
void MAIN_OBJECT::main_func()
{
	for (int i = 0; i < 1; i++)
	{
		
		int buf_size = base_mod_table[i].pInputBuffer[0];
		QString buf_str = "[";
		for (int j = 0; j < buf_size; j++)
		{
			buf_str += QString("%1,").arg(base_mod_table[i].pInputBuffer[j]);
		}
		if (!buf_str.isEmpty())
			buf_str.truncate(buf_str.size());
		buf_str += "]";
		QString msg = QString("Reason %1, Addr %2, buf %3").arg(base_mod_table[i].Reason).arg(base_mod_table[i].Address).arg(buf_str);
		addLog(msg);
		if (buf_size > 4)
		{
			MKOWord tmp_word;
			tmp_word.com_word = base_mod_table[i].pInputBuffer[5];
			QString mko_str = QString("adr %1, sadr %2, tr %3, wcount %4").arg(tmp_word.adr).arg(tmp_word.subadr).arg(tmp_word.trans_dir).arg(tmp_word.count_word);
			addLog(mko_str);
			QVariantList tmp_data;
			for (int j = 6; j < buf_size; j++)
			{
				tmp_data << uint(base_mod_table[i].pInputBuffer[j]);
			}
			int tmp_os = 0;
			//анализ номера магистрали МКО
			int mko_num = base_mod_table[i].pInputBuffer[1]-REASON_MKO0;

			omnibus_slot_thr->get_omnibus_obj()->send_msg(mko_num, 0, tmp_word.com_word, tmp_data, tmp_os);

			//если нет ответа заполняем ОС специальным значением
			if (tmp_os == -1)
				tmp_os = 65536;

			int out_buf_size = 6;
			if ((tmp_word.trans_dir== 1)&&(tmp_word.subadr > 0) && (tmp_word.subadr < 31))
				out_buf_size += tmp_data.count();
			base_mod_table[i].pOutputBuffer[0] = out_buf_size;
			base_mod_table[i].pOutputBuffer[5] = tmp_os;
			for (int j = 0; j < tmp_data.count(); j++)
			{
				base_mod_table[i].pOutputBuffer[6 + j] = tmp_data.at(j).toInt();
			}

			base_mod_table[i].pOutputBuffer[out_buf_size] = 0;
			addLog(QString("out_buf_0 :%1").arg(base_mod_table[i].pOutputBuffer[0]));


		}
		
	}
}
MAIN_OBJECT::MAIN_OBJECT()
{
	//определение файла для логирования
	log_filename = QString("%1/pkm_%2.log").arg("d:/logs").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QFile log_file(log_filename);
	log_file.open(QIODevice::WriteOnly);
	log_file.close();
}

void MAIN_OBJECT::run()
{

	int argc = 1;
	QString app_name = "modcbkm.exe";
	char* argv = const_cast<char *>(app_name.toStdString().c_str());
	
	//нужно создать приложение Qt, чтобы заработали сигналы и слоты
	//QCoreApplication app(argc, &argv);

	//соединение с шиной МКО
	omnibus_slot_thr = new RPC_omnibus_SLOT_Thread;
	omnibus_slot_thr->set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	omnibus_slot_thr->start();

	if (omnibus_slot_thr->wait_connected(3))
	{
		//если соединение прошло успешно
		addLog("connected");
	}
	connect(&SRPCSignalClass::Instance(), &RPCSignalClass::rpc_toLog, this, &MAIN_OBJECT::addLog, Qt::DirectConnection);
	omnibus_slot_thr->get_omnibus_obj()->switch_ab(1, 1, true);
	//соединение с шиной прерываний
	interrupt_signal_thr = new RPC_interrupt_bus_SIGNAL_Thread;
	interrupt_signal_thr->set_connection_params("127.0.0.1", INTERRUPTS_SIGNAL);
	interrupt_signal_thr->start();
	if (interrupt_signal_thr->wait_connected(3))
	{
		//если соединение прошло успешно
		addLog("interrupts connected");
	}

	connect(static_cast<RPC_interrupt_bus_SIGNAL_Object*>(interrupt_signal_thr->get_obj().get()), &RPC_interrupt_bus_SIGNAL_Object::new_interrupt, this, &MAIN_OBJECT::get_new_interrupt, Qt::DirectConnection);


	//app.exec();
	exec();
}


__declspec(dllexport) mod_table* init_pkm(reg_table * _tab)
{
	pIntOutputBuffer[0] = 0;

	//запуск основного потока
	main_obj.start();

	reg_tab = _tab;
	

	return base_mod_table;
}

void MAIN_OBJECT::get_new_interrupt(int _n, short _chan, double _u, double _t)
{
	//обработка прерываний
	main_obj.addLog("==================new interrupt====================");
	//заказ прерывания
	int_needed = true;
	int_chan = _n;


	//pOutputBuffer[0] = 6;
	//pOutputBuffer[1] = REASON_INTERRUPT_I;
	//pOutputBuffer[2] = 0;
	//pOutputBuffer[3] = 0;
	//pOutputBuffer[4] = _n;
	//pOutputBuffer[5] = 0;
}
