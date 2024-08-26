//
// kernel.cpp
//
#include "kernel.h"
#include <circle/string.h>

static const char FromKernel[] = "kernel";

CKernel::CKernel(void)
		: m_Screen(m_Options.GetWidth(), m_Options.GetHeight()),
			m_Timer(&m_Interrupt),
			m_Logger(m_Options.GetLogLevel(), &m_Timer),
			m_Time(),
			m_Graphics(0, 0, false)
{
}

CKernel::~CKernel(void)
{
}

boolean CKernel::Initialize(void)
{
	boolean bOK = TRUE;

	if (bOK)
	{
		bOK = m_Screen.Initialize();
	}

	if (bOK)
	{
		bOK = m_Serial.Initialize(115200);
	}

	if (bOK)
	{
		bOK = m_Interrupt.Initialize();
	}

	if (bOK)
	{
		bOK = m_Timer.Initialize();
	}

	if (bOK)
	{
		CDevice *pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice(), FALSE);
		if (pTarget == 0)
		{
			pTarget = &m_Screen;
		}

		bOK = m_Logger.Initialize(pTarget);
	}

	if (bOK)
	{
		bOK = m_Graphics.Initialize();
	}

	return bOK;
}

TShutdownMode CKernel::Run(void)
{
	const char *time = (char *)m_Timer.GetTimeString();

	while (1)
	{
		continue;
		m_Graphics.ClearScreen(BLACK_COLOR);

		m_Graphics.DrawText(100, 100, WHITE_COLOR, time, C2DGraphics::AlignRight);

		m_Graphics.UpdateDisplay();
	}

	return ShutdownHalt;
}
