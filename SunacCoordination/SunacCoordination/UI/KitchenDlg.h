#pragma once
#include "gridctrlwithpreview.h"
#include "afxwin.h"


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

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlWithPreview m_preKitchen;
	CComboBox m_floorRange;
	CComboBox m_basinType;
	CComboBox m_benchWidth;
	CComboBox m_frigeType;
	CComboBox m_projectLevel;
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonKitchenrange();
};
