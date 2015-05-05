#include "stdafx.h"
#include "Intel.h"

CIntel::CIntel(void)
{
	// Version info
	MakeVersionInfo();

	// m_dwLithography
	m_dwLithography = 0;

	// Architecture info
	MakeArchitectureInfo();

	// m_bDTS & m_bPackageDTS
	if (m_CPUID.EAX(0x06).EAX & 0x1)
	{
		m_bDTS = true;
		if ( m_CPUID.EAX(0x06).EAX & 0x40)
			m_bPackageDTS = true;
		else
			m_bPackageDTS = false;
	}
	else
	{
		m_bDTS = false;
		m_bPackageDTS = false;
	}

	// m_MaxTemp
	DWORD eax = 0;
	DWORD edx = 0;
	Rdmsr(MSR_TEMPERATURE_TARGET, &eax, &edx);
	m_MaxTemp = 100; // (eax & 0xFF0000) >> 16;

	// m_MaxRatio & m_MinRatio
	Rdmsr(MSR_PLATFORM_INFO, &eax, &edx);
	m_MaxRatio = (eax >> 8) & 0xFF;
	m_MinRatio = (edx >> 8) & 0xFF;

	// m_MaxTurboRatio
	Rdmsr(MSR_TURBO_RATIO_LIMIT, &eax, &edx);
	m_MaxTurboRatio = eax & 0xFF;

	// RAPL power related
	if (Rdmsr(MSR_RAPL_POWER_UNIT, &eax, &edx) == TRUE)
	{
		m_bPowerMngt = true;
		m_RaplPowerUnit.m_PowerUnit =1.0 / (1 << (eax & 0xF));
		m_RaplPowerUnit.m_EnergyUnit = 1.0 / (1 << ((eax >> 8) & 0x1F));
		m_RaplPowerUnit.m_TimeUnit = 1.0 / (1 << ((eax >> 16) & 0xF));
	}
}

CIntel::~CIntel(void)
{
}

// ³ÉÔ±º¯Êý
void CIntel::MakePackageTemperature()
{
	DWORD eax = 0;
	DWORD edx = 0;

	Rdmsr(IA32_PACKAGE_THERM_STATUS, &eax, &edx);

	m_CurrentPkgTemp = (m_MaxTemp - ((eax & 0x7F0000) >> 16));

	if (m_MaxCrtPkgTemp < m_CurrentPkgTemp)
		m_MaxCrtPkgTemp = m_CurrentPkgTemp;
}

void CIntel::MakeCoreTemperature(DWORD_PTR threadId)
{
	DWORD_PTR threadAffinity;
	threadAffinity = 1 << threadId;
	DWORD eax = 0;
	DWORD edx = 0;

	RdmsrTx(IA32_THERM_STATUS, &eax, &edx, threadAffinity);

	m_CurrentCoreTemp[threadId / m_NumOfThreads] = (m_MaxTemp - ((eax & 0x7F0000) >> 16));

	if (m_MaxCrtCoreTemp[threadId / m_NumOfThreads] < m_CurrentCoreTemp[threadId / m_NumOfThreads])
		m_MaxCrtCoreTemp[threadId / m_NumOfThreads] = m_CurrentCoreTemp[threadId / m_NumOfThreads];
}

void CIntel::MakeThreadFreq(int nTotalThreads)
{
	DWORD_PTR threadAffinity;
	
	DWORD* eax1 = new DWORD[nTotalThreads];
	DWORD* eax2 = new DWORD[nTotalThreads];
	DWORD* edx1 = new DWORD[nTotalThreads];
	DWORD* edx2 = new DWORD[nTotalThreads];
	double* freq = new double[nTotalThreads];
	double* ex1 = new double[nTotalThreads];
	double* ex2 = new double[nTotalThreads];

	DWORD startTime1 = timeGetTime();
	for (int i = 0; i < nTotalThreads; i++)
	{
		threadAffinity = 1 << i;

	if (WrmsrTx(IA32_PMC0, 0x0, 0x0, threadAffinity) == FALSE)
	{
		m_CurrentThreadFreq[i] = 0;
		goto release;
	}
	if (WrmsrTx(IA32_PERFEVTSEL0, 0x43003C, 0x0, threadAffinity) == FALSE)
	{
		m_CurrentThreadFreq[i] = 0;
		goto release;
	}
	if (WrmsrTx(IA32_PERF_GLOBAL_CTRL, 0x1, 0x0, threadAffinity) == FALSE)
	{
		m_CurrentThreadFreq[i] = 0;
		goto release;
	}

	
	if (RdmsrTx(IA32_PMC0, &eax1[i], &edx1[i], threadAffinity) == FALSE)
	{
		m_CurrentThreadFreq[i] = 0;
		goto release;
	}
	}
	DWORD endTime1 = timeGetTime();
	if ((endTime1 - startTime1) >= 1)
		goto release;
	timeBeginPeriod(m_TimeCaps.wPeriodMin);
	Sleep(990);
	timeEndPeriod(m_TimeCaps.wPeriodMin);

	DWORD startTime2 = timeGetTime();
	for (int i = 0; i < nTotalThreads; i++)
	{
		threadAffinity = 1 << i;
	if (RdmsrTx(IA32_PMC0, &eax2[i], &edx2[i], threadAffinity) == FALSE)
	{
		m_CurrentThreadFreq[i] = 0;
		goto release;
	}
	}
	DWORD endTime2 = timeGetTime();
	if ((endTime2 - startTime2) >= 1)
		goto release;

	double durTime = (endTime2 - startTime1) / 1000.0;

	for (int i = 0; i < nTotalThreads; i++)
	{
	ex1[i] = (edx1[i] & 0xFFFF) * 0x100000000 * 1.0 + eax1[i];
	ex2[i] = (edx2[i] & 0xFFFF) * 0x100000000 * 1.0 + eax2[i];
	freq[i] = (ex2[i] - ex1[i]) / 1000000.0 /durTime;
	m_CurrentThreadFreq[i] = static_cast<DWORD> (freq[i]);
	

	if (m_MaxCrtThreadFreq[i] < m_CurrentThreadFreq[i])
		m_MaxCrtThreadFreq[i] = m_CurrentThreadFreq[i];
	}

	// Release
release:
	delete [] eax1;
	delete [] edx1;
	delete [] eax2;
	delete [] edx2;
	delete [] freq;
	delete [] ex1;
	delete [] ex2;
	return;
}

