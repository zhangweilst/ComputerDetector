#pragma once

#include "WMI.h"
#include "Globals.h"

class CHardware
{
public:
	CHardware(void);
	virtual ~CHardware(void);

// 数据成员
protected:
	CString m_strName;
	CWMI m_WMI;				// CWMI 对象在所有类中统一名字，功能相同

// 函数成员
public:
	CString GetName() { return m_strName; }
};
