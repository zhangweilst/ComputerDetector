#pragma once
#include "CPU.h"
#include "OlsApi.h"

// defines

#define MSR_TEMPERATURE_TARGET 0x1A2
#define IA32_PACKAGE_THERM_STATUS 0x1B1
#define IA32_THERM_STATUS 0x19C
#define IA32_PERFEVTSEL0 0x186
#define IA32_PERFEVTSEL1 0x187
#define IA32_PMC0 0x0C1
#define IA32_PMC1 0x0C2
#define IA32_A_PMC0 0x4C1
#define IA32_A_PMC1 0x4C2
#define IA32_PERF_GLOBAL_CTRL 0x38F
#define MSR_PLATFORM_INFO 0x0CE
#define IA32_PERF_STATUS 0x198
#define MSR_TURBO_RATIO_LIMIT 0x1AD
#define MSR_RAPL_POWER_UNIT 0x606
#define MSR_PKG_ENERGY_STATUS 0x611
#define MSR_DRAM_ENERGY_STATUS 0x619
#define MSR_PP0_ENERGY_STATUS 0x639
#define MSR_PP1_ENERGY_STATUS 0x641

#define IA32_UNKNOWN 0xA00
#define IA32_PENTIUM 0xA01
#define IA32_P6 0xA02
#define IA32_NETBURST 0xA03
#define IA32_CORE_DUO_OR_SOLO 0xA04
#define IA32_CORE 0xA05
#define IA32_ENHANCED_CORE 0xA06
#define IA32_ATOM 0xA07
#define IA32_SILVERMOUNT 0xA08
#define IA32_NEHALEM 0xA09
#define IA32_WESTMERE 0xA0A
#define IA32_WESTMERE_E 0xA0B
#define IA32_SANDYBRIDGE 0xA0C
#define IA32_SANDYBRIDGE_E 0xA0D
#define IA32_IVYBRIDGE 0xA0E
#define IA32_IVYBRIDGE_E 0xA0F
#define IA32_HASWELL 0xA10
#define IA32_HASWELL_E 0xA11
#define IA32_BROADWELL 0xA12

typedef struct tagRaplPowerUnit
{
	double m_PowerUnit;
	double m_EnergyUnit;
	double m_TimeUnit;
}RAPL_POWER_UNIT;


class CIntel : public CCPU
{
public:
	CIntel(void);
	~CIntel(void);

// 数据成员
protected:
	RAPL_POWER_UNIT m_RaplPowerUnit;

// 函数成员
public:
	virtual void MakePackageTemperature();
	virtual void MakeCoreTemperature(DWORD_PTR threadId);
	virtual void MakeThreadFreq(int nTotalThreads);
	virtual void MakeBusAndCPUFreq();
	virtual void MakePowerInfo();
private:
	void MakeVersionInfo();		// Initialize display Family, Model, Stepping and version string,
												// executed once in the constructor
	void MakeArchitectureInfo();
};
