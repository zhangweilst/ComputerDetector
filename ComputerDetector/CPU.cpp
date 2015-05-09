#include "stdafx.h"
#include "CPU.h"

DWORD CCPU::m_MaxCrtPkgTemp;
DWORD* CCPU::m_MaxCrtCoreTemp;
DWORD* CCPU::m_MaxCrtThreadFreq;
DWORD* CCPU::m_MaxCrtThreadPercentage;
DWORD CCPU::m_MaxCPUPercentage;

CCPU::CCPU(void)
{
	// m_bHTT
	if ((m_CPUID.EAX(0x01).EDX & 0x10000000) >> 28)
		m_bHTT = true;
	else
		m_bHTT = false;

	// Number of processors, cores and threads
	m_NumOfProcessors = m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_Processor"), L"CpuStatus").GetCount();
	m_NumOfCores = m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_Processor"), L"NumberOfCores").GetAt(0).intVal;

	if (m_bHTT)
		m_NumOfThreads = m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_Processor"), L"NumberOfLogicalProcessors").GetAt(0).intVal /
		m_NumOfCores;
	else
		m_NumOfThreads = 1;

	// m_MaxCrtCoreTemp初始化为0
	int nTotalCores = m_NumOfProcessors * m_NumOfCores;
	m_MaxCrtCoreTemp = new DWORD[nTotalCores];
	for (int i = 0; i < nTotalCores; i++)
		m_MaxCrtCoreTemp[i] = 0;

	// m_CurrentCoreTemp
	m_CurrentCoreTemp = new DWORD[nTotalCores];

	// m_MaxCrtThreadFreq, m_MaxCrtThreadPercentage初始化为0
	int nTotalThreads = m_NumOfProcessors * m_NumOfCores * m_NumOfThreads;
	m_MaxCrtThreadFreq = new DWORD[nTotalThreads];
	m_MaxCrtThreadPercentage = new DWORD[nTotalThreads];
	for (int i = 0; i < nTotalThreads; i++)
	{
		m_MaxCrtThreadFreq[i] = 0;
		m_MaxCrtThreadPercentage[i] = 0;
	}

	// m_CurrentThreadFreq
	m_CurrentThreadFreq = new DWORD[nTotalThreads];
	m_CurrentThreadPercentage = new DWORD[nTotalThreads];

	// m_strName
	m_strName.Empty();
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000002).EAX));
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000002).EBX));
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000002).ECX));
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000002).EDX));
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000003).EAX));
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000003).EBX));
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000003).ECX));
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000003).EDX));
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000004).EAX));
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000004).EBX));
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000004).ECX));
	m_strName.Append(ASCIITranslate(m_CPUID.EAX(0x80000004).EDX));
	if (m_NumOfProcessors > 1)
		m_strName.AppendFormat(_T(" (%d处理器)"), m_NumOfProcessors);

	// m_TimeCaps
	timeGetDevCaps(&m_TimeCaps, sizeof(m_TimeCaps));

	// m_MaxTurboRatio
	m_MaxTurboRatio = 0;

	// m_bPowerMngt
	m_bPowerMngt = false;
}

CCPU::~CCPU(void)
{
}

// 成员函数

DWORD* CCPU::GetThreadPercentage()
{
	int nTotalThreads = m_NumOfProcessors * m_NumOfCores * m_NumOfThreads;
	double MaxFrequency = m_BusFreq * m_MaxRatio;
	for ( int i = 0; i < nTotalThreads; i++)
	{
		m_CurrentThreadPercentage[i] = static_cast<DWORD>((m_CurrentThreadFreq[i] / MaxFrequency) * 100);
		if (m_CurrentThreadPercentage[i] > 100)
			m_CurrentThreadPercentage[i] = 100;
	}

	return m_CurrentThreadPercentage;
}

DWORD* CCPU::GetMaxCrtThreadPercentage()
{
	int nTotalThreads = m_NumOfProcessors * m_NumOfCores * m_NumOfThreads;
	for ( int i = 0; i < nTotalThreads; i++)
		if ( m_MaxCrtThreadPercentage[i] < m_CurrentThreadPercentage[i])
			m_MaxCrtThreadPercentage[i] = m_CurrentThreadPercentage[i];

	return m_MaxCrtThreadPercentage;
}

DWORD CCPU::GetCPUPercentage()
{
	int nTotalThreads = m_NumOfProcessors * m_NumOfCores * m_NumOfThreads;

	m_CurrentCPUPercentage = 0;
	for ( int i = 0; i < nTotalThreads; i++)
		m_CurrentCPUPercentage += m_CurrentThreadPercentage[i];

	m_CurrentCPUPercentage /= nTotalThreads;
	return m_CurrentCPUPercentage;
}

DWORD CCPU::GetMaxCPUPercentage()
{
	if (m_CurrentCPUPercentage <= 100 && m_MaxCPUPercentage < m_CurrentCPUPercentage)
		m_MaxCPUPercentage = m_CurrentCPUPercentage;

	return m_MaxCPUPercentage;
}
