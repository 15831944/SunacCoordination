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
	//afx_msg void OnBnClickedMfcbuttonLib();
	afx_msg void OnBnClickedMfcbuttonSelectline();
	//afx_msg void OnBnClickedMfcbuttonClose();
	//void UpdateSelectFile(CString selectFile);

	//CGsPreviewCtrl m_preStyle; //����ѡ��Ԥ��ͼ
	//CEdit m_railingInfo;
	CGridCtrlWithPreview m_preRailing;
	CEdit m_width; //���ڿ��
	CEdit m_height; //�����ܸ�
	CEdit m_reverse; //�����߶�
	CString m_selectedFile;
};
