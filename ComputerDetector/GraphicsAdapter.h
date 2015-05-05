#pragma once
#include "Hardware.h"

class CGraphicsAdapter : public CHardware
{
public:
	CGraphicsAdapter(void);
	~CGraphicsAdapter(void);

// 成员变量
protected:
	double m_CoreVoltage;
	static double m_MaxCoreVoltage;
	DWORD m_CoreClock;
	static DWORD m_MaxCoreClock;
	DWORD m_MemoryClock;
	static DWORD m_MaxMemoryClock;
	DWORD m_CoreTemperature;
	static DWORD m_MaxCoreTemperature;
	DWORD m_GPUPercentage;
	static DWORD m_MaxGPUPercentage;
	DWORD m_FanSpeed;
	static DWORD m_MaxFanSpeed;

// 成员函数
public:
	virtual double GetCoreVoltage() { return m_CoreVoltage; }
	virtual DWORD GetCoreClock() { return m_CoreClock; }
	virtual DWORD GetMemoryClock() { return m_MemoryClock; }
	virtual DWORD GetCoreTemperature() { return m_CoreTemperature; }
	virtual DWORD GetGPUPercentage() { return m_GPUPercentage; }
	virtual DWORD GetFanSpeed() { return m_FanSpeed; }

	double GetMaxCoreVoltage() { return m_MaxCoreVoltage; }
	DWORD GetMaxCoreClock() { return m_MaxCoreClock; }
	DWORD GetMaxMemoryClock() { return m_MaxMemoryClock; }
	DWORD GetMaxCoreTemperature() { return m_MaxCoreTemperature; }
	DWORD GetMaxGPUPercentage() { return m_MaxGPUPercentage; }
	DWORD GetMaxFanSpeed() { return m_MaxFanSpeed; }

	virtual void MakeAdapterActivity() {}
};
