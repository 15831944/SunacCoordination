#pragma once


// CRailingDlg �Ի���

class CRailingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRailingDlg)

public:
	CRailingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRailingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RAILING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
