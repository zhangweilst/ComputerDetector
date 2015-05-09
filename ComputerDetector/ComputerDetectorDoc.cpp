
// ComputerDetectorDoc.cpp : CComputerDetectorDoc ���ʵ��
//

#include "stdafx.h"
#include "ComputerDetector.h"
#include "ComputerDetectorDoc.h"
#include "MainFrm.h"
#include "OlsApi.h"
#include "RecordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// �̺߳���/ȫ�ֺ���
UINT __cdecl ThreadProcUpdate(LPVOID pParam);
UINT __cdecl UpdateThreadFreq(LPVOID pParam);
UINT __cdecl UpdateBusAndCPUFreq(LPVOID pParam);
UINT __cdecl UpdatePowerInfo(LPVOID pParam);
VOID CALLBACK PerformanceRecord(HWND, UINT, UINT_PTR, DWORD);

// ��̬��������
DWORD CComputerDetectorDoc::m_RecordCount = 0;
CString CComputerDetectorDoc::m_RecordFileName;

// ��ʵ��

IMPLEMENT_DYNCREATE(CComputerDetectorDoc, CDocument)

BEGIN_MESSAGE_MAP(CComputerDetectorDoc, CDocument)
	ON_COMMAND(ID_FILE_RECORD, &CComputerDetectorDoc::OnFileRecord)
	ON_COMMAND(ID_STOP_RECORD, &CComputerDetectorDoc::OnStopRecord)
	ON_UPDATE_COMMAND_UI(ID_FILE_RECORD, &CComputerDetectorDoc::OnUpdateFileRecord)
END_MESSAGE_MAP()


// CComputerDetectorDoc ����/����

CComputerDetectorDoc::CComputerDetectorDoc()
{
	CString str;

	// CPU�ͺ��жϣ� Intel or AMD
	CString strIntel(_T("GenuineIntel"));
	CString strAMD(_T("AuthenticAMD"));
	str.Append(ASCIITranslate(m_CPUID.EAX(0x0).EBX));
	str.Append(ASCIITranslate(m_CPUID.EAX(0x0).EDX));
	str.Append(ASCIITranslate(m_CPUID.EAX(0x0).ECX));

	if (str == strIntel)		// GenuineIntel
	{
		m_CPUBrand = cpuBrdIntel;
	}
	else if (str == strAMD)		// AuthenticAMD	
	{
		m_CPUBrand = cpuBrdAMD;
	}
	else
		m_CPUBrand = cpuBrdUnknown;

	if (m_CPUBrand == cpuBrdIntel)
		m_CPU = new CIntel;
	else if (m_CPUBrand == cpuBrdAMD)
		m_CPU = new CAMD;
	else
		m_CPU = NULL;

	// GPU�ͺ��жϣ� Nvdia or ATI
	// Crossfire or SLI NOT supported by now
	CString strNvdia(_T("NVIDIA"));
	CString strATI(_T("Advanced Micro Devices, Inc."));
	str.Empty();
	str = m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_VideoController"), L"AdapterCompatibility").GetAt(0).bstrVal;

	if (str == strNvdia)
	{
		m_GPUBrand = gpuBrdNVIDIA;
	}
	else if (str == strATI)
	{
		m_GPUBrand = gpuBrdATI;
	}
	else
		m_GPUBrand = gpuBrdUnknown;

	if (m_GPUBrand == gpuBrdNVIDIA)
		m_GraphicsAdapter = new CNvidia;
	else if (m_GPUBrand == gpuBrdATI)
		m_GraphicsAdapter = new CATI;
	else
		m_CPU = NULL;

	// m_PhysicalMemory
	m_PhysicalMemory = new CPhysicalMemory;

	// m_strHostName
	m_strHostName = m_WMI.GetWMIProperty(bstr_t("ROOT\\CIMV2"), bstr_t("Win32_ComputerSystem"), L"Name").GetAt(0).bstrVal;

	// m_BaseBoard;
	m_BaseBoard = new CBaseBoard;

	// ����Update�߳�
	AfxBeginThread(ThreadProcUpdate, this);
}

CComputerDetectorDoc::~CComputerDetectorDoc()
{
}

// CComputerDetectorDoc ���

#ifdef _DEBUG
void CComputerDetectorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CComputerDetectorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CComputerDetectorDoc ���л�

void CComputerDetectorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

