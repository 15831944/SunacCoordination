#pragma once


// CMoldingsDlg �Ի���

class CMoldingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMoldingsDlg)

public:
	CMoldingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMoldingsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MOLDINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
