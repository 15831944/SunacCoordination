#pragma once


// CDoorDlg �Ի���

class CDoorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDoorDlg)

public:
	CDoorDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDoorDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DOOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
