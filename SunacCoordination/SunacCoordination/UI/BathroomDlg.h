#pragma once


// CBathroomDlg �Ի���

class CBathroomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBathroomDlg)

public:
	CBathroomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBathroomDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BATHROOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