// �߳�
UINT __cdecl ThreadProcUpdate(LPVOID pParam)
{
	// ����ʱ��������������ܵ��������߳�
	AfxBeginThread(UpdateThreadFreq, pParam);
	AfxBeginThread(UpdateBusAndCPUFreq, pParam);
	AfxBeginThread(UpdatePowerInfo, pParam);

	CComputerDetectorDoc* pDoc = (CComputerDetectorDoc*) pParam;

	// ֱ�Ӽ�������
	while(1)
	{
		// CPU Related
		int nTotalCores = pDoc->m_CPU->GetNumOfProcessors() * pDoc->m_CPU->GetNumOfCores();
		int nTotalThreads = nTotalCores * pDoc->m_CPU->GetNumOfThreads();
        
		pDoc->m_CPU->GetThreadPercentage();
		pDoc->m_CPU->GetMaxCrtThreadPercentage();
		pDoc->m_CPU->GetCPUPercentage();
		pDoc->m_CPU->GetMaxCPUPercentage();
		pDoc->m_CPU->MakePackageTemperature();

		for ( int i = 0; i < nTotalCores; i++)
			pDoc->m_CPU->MakeCoreTemperature(i * pDoc->m_CPU->GetNumOfThreads());

		// GPU Related
		pDoc->m_GraphicsAdapter->MakeAdapterActivity();

		// Baseboard temperature
		// pDoc->m_BaseBoard->GetBaseBoardTemperture();

		Sleep(500);
	}

	return 1;
}

UINT __cdecl UpdateThreadFreq(LPVOID pParam)
{
	CComputerDetectorDoc* pDoc = (CComputerDetectorDoc*) pParam;
	int nTotalCores = pDoc->m_CPU->GetNumOfProcessors() * pDoc->m_CPU->GetNumOfCores();
	int nTotalThreads = nTotalCores * pDoc->m_CPU->GetNumOfThreads();

	while (1)
		//for ( int i = 0; i < nTotalThreads; i++)
			pDoc->m_CPU->MakeThreadFreq(nTotalThreads);

	return 1;
}

UINT __cdecl UpdateBusAndCPUFreq(LPVOID pParam)
{
	CComputerDetectorDoc* pDoc = (CComputerDetectorDoc*) pParam;

	while (1)
		pDoc->m_CPU->MakeBusAndCPUFreq();

	return 1;
}

UINT __cdecl UpdatePowerInfo(LPVOID pParam)
{
	CComputerDetectorDoc* pDoc = (CComputerDetectorDoc*) pParam;

	while (1)
		pDoc->m_CPU->MakePowerInfo();

	return 1;
}

// TimerProc
VOID CALLBACK PerformanceRecord(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	// Get pointer to the document
	CMainFrame* pMain = static_cast<CMainFrame*>(AfxGetMainWnd());
	CComputerDetectorDoc* pDoc = static_cast<CComputerDetectorDoc*>(pMain->GetActiveDocument());

	CStdioFile recordFile;		// record file
	CString str;					// string to write to file
	str.Empty();

	if (pDoc->m_RecordCount == 0)
	{
		recordFile.Open(pDoc->m_RecordFileName, CFile::modeCreate | CFile::modeWrite);
		str.Append(_T("��ȡʱ��,CPU������(%),CPU ��װ�¶�(��C),CPU ��װ����(W),GPU ��ѹ(V),GPU ����ʱ��(MHz),GPU �Դ�ʱ��(MHz),GPU �����¶�(��C),GPU ռ����(%)\n"));
	}
	else
		recordFile.Open(pDoc->m_RecordFileName, CFile::modeWrite);

	recordFile.SeekToEnd();

	str.Append(_T("UTC+8: "));
	str.Append(pDoc->GetLocalTimeString().strTimeString);
	str.Append(_T(","));

	// CPU Related
	str.AppendFormat(_T("%d,"), pDoc->GetObjCPU()->GetCPUPercentage());
	str.AppendFormat(_T("%d,"), pDoc->GetObjCPU()->GetPackageTemperature());
	str.AppendFormat(_T("%.2lf,"), pDoc->GetObjCPU()->GetPackagePower());

	// GPU Related
	str.AppendFormat(_T("%.3lf,"), pDoc->GetObjGPU()->GetCoreVoltage());
	str.AppendFormat(_T("%d,"), pDoc->GetObjGPU()->GetCoreClock());
	str.AppendFormat(_T("%d,"), pDoc->GetObjGPU()->GetMemoryClock());
	str.AppendFormat(_T("%d,"), pDoc->GetObjGPU()->GetCoreTemperature());
	str.AppendFormat(_T("%d\n"), pDoc->GetObjGPU()->GetGPUPercentage());

	recordFile.WriteString(str);
	recordFile.Close();
	pDoc->m_RecordCount++;
}

