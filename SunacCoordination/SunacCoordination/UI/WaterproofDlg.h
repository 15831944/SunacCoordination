#pragma once


// CWaterproofDlg �Ի���

class CWaterproofDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CWaterproofDlg)

public:
	CWaterproofDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWaterproofDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WATERPROOF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
