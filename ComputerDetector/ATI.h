#pragma once
#include "GraphicsAdapter.h"
#include "ADL\adl_sdk.h"

// Definitions of the used APIs
typedef int (*ADL_MAIN_CONTROL_CREATE) (ADL_MAIN_MALLOC_CALLBACK, int);
typedef int (*ADL_MAIN_CONTROL_DESTROY) ();
typedef int (*ADL_ADAPTER_NUMBEROFADAPTERS_GET) (int*);
typedef int (*ADL_ADAPTER_ADAPTERINFO_GET) (LPAdapterInfo, int);
typedef int (*ADL_DISPLAY_COLORCAPS_GET) (int, int, int*, int*);
typedef int (*ADL_DISPLAY_COLOR_GET) (int, int, int, int*, int*, int*, int*, int*);
typedef int (*ADL_DISPLAY_COLOR_SET) (int, int, int, int);
typedef int (*ADL_DISPLAY_DISPLAYINFO_GET)	(int, int*, ADLDisplayInfo**, int);
typedef int (*ADL_OVERDRIVE5_TEMPERATURE_GET) (int, int, ADLTemperature*);
typedef int (*ADL_OVERDRIVE5_FANSPEED_GET) (int, int, ADLFanSpeedValue*);
typedef int (*ADL_OVERDRIVE5_CURRENTACTIVITY_GET) (int, ADLPMActivity*);

class CATI : public CGraphicsAdapter
{
public:
	CATI(void);
	~CATI(void);

// 成员变量
protected:
	int m_NumberAdapters;
	LPAdapterInfo m_lpAdapterInfo;
	ADLTemperature m_ADLTemperature;
	ADLPMActivity m_ADLPMActivity;
	ADLFanSpeedValue m_FanSpeedValue;

// 成员函数
public:
	// Memory allocation function
	static void* __stdcall ADL_Main_Memory_Alloc(int iSize);
	// Optional Memory de-allocation function
	static void __stdcall ADL_Main_Memory_Free(void** lpBuffer);

	virtual void MakeAdapterActivity();

// 成员函数指针
	ADL_MAIN_CONTROL_CREATE						ADL_Main_Control_Create;
	ADL_MAIN_CONTROL_DESTROY					ADL_Main_Control_Destroy;
	ADL_ADAPTER_NUMBEROFADAPTERS_GET	ADL_Adapter_NumberOfAdapters_Get;
	ADL_ADAPTER_ADAPTERINFO_GET				ADL_Adapter_AdapterInfo_Get;
	ADL_DISPLAY_COLORCAPS_GET					ADL_Display_ColorCaps_Get;
	ADL_DISPLAY_COLOR_GET							ADL_Display_Color_Get;
	ADL_DISPLAY_COLOR_SET							ADL_Display_Color_Set;
	ADL_DISPLAY_DISPLAYINFO_GET					ADL_Display_DisplayInfo_Get;
	ADL_OVERDRIVE5_TEMPERATURE_GET			ADL_Overdrive5_Temperature_Get;
	ADL_OVERDRIVE5_FANSPEED_GET				ADL_Overdrive5_FanSpeed_Get;
	ADL_OVERDRIVE5_CURRENTACTIVITY_GET	ADL_Overdrive5_CurrentActivity_Get;
};
