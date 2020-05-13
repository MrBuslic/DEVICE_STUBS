#include "PKM.h"

//extern HWND m_W;
extern UCHAR bufpr[], bufper[];
extern UINT mul_ind[];

/* ELF Header (32-bit implementations) */

typedef struct {
  UCHAR	e_ident[16];		/* ELF "magic number" */
  USHORT e_type;		/* Identifies object file type */
  USHORT	e_machine;		/* Specifies required architecture */
  UINT	e_version;		/* Identifies object file version */
  UINT	e_entry;		/* Entry point virtual address */
  UINT	e_phoff;		/* Program header table file offset */
  UINT	e_shoff;		/* Section header table file offset */
  UINT	e_flags;		/* Processor-specific flags */
  USHORT	e_ehsize;		/* ELF header size in bytes */
  USHORT	e_phentsize;		/* Program header table entry size */
  USHORT	e_phnum;		/* Program header table entry count */
  USHORT	e_shentsize;		/* Section header table entry size */
  USHORT	e_shnum;		/* Section header table entry count */
  USHORT	e_shstrndx;		/* Section header string table index */
} Elf32_External_Ehdr;


/* Program header */

//typedef struct {
//  UINT	p_type;		/* Identifies program segment type */
//  UINT	p_offset;		/* Segment file offset */
//  UINT	p_vaddr;		/* Segment virtual address */
//  UINT	p_paddr;		/* Segment physical address */
//  UINT	p_filesz;		/* Segment size in file */
//  UINT	p_memsz;		/* Segment size in memory */
//  UINT	p_flags;		/* Segment flags */
//  UINT	p_align;		/* Segment alignment, file & memory */
//} Elf32_External_Phdr;

/* Section header */

typedef struct {
  UINT	sh_name;		/* Section name, index in string tbl */
  UINT	sh_type;		/* Type of section */
  UINT	sh_flags;		/* Miscellaneous section attributes */
  UINT	sh_addr;		/* Section virtual addr at execution */
  UINT	sh_offset;		/* Section file offset */
  UINT	sh_size;		/* Size of section in bytes */
  UINT	sh_link;		/* Index of another section */
  UINT	sh_info;		/* Additional section information */
  UINT	sh_addralign;	/* Section alignment */
  UINT	sh_entsize;		/* Entry size if section holds table */
} Elf32_External_Shdr;



extern UCHAR bufch[];
extern int WBufCom(UINT adr, int len, UCHAR *buf);
extern QStringList name_regs_st;
extern QStringList name_regs;
extern QStringList name_fregs;
extern UINT adr_contact;
extern UINT adr_maila;
extern UINT adr_maill;
extern Srom lrom[];
extern void prmdi(QString str);
extern UINT fpere(QString str);
extern void prmdier(QString str);
int AdrFromIdent(QString im, UINT &adr,QString& e_im);
int DiapFromAdrRom(int ind, UINT adr,UINT& adr_n,UINT& adr_k);
int OnFindDiap(UINT adr,UINT &adr_ns,UINT& adr_ks);
int NomToAdrRom(int ind, QString str, UINT& adr);
int NomToAdr( QString str, UINT& adr);
int LoadObj(int i,QString name);
Elf32_External_Ehdr headf;
Elf32_External_Shdr* pShdr;
extern UINT mul_tip[];
int last_ind=0;
QString last_file="", last_select="";
int FindIncludeSym(int ind, QString str);
int AttachObj(int i,QString name);
int AdrFromSymAbc(int ind, QString str, UINT& adr);
int FindLocalSym(int ind, QString str, UINT& adr);
int LineFromAdr(UINT adr, QString &str);
int LineFromAdrRom(int ind, UINT adr,QString& str);
int NomToAdrRom(int ind, UINT adr,QString& str);
int AdrFromSymRom(int ind, QString str, UINT& adr);
int MetFromAdr(UINT adr, QString &str);
int last_nom;
int MetFromAdrRom(int ind, QString &str, UINT adr);
int FindInclude(int ind, QString str, UINT& adr);
int FunFromAdrRom(int ind, UINT adr,QString& str,int& ost,
				 Elf32_stab*& pstab,UCHAR* &pstabs );

/////////////////////////////////////////////////////////////
/// Подключение .о файла к .rom, i-индекс в таблице,
/// name-полное имя .rom-файла
int AttachObj(int i,QString name)
{
	QString otl,out1, namob, str1;
	UINT lt,adrs;
	
		UCHAR sn[200];

	namob=name;
	QFile file(namob);
	if(!file.open(QIODevice::ReadOnly))
	{
		prmdier(QString("# Ошибка открытия файла %1").arg(namob));
		  return(1);
	}
	char forELF[6];

	file.read(&(forELF[0]),4);
	if((forELF[1]!=QChar('E'))||(forELF[2]!=QChar('L'))
		||(forELF[3]!=QChar('F')))
	{
		file.close();
		prmdier("#  этот файл не объектный");
		return(1);
	}
	file.seek(0);	//
	file.read((char*)(&headf),sizeof(Elf32_External_Ehdr));
	pShdr=(Elf32_External_Shdr*)calloc(headf.e_shnum, headf.e_shentsize);
	lrom[i].start=headf.e_entry;	// entry point


	 // чтение заголовков секций
	file.seek(headf.e_shoff);	// start Section Header Table
	file.read((char*)(pShdr),headf.e_shnum * headf.e_shentsize);
	file.seek((pShdr+headf.e_shstrndx)->sh_offset);	// start Section Names
	file.read((char *)&sn[0],(pShdr+headf.e_shstrndx)->sh_size);
	for(USHORT is=1; is< headf.e_shnum; is++)
	{
		str1=(const char*)(&sn[(pShdr+is)->sh_name]);
	////////////////////////////////////
		if(str1==".text")
			{
			if((pShdr+is)->sh_size==0) continue;
				file.seek((pShdr+is)->sh_offset);	
			lrom[i].ftext=(pShdr+is)->sh_addr;
			lrom[i].etext=lrom[i].ftext+(pShdr+is)->sh_size;
			lt=lrom[i].etext-lrom[i].ftext;	
		adrs=lrom[i].ftext;
			}
///////////////////////////////////////////
			if(str1==".stab")
			{
		
			lrom[i].pstab=(Elf32_stab*)malloc((pShdr+is)->sh_size);
			if(lrom[i].pstab==NULL)
			{
				prmdier("# Не хватает памяти РС для загрузки отладочной информации");
				prmdier("# если есть загруженные приложения, закройте их ");
				lrom[i].rom="";file.close(); return(1);
			}
			file.seek((pShdr+is)->sh_offset);	
			file.read((char*)lrom[i].pstab,(pShdr+is)->sh_size);
			lrom[i].lstab=(pShdr+is)->sh_size;
			continue;
			}

			if(str1==".stabstr")
			{
		
			lrom[i].pstabst=(UCHAR*)malloc((pShdr+is)->sh_size);
			if(lrom[i].pstabst==NULL)
			{
				prmdier("# Не хватает памяти РС для загрузки отладочной информации");
				prmdier("# если есть загруженные приложения, закройте их ");
				lrom[i].rom="";file.close();return(1);
			}
			file.seek((pShdr+is)->sh_offset);	
			file.read((char*)lrom[i].pstabst,(pShdr+is)->sh_size);
			lrom[i].lstabst=(pShdr+is)->sh_size;
			continue;
			}

			if(str1==".symtab")
			{
		
			lrom[i].psym=(Elf32_External_Sym*)malloc((pShdr+is)->sh_size);
			if(lrom[i].psym==NULL)
			{
				prmdier("# Не хватает памяти РС для загрузки отладочной информации");
				prmdier("# если есть загруженные приложения, закройте их ");
				lrom[i].rom="";file.close();return(1);
			}
			file.seek((pShdr+is)->sh_offset);	
			file.read((char*)lrom[i].psym,(pShdr+is)->sh_size);
			lrom[i].lsym=(pShdr+is)->sh_size;
			continue;
			}

			if(str1==".strtab")
			{
		
			lrom[i].pstrtab=(UCHAR*)malloc((pShdr+is)->sh_size);
			if(lrom[i].pstrtab==NULL)
			{
				prmdier("# Не хватает памяти РС для загрузки отладочной информации");
				prmdier("# если есть загруженные приложения, закройте их ");
				lrom[i].rom="";file.close();return(1);
			}
			file.seek((pShdr+is)->sh_offset);	
			file.read((char*)lrom[i].pstrtab,(pShdr+is)->sh_size);
			lrom[i].lstrtab=(pShdr+is)->sh_size;
			continue;
			}
	 }
	free(pShdr);
	 return (0);
}


