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
}

CBaseBoard::~CBaseBoard(void)
{
}
