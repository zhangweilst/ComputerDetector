#pragma once

#include "stdafx.h"

// defines

// typedefs

typedef enum tagCPUBrand
{
	cpuBrdUnknown = 0,
	cpuBrdIntel = 1,
	cpuBrdAMD = 2
}CPUBrand;

typedef enum tagGPUBrand
{
	gpuBrdUnknown = 0,
	gpuBrdNVIDIA = 1,
	gpuBrdATI = 2
}GPUBrand;

// Global functions
CString ASCIITranslate(DWORD hex);
