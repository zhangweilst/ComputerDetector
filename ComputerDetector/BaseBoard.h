#pragma once
#include "Hardware.h"

class CBaseBoard : public CHardware
{
public:
	CBaseBoard(void);
	~CBaseBoard(void);

// ��Ա����
protected:
	int m_BaseBoardSensorCount;
	float* m_CurrentTemperature;

// ��Ա����
public:
	int GetSensorCount() { return m_BaseBoardSensorCount; }
	float* GetBaseBoardTemperture();
};