///////////////////////////////////////////////////////////////////////////
int LoadObj(int i,QString name)
{
	QString otl, out1, namob, str1;
	UINT a,lt,adrs,len;
	int b;
		UCHAR sn[200];
		int kolp;
	//CWaitCursor m7;

	namob=name;
	QFile file(namob);

	if(!file.open(QIODevice::ReadOnly))
	{
		out1 = QString("# Ошибка открытия файла %1").arg(namob);
		prmdier(out1);
		  return(1);
	}
/////////////////////////
	UCHAR forELF[6];

	file.read((char*)&forELF,4);
	if((forELF[1]!=QChar('E'))||(forELF[2]!=QChar('L'))
		||(forELF[3]!=QChar('F')))
	{
		file.close();
		prmdier("#  этот файл не объектный");
		return(1);
	}
	file.seek(0);	//
////////////////////////////////
	file.read((char*)&headf,sizeof(Elf32_External_Ehdr));
	pShdr=(Elf32_External_Shdr*)calloc(headf.e_shnum, headf.e_shentsize);
	lrom[i].start=headf.e_entry;	// entry point

	 // чтение заголовков секций
	file.seek(headf.e_shoff);	// start Section Header Table
	file.read((char*)pShdr,headf.e_shnum * headf.e_shentsize);
	file.seek((pShdr+headf.e_shstrndx)->sh_offset);	// start Section Names
	file.read((char*)&sn[0],(pShdr+headf.e_shstrndx)->sh_size);
	for(USHORT is=1; is< headf.e_shnum; is++)
	{
		str1=(const char*)(&sn[(pShdr+is)->sh_name]);
	////////////////////////////////////
		if(str1==".text")
		{
			if((pShdr+is)->sh_size==0) continue;
			otl= QString("# секция .text: 0x%1-0x%2: 3 байт").arg((pShdr + is)->sh_addr, 8, 16, QChar('0')).arg((pShdr + is)->sh_addr + (pShdr + is)->sh_size, 8, 16, QChar('0')).arg((pShdr+is)->sh_size);
			prmdi(otl);
			file.seek((pShdr+is)->sh_offset);	
			lrom[i].ftext=(pShdr+is)->sh_addr;
			lrom[i].etext=lrom[i].ftext+(pShdr+is)->sh_size;
			lt=lrom[i].etext-lrom[i].ftext;	
			adrs=lrom[i].ftext;
			for(kolp=1;lt!=0;kolp++)
			{
				if(lt>512) { len=512; lt=lt-512;}
				else { len=lt; lt=0;}
				a=file.read((char*)bufch,len);
				if(a<len)
				{
					out1 = QString("# Ошибка в задании размеров секций; text");
					prmdier(out1);lrom[i].rom=""; 
					file.close(); return(1);
				}
		
				b=WBufCom(adrs,len,bufch);
		
				if(b)
				{
					file.close(); lrom[i].rom=""; return(1);
				}

				adrs=adrs+len;
			}
			continue;
		}
///////////////////////////////////////////////////////////////
		if(str1==".data")
		{
			if((pShdr+is)->sh_size==0) continue;
			otl = QString("# секция .data: 0x%1-0x%2: %3 байт").arg((pShdr+is)->sh_addr, 8, 16, QChar('0')).arg((pShdr+is)->sh_addr+(pShdr+is)->sh_size, 8, 16, QChar('0')).arg((pShdr+is)->sh_size);
			prmdi(otl);
			file.seek((pShdr+is)->sh_offset);	
			lrom[i].fdata=(pShdr+is)->sh_addr;
			lrom[i].edata=lrom[i].fdata+(pShdr+is)->sh_size;
			lt=lrom[i].edata-lrom[i].fdata;	
			adrs=lrom[i].fdata;
			for(kolp=1;lt!=0;kolp++)
			{
				if(lt>512) { len=512; lt=lt-512;}
				else { len=lt; lt=0;}
				a=file.read((char*)bufch,len);
				if(a<len)
				{
					out1 = QString("# Ошибка в задании размеров секций; data");
					prmdier(out1);lrom[i].rom=""; 
					file.close(); return(1);
				}
		
				b=WBufCom(adrs,len,bufch);
				if(b)
				{
					file.close(); lrom[i].rom=""; return(1);
				}

				adrs=adrs+len;
			}
			continue;
		}
///////////////////////////////////////////////////////////////
		if((str1==".rodata")||(str1==".sdata")
				||(str1==".rodata1")||(str1==".data1"))
		{
			if((pShdr+is)->sh_size==0) continue;
			otl = QString("# секция %1: 0x%2-0x%3: %4 байт").arg(str1).arg((pShdr+is)->sh_addr, 8, 16, QChar('0')).arg((pShdr+is)->sh_addr+(pShdr+is)->sh_size, 8, 16, QChar('0')).arg((pShdr+is)->sh_size);
			prmdi(otl);
			file.seek((pShdr+is)->sh_offset);	
			lt=(pShdr+is)->sh_size;
			adrs=(pShdr+is)->sh_addr;
			for(kolp=1;lt!=0;kolp++)
			{
				if(lt>512) { len=512; lt=lt-512;}
				else { len=lt; lt=0;}
				a=file.read((char*)bufch,len);
				if(a<len)
				{
					out1=QString("# Ошибка в задании размеров секций: %1").arg(str1);
					prmdier(out1);lrom[i].rom=""; 
					file.close(); return(1);
				}
		
				b=WBufCom(adrs,len,bufch);
				if(b)
				{
					file.close(); lrom[i].rom=""; return(1);
				}

				adrs=adrs+len;
				}
				continue;
			}
///////////////////////////////////////////////////////////////
			if((str1==".lit4")||(str1==".lit8"))
			{
				if((pShdr+is)->sh_size==0) continue;
				otl = QString("# секция %1: 0x%2-0x%3: %4 байт").arg(str1).arg((pShdr+is)->sh_addr, 8, 16, QChar('0')).arg((pShdr+is)->sh_addr+(pShdr+is)->sh_size, 8, 16, QChar('0')).arg((pShdr+is)->sh_size);
				prmdi(otl);
				file.seek((pShdr+is)->sh_offset);	
				lt=(pShdr+is)->sh_size;
				adrs=(pShdr+is)->sh_addr;
				for(kolp=1;lt!=0;kolp++)
				{
				if(lt>512) { len=512; lt=lt-512;}
				else { len=lt; lt=0;}
				a=file.read((char*)bufch,len);
				if(a<len)
				{
					out1=QString("# Ошибка в задании размеров секций ;%1").arg(str1);
					prmdier(out1);lrom[i].rom=""; 
					file.close(); return(1);
				}
		
				b=WBufCom(adrs,len,bufch);
				if(b)
				{
					file.close(); lrom[i].rom=""; return(1);
				}

				adrs=adrs+len;
				}
				continue;
			}
///////////////////////////////////////////////////////////////

			if((str1==".bss")||(str1==".sbss"))
			{
				for(int j=0;j<512;j++) bufch[j]=0;
				if((pShdr+is)->sh_size==0) continue;
				otl = QString("# секция %1: 0x%2-0x%3: %4 байт").arg(str1).arg((pShdr+is)->sh_addr, 8, 16, QChar('0')).arg((pShdr+is)->sh_addr+(pShdr+is)->sh_size, 8, 16, QChar('0')).arg((pShdr+is)->sh_size);
				prmdi(otl);
				lrom[i].fbss=(pShdr+is)->sh_addr;
				lrom[i].end=lrom[i].fbss+(pShdr+is)->sh_size;
				lt=lrom[i].end-lrom[i].fbss;	
				adrs=lrom[i].fbss;
				for(kolp=1;lt!=0;kolp++)
					{
			
						if(lt>512) { len=512; lt=lt-512;}
							else { len=lt; lt=0;}
		
						b=WBufCom(adrs,len,bufch);
						if(b)
						{
							file.close();lrom[i].rom=""; 
							return(1);
						}
						adrs+=len;
					}
			}
///////////////////////////////////////////
			if(str1==".stab")
			{
		
				lrom[i].pstab=(Elf32_stab*)malloc((pShdr+is)->sh_size);
				if(lrom[i].pstab==NULL)
				{
					prmdier("# Не хватает памяти РС для загрузки отладочной информации");
					prmdier("# если есть загруженные приложения, закройте их ");
					lrom[i].rom="";file.close();return(1);
				}
				file.seek((pShdr+is)->sh_offset);	
				file.read((char*)lrom[i].pstab,(pShdr+is)->sh_size);
				lrom[i].lstab=(pShdr+is)->sh_size;
				continue;
			}

			if(str1==".stabstr")
			{
		
				lrom[i].pstabst=(UCHAR*)malloc((pShdr+is)->sh_size);
				if(lrom[i].pstabst==NULL)
				{
					prmdier("# Не хватает памяти РС для загрузки отладочной информации");
					prmdier("# если есть загруженные приложения, закройте их ");
					lrom[i].rom="";file.close();return(1);
				}
				file.seek((pShdr+is)->sh_offset);	
				file.read((char*)lrom[i].pstabst,(pShdr+is)->sh_size);
				lrom[i].lstabst=(pShdr+is)->sh_size;
				continue;
			}

			if(str1==".symtab")
			{
		
				lrom[i].psym=(Elf32_External_Sym*)malloc((pShdr+is)->sh_size);
				if(lrom[i].psym==NULL)
				{
					prmdier("# Не хватает памяти РС для загрузки отладочной информации");
					prmdier("# если есть загруженные приложения, закройте их ");
					lrom[i].rom="";file.close();return(1);
				}
				file.seek((pShdr+is)->sh_offset);	
				file.read((char*)lrom[i].psym,(pShdr+is)->sh_size);
				lrom[i].lsym=(pShdr+is)->sh_size;
				continue;
			}

			if(str1==".strtab")
			{
		
				lrom[i].pstrtab=(UCHAR*)malloc((pShdr+is)->sh_size);
				if(lrom[i].pstrtab==NULL)
				{
					prmdier("# Не хватает памяти РС для загрузки отладочной информации");
					prmdier("# если есть загруженные приложения, закройте их ");
					lrom[i].rom="";file.close();return(1);
				}
				file.seek((pShdr+is)->sh_offset);	
				file.read((char*)lrom[i].pstrtab,(pShdr+is)->sh_size);
				lrom[i].lstrtab=(pShdr+is)->sh_size;
				continue;
			}
	 }
	free(pShdr);
	return (0);
}


