#pragma once


// CKitchenDlg �Ի���

class CKitchenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKitchenDlg)

public:
	CKitchenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKitchenDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_KITCHEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
