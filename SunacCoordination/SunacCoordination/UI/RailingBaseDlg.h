#pragma once
/*
#include "gridctrlwithpreview.h"
#include "afxcmn.h"


// CRailingBaseDlg �Ի���

class CRailingBaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRailingBaseDlg)

public:
	CRailingBaseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRailingBaseDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RAILINGBASE };

	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01

	afx_msg void OnBnClickedMfcbuttonOK();
	CString GetSelectedFile(){return m_selectedFile;}
	void SetParent(CDialog * parent){m_parent = parent;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	int InitTree();
	int FillTreeItem();
	int InitLib();
	void RefreshPreviewFiles();
	DECLARE_MESSAGE_MAP()
	
private:

	CGridCtrlWithPreview m_libPreview;
	CTreeCtrl m_category;
	CImageList m_treeImages;
	HTREEITEM m_hroot;
	HTREEITEM m_hlibTieYi;
	HTREEITEM m_hlibBoLi;

	HTREEITEM m_selItem;
	
	CString m_selectedFile;
	CDialog * m_parent;

	vCString m_allFilePathNames;
	
public:
	afx_msg void OnNMClickTreeRailingbase(NMHDR *pNMHDR, LRESULT *pResult);
};
*/
