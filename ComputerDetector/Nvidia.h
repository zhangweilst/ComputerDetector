#pragma once
#include "GraphicsAdapter.h"
#include "NVAPI\nvapi.h"
#pragma comment(lib, "nvapi.lib")

class CNvidia : public CGraphicsAdapter
{
public:
	CNvidia(void);
	~CNvidia(void);

// ��Ա����
protected:
	NvPhysicalGpuHandle m_nvGPUHandle;
	NvU32 m_gpuCount;
	NvU32 m_noDisplay;

	NV_GPU_THERMAL_SETTINGS m_settings;
	NV_GPU_CLOCK_FREQUENCIES m_clkFreqs;

// ��Ա����
public:
	virtual void MakeAdapterActivity();
};
