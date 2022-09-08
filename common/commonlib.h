#ifndef __COMMON_LIB_H__
#define __COMMON_LIB_H__

#include "Algorithm.h"
#include "BatRunner.h"
//#include "Commu.h"
#include "cTimer.h"
//#include "Hal.h"
//#include "Ping.h"
//#include "Pipe.Shell.h"
#include "Signal.h"
#include "StringBase.h"
//#include "TcpUdp.h"
//#include "Uart.h"
#include "CritSection.h"
//#include "USB.h"
//#include "SSH.h"

//窗口上显示的信息为 please input + spec
string COMMON_API requestExternInput(int cordId, int iCount);
bool COMMON_API brequestExternInput( int iCount,string* strDutid,string* strBarCode);
#endif
