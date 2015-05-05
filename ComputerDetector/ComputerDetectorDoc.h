
// ComputerDetectorDoc.h : CComputerDetectorDoc ��Ľӿ�
//

#pragma once
#include "CPUID.h"
#include "WMI.h"
#include "CPU.h"
#include "BaseBoard.h"
#include "PhysicalMemory.h"
#include "Intel.h"
#include "AMD.h"
#include "Nvidia.h"
#include "ATI.h"

typedef struct tagLocalTimeFormat
{
	CString strTimeString;
	CString strTimeNumerical;
} LocalTimeForamt;

class CComputerDetectorDoc : public CDocument
{
protected: // �������л�����
	CComputerDetectorDoc();
	DECLARE_DYNCREATE(CComputerDetectorDoc)

// ��д
public:
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CComputerDetectorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ��Ա����
protected:
	CCPUID m_CPUID;					// CPUID ��������ִ��CPUIDָ��
	CWMI m_WMI;
	CPUBrand m_CPUBrand;
	GPUBrand m_GPUBrand;

	DWORD m_TimeInterval;		// ��־��¼ʱ����
	UINT_PTR	m_RecordTimer;

public:
	CCPU* m_CPU;
	CGraphicsAdapter* m_GraphicsAdapter;
	CBaseBoard* m_BaseBoard;
	CPhysicalMemory* m_PhysicalMemory;
	CString m_strHostName;					// Name of computer
	static DWORD m_RecordCount;			// Won't wrap-around in 100 years
	static CString m_RecordFileName;		// Performance record file name

// ��Ա����
public:
	CCPU* GetObjCPU();
	CGraphicsAdapter* GetObjGPU();
	CBaseBoard* GetObjBaseBoard();
	CPhysicalMemory* GetObjPhysicalMemory();
	CString GetHostName();
	LocalTimeForamt GetLocalTimeString();

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileRecord();
	afx_msg void OnStopRecord();
	afx_msg void OnUpdateFileRecord(CCmdUI *pCmdUI);
};