///////////////////////////////////////////////////////////////////////////
/// поиск прграммного адреса по номеру строки в тексте конкретной программы
/// входная строка <имя файла>:< номер строки>, возврaщает 0 в случае успеха
int NomToAdrRom(int ind, QString str, UINT& adr)
{
	QString str1,s, strf,otl,otlsym,sleft,sright,com_dir;
	USHORT nom, kolstr;
	int in,i,k, j,i1;
	Elf32_stab* t_pstab;
	UINT t_lstab, lsek;
	UCHAR* t_pstabst, *t_pend;
	UINT t_lstabst, start_adr;

	s=str.trimmed();
	s = s.toLower();
	s.replace('\\', '/');
	if(lrom[ind].pstab==0) return(1);	// нет отладочной информации
	in=s.lastIndexOf(':');
	strf=s.left(in);
	s = s.right(s.count() - in - 1);
	nom=(USHORT)fpere(s);
	t_pstab=lrom[ind].pstab;
	t_pstabst=lrom[ind].pstabst;
	t_lstab=lrom[ind].lstab;
	t_lstabst=lrom[ind].lstabst;
	t_pend=((UCHAR*)t_pstab)+t_lstab;
	///// цикл по подсекциям
	while((UCHAR*)t_pstab<t_pend)
	{
////////////////////
//if(t_pstab->n_strx)
//{ otlsym=(const unsigned  char*)(t_pstabst+((t_pstab)->n_strx));
//}
//else otlsym="";
//otl.Format("%08X %02X %05d %08X\t%s",
//t_pstab->n_strx,t_pstab->n_type,t_pstab->n_desc,t_pstab->n_value,otlsym);
//prmdi(otl);
/////////////////////
		start_adr=0;
	kolstr=t_pstab->n_desc;
	lsek=t_pstab->n_value;
	com_dir=(const char*)(t_pstabst+((t_pstab+1)->n_strx));
	com_dir = com_dir.toLower();
	com_dir.replace('\\', '/');
	i1 = com_dir.count();
	for(j=1; j<=kolstr;j++)
	{
////////////////////
//if((t_pstab+j)->n_strx)
//{ otlsym=(const unsigned  char*)(t_pstabst+((t_pstab+j)->n_strx));
//}
//else otlsym="";
//otl.Format("%08X %02X %05d %08X\t%s",
//(t_pstab+j)->n_strx,(t_pstab+j)->n_type,(t_pstab+j)->n_desc,(t_pstab+j)->n_value,otlsym);
//prmdi(otl);
/////////////////////

		/// поиск первого появления названия файла
		if((t_pstab+j)->n_type!=0x84) continue;
		if(start_adr>((t_pstab+j)->n_value)) continue;
 	s=(const char*)(t_pstabst+((t_pstab+j)->n_strx));
	s = s.toLower();
	s = s.replace('\\', '/');

	for(;;)
	{
		in=s.indexOf("..");if(in==(-1)) break;
		if(in!=(-1))
		{
			sleft=s.left(in-1);
			s = s.right(s.count() - in - 2);
			in=sleft.lastIndexOf('/'); if(in==(-1) ) break;
			sright=sleft.left(in); s=sright+s;
		}
	}
	in=s.indexOf(':'); if(in==(-1)) s=com_dir+s;
	if (s!=strf) continue;
		start_adr=(t_pstab+j)->n_value;
		if((((t_pstab+j)->n_value)==((t_pstab+j-1)->n_value)) &&
		(((t_pstab+j-1)->n_type)==0x44))
		{
			if((t_pstab+j-1)->n_desc==nom)
			{
				adr=((t_pstab+j-1)->n_value);
				return(0);
			}
		}
			/// далее по типам 0х44 до конца include
		for(k=j+1;k<=kolstr;k++)
		{

////////////////////
//if((t_pstab+k)->n_strx)
//{ otlsym=(const unsigned  char*)(t_pstabst+((t_pstab+k)->n_strx));
//}
//else otlsym="";
//otl.Format("%08X %02X %05d %08X\t%s",
//(t_pstab+k)->n_strx,(t_pstab+k)->n_type,(t_pstab+k)->n_desc,(t_pstab+k)->n_value,otlsym);
//prmdi(otl);
/////////////////////

			if(((t_pstab+k)->n_type!=0x84) && ((t_pstab+k)->n_type!=0x44))
				continue;
			if((t_pstab+k)->n_type==0x44)
			{
				if((t_pstab+k)->n_desc==nom)
				{
					adr=(t_pstab+k)->n_value;
					return(0);
				}
				start_adr=(t_pstab+k)->n_value;
				continue;
			}
			if((t_pstab+k)->n_value<=start_adr) continue;
			else break;
		}
		if(start_adr) return(1);
	}
		/// на новую подсекцию
	t_pstab+=(kolstr+1); t_pstabst +=lsek;
	}
	return(1);
}



