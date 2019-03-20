#include "KPRD_imitator.h"
//#include "kprd_socket_rpc.h"

KPRD_imitator::KPRD_imitator()  
{
	widg = new QWidget(this);
	setCentralWidget(widg);
	setWindowTitle("Имитатор КПРД");

	pause_btn = new QPushButton("П", widg);
	pause_btn->setFixedWidth(150);
	pause_btn->setFixedHeight(100);
	zero_btn = new QPushButton("0", widg);
	zero_btn->setFixedWidth(150);
	zero_btn->setFixedHeight(100);
	one_btn = new QPushButton("1", widg);
	one_btn->setFixedWidth(150);
	one_btn->setFixedHeight(100);

	generetors_label = new QLabel("Генераторы", widg);
	pause_kod_label = new QLabel("Код: ", widg);
	zero_kod_label = new QLabel("Код: ", widg);
	one_kod_label = new QLabel("Код: ", widg);
	antenna_label = new QLabel("Антенна: ", widg);
	attenuation_label = new QLabel("Ослабление: ", widg);

	log_edit = new QTextEdit(widg);
	log_edit->setReadOnly(true);
	_scroll_bar = new QScrollBar();
	_scroll_bar = log_edit->verticalScrollBar();

	gridLayout = new QGridLayout(widg);

	gridLayout->addWidget(generetors_label, 0, 0, 1, 3, Qt::AlignHCenter);
	gridLayout->addWidget(pause_btn, 1, 0);
	gridLayout->addWidget(zero_btn, 1, 1);
	gridLayout->addWidget(one_btn, 1, 2);
	gridLayout->addWidget(pause_kod_label, 2, 0);
	gridLayout->addWidget(zero_kod_label, 2, 1);
	gridLayout->addWidget(one_kod_label, 2, 2);
	gridLayout->addWidget(antenna_label, 3, 0);
	gridLayout->addWidget(attenuation_label, 4, 0);
	gridLayout->addWidget(log_edit, 5, 0, 1, 3);


	qulonglong u1 = 0x80FFF02000700;
	qulonglong u2 = 0xFFFFFFFFFFFFF;

	QVariantList maskListTest;
	maskListTest << 0x70FFF01000007 << 0x70FFF01000007 << 0x70FFF01000007 << 0x70FFF01000007 << 0x70FFF01000007 << 0x70FFF01000007;
	maskListTest << 0x70FFF02000700 << 0x70FFF02000700 << 0x70FFF02000700 << 0x70FFF02000700 << 0x70FFF02000700 << 0x70FFF02000700;
	maskListTest << 0x70FFF04070000 << 0x70FFF04070000 << 0x70FFF04070000 << 0x70FFF04070000 << 0x70FFF04070000 << 0x70FFF04070000;
	QVariantList dataListTest;
	dataListTest << 0xFFFFFFFFFFFFB << 0xFFFFFFFFFFFFB << 0xFFFFFFFFFFFFB << 0xFFFFFFFFFFFFB << 0xFFFFFFFFFFFFB << 0xFFFFFFFFFFFFB;
	dataListTest << 0xFFFFFFFFFFBFF << 0xFFFFFFFFFFBFF << 0xFFFFFFFFFFBFF << 0xFFFFFFFFFF9FF << 0xFFFFFFFFFF9FF << 0xFFFFFFFFFFBFF;
	dataListTest << 0xFFFFFFFF9FFFF << 0xFFFFFFFF9FFFF << 0xFFFFFFFFBFFFF << 0xFFFFFFFFBFFFF << 0xFFFFFFFFBFFFF << 0xFFFFFFFFBFFFF;

	connect(this, &KPRD_imitator::test, this, &KPRD_imitator::dataIn);
	emit test(maskListTest, dataListTest);

}

KPRD_imitator::~KPRD_imitator()
{
	delete pause_btn;
	delete zero_btn;
	delete one_btn;
	delete generetors_label;
	delete pause_kod_label;
	delete zero_kod_label;
	delete one_kod_label;
	delete antenna_label;
	delete attenuation_label;
	delete _scroll_bar;
	delete gridLayout;
	delete widg;

}

void KPRD_imitator::set_antenna_label(qulonglong val)
{
	switch (val)
	{
		case 14: antenna_label->setText("Антенна: МНА2+Y");
			break;
		case 13: antenna_label->setText("Антенна: МНА1-Y");
			break;
		case 11: antenna_label->setText("Антенна: МНА2+Y");
			break;
		case 7: antenna_label->setText("Антенна: МНА2-Y");
			break;
	default: antenna_label->setText("Антенна: ");
		break;
	}
}

bool KPRD_imitator::isset(qulonglong x, qulonglong n)
{
	return (x & ((qulonglong)1 << n)) != 0;
}


