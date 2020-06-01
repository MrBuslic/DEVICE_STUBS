#include "PKM.h"

UINT PtoF(UINT adr);
///////////////////////////////////////////////////
UINT PtoF(UINT adr)
{
	
	if((adr>=0)&&(adr<=0x7FFFFFFF)) return((adr + 0x40000000)&0xBFFFFFFF);
	if((adr>=0x80000000)&&(adr<=0x9FFFFFFF)) return((adr & 0x3FFFFFFF)&0xBFFFFFFF);
	if((adr>=0xA0000000)&&(adr<=0xBFFFFFFF)) return((adr & 0x1FFFFFFF)&0xBFFFFFFF);
	return(adr&0xBFFFFFFF);
}
////////////////////////////////////////////////////////////////////////
