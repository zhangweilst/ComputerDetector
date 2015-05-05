#include "stdafx.h"
#include "AMD.h"

CAMD::CAMD()
{
	//Version info
	MakeVersionInfo();
}

CAMD::~CAMD()
{
}

void CAMD::MakeVersionInfo()
{
	DWORD BaseFamily;
	DWORD ExtendedFamily;
	DWORD BaseModel;
	DWORD ExtendedModel;
	DWORD Stepping;

	Stepping = m_CPUID.EAX(0x01).EAX & 0xF;
	BaseModel = (m_CPUID.EAX(0x01).EAX >> 4) & 0xF;
	BaseFamily = (m_CPUID.EAX(0x01).EAX >> 8) & 0xF;

	if (BaseFamily == 0x0F)
		ExtendedFamily = (m_CPUID.EAX(0x01).EAX >> 20) & 0xFF;
	else
		ExtendedFamily = 0;

	if (BaseFamily == 0x0F)
		ExtendedModel = (m_CPUID.EAX(0x01).EAX >> 16) & 0xF;
	else
		ExtendedModel = 0;

	m_SteppingID = Stepping;
	m_DispModel = (ExtendedModel << 4) + BaseModel;
	m_DispFamily = ExtendedFamily + BaseFamily;
	m_strVersionInfo.Format(_T("Family %X Model %X Stepping %X"),
		m_DispFamily, m_DispModel, m_SteppingID);
}