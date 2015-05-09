#include "stdafx.h"
#include "BaseBoard.h"

CBaseBoard::CBaseBoard(void)
{
	// m_strName
	m_strName = m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_ComputerSystem"), L"Manufacturer").GetAt(0).bstrVal;
	m_strName.Append(" ");
	CString strAppend;
	strAppend = m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_BaseBoard"), L"Product").GetAt(0).bstrVal;
	m_strName += strAppend;

	// m_CurrentTemperature
	/***********************************************************
	 * Currently Win32_TemperatureProbe class of WMI ROOT\CIMV2 namespace *
	 * is not ready for realtime temperature monitoring(See MSDN). Instead, we   *
	 * use MSAcpi_ThermalZoneTemperature class of ROOT\CIMV2. If it returns    *
	 * nothing, we will assume baseboard temperature is not supported.               *
	 ***********************************************************/
	m_BaseBoardSensorCount = m_WMI.GetWMIProperty(bstr_t("ROOT\\WMI"),
		bstr_t("MSAcpi_ThermalZoneTemperature"), L"Active").GetCount();
	if (m_BaseBoardSensorCount == 0)
		m_CurrentTemperature = NULL;
	else
		m_CurrentTemperature = new float[m_BaseBoardSensorCount];
}

// ³ÉÔ±º¯Êý
float* CBaseBoard::GetBaseBoardTemperture()
{
	for (int i = 0; i < m_BaseBoardSensorCount; i++)
	{
		m_CurrentTemperature[i] = static_cast<float>(m_WMI.GetWMIProperty(bstr_t("ROOT\\WMI"),
		bstr_t("MSAcpi_ThermalZoneTemperature"), L"CurrentTemperature").GetAt(i).uintVal);
		m_CurrentTemperature[i] = (m_CurrentTemperature[i] - 2732) / 10;
	}

	return m_CurrentTemperature;
}

CBaseBoard::~CBaseBoard(void)
{
}