/////////////////////////////////////////////////////////
/// определение адреса по номеру строки
int NomToAdr( QString str, UINT& adr)
{
	QString s,otl;
	int k,i,in,k1,k2,k3,k4;
	UINT a;

	s=str;
	in=s.lastIndexOf(':');
	if(in==(-1)) return(1);
	if(s[in+1].isDigit())
	{
		if(lrom[last_ind].rom != "")
		{
			k=NomToAdrRom(last_ind,s,a);
			if(!k){adr=a; return(0);}
		}
		for(i=0;lrom[i].rom!="";i++)
		{
			if(i==last_ind) continue;
			k=NomToAdrRom(i,s,a);
			if(k) continue;
			adr=a; last_ind=i; 
			mul_ind[0]=i;return(0);
		}
		return(1);
	}
// не число
	else
	{
		if(lrom[last_ind].rom != "")
		{

			k1=FindInclude(last_ind,s,a);
			if((!k1) &&(a)) 
			{
				adr=a; mul_ind[0]=last_ind;///////////////////////////
				return(0);
			}
			if(k1==1)
		
			{

				k4=FindIncludeSym(last_ind,s);
				if(!k4)
				{

					k2=FindLocalSym(last_ind,s,a);
					if((!k2) &&(a)) 
					{adr=a;mul_ind[0]=last_ind; return(0);}
					if(!k2)
					{
						k3=AdrFromSymRom(last_ind,s,a);
						if(!k3)
						{
							adr=a;mul_ind[0]=last_ind;
							mul_tip[0]=2;					
							return(0);
						}
					}
				}

			}
			if(k1==2)
			{
				k2=FindLocalSym(last_ind,s,a);
			
				if((!k2) &&(a)) 
				{adr=a; mul_ind[0]=last_ind;return(0);}
				if((!k2)||(k1==2))
				{
					k3=AdrFromSymRom(last_ind,s,a);
					if(!k3)
					{
						adr=a; mul_ind[0]=last_ind;
						mul_tip[0]=2;					
						return(0);
					}
				}
			}

				
		}	
		for(i=0;lrom[i].rom!="";i++)
		{
			if(i==last_ind) continue;

			k1=FindInclude(i,s,a);
			if((!k1) &&(a)) 
				{adr=a; last_ind=i;mul_ind[0]=last_ind;return(0);}
		
				if(k1==1)
			{

				k4=FindIncludeSym(i,s);
				if(!k4)
				{
					k2=FindLocalSym(i,s,a);
					if((!k2) &&(a)) 
						{adr=a; last_ind=i;mul_ind[0]=last_ind;return(0);}
					if(!k2)
					{
						k3=AdrFromSymRom(i,s,a);
						if(!k3)
						{
							adr=a; last_ind=i;mul_ind[0]=last_ind;
							mul_tip[0]=2;						
							return(0);
						}
					}
				}
			}
			if(k1==2)
			{
				k2=FindLocalSym(i,s,a);
				if((!k2) &&(a)) 
					{adr=a; last_ind=i;mul_ind[0]=last_ind;return(0);}
				if((!k2)||(k1==2))
				{
					k3=AdrFromSymRom(i,s,a);
					if(!k3)
					{
						adr=a; last_ind=i;mul_ind[0]=last_ind;
						mul_tip[0]=2;							
						return(0);
					}
				}
			}

		}

	}

	for(i=0;lrom[i].rom!="";i++)
	{
		k3=AdrFromSymAbc(i,s,a);
		if(!k3)
		{
			adr=a; last_ind=i;mul_ind[0]=last_ind;
			mul_tip[0]=1;							
			return(0);
		}
			
	}
	
	
	return(1);
}
/////////////////////////////////////////////////////////////////
// поиск символа в файле, ind-индекс, сначала находит include
// в нем ищет типы 0х20 (глобальная переменная),
// 0х24 ( функция ), 0х26 ( переменная сегмента data),
// 0х28 ( переменная сегмента bss )
// возвращает 0-все в норме
//			 1 - не найдено ничего
//			 2 - найден include
int FindInclude(int ind, QString str, UINT& adr)
{
	QString com_dir,str1,s,sym, strf,otl,sleft,sright;
	USHORT  kolstr;
	int in,i,k, j,i1, princl;
	Elf32_stab* t_pstab;
	UINT t_lstab, lsek;
	UCHAR* t_pstabst, *t_pend;
	UINT t_lstabst, start_adr;

	adr=0;princl=0;
	s = str.trimmed();
	s.replace('\\', '/');
	if(lrom[ind].pstab==0) return(1);	// нет отладочной информации
	in=s.lastIndexOf(':');
	strf=s.left(in);  
	strf = strf.toLower();
	sym = s.right(s.count() - in - 1);
	t_pstab=lrom[ind].pstab;
	t_pstabst=lrom[ind].pstabst;
	t_lstab=lrom[ind].lstab;
	t_lstabst=lrom[ind].lstabst;
	t_pend=((UCHAR*)t_pstab)+t_lstab;
	///// цикл по подсекциям
	while((UCHAR*)t_pstab<t_pend)
	{
		start_adr=0;
		kolstr=t_pstab->n_desc;
		lsek=t_pstab->n_value;
		com_dir=(const char*)(t_pstabst+((t_pstab+1)->n_strx));
		com_dir = com_dir.toLower();
		com_dir.replace('\\', '/');
	
		for(j=1; j<=kolstr;j++)
		{

			/// поиск первого появления названия файла
			if((t_pstab+j)->n_type!=0x84) continue;
			if(start_adr>((t_pstab+j)->n_value)) continue;
 			s=(const char*)(t_pstabst+((t_pstab+j)->n_strx));
			s = s.toLower();
			s.replace('\\', '/');
			for(;;)
			{
				in = s.indexOf(".."); if (in == (-1)) break;
				if (in != (-1))
				{
					sleft = s.left(in - 1);
					s = s.right(s.count() - in - 2);
					in = sleft.lastIndexOf('/'); if (in == (-1)) break;
					sright = sleft.left(in); s = sright + s;
				}

			}
			in=s.indexOf(':'); if(in==(-1)) s=com_dir+s;
			if (s!=strf) continue;
			princl=2;
			start_adr=(t_pstab+j)->n_value;
			/// далее по типам  до конца include
			for(k=j+1;k<=kolstr;k++)
			{

				if(((t_pstab+k)->n_type!=0x84)
					&& ((t_pstab+k)->n_type!=0x20)
					&& ((t_pstab+k)->n_type!=0x24)
					&& ((t_pstab+k)->n_type!=0x26)
					&& ((t_pstab+k)->n_type!=0x28) )
					continue;
				if((t_pstab+k)->n_type!=0x84)
				{
 					s=(const char*)(t_pstabst+((t_pstab+k)->n_strx));
					in=s.indexOf(':');
					if(in==(-1)) continue;
					str1=s.left(in);
					if(str1==sym)
					{
						adr=(t_pstab+k)->n_value;
						if(adr==0) return(2);
						if (((t_pstab+k)->n_type==0x26)
						   ||((t_pstab+k)->n_type==0x28) ) mul_tip[0]=2;
						else mul_tip[0]=1;

						return(0);
					}
					start_adr=(t_pstab+k)->n_value;
					continue;
				}
				if((t_pstab+k)->n_value<=start_adr) continue;
				else break;
			}
			if(adr) return(0);
		}
			/// на новую подсекцию
		t_pstab+=(kolstr+1); t_pstabst +=lsek;
	}
	if(princl) return(2);
	return(1);
}

