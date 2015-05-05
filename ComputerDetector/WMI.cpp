// WMI implemention

#include "stdafx.h"
#include "WMI.h"

int CWMI::m_wmiCount;
IWbemLocator* CWMI::m_pLoc = NULL;
IWbemServices* CWMI::m_pSvc = NULL;

CWMI::CWMI()
{
	m_pEnumerator = NULL;
	m_pClsObj = NULL;

	if (++m_wmiCount == 1)		// Initialize once please
		WMIInitialize();
}

int CWMI::WMIInitialize()
{
	HRESULT hres;

	hres = CoInitialize(0);
	if( FAILED(hres))
	{
		AfxMessageBox("Failed to initialize COM library.");
		return 1;
	}

	hres = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,
		NULL
		);

	if( FAILED(hres))
	{
		AfxMessageBox("Failed to initialize security.");
		CoUninitialize();
		return 1;
	}

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		(LPVOID*) &m_pLoc
		);

	if( FAILED(hres))
	{
		AfxMessageBox("Failed to create IWbemLocator object.");
		CoUninitialize();
		return 1;
	}

	hres = m_pLoc->ConnectServer(        
        _bstr_t(L"ROOT\\CIMV2"),	// WMI namespace
        NULL,
        NULL,
        0,
        NULL,              
        0,   
        0,
        &m_pSvc
        );                              
    
	if( FAILED(hres))
	{
		AfxMessageBox("Could not connect.");
		m_pLoc->Release();
		CoUninitialize();
		return 1;
	}

	hres = CoSetProxyBlanket(
       m_pSvc,
       RPC_C_AUTHN_WINNT,
       RPC_C_AUTHZ_NONE,
       NULL,
       RPC_C_AUTHN_LEVEL_CALL,
       RPC_C_IMP_LEVEL_IMPERSONATE,
       NULL,
       EOAC_NONE 
    );

	if( FAILED(hres))
	{
		AfxMessageBox("Could not set proxy blanket.");
		m_pSvc->Release();
		m_pLoc->Release();
		CoUninitialize();
		return 1;
	}

	return 0;
}

CArray<VARIANT, VARIANT>& CWMI::GetWMIProperty(BSTR className, LPCWSTR prop)
{
	m_PropArray.RemoveAll();

	CString strClsName;
	strClsName.Empty();
	strClsName = className;

	CString str(_T("SELECT * FROM "));
	str += strClsName;
	BSTR bstrQuery = str.AllocSysString();

	VARIANT vtProp;

	HRESULT hres;

	hres = m_pSvc->ExecQuery(
		bstr_t("WQL"),
		bstrQuery,
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&m_pEnumerator
		);

	if( FAILED(hres))
	{
		AfxMessageBox("Query failed.");
	}
	else
	{
		ULONG uReturn = 0;
		while( m_pEnumerator)
		{
			hres = m_pEnumerator->Next(WBEM_INFINITE, 1, &m_pClsObj, &uReturn);
			if ( uReturn == 0)
				break;

			hres = m_pClsObj->Get(prop, 0, &vtProp, 0, 0);

			m_PropArray.Add(vtProp);
		}
	}

	return m_PropArray;
}

CWMI::~CWMI()
{
	// Cleanup
	if (--m_wmiCount == 0)
	{
		m_pSvc->Release();
		m_pLoc->Release();
	}
	::CoUninitialize();
}
