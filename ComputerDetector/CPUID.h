// CPUID ����

#pragma once

#include "stdafx.h"

// General Purpose Register EAX, EBX, ECX, EDX struct definition
typedef struct tagGPRegister
	{
		DWORD EAX;
		DWORD EBX;
		DWORD ECX;
		DWORD EDX;
	}GPRegister;

class CCPUID
{
public:
	CCPUID();

// ��Ա����
public:
	bool CPUIDSupported();
	GPRegister EAX(DWORD eax);
	GPRegister EAXECX(DWORD eax, DWORD ecx);

// ��Ա����
protected:

};
