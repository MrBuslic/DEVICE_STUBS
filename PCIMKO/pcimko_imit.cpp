#include "omnibus_rpc.h"
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

RPC_omnibus_SLOT_Thread omnibus_slot_thr;

int bus_n = 0;

int __declspec(dllexport) Init_MKO_PCI_C(int _num_card)
{
	omnibus_slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	omnibus_slot_thr.start(); // вот тут падает

	if (!omnibus_slot_thr.wait_connected(3))
	{
		return 1;
	}

	return 0;
}

void __declspec(dllexport) Exit_MKO_PCI_C(int _num_card)
{

}

int __declspec(dllexport) MkoSend_C(int _num_card,//номер карточки(0)
	int _n_tmk,//номер канала карточки(0)
	ushort _ks,//КC
	ushort *_is,//указатель на массив слов данных, минимум 32 элемента
	ushort *_res,//ответное слово,и слова данных если есть, если нет приходит 0xffff
	bool _event = true)
{
	QVariantList words;
	int os;
	MKOWord tmp_mko_word;
	tmp_mko_word.com_word = _ks;
	if (tmp_mko_word.trans_dir == 0)
		for (int i = 0; i < tmp_mko_word.count_word; i++)
		{
			words << _is[i];
		}
	omnibus_slot_thr.get_omnibus_obj()->send_msg(1, bus_n, _ks, words, os);
	if (os == -1)
	{
		_res[0] = 0xffff;
	}
	else
	{
		_res[0] = os;
		for (int i = 0; i < tmp_mko_word.count_word; i++)
		{
			_res[i+1] = words[i].toInt();
		}
	}
	return 0;
}

void __declspec(dllexport) MkoSend_SetNbus_C(int _n_bus)//выюор канала резервирования МКО (0-основной, 1-резервный)
{
	bus_n = _n_bus;
}