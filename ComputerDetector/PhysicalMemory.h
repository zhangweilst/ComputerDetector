#pragma once
#include "Hardware.h"

class CPhysicalMemory : public CHardware
{
public:
	CPhysicalMemory(void);
	~CPhysicalMemory(void);

// 成员变量
protected:
	CArray<VARIANT, VARIANT> m_MemoryLocator;
	CArray<VARIANT, VARIANT> m_MemoryCapacity;
	CArray<VARIANT, VARIANT> m_MemorySerialNumber;
	CArray<VARIANT, VARIANT> m_MemorySpeed;

	double m_TotalMemoryCapacity;
	int m_NumOfDimms;

// 成员函数
public:
	CArray<VARIANT, VARIANT>& GetMemoryLocator() { return m_MemoryLocator; }
	CArray<VARIANT, VARIANT>& GetMemoryCapacity() { return m_MemoryCapacity; }
	CArray<VARIANT, VARIANT>& GetMemorySerialNumber() { return m_MemorySerialNumber; }
	CArray<VARIANT, VARIANT>& GetMemorySpeed() { return m_MemorySpeed; }
	double GetTotalMemoryCapacity() { return m_TotalMemoryCapacity; }
	int GetNumOfDimms() { return m_NumOfDimms; }
};
