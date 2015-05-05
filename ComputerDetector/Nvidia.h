#pragma once
#include "GraphicsAdapter.h"
#include "NVAPI\nvapi.h"
#pragma comment(lib, "nvapi.lib")

class CNvidia : public CGraphicsAdapter
{
public:
	CNvidia(void);
	~CNvidia(void);

// 成员变量
protected:
	NvPhysicalGpuHandle m_nvGPUHandle;
	NvU32 m_gpuCount;
	NvU32 m_noDisplay;

	NV_GPU_THERMAL_SETTINGS m_settings;
	NV_GPU_CLOCK_FREQUENCIES m_clkFreqs;

// 成员函数
public:
	virtual void MakeAdapterActivity();
};
