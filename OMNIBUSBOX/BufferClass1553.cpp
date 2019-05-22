#include "BufferClass1553.h"


int rpc_buffer_class::create_msg_addr(int addr, int saddr, int mko)
{
	MsgAddr tmp_msg;
	tmp_msg.addr = addr;
	tmp_msg.saddr = saddr;
	tmp_msg.mko = mko;
	msg_ind++;
	msg_addrs.insert(msg_ind, tmp_msg);
	return msg_ind;
}

int rpc_buffer_class::create_list_addr(int addr, int saddr, int mko, int list_size, int tr)
{
	LstAddr tmp_lst;
	tmp_lst.addr = addr;
	tmp_lst.saddr = saddr;
	tmp_lst.mko = mko;
	tmp_lst.words = QVariantList();
	tmp_lst.lst_size = list_size;

	MkoWord tmp_KS;
	tmp_KS.adr = addr;
	tmp_KS.subadr = saddr;
	tmp_KS.count = 0;
	tmp_KS.tr = tr;
	QMutexLocker lock(&msg_mutex);
	lst_addrs[tmp_KS.cw] = tmp_lst;
	return tmp_KS.cw;
}

void rpc_buffer_class::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	if (os == -1)
		return;

	MkoWord tmp_cwd;
	tmp_cwd.cw = cwd;
	tmp_cwd.count = 0;

	QMutexLocker lock(&msg_mutex);

	QMap<int, LstAddr>::iterator itr = lst_addrs.find(tmp_cwd.cw);
	if (itr == lst_addrs.end())
		return;

	itr->words << QVariant(words);
	if (itr->words.count() > itr->lst_size)
		itr->words.pop_front();
}