////////////////////////////////////////////////////////////////
// поиск символа в psym
int AdrFromSymRom(int ind, QString str, UINT& adr)
{
	QString str1,s,sym, strf,otl,sl,st;
	int in,i,i1,priz4=0;
	Elf32_External_Sym* t_psym=0;
	UINT t_lsym;
	UCHAR* t_pstrtab=0, *t_pend=0;
 
	s=str.trimmed();
	if(lrom[ind].psym==0) return(1);	// нет отладочной информации
	in=s.lastIndexOf(':');
	sl=s.left(in);
	sl = sl.toUpper();
	sl.replace('\\', '/');
	i1=sl.indexOf('/');
	if(i1==(-1)) sl="";
	if(sl=="")
	{
		sym = s.right(s.count() - in - 1);
		t_psym=lrom[ind].psym;
		t_pstrtab=lrom[ind].pstrtab;
		t_lsym=lrom[ind].lsym;
		t_pend=((UCHAR*)t_psym)+t_lsym;

		for(i=0;((UCHAR*)(t_psym+i))<t_pend;i++)
		{
			if(/*(((t_psym+i)->st_info)!=0x11) && (((t_psym+i)->st_info)!=0x12)
				&&*/(((t_psym+i)->st_info)!=0x1)&&(((t_psym+i)->st_info)!=0x2))
			continue;
			s=(const char*)(t_pstrtab+((t_psym+i)->st_name));
			in=s.indexOf(':');
			if(in!=(-1))  str1=s.left(in);
			else str1=s;
			in=str1.indexOf('.');
			if(in!=(-1)) str1=str1.left(in);
			if(str1!=sym) continue;
			adr=(t_psym+i)->st_value; 
			return(0);
		}
		adr=0; return(1);
	}
	else
	{
		for(i=0;((UCHAR*)(t_psym+i))<t_pend;i++)
		{
			if(	(((t_psym+i)->st_info)!=0x1)&&(((t_psym+i)->st_info)!=0x2)&&(((t_psym+i)->st_info)!=0x4))
			continue;
			if(((t_psym+i)->st_info)==0x4)
			{
				st=(const char*)(t_pstrtab+((t_psym+i)->st_name));	
				st = st.toUpper();
				st.replace('\\', '/');

				if(st==sl) priz4=1;else priz4=0;
			}
			if(priz4)
			{

				s=(const char*)(t_pstrtab+((t_psym+i)->st_name));
				in=s.indexOf(':');
				if(in!=(-1))  str1=s.left(in);
				else str1=s;
				in=str1.indexOf('.');
				if(in!=(-1)) str1=str1.left(in);
				if(str1!=sym) continue;
				adr=(t_psym+i)->st_value;
				return(0);
			}
	}
	adr=0;
	return(1);


	}
}
//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/// поиск строки в тексте конкретной программы по адресу
/// выходная строка <имя файла>:< номер строки>, возврaщает 0 в случае успеха

