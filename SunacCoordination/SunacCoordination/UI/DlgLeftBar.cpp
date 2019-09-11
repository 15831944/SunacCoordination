// DlgLeftBar.cpp : implementation file
//

#include "stdafx.h"
#include "../Command/Command.h"
#include "../Command/CommandWindowDetail.h"
#include "../Command/CommandWindowTable.h"
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
	ON_WM_SIZE()
	ON_MESSAGE(WM_MENU_CLICKED, OnClickedPopupMenu)
END_MESSAGE_MAP()


// DlgLeftBar message handlers


int DlgLeftBar::FillTreeItem()
{
	/*HTREEITEM  hItem= m_treeCtrlMenu.InsertItem(_T("�Ŵ�"));

	m_treeItemWindows = m_treeCtrlMenu.InsertItem(_T("�ⴰ"), hItem);
	m_treeItemWindowsQuantity = m_treeCtrlMenu.InsertItem(_T("�ⴰ����"), hItem);

	m_treeItemDoor = m_treeCtrlMenu.InsertItem(_T("��"), hItem);
	m_treeItemDoorQuantity = m_treeCtrlMenu.InsertItem(_T("������"), hItem);
	m_treeItemWindowTable = m_treeCtrlMenu.InsertItem(_T("�Ŵ���"), hItem);
	m_treeItemWindowDetail = m_treeCtrlMenu.InsertItem(_T("�Ŵ���ͼ"), hItem);


	//////////////////////////////////////////////////////////////////////////
	hItem = m_treeCtrlMenu.InsertItem(_T("����"));

	m_treeItemKitchen = m_treeCtrlMenu.InsertItem(_T("����"), hItem);
	m_treeItemKitchenQuantity = m_treeCtrlMenu.InsertItem(_T("��������"), hItem);
	m_treeItemBathroom = m_treeCtrlMenu.InsertItem(_T("������"), hItem);
	m_treeItemBathroomQuantity = m_treeCtrlMenu.InsertItem(_T("����������"), hItem);

	//////////////////////////////////////////////////////////////////////////
	m_treeItemRailing = m_treeCtrlMenu.InsertItem(_T("����"));

	hItem = m_treeCtrlMenu.InsertItem(_T("�յ�"));

	hItem = m_treeCtrlMenu.InsertItem(_T("��ˮ"));

	hItem = m_treeCtrlMenu.InsertItem(_T("���"));

	hItem = m_treeCtrlMenu.InsertItem(_T("����"));*/

	m_treeCtrlMenu.SetItemHeight(30);
	m_treeCtrlMenu.InsertItem(_T("��׼��ƽ���"));
	m_treeCtrlMenu.InsertItem(_T("��׼�������"));

	HTREEITEM hItem = m_treeCtrlMenu.InsertItem(_T("��׼��Ʒ��"));
	m_treeCtrlMenu.InsertItem(_T("�Ŵ�"), hItem);
	m_treeCtrlMenu.InsertItem(_T("����"), hItem);
	m_treeCtrlMenu.InsertItem(_T("��ǽͿ��"), hItem);
	m_treeCtrlMenu.InsertItem(_T("������"), hItem);

	hItem = m_treeCtrlMenu.InsertItem(_T("��׼�ռ�ģ���"));
	m_treeCtrlMenu.InsertItem(_T("����"), hItem);
	m_treeCtrlMenu.InsertItem(_T("�յ�"), hItem);
	m_treeCtrlMenu.InsertItem(_T("����"), hItem);
	m_treeCtrlMenu.InsertItem(_T("����"), hItem);

	m_treeCtrlMenu.InsertItem(_T("��׼����"));
	m_treeCtrlMenu.InsertItem(_T("��׼��ͼ"));

	//�󶨵����˵�
	m_treeCtrlMenu.SetMenuID(IDR_LEFTBAR_MENU);
	m_treeCtrlMenu.SetMenuWidth(150);

	return 0;
}


/*

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
	else if (m_treeItemWindowDetail == curHItem)
	{
		CMD_SUNACWINDOWDetail();
	}


	*pResult = 0;
}
*/

LRESULT DlgLeftBar::OnClickedPopupMenu(WPARAM mID, LPARAM notUsed)
{
	switch (mID)
	{
	case ID_COMMAND_WINDOW:
		CMD_SUNACWINDOW();
		break;
	case ID_COMMAND_WINDOWQUANTITY:
		CMD_SUNACSTATISTICS();
		break;
	case ID_COMMAND_WINDOWTABLE:
		CMD_SUNACWINDOWTable();
		break;
	case ID_COMMAND_WINDOWDETAIL:
		CMD_SUNACWINDOWDetail();
		break;
	case ID_COMMAND_KITCHEN:
		CMD_SUNACKITCHEN();
		break;
	case ID_COMMAND_KITCHENQUANTITY:
		CMD_SUNACSTATISTICS();
		break;
	case ID_COMMAND_RAILING:
		CMD_SUNACRAILING();
		break;
	default:
		break;
	}

	return 0;
}

void DlgLeftBar::OnSize(UINT nType, int cx, int cy)
{
	CAcUiDialog::OnSize(nType, cx, cy);
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);
	rect.DeflateRect(5, 5);
	m_treeCtrlMenu.MoveWindow(rect);
}

BOOL DlgLeftBar::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_treeCtrlMenu.SetItemHeight(40);

	FillTreeItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
