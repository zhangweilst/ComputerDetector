#include "stdafx.h"
#include "GraphicsAdapter.h"

// ��̬��������
double CGraphicsAdapter::m_MaxCoreVoltage = 0.0;
DWORD CGraphicsAdapter::m_MaxCoreClock;
DWORD CGraphicsAdapter::m_MaxMemoryClock;
DWORD CGraphicsAdapter::m_MaxCoreTemperature;
DWORD CGraphicsAdapter::m_MaxGPUPercentage;
DWORD CGraphicsAdapter::m_MaxFanSpeed;

CGraphicsAdapter::CGraphicsAdapter(void)
{
	// �Կ��ܲ�֧�ֵĲ�����0
	m_CoreVoltage = 0.0;
	m_GPUPercentage = 0;
	m_FanSpeed = 0;
	
	// m_strName
	m_strName = m_WMI.GetWMIProperty(bstr_t("Win32_VideoController"), L"Name").GetAt(0).bstrVal;
}

CGraphicsAdapter::~CGraphicsAdapter(void)
{
}