int LineFromAdrRom(int ind, UINT adr,QString& str)
{
	QString com_dir_old="",file_old="", sleft,sright,str1,s, strf,otl,com_dir,str_old;
	USHORT nom, kolstr,nom1;
	int in, k,i1 ,j,prold=0;
	Elf32_stab* t_pstab;
	UINT t_lstab, lsek;
	UCHAR* t_pstabst, *t_pend;
	UINT t_lstabst, start_adr;
	if(lrom[ind].pstab==0) return(1);	// нет отладочной информации
	t_pstab=lrom[ind].pstab;
	t_pstabst=lrom[ind].pstabst;
	t_lstab=lrom[ind].lstab;
	t_lstabst=lrom[ind].lstabst;
	t_pend=((UCHAR*)t_pstab)+t_lstab;
	///// цикл по подсекциям
	nom1=0;
	start_adr=0;
	while((UCHAR*)t_pstab<t_pend)
	{
		start_adr=0;
		kolstr=t_pstab->n_desc;	// количество структур в секции
		lsek=t_pstab->n_value;
		com_dir=(const char*)(t_pstabst+((t_pstab+1)->n_strx));
		com_dir = com_dir.toLower();
		com_dir.replace('\\', '/');
		for(j=1; j<=kolstr;j++)
		{

			/// поиск первого появления названия файла
			if((t_pstab+j)->n_type!=0x84) continue;
 			str=(const char*)(t_pstabst+((t_pstab+j)->n_strx));
			str.replace('\\', '/');
			for(;;)
			{
				in = str.indexOf(".."); if (in == (-1)) break;
				if (in != (-1))
				{
					sleft = str.left(in - 1);
					str = str.right(str.count() - in - 2);
					in = sleft.lastIndexOf('/'); if (in == (-1)) break;
					sright = sleft.left(in); str = sright + str;
				}
			}
			in=str.indexOf(':'); if(in==(-1)) str=com_dir+str;
			if(start_adr>((t_pstab+j)->n_value)) continue;
	///////////////////
			if(file_old=="")
			{
				file_old=str;
				com_dir_old=com_dir;
			}
			if((file_old!="")&&(adr<((t_pstab+j)->n_value)))
			{
				file_old.replace('\\', '/');
/////
				for(;;)
				{
					in = file_old.indexOf(".."); if (in == (-1)) break;
					if (in != (-1))
					{
						sleft = file_old.left(in - 1);
						file_old = file_old.right(file_old.count() - in - 2);
						in = sleft.lastIndexOf('/'); if (in == (-1)) break;
						sright = sleft.left(in); file_old = sright + file_old;
					}

				}
				in=file_old.indexOf(':'); 
				if(in==(-1))
					file_old=com_dir_old+file_old;

				str=file_old+":"+QString::number(nom1);
				str.replace('/', '\\');
				if(nom1==0)
					return(1);
				return(0);
			}
			else
			{
				file_old=str;//continue;
			}
			start_adr=(t_pstab+j)->n_value;
			if((adr<start_adr)&&(nom1==0)) return(1);
			if((((t_pstab+j)->n_value)==((t_pstab+j-1)->n_value)) &&
			 (((t_pstab+j-1)->n_type)==0x44))
			{
				nom=(t_pstab+j-1)->n_desc;
				if((j+1)<=kolstr)
				{
					for(;;)
					{
						if((((t_pstab+j+1)->n_type)==0x44)&&
							((((t_pstab+j+1)->n_value))==((t_pstab+j)->n_value)))
						{
							nom=(t_pstab+j+1)->n_desc;
							j++; continue;
						}
						else break;
					}
				}
			
			if(adr==start_adr)
			{


				str=str+":"+QString::number(nom);
				str.replace('/', '\\');

				return(0);
			}
		}
			/// далее по типам 0х44 до конца include
		for(k=j+1;k<=kolstr;k++)
		{

			if(((t_pstab+k)->n_type!=0x84) && ((t_pstab+k)->n_type!=0x44))
				continue;
			//////////////////////////////////////////////////////////////////////////

			if((t_pstab+k)->n_type==0x84)
			{

			str=(const char*)(t_pstabst+((t_pstab+k)->n_strx));
			str.replace('\\', '/');

			prold=0;
			if(str!=file_old)
			{
				prold=1;str_old=file_old;file_old=str;
			}
			for(;;)
			{
				in = str.indexOf(".."); if (in == (-1)) break;
				if (in != (-1))
				{
					sleft = str.left(in - 1);
					str = str.right(str.count() - in - 2);
					in = sleft.lastIndexOf('/'); if (in == (-1)) break;
					sright = sleft.left(in); str = sright + str;
				}
			}
			in=str.indexOf(':'); if(in==(-1)) str=com_dir+str;
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		if(((t_pstab+k)->n_type==0x44) && 
			((t_pstab+k)->n_value<adr))
			nom1=(t_pstab+k)->n_desc;
		if((t_pstab+k)->n_type==0x44)
		{
			nom=(t_pstab+k)->n_desc;
			if((k+1)<=kolstr)
			{
				for(;;)
				{
					if((((t_pstab+k+1)->n_type)==0x44)&&
//						if(((((t_pstab+k+1)->n_type)==0x44)||((t_pstab+k+1)->n_type)==0x24))&&
						((((t_pstab+k)->n_value))==((t_pstab+k+1)->n_value)))
					{
						nom=(t_pstab+k+1)->n_desc;
						k++; continue;
					}
					else break;
				}
			}

			if(adr==((t_pstab+k)->n_value))
			{
				//str1.Format("%d",(t_pstab+k)->n_desc);//if(prold) ///////////////////////////
//				{
//					if (str_old!="") 
//						str=str_old;
//				}
				str=str+":"+QString::number((t_pstab + k)->n_desc);
				str.replace('/', '\\');
				
				return(0);
			}
////////////////////////////////////////////////////////////////
			if(adr>=(t_pstab+k)->n_value)
			{
			nom1=(t_pstab+k)->n_desc;
			
			}
			if((adr)<((t_pstab+k)->n_value))
			{
			
				if(nom1==0) return 1;
				if(prold)
									{
					if (str_old!="")
						str=str_old;
				}
				str=str+":"+QString::number(nom1);
				str.replace('/', '\\');
				return(0);
			}
			nom=(t_pstab+k)->n_desc;

				start_adr=(t_pstab+k)->n_value;
				continue;
			}
		}
	}
		/// на новую подсекцию
	t_pstab+=(kolstr+1); t_pstabst +=lsek;
	}
	return(1);
}


//////////////////////////////////////////////////////////////////////////
//определение файла и строки по адресу ( останов по символу )
int LineFromAdr(UINT adr, QString &str)
{
	QString line;

	for(int i=0;lrom[i].rom!="";i++)
	{
		if((adr<lrom[i].ftext) || (adr>lrom[i].etext)) continue;
		int	k=LineFromAdrRom(i,adr,line);
		if(k) return(1);
		str=line; return(0);
	}
		return(1);
}
//////////////////////////////////////////////////////////
int FindLocalSym(int ind, QString str, UINT& adr)
{
	QString sleft,sright,str1,s,sym, str_sh,strf,otl;
	int in,i,i1;
	Elf32_External_Sym* t_psym;
	UINT t_lsym;
	UCHAR* t_pstrtab, *t_pend;
 
	s=str.trimmed();
	if(lrom[ind].psym==0) return(1);	// нет отладочной информации
	in=s.lastIndexOf(':');
	strf=s.left(in);
	strf = strf.toLower();
	strf.replace('\\', '/');
	int pr3=0;	

	sym = s.right(s.count() - in - 1);
	in=strf.lastIndexOf('/');
	if(in!=(-1))
	{
		str_sh = strf.right(s.count() - in - 1);
	}
	t_psym=lrom[ind].psym;
	t_pstrtab=lrom[ind].pstrtab;
	t_lsym=lrom[ind].lsym;
	t_pend=((UCHAR*)t_psym)+t_lsym;
	adr=0;
	for(i=0;((UCHAR*)(t_psym+i))<t_pend;i++)
	{

		if(((t_psym+i)->st_info)!=0x04) 
			continue;
		s=(const char*)(t_pstrtab+((t_psym+i)->st_name));
		s = s.toLower();
		s.replace('\\', '/');
		for(;;)
		{
			in = s.indexOf(".."); if (in == (-1)) break;
			if (in != (-1))
			{
				sleft = s.left(in - 1);
				s = s.right(s.count() - in - 2);
				in = sleft.lastIndexOf('/'); if (in == (-1)) break;
				sright = sleft.left(in); s = sright + s;
			}
		}
		in=s.indexOf(':');
		if((strf!=s)&&(str_sh!=s)) continue;
		/// далее ищем тип =1
		i++; pr3=1;
		for(;((UCHAR*)(t_psym+i))<t_pend;i++)
		{
			if(((t_psym+i)->st_info)!=0x01)
			{
					break;
			}
			s=(const char*)(t_pstrtab+((t_psym+i)->st_name));
			if(s==sym) 
			{
			adr=(t_psym+i)->st_value; return(0);
			}
		}
	}
	adr=0;
	if(pr3) return(0);return(1);
}
////////////////////////////////////////////////////////////////
// поиск #iclude в psym
int FindIncludeSym(int ind, QString str)
{
	QString sleft, sright,str_sh,str1,s,sym, strf,otl;
	int in,i,i1;
	Elf32_External_Sym* t_psym;
	UINT t_lsym;
	UCHAR* t_pstrtab, *t_pend;
 
	s = str.trimmed();
	if(lrom[ind].psym==0) return(1);	// нет отладочной информации
	in=s.lastIndexOf(':');if(in==(-1)) return(1);
	sym=s.left(in);
	sym = sym.toLower();
	sym.replace('\\','/');
	for(;;)
	{
		in = sym.indexOf(".."); if (in == (-1)) break;
		if (in != (-1))
		{
			sleft = sym.left(in - 1);
			sym = sym.right(sym.count() - in - 2);
			in = sleft.lastIndexOf('/'); if (in == (-1)) break;
			sright = sleft.left(in); sym = sright + sym;
		}
	}
	in=sym.lastIndexOf('/');
	if(in!=(-1))
	{
		str_sh=sym.right(sym.count() - in -1);
	}
	t_psym=lrom[ind].psym;
	t_pstrtab=lrom[ind].pstrtab;
	t_lsym=lrom[ind].lsym;
	t_pend=((UCHAR*)t_psym)+t_lsym;

	for(i=0;((UCHAR*)(t_psym+i))<t_pend;i++)
	{
	if((((t_psym+i)->st_info)!=0x4)&&(((t_psym+i)->st_info)!=0x6))
	continue;
	s=(const char*)(t_pstrtab+((t_psym+i)->st_name));

	s.replace('\\', '/');
	for(;;)
	{
		in = s.indexOf(".."); if (in == (-1)) break;
		if (in != (-1))
		{
			sleft = s.left(in - 1);
			s = s.right(s.count() - in - 2);
			in = sleft.lastIndexOf('/'); if (in == (-1)) break;
			sright = sleft.left(in); s = sright + s;
		}
	}
	s = s.toLower();
	if((s==sym) ||(s==str_sh))	 return(0);
	}
	 return(1);
}
////////////////////////////////////////////////////////////////
// поиск символа с абс. адресом в psym
int AdrFromSymAbc(int ind, QString str, UINT& adr)
{
	QString str1,s,sym, strf,otl;
	int in,i;
	Elf32_External_Sym* t_psym;
	UINT t_lsym;
	UCHAR* t_pstrtab, *t_pend;
 
	s=str.trimmed();
	if(lrom[ind].psym==0) return(1);	// нет отладочной информации
	in=s.lastIndexOf(':');
	sym = s.right(s.count() - in - 1);
	t_psym=lrom[ind].psym;
	t_pstrtab=lrom[ind].pstrtab;
	t_lsym=lrom[ind].lsym;
	t_pend=((UCHAR*)t_psym)+t_lsym;

	for(i=0;((UCHAR*)(t_psym+i))<t_pend;i++)
	{
		if((((t_psym+i)->st_info)!=0x11) && (((t_psym+i)->st_info)!=0x12))
		continue;
		s=(const char*)(t_pstrtab+((t_psym+i)->st_name));
		in=s.indexOf(':');if(in!=(-1))  str1=s.left(in);
		else str1=s;
		if(str1!=sym) continue;
		adr=(t_psym+i)->st_value; return(0);
	}
	adr=0; return(1);
}
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/// поиск функции в тексте конкретной программы по адресу
/// выходная строка <имя файла>,
// <указатель на тип=0х24>,<указатель на начало сегмента stabstr>
// <кол-во структур до конца сегмента stab>, возврaщает 0 в случае успеха
int FunFromAdrRom(int ind, UINT adr,QString& str,int &ost,
				 Elf32_stab* &pstab,UCHAR* &pstabs )
{
	QString sleft,sright,str1,s, strf,otl,com_dir;
	USHORT nom=0, kolstr;
	int in, k,i1,i2 ,j;
	Elf32_stab* t_pstab;
	UINT t_lstab, lsek;
	UCHAR* t_pstabst, *t_pend;
	UINT t_lstabst, start_adr;

	if(lrom[ind].pstab==0) return(1);	// нет отладочной информации
	t_pstab=lrom[ind].pstab;
	t_pstabst=lrom[ind].pstabst;
	t_lstab=lrom[ind].lstab;
	t_lstabst=lrom[ind].lstabst;
	t_pend=((UCHAR*)t_pstab)+t_lstab;
	///// цикл по подсекциям
	start_adr=0;
	while((UCHAR*)t_pstab<t_pend)
	{
		start_adr=0;
		kolstr=t_pstab->n_desc;	// количество структур в секции
		lsek=t_pstab->n_value;
		com_dir=(const char*)(t_pstabst+((t_pstab+1)->n_strx));
		com_dir = com_dir.toLower();
		com_dir.replace('\\', '/');
		for(j=1; j<=kolstr;j++)
		{

			/// поиск первого появления названия файла
			if((t_pstab+j)->n_type!=0x84) continue;
 			str=(const char*)(t_pstabst+((t_pstab+j)->n_strx));
			str.replace('\\', '/');
	
			for(;;)
			{
				in = str.indexOf(".."); if (in == (-1)) break;
				if (in != (-1))
				{
					sleft = str.left(in - 1);
					str = str.right(str.count() - in - 2);
					in = sleft.lastIndexOf('/'); if (in == (-1)) break;
					sright = sleft.left(in); str = sright + str;
				}
			}
			in=str.indexOf(':'); if(in==(-1)) str=com_dir+str;
			if(start_adr>((t_pstab+j)->n_value)) continue;
				start_adr=(t_pstab+j)->n_value;
			if(adr<start_adr) return(1);
				if((((t_pstab+j)->n_value)==((t_pstab+j-1)->n_value)) &&
				(((t_pstab+j-1)->n_type)==0x44))
				{
					if(adr==start_adr)
					{
						str.replace('/', '\\');
						for(i2=j;i2<=kolstr;i2++)
						{
							if(((t_pstab+i2)->n_type)!=0x24) continue;
							ost=kolstr-i2; pstab=t_pstab+i2;
							pstabs=t_pstabst;

							return(0);
						}
						return(1);
					}
				}
					/// далее по типам 0х44 до конца include
				for(k=j+1;k<=kolstr;k++)
				{

					if(((t_pstab+k)->n_type!=0x84) && ((t_pstab+k)->n_type!=0x44))
						continue;

			//////////////////////////////////////////////////////////////////////////
					if((t_pstab+k)->n_type==0x84)
					{

						str=(const char*)(t_pstabst+((t_pstab+k)->n_strx));
						str.replace('\\', '/');
						for(;;)
						{
							in = str.indexOf(".."); if (in == (-1)) break;
							if (in != (-1))
							{
								sleft = str.left(in - 1);
								str = str.right(str.count() - in - 2);
								in = sleft.lastIndexOf('/'); if (in == (-1)) break;
								sright = sleft.left(in); str = sright + str;
							}
						}
						in=str.indexOf(':'); if(in==(-1)) str=com_dir+str;
					}

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
					
			
			
					if((t_pstab+k)->n_type==0x44)
					{
					if(adr==(t_pstab+k)->n_value)
					{
			
						str1 = QString::number((t_pstab+k)->n_desc);// str=str+":"+str1;
						str.replace('/', '\\');
						for(i2=k;i2<=kolstr;i2++)
						{
							if(((t_pstab+i2)->n_type)!=0x24) continue;
							ost=kolstr-i2; pstab=t_pstab+i2;
							pstabs=t_pstabst;

						return(0);
						}
				
						return(1);
					}
					if((adr)<((t_pstab+k)->n_value))
					{
			
						str1 = QString::number(nom); 
						str.replace('/', '\\');
						for(i2=k;i2<=kolstr;i2++)
						{
							if(((t_pstab+i2)->n_type)!=0x24) continue;
							ost=kolstr-i2; pstab=t_pstab+i2;
							pstabs=t_pstabst;

						return(0);
						}
						return(1);
					}
					start_adr=(t_pstab+k)->n_value;
					continue;
				}
			}
		}
			/// на новую подсекцию
		t_pstab+=(kolstr+1); t_pstabst +=lsek;
	}
	return(1);
}


//////////////////////////////////////////////////////////////////////////
//// вывод локальных переменных в Results.txt
//////////////////////////////////////////////////////////////////////////
// определение диапазона адресов строки по адресу 
int OnFindDiap(UINT adr,UINT &adr_ns,UINT& adr_ks)
{
	QString line;
	
	adr_ks=0xFFFFFFFF;
		for(int i=0;lrom[i].rom!=0;i++)
	{
		if((adr<lrom[i].ftext) || (adr>lrom[i].etext)) continue;
	int	k=DiapFromAdrRom(i,adr,adr_ns,adr_ks);
		if(k) return(1);
		 return(0);
	}
		return(1);
}
//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/// поиск диапазона адресов строки в  конкретной программе по адресу в строке
///  возврaщает 0 в случае успеха
int DiapFromAdrRom(int ind, UINT adr,UINT& adr_n,UINT& adr_k)
{
	QString sleft,sright,str1,s, strf,otl,com_dir;
	USHORT  kolstr;
	int  k;
	Elf32_stab* t_pstab;
	UINT t_lstab, lsek;
	UCHAR* t_pstabst, *t_pend;
	UINT t_lstabst;

	if(lrom[ind].pstab==0) return(1);	// нет отладочной информации
	adr_n=0;
	t_pstab=lrom[ind].pstab;
	t_pstabst=lrom[ind].pstabst;
	t_lstab=lrom[ind].lstab;
	t_lstabst=lrom[ind].lstabst;
	t_pend=((UCHAR*)t_pstab)+t_lstab;
	///// цикл по подсекциям
	while((UCHAR*)t_pstab<t_pend)
	{
		kolstr=t_pstab->n_desc;	// количество структур в секции
		lsek=t_pstab->n_value;
	
		for(k=1; k<=kolstr;k++)
			{

				/// далее по типам 0х44 до конца include

				if((t_pstab+k)->n_type!=0x44)
					continue;

				if((adr>=(t_pstab+k)->n_value)&&(adr_n<(t_pstab+k)->n_value))
				adr_n=(t_pstab+k)->n_value;

				if(adr<(t_pstab+k)->n_value)
				{
					adr_k=(t_pstab+k)->n_value;
				return(0);
				}


			}
			/// на новую подсекцию
		t_pstab+=(kolstr+1); t_pstabst +=lsek;
	}

	if(!adr_n)
	return(1);
	else return(0);
}


////////////////////////////////////////////
QStringList No_Met={
	"__gnu_compiled_c",
		"gcc2_compiled.",
		"_ftext",
		"_etext",
		""
};
////////////////////////////////////////////////////////////////
// поиск символа в psym по адресу
int MetFromAdrRom(int ind, QString &str, UINT adr)
{
	QString str1,sl,s,sym, strf,otl;
	int in,i,j;
	Elf32_External_Sym* t_psym;
	UINT t_lsym;
	UCHAR* t_pstrtab, *t_pend;
 
	str=""; 

	if(lrom[ind].psym==0) return(1);	// нет отладочной информации
	t_psym=lrom[ind].psym;
	t_pstrtab=lrom[ind].pstrtab;
	t_lsym=lrom[ind].lsym;
	t_pend=((UCHAR*)t_psym)+t_lsym;

	for(i=0;((UCHAR*)(t_psym+i))<t_pend;i++)
	{
		if((((t_psym+i)->st_info)!=0x11) && (((t_psym+i)->st_info)!=0x12)
			&&(((t_psym+i)->st_info)!=0x1))
		continue;
		if(adr!=((t_psym+i)->st_value)) continue;
		s=(const char*)(t_pstrtab+((t_psym+i)->st_name));
		in=s.indexOf(':');if(in!=(-1))  str1=s.left(in);
		else str1=s; 
		//for(j=0;No_Met[j]!="";j++)
		//{
		//	if(No_Met[j]==str1) break;
		//}
		//if(No_Met[j]==str1) continue;
	
		if (No_Met.contains(str1))
			continue;

		str=str1; return(0);
	}
	return(1);
}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//определение метки по адресу ( останов по символу )
int MetFromAdr(UINT adr, QString &str)
{
	QString line;
	str="";
	for(int i=0;lrom[i].rom!="";i++)
	{
		if((adr<lrom[i].ftext) || (adr>lrom[i].etext)) continue;
		int	k=MetFromAdrRom(i, str,  adr);
		if(k) return(1);
		return(0);
	}
	return(1);
}
//////////////////////////////////////////////////////////
#define WM_MDI30                   WM_APP+30
UINT adr_vib;
QString met_vib;
UINT multi_adr[200];
QString multi_met[200];
UINT mul_ind[200];
UINT mul_tip[200];	//=1 global, =2 static
int multi_ind;
//////////////////////////////////////////////////////////
//определение адресов идентификаторов при отсутствии пути к исходнику
// im - входное имя
// adr - сюда запишется адрес
// e_im - сюда запишется имя программы и идентификатор - <ghja.e:met1>
//
int AdrFromIdent(QString im, UINT &adr,QString& e_im)
{

	UINT a;
	int i;
	QString prog, met,imful,otl;

	imful=im.trimmed();
	if(imful=="")
		
		return(1);
		
	for(;;)
	{
		QChar ch=imful.at(0);
		if((ch==QChar('$')) ||(ch==QChar('%'))||(ch==QChar('^'))||(ch==QChar('&'))||(ch==QChar('*'))
			||(ch==QChar('(')) ||(ch==QChar(')')) ||(ch==QChar('-')) ||(ch==QChar('='))
			||(ch.isDigit())|| (ch==QChar(' '))
			||(ch==QChar('+')) ||(ch==QChar('|'))||(ch==QChar('\\')) ||(ch==QChar('['))
			||(ch==QChar(']')) ||(ch==QChar('{'))||(ch==QChar('}')) ||(ch==QChar(';'))
			||(ch==QChar('"')) ||(ch==QChar(':'))||(ch==QChar('\'')) ||(ch==QChar(','))
			||(ch==QChar('.')) ||(ch==QChar('/'))||(ch==QChar('<')) ||(ch==QChar('>'))||(ch==QChar('~')))
		{
			imful.remove(0, 1);
			if(imful=="") return(1);
			continue;
		}
		break;
	}
	for(;;)
	{
		if(imful=="")	 return(1);
		
		int dd=imful.count(); dd--;
		QChar ch=imful.at(dd);
		if((ch==QChar('$')) ||(ch==QChar('%'))||(ch==QChar('^'))||(ch==QChar('&'))||(ch==QChar('*'))
			||(ch==QChar('(')) ||(ch==QChar(')')) ||(ch==QChar('-')) ||(ch==QChar('='))
			||(ch==QChar('+')) ||(ch==QChar('|'))||(ch==QChar('\\')) ||(ch==QChar('['))
			||(ch==QChar(']')) ||(ch==QChar('{'))||(ch==QChar('}')) ||(ch==QChar(';'))||(ch==QChar(' '))
			||(ch==QChar('"')) ||(ch==QChar(':'))||(ch==QChar('\'')) ||(ch==QChar(','))
			||(ch==QChar('.')) ||(ch==QChar('/'))||(ch==QChar('<')) ||(ch==QChar('>'))||(ch==QChar('~')))
		{
			imful.chop(1);
			//imful.SetAt(dd,' '); imful.TrimRight();if(imful=="") return(1);
			continue;
		}
		break;
	}

	multi_met[0]=""; multi_ind=0;
	int in=imful.indexOf(":");
	if(in==(-1))
	{
		prog=""; met=imful;
	}
	else
	{
		prog=imful.left(in);
		met=imful.right(imful.count() - in -1);
	}

	//// поиск в *.e
	QString right,part;
	int k,in1,lprog;

	for(i=0;lrom[i].rom!="";i++)
	{
		if(lrom[i].psym==0) continue;	// нет отладочной информации
		right=lrom[i].rom;

		if(prog!="")
		{
			lprog=prog.count();
			right=lrom[i].rom;
			right=right.right(lprog);
			right = right.toLower();
			prog = prog.toLower();
			if(right!=prog) continue;
		}
		int		k3=AdrFromSymRom(i,met,a);
		if(k3) continue;
		if(!k3)
		{
			if(multi_ind)
			{
				k=multi_ind;
				if(k==multi_ind)	//не нашли
				{
					multi_adr[multi_ind]=a;
					mul_ind[multi_ind]=i;
					in=right.lastIndexOf('\\');
					in1=right.lastIndexOf('/');
					if(in==(-1)) in=in1;
						else {
							if((in1!=(-1))&&(in!=(-1))) 
						{
						if(in<=in1) in=in1;
						}
						}
				int inl=right.count();right=right.right(inl-in-1);
				multi_met[multi_ind]=right+':'+met;
mul_tip[multi_ind]=2;
				multi_ind++;
					continue;
					}
					}
				
						
				///////первый раз	
				if(!multi_ind)
				{
				multi_adr[multi_ind]=a; last_ind=i;
					mul_ind[multi_ind]=i;
					in=right.lastIndexOf('\\');
					in1=right.lastIndexOf('/');
					if(in==(-1)) in=in1;
					else
					{
						if((in1!=(-1))&&(in!=(-1))) 
						{
						if(in<=in1) in=in1;
						}
					}
				int inl=right.count();right=right.right(inl-in-1);
				multi_met[multi_ind]=right+':'+met;
mul_tip[multi_ind]=2;
				multi_ind++;
					continue;
				}
				}
		}


		for(i=0;lrom[i].rom!=0;i++)
	{
				right=lrom[i].rom;
				if(lrom[i].psym==0) break;	// нет отладочной информации

			if(prog!="")
			{
				lprog=prog.count();
				right=lrom[i].rom;
				right=right.right(lprog);
				right =right.toLower();
				prog = prog.toLower();
			if(right!=prog) continue;

				
			}
				int	k3=AdrFromSymAbc(i,met,a);
			if(k3) continue;
			if(!k3)
			{
						if(multi_ind)
						{
							k=multi_ind;
						if(k==multi_ind)	//не нашли
					{
						multi_adr[multi_ind]=a;
						mul_ind[multi_ind]=i;	// это индекс в lrom
						in=right.lastIndexOf('\\');
						in1=right.lastIndexOf('/');
						if(in==(-1)) in=in1;
							else {
								if((in1!=(-1))&&(in!=(-1))) 
							{
							if(in<=in1) in=in1;
							}
							}
					int inl=right.count();right=right.right(inl-in-1);
					multi_met[multi_ind]=right+':'+met;
	mul_tip[multi_ind]=1;
					multi_ind++;
						continue;
						}
						}
					///////первый раз	
					if(!multi_ind)
					{
					multi_adr[multi_ind]=a; last_ind=i;
						mul_ind[multi_ind]=i;
						in=right.lastIndexOf('\\');
						in1=right.lastIndexOf('/');
						if(in==(-1)) in=in1;
						else
						{
							if((in1!=(-1))&&(in!=(-1))) 
							{
							if(in<=in1) in=in1;
							}
						}
					int inl=right.count();right=right.right(inl-in-1);
					multi_met[multi_ind]=right+':'+met;
	mul_tip[multi_ind]=1;
					multi_ind++;
						continue;
					}
				}
		}
	if(!multi_ind)
	{ e_im="";		return(1);}
		if(multi_ind==1)
		{adr=multi_adr[0]; 
		e_im=multi_met[0]; return(0);}
	//SendMessage(m_W,WM_MDI30,0,0);
			if(met_vib=="")
			{e_im="";return(1);}
			else 
			{
				adr=adr_vib; e_im=met_vib;return(0);
			}
}	

