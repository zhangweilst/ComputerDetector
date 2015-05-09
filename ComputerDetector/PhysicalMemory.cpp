#include "stdafx.h"
#include "PhysicalMemory.h"

CPhysicalMemory::CPhysicalMemory(void)
{
	// Initialize member variables
	m_TotalMemoryCapacity = 0;
	m_NumOfDimms = 0;

	// CArray type member variables, m_NumOfDimms
	CString locate;
	int rawNum = m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_PhysicalMemory"), L"DeviceLocator").GetCount();
	for (int i = 0; i < rawNum; i++)
	{
		locate = m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_PhysicalMemory"), L"DeviceLocator").GetAt(i).bstrVal;
		if (locate.Find(_T("DIMM")) != -1)
		{
			m_MemoryLocator.Add(m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_PhysicalMemory"), L"DeviceLocator").GetAt(i));
			m_MemoryCapacity.Add(m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_PhysicalMemory"), L"Capacity").GetAt(i));
			m_MemorySerialNumber.Add(m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_PhysicalMemory"), L"SerialNumber").GetAt(i));
			m_MemorySpeed.Add(m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_PhysicalMemory"), L"Speed").GetAt(i));
			m_NumOfDimms++;
		}
	}

	// m_TotalMemoryCapacity
	for (int j = 0; j < m_NumOfDimms; j++)
		m_TotalMemoryCapacity += m_MemoryCapacity.GetAt(j).dblVal;
}

CPhysicalMemory::~CPhysicalMemory(void)
{
}
