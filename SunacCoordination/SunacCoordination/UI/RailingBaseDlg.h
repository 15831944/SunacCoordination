#pragma once
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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int InitTree();
	int FillTreeItem();
	int InitLib();

	DECLARE_MESSAGE_MAP()

private:

	CGridCtrlWithPreview m_libPreview;
	CTreeCtrl m_category;
	CImageList m_treeImages;
	HTREEITEM m_hroot;
	HTREEITEM m_hlibTieYi;
	HTREEITEM m_hlibBoLi;

	
};
