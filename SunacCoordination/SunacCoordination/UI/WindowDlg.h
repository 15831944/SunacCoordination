#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"

// CWindowDlg �Ի���

class CWindowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWindowDlg)

public:
	CWindowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWindowDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //�ͷŷ�ģ̬�Ի����ڴ�

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlWithPreview m_preWindow;
	CComboBox m_areaType;
	CComboBox m_openType;
	CComboBox m_openAmount;
	CComboBox m_openWidth;
	CEdit m_width;
	CEdit m_height;
	afx_msg void OnBnClickedMfcbuttonInsert();
	afx_msg void OnBnClickedMfcbuttonSearch();
};
