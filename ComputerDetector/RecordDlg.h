#pragma once

// CRecordDlg 对话框

class CRecordDlg : public CDialog
{
	DECLARE_DYNAMIC(CRecordDlg)

public:
	CRecordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecordDlg();

// 对话框数据
	enum { IDD = IDD_RECORD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_TimeInterval;		// 日志记录时间间隔
};
