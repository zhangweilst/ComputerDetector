// WMI definitions

#pragma once

#include "stdafx.h"
#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")

typedef struct tagWMIInfo
{
	CArray<VARIANT, VARIANT&> PropArray;
	int nCount;
}WMIInfo;

class CWMI
{
public:
	CWMI();
	~CWMI();

// ��Ա����
protected:
	static IWbemLocator* m_pLoc;
	static IWbemServices* m_pSvc;
	static int m_wmiCount;
	IEnumWbemClassObject* m_pEnumerator;
	IWbemClassObject* m_pClsObj;
	CArray<VARIANT, VARIANT> m_PropArray;

// ��Ա����
protected:
	int WMIInitialize();
public:
	CArray<VARIANT, VARIANT>& GetWMIProperty(BSTR className, LPCWSTR prop);
};
