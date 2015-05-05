// RecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerDetector.h"
#include "RecordDlg.h"


// CRecordDlg 对话框

IMPLEMENT_DYNAMIC(CRecordDlg, CDialog)

CRecordDlg::CRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordDlg::IDD, pParent)
	, m_TimeInterval(0)
{

}

CRecordDlg::~CRecordDlg()
{
}

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIME_INTERVAL, m_TimeInterval);
}


BEGIN_MESSAGE_MAP(CRecordDlg, CDialog)
END_MESSAGE_MAP()


// CRecordDlg 消息处理程序
