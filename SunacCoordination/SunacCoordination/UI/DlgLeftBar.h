#pragma once

#include "../res/resource.h"
#include "afxcmn.h"
// DlgLeftBar dialog

class DlgLeftBar : public CAcUiDialog
{
	DECLARE_DYNAMIC(DlgLeftBar)

public:
	DlgLeftBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgLeftBar();

// Dialog Data
	enum { IDD = IDD_DIALOG_LEFTBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	int InitTreeCtrl();
	int FillTreeItem();
	CTreeCtrl m_treeCtrlMenu;


	HTREEITEM m_treeItemWindows; //�Ŵ�
	HTREEITEM m_treeItemWindowsQuantity; //�Ŵ�����Quantitysurvey
	HTREEITEM m_treeItemDoor; //�Ŵ�
	HTREEITEM m_treeItemDoorQuantity; //������
	HTREEITEM m_treeItemWindowTable; //�Ŵ���

	HTREEITEM m_treeItemKitchen; //����
	HTREEITEM m_treeItemKitchenQuantity; //����
	HTREEITEM m_treeItemBathroom; //����
	HTREEITEM m_treeItemBathroomQuantity; //����

	HTREEITEM m_treeItemRailing;//����
	HTREEITEM m_treeItemAircondition; //�յ�
	HTREEITEM m_treeItemWaterproof; //��ˮ
	HTREEITEM m_treeItemFilling; //���
	HTREEITEM m_treeItemfacade; //����


public:
	afx_msg void OnTvnSelchangedTreeMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTreeMenu(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