void CIntel::MakeBusAndCPUFreq()
{
	DWORD eax1;
	DWORD eax2;
	DWORD edx1;
	DWORD edx2;
	double freq;

	// Bus frequency
	DWORD startTime = timeGetTime();
	Rdtsc(&eax1, &edx1);
	timeBeginPeriod(m_TimeCaps.wPeriodMin);
	Sleep(500);
	timeEndPeriod(m_TimeCaps.wPeriodMin);
	Rdtsc(&eax2, &edx2);
	DWORD endTime = timeGetTime();

	DWORD delta = (endTime - startTime) - 500;
	if (delta < 1)
	{
		freq = (edx2 * 0x100000000 * 1.0 + eax2) - (edx1 * 0x100000000 * 1.0 + eax1);
		m_BusFreq = freq / 500000.0 / m_MaxRatio;
	}

	// CPU frequency
	Rdmsr(IA32_PERF_STATUS, &eax1, &edx1);
	m_CPUToBusRatio = (eax1 >> 8) & 0xFF;
	m_CPUFreq = m_CPUToBusRatio * m_BusFreq;
}

void CIntel::MakePowerInfo()
{
	DWORD eax1, eax2;
	DWORD eax3, eax4;
	DWORD eax5, eax6;
	DWORD eax7, eax8;
	DWORD edx1, edx2;
	DWORD edx3, edx4;
	DWORD edx5, edx6;
	DWORD edx7, edx8;

	Rdmsr(MSR_PKG_ENERGY_STATUS, &eax1, &edx1);
	Rdmsr(MSR_DRAM_ENERGY_STATUS, &eax3, &edx3);
	Rdmsr(MSR_PP0_ENERGY_STATUS, &eax5, &edx5);
	Rdmsr(MSR_PP1_ENERGY_STATUS, &eax7, &edx7);
	Sleep(500);
	Rdmsr(MSR_PKG_ENERGY_STATUS, &eax2, &edx2);
	Rdmsr(MSR_DRAM_ENERGY_STATUS, &eax4, &edx4);
	Rdmsr(MSR_PP0_ENERGY_STATUS, &eax6, &edx6);
	Rdmsr(MSR_PP1_ENERGY_STATUS, &eax8, &edx8);

	m_PackagePower = (eax2 - eax1) * m_RaplPowerUnit.m_EnergyUnit / 0.5;
	m_DRAMPower = (eax4 - eax3) * m_RaplPowerUnit.m_EnergyUnit / 0.5;
	m_CoresPower = (eax6 - eax5) * m_RaplPowerUnit.m_EnergyUnit / 0.5;
	m_GraphicsPower = (eax8 - eax7) * m_RaplPowerUnit.m_EnergyUnit / 0.5;
}

// private
void CIntel::MakeVersionInfo()
{
	DWORD FamilyID;
	DWORD ExFamilyID;
	DWORD Model;
	DWORD ExModelID;
	DWORD SteppingID;

	SteppingID = m_CPUID.EAX(0x01).EAX & 0xF;
	Model = (m_CPUID.EAX(0x01).EAX >> 4) & 0xF;
	FamilyID = (m_CPUID.EAX(0x01).EAX >> 8) & 0xF;

	if (FamilyID == 0x0F)
		ExFamilyID = (m_CPUID.EAX(0x01).EAX >> 20) & 0xFF;
	else
		ExFamilyID = 0;

	if (FamilyID == 0x06 || FamilyID == 0x0F)
		ExModelID = (m_CPUID.EAX(0x01).EAX >> 16) & 0xF;
	else
		ExModelID = 0;

	m_SteppingID = SteppingID;
	m_DispModel = (ExModelID << 4) + Model;
	m_DispFamily = ExFamilyID + FamilyID;
	m_strVersionInfo.Format(_T("Family %X Model %X Stepping %X"),
		m_DispFamily, m_DispModel, m_SteppingID);
}

