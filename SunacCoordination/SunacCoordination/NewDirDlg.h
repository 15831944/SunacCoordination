#pragma once


// CNewDirDlg �Ի���

class CNewDirDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CNewDirDlg)

public:
	CNewDirDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewDirDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NewDir };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_sNewDir;
	afx_msg void OnBnClickedOk();
	CEdit m_NewDirEdit;
};
