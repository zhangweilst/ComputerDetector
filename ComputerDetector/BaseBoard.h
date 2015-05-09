#pragma once
#include "Hardware.h"

class CBaseBoard : public CHardware
{
public:
	CBaseBoard(void);
	~CBaseBoard(void);

// 成员变量
protected:
	int m_BaseBoardSensorCount;
	float* m_CurrentTemperature;

// 成员函数
public:
	int GetSensorCount() { return m_BaseBoardSensorCount; }
	float* GetBaseBoardTemperture();
};