void CIntel::MakeArchitectureInfo()
{
	switch(m_DispFamily)
	{
	case 0x05:
		switch(m_DispModel)
		{
		case 0x01:
		case 0x02:
		case 0x05:
			m_strMicorArchitecture = _T("Pentium");
			m_dwMicorArchitecture = IA32_PENTIUM;
			break;
		default:
			m_strMicorArchitecture = _T("Unknown");
			m_dwMicorArchitecture = IA32_UNKNOWN;
			break;
		}
		break;

	case 0x06:
		switch(m_DispModel)
		{
		case 0x01:
		case 0x03:
		case 0x05:
		case 0x07:
		case 0x08:
		case 0x0A:
		case 0x0B:
			m_strMicorArchitecture = _T("P6");
			m_dwMicorArchitecture = IA32_P6;
			break;
		case 0x1C:
		case 0x26:
		case 0x27:
		case 0x35:
			m_strMicorArchitecture = _T("ATOM");
			m_dwMicorArchitecture = IA32_ATOM;
			m_dwLithography = 45;
			break;
		case 0x36:
		case 0x4D:
		case 0x37:
			m_strMicorArchitecture = _T("Sivermount");
			m_dwMicorArchitecture = IA32_SILVERMOUNT;
			m_dwLithography = 22;
			break;
		case 0x0E:
			m_strMicorArchitecture = _T("Core Duo / Solo");
			m_dwMicorArchitecture = IA32_CORE_DUO_OR_SOLO;
			m_dwLithography = 65;
			break;
		case 0x0F:
			m_strMicorArchitecture = _T("Core");
			m_dwMicorArchitecture = IA32_CORE;
			m_dwLithography = 65;
			break;
		case 0x17:
		case 0x1D:
			m_strMicorArchitecture = _T("Enhanced Core");
			m_dwMicorArchitecture = IA32_ENHANCED_CORE;
			m_dwLithography = 45;
			break;
		case 0x1A:
		case 0x2E:
			m_strMicorArchitecture = _T("Nehalem");
			m_dwMicorArchitecture = IA32_NEHALEM;
			m_dwLithography = 45;
			break;
		case 0x1E:
		case 0x1F:
		case 0x25:
		case 0x2C:
			m_strMicorArchitecture = _T("Westmere");
			m_dwMicorArchitecture = IA32_WESTMERE;
			m_dwLithography = 32;
			break;
		case 0x2A:
			m_strMicorArchitecture = _T("Sandy Bridge");
			m_dwMicorArchitecture = IA32_SANDYBRIDGE;
			m_dwLithography = 32;
			break;
		case 0x2F:
			m_strMicorArchitecture = _T("Westmere-E");
			m_dwMicorArchitecture = IA32_WESTMERE_E;
			m_dwLithography = 32;
			break;
		case 0x2D:
			m_strMicorArchitecture = _T("Sandy Bridge-E");
			m_dwMicorArchitecture = IA32_SANDYBRIDGE_E;
			m_dwLithography = 32;
			break;
		case 0x3A:
			m_strMicorArchitecture = _T("Ivy Bridge");
			m_dwMicorArchitecture = IA32_IVYBRIDGE;
			m_dwLithography = 22;
			break;
		case 0x3E:
			m_strMicorArchitecture = _T("Ivy Bridge-E");
			m_dwMicorArchitecture = IA32_IVYBRIDGE_E;
			m_dwLithography = 22;
			break;
		case 0x3C:
		case 0x45:
		case 0x46:
			m_strMicorArchitecture = _T("Haswell");
			m_dwMicorArchitecture = IA32_HASWELL;
			m_dwLithography = 22;
			break;
		case 0x3F:
			m_strMicorArchitecture = _T("Haswell-E");
			m_dwMicorArchitecture = IA32_HASWELL_E;
			m_dwLithography = 22;
			break;
		default:
			m_strMicorArchitecture = _T("Unknown");
			m_dwMicorArchitecture = IA32_UNKNOWN;
			break;
		}
		break;

	case 0x0F:
		m_strMicorArchitecture = _T("NetBurst");
		m_dwMicorArchitecture = IA32_NETBURST;
		break;

	default:
		m_strMicorArchitecture = _T("Unknown");
		m_dwMicorArchitecture = IA32_UNKNOWN;
		break;
	}
}
