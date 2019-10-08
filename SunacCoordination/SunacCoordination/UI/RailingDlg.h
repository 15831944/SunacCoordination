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

	virtual INT_PTR DoModal();

// �Ի�������
	enum { IDD = IDD_DIALOG_RAILING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	//��ģ̬�Ի�����غ���
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //�ͷŷ�ģ̬�Ի����ڴ�
	bool m_isMoldless;//�Ƿ��ģ̬�Ի���
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