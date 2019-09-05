// DlgLeftBar.cpp : implementation file
//

#include "stdafx.h"
#include "..\main.h"
#include "DlgLeftBar.h"
#include "afxdialogex.h"


// DlgLeftBar dialog

IMPLEMENT_DYNAMIC(DlgLeftBar, CDialog)

DlgLeftBar::DlgLeftBar(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(DlgLeftBar::IDD, pParent)
{

}

DlgLeftBar::~DlgLeftBar()
{
}

void DlgLeftBar::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_MENU, m_treeCtrlMenu);
}


BEGIN_MESSAGE_MAP(DlgLeftBar, CAcUiDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MENU, &DlgLeftBar::OnTvnSelchangedTreeMenu)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_MENU, &DlgLeftBar::OnNMRClickTreeMenu)
END_MESSAGE_MAP()


// DlgLeftBar message handlers


int DlgLeftBar::InitTreeCtrl()
{
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
	m_treeCtrlMenu.ModifyStyle(1, dwViewStyle);


	//����ͼ��
	//UINT uiBmpId = IDB_BITMAP3;
	//CBitmap bmp;
	//if (!bmp.LoadBitmap(uiBmpId))
	//{
	//	TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
	//	ASSERT(FALSE);
	//	return false;
	//}

	//BITMAP bmpObj;
	//bmp.GetBitmap(&bmpObj);

	//m_treeImages.Create(16, bmpObj.bmHeight, ILC_MASK | ILC_COLOR24, 0, 0);
	//m_treeImages.Add(&bmp, RGB(255, 0, 255));
	//m_treeCtrlMenu.SetImageList(&m_treeImages, TVSIL_NORMAL);

	return true;
}

int DlgLeftBar::FillTreeItem()
{
	HTREEITEM  hItem= m_treeCtrlMenu.InsertItem(_T("�Ŵ�"), 12, 12);
	//m_treeCtrlMenu.SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);

	m_treeItemWindows = m_treeCtrlMenu.InsertItem(_T("�ⴰ"), 12, 12, hItem);
	m_treeItemWindowsQuantity = m_treeCtrlMenu.InsertItem(_T("�ⴰ����"), 12, 12, hItem);

	m_treeItemDoor = m_treeCtrlMenu.InsertItem(_T("��"), 12, 12, hItem);
	m_treeItemDoorQuantity = m_treeCtrlMenu.InsertItem(_T("������"), 12, 12, hItem);
	m_treeItemWindowTable = m_treeCtrlMenu.InsertItem(_T("�Ŵ���"), 12, 12, hItem);


	//////////////////////////////////////////////////////////////////////////
	hItem = m_treeCtrlMenu.InsertItem(_T("����"), 12, 12);
	//m_treeCtrlMenu.SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);

	m_treeItemKitchen = m_treeCtrlMenu.InsertItem(_T("����"), 12, 12, hItem);
	m_treeItemKitchenQuantity = m_treeCtrlMenu.InsertItem(_T("��������"), 12, 12, hItem);
	m_treeItemBathroom = m_treeCtrlMenu.InsertItem(_T("������"), 12, 12, hItem);
	m_treeItemBathroomQuantity = m_treeCtrlMenu.InsertItem(_T("����������"), 12, 12, hItem);

	//////////////////////////////////////////////////////////////////////////
	m_treeItemRailing = m_treeCtrlMenu.InsertItem(_T("����"), 12, 12);
	//m_treeCtrlMenu.SetItemState(m_treeItemRailing, TVIS_BOLD, TVIS_BOLD);

	hItem = m_treeCtrlMenu.InsertItem(_T("�յ�"), 12, 12);
	//m_treeCtrlMenu.SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);

	hItem = m_treeCtrlMenu.InsertItem(_T("��ˮ"), 12, 12);
	//m_treeCtrlMenu.SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);

	hItem = m_treeCtrlMenu.InsertItem(_T("���"), 12, 12);
	//m_treeCtrlMenu.SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);

	hItem = m_treeCtrlMenu.InsertItem(_T("����"), 12, 12);
	//m_treeCtrlMenu.SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);



	return 0;
}



void DlgLeftBar::OnTvnSelchangedTreeMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	HTREEITEM curHItem = pNMTreeView->itemNew.hItem;

	if (m_treeItemWindows==curHItem)
	{
		CMD_SUNACWINDOW();
	}
	else if (m_treeItemWindowsQuantity==curHItem)
	{
		CMD_SUNACSTATISTICS();
	}
	else if (m_treeItemKitchen == curHItem)
	{
		CMD_SUNACKITCHEN();
	}
	else if (m_treeItemKitchenQuantity == curHItem)
	{
		CMD_SUNACSTATISTICS();
	}
	else if (m_treeItemRailing == curHItem)
	{
		CMD_SUNACRAILING();
	}
	else if (m_treeItemWindowTable == curHItem)
	{
		CMD_SUNACWINDOWTable();
	}


	*pResult = 0;
}


void DlgLeftBar::OnNMRClickTreeMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


BOOL DlgLeftBar::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	InitTreeCtrl();
	FillTreeItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