/*	
	FREQ_P_CLOCK    = 0,
	FREQ_P_DATA     = 1,
	FREQ_P_ENABLE   = 2,	// сброс


	FREQ_0_CLOCK    = 8,
	FREQ_0_DATA     = 9,
	FREQ_0_ENABLE   = 10,

	FREQ_1_CLOCK    = 16,
	FREQ_1_DATA     = 17,
	FREQ_1_ENABLE   = 18,

	KPI_P           = 24,
	KPI_0           = 25,
	KPI_1           = 26,
	KPI_R           = 27,

	ATTEN_1_1       = 32,
	ATTEN_1_2       = 33,
	ATTEN_1_4       = 34,
	ATTEN_1_8       = 35,
	ATTEN_1_16      = 36,
	ATTEN_1_32      = 37,

	ATTEN_2_1       = 38,
	ATTEN_2_2       = 39,
	ATTEN_2_4       = 40,
	ATTEN_2_8       = 41,
	ATTEN_2_16      = 42,
	ATTEN_2_32      = 43,

	MNA2_PLUS_U     = 48,
	MNA1_MINUS_U    = 49,
	MNA1_PLUS_U     = 50,
	MNA2_MINUS_U    = 51
*/

void KPRD_imitator::dataIn(QVariantList maskList, QVariantList dataList)
{
	if (maskList.isEmpty() || dataList.isEmpty())
		return;
	KPIString = "";
	for (auto const& i : boost::combine(maskList, dataList)) // range based
	{
		QVariant MASKVar, DATAVar;
		boost::tie(MASKVar, DATAVar) = i;
		qulonglong MASK = MASKVar.toULongLong();
		qulonglong DATA = DATAVar.toULongLong();
		qulonglong res = MASK & DATA;

		auto val1 = (res << 26 >> 58);
		auto val2 = (res << 20 >> 58);
		qulonglong attenuation_val = val1 + val2;
		attenuation_label->setText("Ослабление: " + QString::number(attenuation_val));
		qulonglong antenna_name = res << 12 >> 60;
		set_antenna_label(antenna_name);

		if (isset(MASK, 2)) // FREQ_P
		{
			auto FREQ_ENABLE = &FREQ_P_ENABLE;
			auto FREQ_CLOCK = &FREQ_P_CLOCK;
			auto FREQ_DATA = &FREQ_P_DATA;
			auto FREQ_code = &FREQ_P_code;
			auto kod_label = pause_kod_label;

			if (isset(res, 2))	// КОД обнуляется
				*FREQ_code = 0;

			if (!isset(res, 2))	// FREQ_1_ENABLE
			{
				*FREQ_ENABLE = true;

				if (isset(res, 0))
				{
					*FREQ_CLOCK = isset(res, 0);
					*FREQ_DATA = isset(res, 1);
					*FREQ_code = (*FREQ_code << 1) + *FREQ_DATA;
					kod_label->setText("Код: " + QString::number(*FREQ_code));
					if(isset(res, 24))
						KPIString += "P";
				}
			}
		}

		if (isset(MASK, 10)) // FREQ_0
		{
			auto FREQ_ENABLE = &FREQ_0_ENABLE;
			auto FREQ_CLOCK = &FREQ_0_CLOCK;
			auto FREQ_DATA = &FREQ_0_DATA;
			auto FREQ_code = &FREQ_0_code;
			auto kod_label = zero_kod_label;

			if(isset(res, 10))	// КОД обнуляется
				*FREQ_code = 0;

			if (!isset(res, 10))	// FREQ_0_ENABLE
			{
				*FREQ_ENABLE = true;
				if (isset(res, 8))
				{
					*FREQ_CLOCK = isset(res, 8);
					*FREQ_DATA = isset(res, 9);
					*FREQ_code = (*FREQ_code << 1) + *FREQ_DATA;
					kod_label->setText("Код: " + QString::number(FREQ_0_code));
					if (isset(res, 25))
						KPIString += "0";
				}
			}
		}

		if (isset(MASK, 18)) // FREQ_1
		{
			auto FREQ_ENABLE = &FREQ_1_ENABLE;
			auto FREQ_CLOCK = &FREQ_1_CLOCK;
			auto FREQ_DATA = &FREQ_1_DATA;
			auto FREQ_code = &FREQ_1_code;
			auto kod_label = one_kod_label;

			if (isset(res, 18))	// КОД обнуляется
				*FREQ_code = 0;

			if (!isset(res, 18))	// FREQ_1_ENABLE
			{
				*FREQ_ENABLE = true;

				if (isset(res, 16))
				{
					*FREQ_CLOCK = isset(res, 16);
					*FREQ_DATA = isset(res, 17);
					*FREQ_code = (*FREQ_code << 1) + *FREQ_DATA;
					kod_label->setText("Код: " + QString::number(*FREQ_code));
					if (isset(res, 26))
						KPIString += "1";
				}
			}
		}
		log_edit->setText("Выдано: " + KPIString + "\n");
	}	// for
}
