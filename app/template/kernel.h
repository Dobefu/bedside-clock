//
// kernel.h
//
#ifndef _kernel_h
#define _kernel_h

#include <circle/2dgraphics.h>
#include <circle/actled.h>
#include <circle/devicenameservice.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/koptions.h>
#include <circle/logger.h>
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/time.h>
#include <circle/timer.h>
#include <circle/types.h>

enum TShutdownMode
{
	ShutdownNone,
	ShutdownHalt,
	ShutdownReboot
};

class CKernel
{
public:
	CKernel(void);
	~CKernel(void);

	boolean Initialize(void);

	TShutdownMode Run(void);

private:
	// do not change this order
	CActLED m_ActLED;
	CKernelOptions m_Options;
	CDeviceNameService m_DeviceNameService;
	CScreenDevice m_Screen;
	CSerialDevice m_Serial;
	CExceptionHandler m_ExceptionHandler;
	CInterruptSystem m_Interrupt;
	CTimer m_Timer;
	CLogger m_Logger;

	CTime m_Time;
	C2DGraphics m_Graphics;
};

#endif
