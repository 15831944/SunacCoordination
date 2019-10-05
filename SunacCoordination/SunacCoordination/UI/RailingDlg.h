#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"

// CRailingDlg �Ի���

class CRailingDlg : public CAcUiDialog
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
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedInsertToCAD();
	afx_msg void OnBnClickedButtonSelectline();


	CGridCtrlWithPreview m_preRailing;
	CString m_selectedFile;
	CComboBox m_type;
	
	double m_height;// ���˸߶�	
	double m_heightBase;// �����߶�	
	double m_width;// ���˳���
};

extern CRailingDlg* g_railingDlg;
BOOL CloseRailingDlg();