#include "stdafx.h"
#include "Nvidia.h"

CNvidia::CNvidia(void)
{
	// NVAPI initialize
	NvAPI_Status nvResult = NVAPI_OK;
	nvResult = NvAPI_Initialize();
	if (nvResult != NVAPI_OK)
		AfxMessageBox(_T("Error Initialize NVAPI."));

	// Physical gpu enumeration
	nvResult = NvAPI_EnumPhysicalGPUs(&m_nvGPUHandle, &m_gpuCount);
	if (nvResult != NVAPI_OK)
		AfxMessageBox(_T("Error Get PhysicalGPU."));

	// version
	m_settings.version = NV_GPU_THERMAL_SETTINGS_VER_1;
	m_clkFreqs.version = NV_GPU_CLOCK_FREQUENCIES_VER;
	m_clkFreqs.ClockType = NV_GPU_CLOCK_FREQUENCIES_CURRENT_FREQ;
}

CNvidia::~CNvidia(void)
{
}

void CNvidia::MakeAdapterActivity()
{
	int iResult;
	if (NvAPI_GPU_GetThermalSettings(m_nvGPUHandle, 0, &m_settings) != NVAPI_OK)
		m_CoreTemperature = 0;
	else
		m_CoreTemperature = m_settings.sensor[0].currentTemp;

	if (NvAPI_GPU_GetTachReading(m_nvGPUHandle, static_cast<NvU32*>(&m_FanSpeed)) != NVAPI_OK)
		m_FanSpeed = 0;

	iResult = NvAPI_GPU_GetAllClockFrequencies(m_nvGPUHandle, &m_clkFreqs);
	if (iResult != NVAPI_OK)
	{
		m_CoreClock = 0;
		m_MemoryClock = 0;
	}
	else
	{
		m_CoreClock = (DWORD)(m_clkFreqs.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency / 1000);
		m_MemoryClock = (DWORD)(m_clkFreqs.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency / 1000);
	}
	
	if (m_MaxCoreTemperature < m_CoreTemperature)
		m_MaxCoreTemperature = m_CoreTemperature;
	if (m_MaxCoreClock < m_CoreClock)
		m_MaxCoreClock = m_CoreClock;
	if (m_MaxMemoryClock < m_MemoryClock)
		m_MaxMemoryClock = m_MemoryClock;
	if (m_MaxFanSpeed < m_FanSpeed)
		m_MaxFanSpeed = m_FanSpeed;
}
