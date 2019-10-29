#pragma once
#include "afxwin.h"
#include "ComFun_MFC.h"
#include "GridCtrlWithPreview.h"
#include "../Object/Railing/AttrRailing.h"

// CRailingDlg 对话框

class CRailingDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CRailingDlg)

public:
	CRailingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRailingDlg();

	virtual INT_PTR DoModal();

// 对话框数据
	enum { IDD = IDD_DIALOG_RAILING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	//非模态对话框相关函数
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //释放非模态对话框内存
	bool m_isMoldless;//是否非模态对话框

public:
	afx_msg void OnBnClickedInsertToCAD();
	afx_msg void OnBnClickedButtonSelectline();
	afx_msg void OnCbnSelchangeComboRailingtype();
	afx_msg void OnBnClickedCheckAutoindex();
	afx_msg void OnSelChangedPreview(NMHDR *pNMHDR, LRESULT *pResult);
	void UpdateRailingToGrid(eRailingType p_railetype);
	CString RailingSize(int i);

	CGridCtrlWithPreview m_preRailing;
	CString m_selectedFile;
	CComboBox m_type;
	double m_height;// 栏杆高度	
	double m_width;// 栏杆长度
	CEdit m_railingNumber;
};

extern CRailingDlg* g_railingDlg;
BOOL CloseRailingDlg();