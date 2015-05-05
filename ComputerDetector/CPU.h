#pragma once
#include <Mmsystem.h>
#include "Hardware.h"
#include "CPUID.h"

class CCPU : public CHardware
{
public:
	CCPU(void);
	~CCPU(void);

// 数据成员
protected:
	CCPUID m_CPUID;				// CCPUID 对象在所有类中统一名字，功能相同

	int m_NumOfProcessors;		// Total processors
	int m_NumOfCores;				// Cores per processor
	int m_NumOfThreads;			// Threads per core
	bool m_bHTT;						// Hyper-Threading or NOT
	bool m_bDTS;						// Digital Temperature Sensor or not
	bool m_bPackageDTS;			// Package DTS or not
	bool m_bPowerMngt;			// Power management or not
	DWORD m_DispFamily;
	DWORD m_DispModel;
	DWORD m_SteppingID;
	CString m_strVersionInfo;		// Family, Model and Stepping ID

	DWORD m_MaxTemp;			//Tjmax值
	DWORD m_CurrentPkgTemp;
	DWORD* m_CurrentCoreTemp;
	DWORD* m_CurrentThreadFreq;
	DWORD* m_CurrentThreadPercentage;
	DWORD m_CurrentCPUPercentage;
	static DWORD m_MaxCrtPkgTemp;
	static DWORD* m_MaxCrtCoreTemp;
	static DWORD* m_MaxCrtThreadFreq;
	static DWORD* m_MaxCrtThreadPercentage;
	static DWORD m_MaxCPUPercentage;
	DWORD m_MaxRatio;			// Max frequency doubling (Non-Turbo for Intel)
	DWORD m_MaxTurboRatio;	// Max turbo frequency doubling (Intel only)
	DWORD m_MinRatio;				// Min frequency doubling
	DWORD m_CPUToBusRatio;
	double m_BusFreq;
	double m_CPUFreq;
	double m_PackagePower;
	double m_CoresPower;
	double m_GraphicsPower;
	double m_DRAMPower;

	CString m_strMicorArchitecture;		// IA32 microarchitecture string
	DWORD m_dwMicorArchitecture;		// IA32 microarchitecture enumeration
	DWORD m_dwLithography;

	TIMECAPS m_TimeCaps;

// 函数成员
public:
	DWORD GetPackageTemperature() {return m_CurrentPkgTemp;}
	DWORD* GetCoreTemperature() { return m_CurrentCoreTemp; }
	DWORD* GetThreadFreq() { return m_CurrentThreadFreq; }

	virtual void MakePackageTemperature() {}
	virtual void MakeCoreTemperature(DWORD_PTR threadId) {}
	virtual void MakeThreadFreq(int nTotalThreads) {}
	virtual void MakeBusAndCPUFreq() {}
	virtual void MakePowerInfo() {}

	DWORD GetMaxTemperature() { return m_MaxTemp; }
	DWORD GetMaxCrtPkgTemp() { return m_MaxCrtPkgTemp; }
	DWORD* GetMaxCrtCoreTemp() { return m_MaxCrtCoreTemp; }
	DWORD* GetMaxCrtThreadFreq() { return m_MaxCrtThreadFreq; }
	DWORD GetMaxTurboRatio() { return m_MaxTurboRatio; }
	DWORD GetMinRatio() { return m_MinRatio; }
	DWORD GetCPUToBusRatio() { return m_CPUToBusRatio; }
	double GetBusFreq() { return m_BusFreq; }
	double GetCPUFreq() { return m_CPUFreq; }
	double GetPackagePower() { return m_PackagePower; }
	double GetCoresPower() { return m_CoresPower; }
	double GetGraphicsPower() { return m_GraphicsPower; }
	double GetDRAMPower() { return m_DRAMPower; }

	int GetNumOfProcessors() { return m_NumOfProcessors; }
	int GetNumOfCores() { return m_NumOfCores; }
	int GetNumOfThreads() { return m_NumOfThreads; }
	int GetTotalThreads() { return m_NumOfCores * m_NumOfThreads; }
	bool IsHTT() { return m_bHTT; }
	bool IsDTS() { return m_bDTS; }
	bool IsPackageDTS() { return m_bPackageDTS; }
	bool IsPowerMngt() { return m_bPowerMngt; }

	CString GetVersionInfo() { return m_strVersionInfo; }
	CString GetMicroArchitecture() { return m_strMicorArchitecture; }
	DWORD GetLithography() { return m_dwLithography; }

	DWORD* GetThreadPercentage();
	DWORD* GetMaxCrtThreadPercentage();
	DWORD GetCPUPercentage();
	DWORD GetMaxCPUPercentage();
};
