#pragma once


// CWaterproofDlg �Ի���

class CWaterproofDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWaterproofDlg)

public:
	CWaterproofDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWaterproofDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WATERPROOF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
