#ifndef _OMNIBUS_INT_HPP_
#define _OMNIBUS_INT_HPP_

#include "BTI1553.H"
#define _WIN32
#include <windows.h>

class OmniDriver_interface
{
  public:
    virtual BOOL ListMultiBlockRd(LPUSHORT buf, LPINT blkcountptr, LISTADDR listaddr, HCORE handleval)
    {
      return 0;
    }
	virtual void MsgDataWr(LPUSHORT buf,INT count, MSGADDR msgaddr, HCORE handleval)
	{
	}
	virtual ~OmniDriver_interface(){}
};

#endif