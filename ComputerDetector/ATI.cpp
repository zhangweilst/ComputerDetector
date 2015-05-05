#include "stdafx.h"
#include "ATI.h"

CATI::CATI(void)
{
	// ADL load
	HINSTANCE hDLL;
	hDLL = LoadLibrary(_T("atiadlxx.dll"));
	if (hDLL == NULL)
		hDLL = LoadLibrary(_T("atiadlxy.dll"));

	if (hDLL == NULL)
	{
		AfxMessageBox(_T("ADL library not found."));
		return;
	}

	// APIs initialization
	ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE) GetProcAddress(hDLL, "ADL_Main_Control_Create");
	ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY) GetProcAddress(hDLL,"ADL_Main_Control_Destroy");
	ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET) GetProcAddress(hDLL, "ADL_Adapter_NumberOfAdapters_Get");
	ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET) GetProcAddress(hDLL, "ADL_Display_ColorCaps_Get");
	ADL_Display_ColorCaps_Get = (ADL_DISPLAY_COLORCAPS_GET) GetProcAddress(hDLL, "ADL_Adapter_AdapterInfo_Get");
	ADL_Display_Color_Get = (ADL_DISPLAY_COLOR_GET) GetProcAddress(hDLL, "ADL_Display_Color_Get");
	ADL_Display_Color_Set = (ADL_DISPLAY_COLOR_SET) GetProcAddress(hDLL, "ADL_Display_Color_Set");
	ADL_Display_DisplayInfo_Get = (ADL_DISPLAY_DISPLAYINFO_GET) GetProcAddress(hDLL, "ADL_Display_DisplayInfo_Get");
	ADL_Overdrive5_Temperature_Get = (ADL_OVERDRIVE5_TEMPERATURE_GET) GetProcAddress(hDLL, "ADL_Overdrive5_Temperature_Get");
	ADL_Overdrive5_FanSpeed_Get = (ADL_OVERDRIVE5_FANSPEED_GET) GetProcAddress(hDLL, "ADL_Overdrive5_FanSpeed_Get");
	ADL_Overdrive5_CurrentActivity_Get = (ADL_OVERDRIVE5_CURRENTACTIVITY_GET) GetProcAddress(hDLL, "ADL_Overdrive5_CurrentActivity_Get");
	if (ADL_Main_Control_Create == NULL ||
		ADL_Main_Control_Destroy == NULL ||
		ADL_Adapter_NumberOfAdapters_Get == NULL ||
		ADL_Adapter_AdapterInfo_Get == NULL ||
		ADL_Display_ColorCaps_Get == NULL ||
		ADL_Display_Color_Get == NULL ||
		ADL_Display_Color_Set == NULL ||
		ADL_Display_DisplayInfo_Get == NULL ||
		ADL_Overdrive5_Temperature_Get == NULL ||
		ADL_Overdrive5_FanSpeed_Get == NULL ||
		ADL_Overdrive5_CurrentActivity_Get == NULL
		)
	{
		AfxMessageBox(_T("ADL's API is missing."));
	}

	// Initialize ADL
	if (ADL_Main_Control_Create(static_cast<ADL_MAIN_MALLOC_CALLBACK>(&CATI::ADL_Main_Memory_Alloc), 1) != ADL_OK)
	{
		AfxMessageBox(_T("ADL Initialization Error!\n"));
	}

	// m_NumberAdapters
	if (ADL_Adapter_NumberOfAdapters_Get(&m_NumberAdapters) != ADL_OK)
	{
		AfxMessageBox(_T("Cannot get the number of adapters.\n"));
		return;
	}

	// m_lpAdapterInfo
	if (m_NumberAdapters > 0)
	{
		m_lpAdapterInfo = static_cast<LPAdapterInfo>(malloc(sizeof(AdapterInfo) * m_NumberAdapters));
		memset(m_lpAdapterInfo, '\0', sizeof(AdapterInfo) * m_NumberAdapters);

		ADL_Adapter_AdapterInfo_Get(m_lpAdapterInfo, sizeof(AdapterInfo) * m_NumberAdapters);
	}
}

CATI::~CATI(void)
{
}

// ³ÉÔ±º¯Êý

void* __stdcall CATI::ADL_Main_Memory_Alloc(int iSize)
{
	void* lpBuffer = malloc(iSize);
	return lpBuffer;
}

void __stdcall CATI::ADL_Main_Memory_Free(void** lpBuffer)
{
	if (*lpBuffer != NULL)
	{
		free(*lpBuffer);
		*lpBuffer = NULL;
	}
}

void CATI::MakeAdapterActivity()
{
	int nvResult;
	CString str;
	int iAdapterIndex = m_lpAdapterInfo[0].iAdapterIndex;

	nvResult = ADL_Overdrive5_CurrentActivity_Get(iAdapterIndex, &m_ADLPMActivity);
	if (nvResult != ADL_OK)
	{
		m_CoreVoltage = 0;
		m_CoreClock = 0;
		m_MemoryClock = 0;
		m_GPUPercentage = 0;
	}
	m_CoreVoltage = m_ADLPMActivity.iVddc / 1000.0;
	m_CoreClock = m_ADLPMActivity.iEngineClock / 100;
	m_MemoryClock = m_ADLPMActivity.iMemoryClock / 100;
	m_GPUPercentage = m_ADLPMActivity.iActivityPercent;

	if (ADL_Overdrive5_Temperature_Get( iAdapterIndex, 0, &m_ADLTemperature) != ADL_OK)
		m_CoreTemperature = 0;
	else
		m_CoreTemperature = m_ADLTemperature.iTemperature / 1000;

	if (ADL_Overdrive5_FanSpeed_Get( iAdapterIndex, 0, &m_FanSpeedValue) != ADL_OK)
		m_FanSpeed = 0;
	else
		m_FanSpeed = m_FanSpeedValue.iFanSpeed;

	if ( m_MaxCoreVoltage < m_CoreVoltage)
		m_MaxCoreVoltage = m_CoreVoltage;
	if ( m_MaxCoreClock < m_CoreClock)
		m_MaxCoreClock = m_CoreClock;
	if ( m_MaxMemoryClock < m_MemoryClock)
		m_MaxMemoryClock = m_MemoryClock;
	if ( m_MaxCoreTemperature < m_CoreTemperature)
		m_MaxCoreTemperature = m_CoreTemperature;
	if ( m_MaxGPUPercentage < m_GPUPercentage)
		m_MaxGPUPercentage = m_GPUPercentage;
	if ( m_MaxFanSpeed < m_FanSpeed)
		m_MaxFanSpeed = m_FanSpeed;
}
