#include "BTICARD/BufferClass.h"


int rpc_buffer_class::get_msgs_size()
{
	QMutexLocker lock(&msg_mutex);
	return msgs.count();
}
SEQRECORD1553* rpc_buffer_class::get_msg()
{
	QMutexLocker lock(&msg_mutex);
	last_msg = msgs.takeFirst();
	return &last_msg;
}

SEQRECORDMORE1553* rpc_buffer_class::get_more()
{
	QMutexLocker lock(&msg_mutex);
	more_msg = msgs_more.takeFirst();
	return &more_msg;
}

void rpc_buffer_class::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	QMutexLocker lock(&msg_mutex);

	MkoWord tmp_cwd;
	tmp_cwd.cw = cwd;
	SEQRECORD1553 tmp_rec;
	SEQRECORDMORE1553 tmp_rec_more;

	tmp_rec.timestamp = dt.toLongLong() & 0x00000000FFFFFFFF;
	tmp_rec_more.timestamph = (dt.toLongLong() & 0xFFFFFFFF00000000) >> 32;
	tmp_rec.activity = MSGACT1553_RCVCWD1 | (mko << MSGACT1553_CHSHIFT) | ((line == 1) ? MSGACT1553_BUS : 0) | ((os == -1) ? 0 : MSGACT1553_RCVSWD1);
	tmp_rec.cwd1 = cwd;
	if (os != -1)
	{
		tmp_rec.swd1 = os;
		tmp_rec.error = 0;
	}
	else
	{
		tmp_rec.swd1 = 0xFF;
		tmp_rec.error = MSGERR1553_ANYERR | MSGERR1553_NORESP;
	}

	tmp_rec.datacount = words.count();

	for (int i = 0; i < words.count(); i++)
	{
		tmp_rec.data[i] = words[i].toInt();
	}
	msgs.push_back(tmp_rec);
	msgs_more.push_back(tmp_rec_more);
}

