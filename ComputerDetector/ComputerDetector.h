
// ComputerDetector.h : ComputerDetector Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CComputerDetectorApp:
// �йش����ʵ�֣������ ComputerDetector.cpp
//

class CComputerDetectorApp : public CWinAppEx
{
public:
	CComputerDetectorApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CComputerDetectorApp theApp;
