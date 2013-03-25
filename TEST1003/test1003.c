#include "test1003.h"

int CVICALLBACK TCPCallback (unsigned int handle, int xType, 
    int errCode, void *callbackData)
{
  return 0;
}

/*------------------------------------------------------------------------*/
/*  Протокольные функции клиента                 */
/*------------------------------------------------------------------------*/

static int messageToServer (int gTCPConnectionHandle, int comm, int np, int mode, int len, char *pMsg)
{
  return 0;
}

int  __stdcall test1003_open (char *ip, unsigned int port,
                            int gTCPConnectionHandle[])
{
  return 0;
}

int  __stdcall test1003_close (int gTCPConnectionHandle)
{
  return 0;
}

int  __stdcall test1003_set_callback (int gTCPConnectionHandle, void *callback, void *data)
{
 return 0;
}

int  __stdcall test1003_set_mode (int gTCPConnectionHandle, int mode)
{
  return 0;
}

int  __stdcall test1003_set_programm (int gTCPConnectionHandle, int np, int nn,
                                    char *arg3)
{
    return 0;
}

int  __stdcall test1003_set_time (int gTCPConnectionHandle, char *data, char *time)
{
  return 0;
}

int  __stdcall test1003_run_programm (int gTCPConnectionHandle, int np, int cickl)
{
   return 0;
}

int  __stdcall test1003_run_selftest (int gTCPConnectionHandle)
{
    return 0;
}

int  __stdcall test1003_stop (int gTCPConnectionHandle)
{
    return 0;
}

int  __stdcall test1003_state_Q (int gTCPConnectionHandle, unsigned int state[], char message[], float procent[])
{
   return 0;
}

int  __stdcall test1003_reset (int gTCPConnectionHandle)
{
    return 0;
}

int  __stdcall test1003_revision_query (int gTCPConnectionHandle, char vdrv[],
                                      char vsrv[])
{
    return 0;
}

int  __stdcall test1003_error_message (int gTCPConnectionHandle, int error, char message[])
{
    return 0;
}

