////#ifndef _ADD_TO_THDEB_
////#define _ADD_TO_THDEB_
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
////#include "Dima_Files/Dima_Func.h" ///////	
////extern      UINT      gruppa[];   //Для чтения группы романовских чисел.
////extern int OnLoadRom(void);
////extern int InitKernel();
////extern int CSum_OPO(UINT adrs, UINT adre, UINT* sum, UINT start_sum, UINT crc32);
//
//
//
//CString dim_str = direct;
//dim_str.TrimLeft();
//dim_str.Replace('(', ' ');
//dim_str.Replace(')', ' ');
//dim_str.Replace(',', ' ');
////////////////////////////////
//			int aaa = 0;
//			CString my_s[15];
//			for(int ii = 0; ii<15; ii++)
//			{
//				aaa = dim_str.FindOneOf(" \t");
//				if(aaa == -1)
//				{
//					my_s[ii] = dim_str;
//					break;
//				}
//				my_s[ii] = dim_str.Left(aaa);
//				dim_str.Delete(0, aaa);
//				dim_str.TrimLeft();
//			}
//
////////////////////////////////
	//if(my_s[0].CompareNoCase("loadrom") == 0)
	//{
	//	if(potkl==1)
	//	{
	//		prmdier(direct);
	//		prmdier("#Данное действие возможно только при остановленном процессоре");
	//		continue;
	//	}
	//	
	//	if((my_s[1].IsEmpty())||(!my_s[2].IsEmpty())) {prmdier(Errs); continue;}
	//	prmdi(direct);

	//	OnLoadMro(my_s[1]);
	//	continue;
	//}
