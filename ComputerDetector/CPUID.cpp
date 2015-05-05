// CPUID 实现
#include "stdafx.h"
#include "CPUID.h"

CCPUID::CCPUID()
{
	if (!CPUIDSupported())
	{
		AfxMessageBox("CPUID不被支持，程序即将退出");	
		AfxAbort();
	}
}

bool CCPUID::CPUIDSupported()
{
	// Check cpuid support by bit 21 of EFLAGS
	DWORD bit21Set = 0x200000;
	DWORD bit21Clear = 0;
	DWORD bit21AfterSet;
	DWORD bit21AfterClear;

	__asm
	{
		push bit21Set
		popfd
		pushfd
		pop bit21AfterSet

		push bit21Clear
		popfd
		pushfd
		pop bit21AfterClear
	}

	if (((bit21AfterSet & bit21Set) == bit21Set) && ((bit21AfterClear & bit21Set) == bit21Clear))
		return true;
	else
		return false;
}

// CPUID input entered in EAX
GPRegister CCPUID::EAX(DWORD eax)
{
	GPRegister gpr;
	DWORD inputEAX = eax;

	DWORD outputEAX;
	DWORD outputEBX;
	DWORD outputECX;
	DWORD outputEDX;

	__asm
	{
		mov eax, inputEAX
		cpuid
		mov outputEAX, eax
		mov outputEBX, ebx
		mov outputECX, ecx
		mov outputEDX, edx
	}

	gpr.EAX = outputEAX;
	gpr.EBX = outputEBX;
	gpr.ECX = outputECX;
	gpr.EDX = outputEDX;

	return gpr;
}

// CPUID input entered in EAX and ECX
GPRegister CCPUID::EAXECX(DWORD eax, DWORD ecx)
{
	GPRegister gpr;
	DWORD inputEAX = eax;
	DWORD inputECX = ecx;

	DWORD outputEAX;
	DWORD outputEBX;
	DWORD outputECX;
	DWORD outputEDX;

	__asm
	{
		mov eax, inputEAX
		mov ecx, inputECX
		cpuid
		mov outputEAX, eax
		mov outputEBX, ebx
		mov outputECX, ecx
		mov outputEDX, edx
	}

	gpr.EAX = outputEAX;
	gpr.EBX = outputEBX;
	gpr.ECX = outputECX;
	gpr.EDX = outputEDX;

	return gpr;
}
