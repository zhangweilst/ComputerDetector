#pragma once

// CRecordDlg �Ի���

class CRecordDlg : public CDialog
{
	DECLARE_DYNAMIC(CRecordDlg)

public:
	CRecordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecordDlg();

// �Ի�������
	enum { IDD = IDD_RECORD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_TimeInterval;		// ��־��¼ʱ����
};
