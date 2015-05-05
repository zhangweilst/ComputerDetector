// CPUID 定义

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

// 成员函数
public:
	bool CPUIDSupported();
	GPRegister EAX(DWORD eax);
	GPRegister EAXECX(DWORD eax, DWORD ecx);

// 成员变量
protected:

};