// ��Ա����

CCPU* CComputerDetectorDoc::GetObjCPU()
{
	return m_CPU;
}

CGraphicsAdapter* CComputerDetectorDoc::GetObjGPU()
{
	return m_GraphicsAdapter;
}

CBaseBoard* CComputerDetectorDoc::GetObjBaseBoard()
{
	return m_BaseBoard;
}

CPhysicalMemory* CComputerDetectorDoc::GetObjPhysicalMemory()
{
	return m_PhysicalMemory;
}

CString CComputerDetectorDoc::GetHostName()
{
	return m_strHostName;
}

LocalTimeForamt CComputerDetectorDoc::GetLocalTimeString()
{
	LocalTimeForamt timeStr;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	timeStr.strTimeString.Format(_T("%d-%d-%d "), sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	timeStr.strTimeNumerical.Format(_T("%d"), sysTime.wYear);

	// strTimeString
	if (sysTime.wHour < 10)
		timeStr.strTimeString.AppendFormat(_T("0%d:"), sysTime.wHour);
	else
		timeStr.strTimeString.AppendFormat(_T("%d:"), sysTime.wHour);

	if (sysTime.wMinute < 10)
		timeStr.strTimeString.AppendFormat(_T("0%d:"), sysTime.wMinute);
	else
		timeStr.strTimeString.AppendFormat(_T("%d:"), sysTime.wMinute);

	if (sysTime.wSecond < 10)
		timeStr.strTimeString.AppendFormat(_T("0%d"), sysTime.wSecond);
	else
		timeStr.strTimeString.AppendFormat(_T("%d"), sysTime.wSecond);

	// strTimeNumerical
	if (sysTime.wMonth < 10)
		timeStr.strTimeNumerical.AppendFormat(_T("0%d"), sysTime.wMonth);
	else
		timeStr.strTimeNumerical.AppendFormat(_T("%d"), sysTime.wMonth);

	if (sysTime.wDay < 10)
		timeStr.strTimeNumerical.AppendFormat(_T("0%d"), sysTime.wDay);
	else
		timeStr.strTimeNumerical.AppendFormat(_T("%d"), sysTime.wDay);

	if (sysTime.wHour < 10)
		timeStr.strTimeNumerical.AppendFormat(_T("0%d"), sysTime.wHour);
	else
		timeStr.strTimeNumerical.AppendFormat(_T("%d"), sysTime.wHour);

	if (sysTime.wMinute < 10)
		timeStr.strTimeNumerical.AppendFormat(_T("0%d"), sysTime.wMinute);
	else
		timeStr.strTimeNumerical.AppendFormat(_T("%d"), sysTime.wMinute);
	
	if (sysTime.wSecond < 10)
		timeStr.strTimeNumerical.AppendFormat(_T("0%d"), sysTime.wSecond);
	else
		timeStr.strTimeNumerical.AppendFormat(_T("%d"), sysTime.wSecond);

	return timeStr;
}

// CComputerDetectorDoc ����

void CComputerDetectorDoc::OnFileRecord()
{
	// TODO: �ڴ���������������
	CRecordDlg aDlg;

	if (aDlg.DoModal() == IDOK)
	{
		if (m_RecordCount)
			KillTimer(NULL, m_RecordTimer);
		m_TimeInterval = aDlg.m_TimeInterval;
		m_RecordCount = 0;
		m_RecordFileName = _T("PerformanceRecord");
		m_RecordFileName.Append(GetLocalTimeString().strTimeNumerical);
		m_RecordFileName.Append(_T(".csv"));
		m_RecordTimer = SetTimer(NULL, NULL, 1000 * m_TimeInterval, static_cast<TIMERPROC>(PerformanceRecord));
	}
}

void CComputerDetectorDoc::OnUpdateFileRecord(CCmdUI *pCmdUI)
{
	if (m_RecordCount)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CComputerDetectorDoc::OnStopRecord()
{
	if (m_RecordCount)
		KillTimer(NULL, m_RecordTimer);

	m_RecordCount = 0;
}
