#pragma once

#include "WMI.h"
#include "Globals.h"

class CHardware
{
public:
	CHardware(void);
	virtual ~CHardware(void);

// ���ݳ�Ա
protected:
	CString m_strName;
	CWMI m_WMI;				// CWMI ��������������ͳһ���֣�������ͬ

// ������Ա
public:
	CString GetName() { return m_strName; }
};