//	if(my_s[0].CompareNoCase("pwU") == 0)
//	{
//		if((my_s[1].IsEmpty())||(!my_s[2].IsEmpty())) {prmdier(Errs); continue;}
//		prmdi(direct);
////		pwU(fpere(my_s[1]));
//		continue;
//	}
//	/////////////
//	if(my_s[0].CompareNoCase("pwPut_27") == 0)
//	{
//		if((my_s[1].IsEmpty())||(!my_s[2].IsEmpty())) {prmdier(Errs); continue;}
//		prmdi(direct);
////		pw27(fpere(my_s[1]));
//		continue;
//	}
//	/////////////
//	if(my_s[0].CompareNoCase("scReset")==0)
//	{
//		if(!my_s[1].IsEmpty()){prmdier(Errs); continue;}
//		prmdi(direct);
////		scReset();
//		continue;
//	}
//	////////////
//	if(my_s[0].CompareNoCase("pwPut_Ctr") == 0)
//	{
//		if((my_s[1].IsEmpty())||(!my_s[2].IsEmpty())) {prmdier(Errs); continue;}
//		prmdi(direct);
////		pwCtr(fpere(my_s[1]));
//		continue;
//	}
//	/////////////
//	if(my_s[0].CompareNoCase("ptPut_U") == 0)
//	{
//		
//		if((my_s[1].IsEmpty())||(!my_s[2].IsEmpty())) {prmdier(Errs); continue;}
//
//		prmdi(direct);
////		ptPut_U((float)atof(my_s[1]));
//		continue;
//	}
//	/////////////
//	if(my_s[0].CompareNoCase("ptPut_on") == 0)
//	{	
//		if((my_s[1].IsEmpty())|| (my_s[2].IsEmpty()) ||(!my_s[3].IsEmpty())) {prmdier(Errs); continue;}
//
//		prmdi(direct);
////		ptPut_on(fpere(my_s[1]), fpere(my_s[2]));
//		continue;
//	}
//	/////////////
//	if(my_s[0].CompareNoCase("ptPut_off") == 0)
//	{
//		if((my_s[1].IsEmpty())|| (my_s[2].IsEmpty()) ||(!my_s[3].IsEmpty())) {prmdier(Errs); continue;}
//
//		prmdi(direct);
////		ptPut_off(fpere(my_s[1]), fpere(my_s[2]));
//		continue;
//	}
//	//////////////
//	if(my_s[0].CompareNoCase("ptGet_i0") == 0)
//	{
//		if((my_s[1].IsEmpty())|| (my_s[2].IsEmpty()) ||(!my_s[3].IsEmpty())) {prmdier(Errs); continue;}
//
//		prmdi(direct);
////		gruppa[0] = ptGet_i0(fpere(my_s[1]), fpere(my_s[2]));
//	    	gruppa[0] = 0;
//		continue;
//	}
//	//////////////
//	if(my_s[0].CompareNoCase("ptGet_u1") == 0)
//	{
//		if((my_s[1].IsEmpty())|| (my_s[2].IsEmpty()) ||(!my_s[3].IsEmpty())) {prmdier(Errs); continue;}
//
//		prmdi(direct);
////		gruppa[0] = ptGet_u1(fpere(my_s[1]), fpere(my_s[2]));
////		gruppa[0] = 0;
//		continue;
//	}
//	//////////////
//	if(my_s[0].CompareNoCase("ptGet_flg") == 0)
//	{
//		if(!my_s[1].IsEmpty()){prmdier(Errs); continue;}
//
//		prmdi(direct);
////		gruppa[0] = ptGet_flg();
////		gruppa[0] = 0;
//		continue;
//	}
//	///////////
//	//imPut_on(UINT MapSby, UINT Lin, UINT L, UINT K, UINT U, UINT Cnt);
//	if(my_s[0].CompareNoCase("imPut_on") == 0)
//	{
//				
//		if(my_s[1].IsEmpty() || my_s[2].IsEmpty() || my_s[3].IsEmpty() || my_s[4].IsEmpty() || (my_s[5].IsEmpty()) || (my_s[6].IsEmpty()) || (!my_s[7].IsEmpty()))
//		{prmdier(Errs); continue;}
//		/////////////////////
//		prmdi(direct);
//
////		imPut_on(fpere(my_s[1]), fpere(my_s[2]), fpere(my_s[3]), fpere(my_s[4]), fpere(my_s[5]), fpere(my_s[6]));
//		continue;
//	}
//	///////////////
//	if(my_s[0].CompareNoCase("imPut_off") == 0)
//	{
//		if(!my_s[1].IsEmpty()){prmdier(Errs); continue;}
//
//		prmdi(direct);
////		imPut_off();
//		continue;
//	}
//	////////////////
//	if(my_s[0].CompareNoCase("imGet_on") == 0)
//	{
//		if((my_s[1].IsEmpty())|| (my_s[2].IsEmpty()) ||(!my_s[3].IsEmpty())) {prmdier(Errs); continue;}
//
//		prmdi(direct);
////		imGet_on(fpere(my_s[1]), fpere(my_s[2]));
//		continue;
//	}
//	////////////////
//	if(my_s[0].CompareNoCase("imGet_off") == 0)
//	{
//		if(!my_s[1].IsEmpty()){prmdier(Errs); continue;}
//		prmdi(direct);
////		gruppa[0] = imGet_off();
//		gruppa[0] = 0;
//		continue;
//	}
//	////////////////
//	if(my_s[0].CompareNoCase("pko") == 0)
//	{
//		if((my_s[1].IsEmpty())|| (my_s[2].IsEmpty()) ||(!my_s[3].IsEmpty())) {prmdier(Errs); continue;}
//
//		UINT rs_ch = fpere(my_s[2]);
//		UINT tr_ch = fpere(my_s[1]);
//		if(tr_ch > 7){prmdier(Errs); continue;}
//		if((rs_ch != 0)&&(rs_ch != 1)&&(rs_ch != 2)&&(rs_ch != 4)) {prmdier(Errs); continue;}
//		prmdi(direct);
////		pko(tr_ch, rs_ch);
//		continue;
//	}
//	//////////////////
//	if(my_s[0].CompareNoCase("mkslinInit") == 0)
//	{
//		if(my_s[1].IsEmpty() || my_s[2].IsEmpty() || my_s[3].IsEmpty() || my_s[4].IsEmpty() || (my_s[5].IsEmpty()) || (my_s[6].IsEmpty()))
//		{prmdier(Errs); continue;}
//		/////////////////////
//		prmdi(direct);
//
////		mkslinInit(fpere(my_s[1]), fpere(my_s[2]), fpere(my_s[3]), fpere(my_s[4]), fpere(my_s[5]), fpere(my_s[6]),
////												   fpere(my_s[7]), fpere(my_s[8]), fpere(my_s[9]), fpere(my_s[10]));
//		///??????????????????
//		continue;
//	}
//	//////////////////
//	if(my_s[0].CompareNoCase("Pause") == 0)
//	{
//		CString TextWin = "Для продолжения нажми кнопку ОК или клавишу Enter", 
//				TextHead = "Приостанов";
//		//////////
//		CString tmp_pause = direct;
//		CString my_s1[3] = {"","",""};
//		tmp_pause.TrimLeft();
//
//		///////////
//		int bbb = 0, ccc = 0;
//					
//		bbb = tmp_pause.Find("(");
//		ccc = tmp_pause.Find(")");
//		if(ccc > bbb)
//		{
//			tmp_pause = tmp_pause.Mid(bbb+1, ccc-bbb-1);
//			for(int rr = 0; rr<3; rr++)
//			{
//				bbb = tmp_pause.Find(",");
//				if(bbb == -1)
//				{
//					my_s1[rr] = tmp_pause;
//					break;
//				}
//				my_s1[rr] = tmp_pause.Left(bbb);
//				tmp_pause.Delete(0, bbb+1);
//				tmp_pause.TrimLeft();
//
//			}
//		}
//		////////////////////////////
//		UINT type_pause = 0;
//		if(!my_s1[0].IsEmpty())
//		{
//			type_pause  = fpere(my_s1[0]);
//			//////
//			if(!my_s1[1].IsEmpty()) TextWin = my_s1[1];
//			if(!my_s1[2].IsEmpty()) TextHead = my_s1[2];
//			//////
//		}
//		/////////////////////////////////////////////
//		switch(type_pause)
//		{
//		case 0: {type_pause =  MB_OK|MB_ICONINFORMATION; break;}
//		case 1: {type_pause =  MB_YESNO|MB_ICONINFORMATION; break;}
//		case 2: {type_pause =  MB_YESNOCANCEL|MB_ICONINFORMATION; break;}
//		case 3: {type_pause =  MB_OKCANCEL|MB_ICONINFORMATION; break;}
//		};
//
//
//		int out = ::MessageBox(NULL, TextWin, TextHead, type_pause);
////		switch(out)
////		{
////		case IDCANCEL: {gruppa[0]  = -1; break;}
////		case IDNO: {gruppa[0]  = 0; break;}
////		case IDYES:
////		case IDOK:	{gruppa[0]  = 1; break;};		
////		};
//
//		//////////
//		continue;
//	}
//	/////////////////////////////
//	if(my_s[0].CompareNoCase("pwGet_U") == 0)
//	{
//		if(!my_s[1].IsEmpty())
//		{prmdier(Errs); continue;}
//		/////////////////////
//		prmdi(direct);
//		CString out_s;
////		double vv = Get_Curr_Pit();
////		out_s.Format("#Напряжение: %.f, В",vv);
////		prmdi(out_s);
////		gruppa[0]  = vv*1000;
////		gruppa[0] = 0;
//	
//		continue;
//	}
//	//////////////////////////////
//	if(my_s[0].CompareNoCase("pwGet_I") == 0)
//	{	
//		if(!my_s[1].IsEmpty())
//		{prmdier(Errs); continue;}
//		/////////////////////
//		prmdi(direct);
//		CString out_s;
////		double aa = Get_Curr_I();
//		//out_s.Format("#Напряжение: %.f, В",vv);
//		//prmdi(out_s);
////		gruppa[0]  = aa*1000;
////		gruppa[0] = 0;
//		continue;
//	}
//	/////////////////////////////////
//	if(my_s[0].CompareNoCase("pwGet_P") == 0)
//	{	
//		if(!my_s[1].IsEmpty())
//		{prmdier(Errs); continue;}
//		/////////////////////
//		prmdi(direct);
////		CString out_s;
////		double aa = Get_Curr_I();
////		double vv = Get_Curr_Pit();
//		//////
//// 		union
//// 		{
//// 			float ff;
//// 			UINT  UI;
//// 		}uu;
//		/////
////		uu.ff = (float)(aa*vv);
//		//////
////		if(uu.ff < 0) uu.ff = 0;
//		//gruppa[0]  = (int)(aa*vv*1000);
////		gruppa[0] = uu.UI;
////		gruppa[0] = 0;
//		continue;
//	}
//	/////////////
//	if(my_s[0].CompareNoCase("hold") == 0)
//	{
//		if((my_s[1].IsEmpty())||(!my_s[2].IsEmpty())) {prmdier(Errs); continue;}
//		prmdi(direct);
////		hold(fpere(my_s[1]));
//		continue;
//	}
//	////////////////////////////////////////////////////
//	if(my_s[0].CompareNoCase("CSum") == 0)
//	{
//		CString rr;
//		int iVM;
//		if((my_s[1].IsEmpty()) || (my_s[2].IsEmpty()) || (my_s[3].IsEmpty()) || (!my_s[4].IsEmpty())) {prmdier(Errs); continue;}
//
//		//int CSum_OPO(UINT adrs, UINT adre, UINT* sum, UINT start_sum, UINT crc32);
//		prmdi(direct);
//		for(iVM=1;iVM<=kolwm;iVM++)
//		{
//			if(VektVM[iVM]==0) continue;
//			
//		if(CSum_OPO(fpere("0x"+my_s[1]), fpere("0x"+my_s[2]), gruppa, fpere("0x"+my_s[3]), /*1*/0,iVM))
//		{
//			prmdier(Errs); continue;
//		}
//	rr.Format("#SumVM%d=0x%X",(iVM-1),gruppa[0]);prmdi(rr);
//	if(iVM==1) gruppa1[0]=gruppa[0];
//	if(iVM==2) gruppa2[0]=gruppa[0];
//	if(iVM==3) gruppa3[0]=gruppa[0];
//	if(iVM==4) gruppa4[0]=gruppa[0];
//		continue;
//		}
//		continue;
//	}
//	//////////////////
//
//	if(my_s[0].CompareNoCase("Start_PKM_Dll") == 0)
//	{
//		if(potkl==1)
//		{
//			prmdier(direct);
//			prmdier("#Данное действие возможно только при остановленном процессоре");
//			continue;
//		}
//		
//		if(my_s[1].IsEmpty())
//		{
//			CFileDialog filrezk(TRUE);
//			TCHAR titlek[]="     Выбор  файла ";
//			CString fstrokk="Все файлы (*.*)";
//			filrezk.m_ofn.lpstrTitle=titlek;
//			fstrokk+=(TCHAR)NULL;fstrokk+=(TCHAR)NULL;
//			filrezk.m_ofn.lpstrFilter=fstrokk;
//			filrezk.m_ofn.Flags|=OFN_NOCHANGEDIR;
//			
//			filrezk.DoModal();
//
//			my_s[1]=filrezk.m_ofn.lpstrFile;
//			direct +=" "+my_s[1];
//		}
//
//		int jn,i,j;
//
//	    dlln=0; //Пока нет DLL
//		wkltakt=0; //Тактовое разбуж.не включено.
//		//--- Определение наличия DLL/тогда dlln=1/
//		//--- или отсутствия/dlln=0/ DLL.          Олег !
//		//------------------------------------------------------
//        jn=0;
//		pRegTable = &RegTable;
//		pRegTable->pTextToFile = &printklp; 
//		pRegTable->pTextToWindow = &printklp; 
//		if ((dlln = (short)load_pkm_dll_2(my_s[1])) != 0)
//		                           // Загрузка PKM.DLL
//		{		
//			//-- Определение максимального идентификационного
//			//--   номера модели./jn/                  Олег !
//	        jn=0;      
//			for (i = 0; pModTable[i].ID != 0; i++)
//			{
//				if ((UINT)pModTable[i].ID >(UINT) jn) jn = pModTable[i].ID;
//			}
//		}
//
//		if(jn>150)
//		{printkl("# Большое кол-во моделей. Отказ.");continue;}
//		
//		//здесь должна быть перенастройка модельных будильников
//	
//		//-- Формирование структуры будильника под состав моделей:
//    
//		for(i=0;i<1600;i++)      wus[i]=0;  //Чистка
//       
//		for(i=1;i<=kolwm;i++)
//		{   
//			bazuk[i]->pak0=0; bazuk[i]->pak1=0; bazuk[i]->pak2=0;
//			bazuk[i]->pak3=0; bazuk[i]->pak4=0; bazuk[i]->pak5=0;
//		}
//
//      //--------- Учредим 6 КМК и 6 ВМ в будильнике ---------                    
//	    for(i=0;i<1600;i++)  wus[i]=0;   //Общая его чистка
//        //----------------------------------------------------
//		for(i=1;i<=6;i++)      //Цикл по ВМ,  i - мой номер ВМ
//		{
//			for(j=0;j<=5;j++)    //Цикл по КМК, j - беликовский номер
//			{
//				int ii=(i-1)*48; ii=ii+j*8;    //Начальный индекс восьмерки в wus[].
//				wus[ii]=1;   //признак KMK
//				wus[ii+1]=j; //номер КМК
//				wus[ii+2]=0; //пока не активен
//				wus[ii+5]=i; //мой номер ВМ
//			}
//		}// -------с ВМ и КМК разделались
//      //---------------------  Теперь модели с 288 индекса в wus[].
//        if(jn>0)   //Есть модели
//		{
//			j=0;
//			int ii;
//			for(i=288;  ;i=i+8)
//			{
//				wus[i]=2;    //признак модели
//				wus[i+1]=j;  //номер этой модели
//				j=j+1;
//				if(j>jn) {ii=i; break;}
//			}
//			wus[ii+10]=3;  //Конец будильника с моделями.
//		}
//		else     //нет моделей
//			wus[290]=3;	         //Конец будильника без моделей
//		                          
//		Tmin=0xfffffffffffffff;  //Минимальное время в пустом будильнике
//	
//		if(dlln!=0)//НАЧАЛЬНЫЙ установ моделей
//		{
//			for(int i=0; pModTable[i].ID!=0;i++)
//			{ 
//				if(pModTable[i].Reason==REASON_INIT)
//				{
//					vxod= (UINT *)pModTable[i].pInputBuffer;
//					vixod=(UINT *)pModTable[i].pOutputBuffer;
//					ident= (UINT)pModTable[i].ID;
//					ukf=pModTable[i].Subrut;
//					vxod[0]=0; 
//					(*ukf)( ); 
//					pes=0;
//					zakaz(0);                        
//					pes=1;
//				}		  
//			}
//		}//---- КОНЕЦ начальной установки моделей ---------------
//	
//
//		prmdi(direct);
//		continue;
//	}
//
//	/////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
////#endif