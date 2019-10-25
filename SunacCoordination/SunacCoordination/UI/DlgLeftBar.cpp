// DlgLeftBar.cpp : implementation file
//

#include "stdafx.h"
#include "../Command/Command.h"
#include "../Command/CommandWindowDetail.h"
#include "../Command/CommandWindowTable.h"
#include "DlgLeftBar.h"
#include "res/resource.h"
#include "..\ProjectorFileMrg\ProjectFileMrg.h"
#include "ProjectManagementDlg.h"


// DlgLeftBar dialog

IMPLEMENT_DYNAMIC(DlgLeftBar, CAcUiDialog)

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
	DDX_Control(pDX, IDC_TAB_LEFTBAR, m_tab);
	DDX_Control(pDX, IDC_TREE_ProjectManagement, m_treeProjectManagement);
}


BEGIN_MESSAGE_MAP(DlgLeftBar, CAcUiDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_MENU_CLICKED, OnClickedPopupMenu)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LEFTBAR, OnTabSelChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_ProjectManagement, &DlgLeftBar::OnNMDblclkTreeProjectmanagement)
END_MESSAGE_MAP()


// DlgLeftBar message handlers


int DlgLeftBar::FillTreeItem()
{
	HTREEITEM hItem = m_treeCtrlMenu.InsertItem(_T("��׼��Ʒ��"));
	m_treeCtrlMenu.InsertItem(_T("�Ŵ�"), hItem);
	m_treeCtrlMenu.InsertItem(_T("����"), hItem);

	hItem = m_treeCtrlMenu.InsertItem(_T("��׼�ռ�ģ���"));
	m_treeCtrlMenu.InsertItem(_T("����"), hItem);
	m_treeCtrlMenu.InsertItem(_T("�յ�"), hItem);

	//�󶨵����˵�
	m_treeCtrlMenu.SetMenuID(IDR_LEFTBAR_MENU);
	m_treeCtrlMenu.SetMenuWidth(150);

	return 0;
}

int DlgLeftBar::FillProjectManagementTreeItem()
{
	HICON Icon[3];
	CImageList *ImageList = new CImageList();
	ImageList->Create(16,16,ILC_COLOR24,12,12);
	Icon[0] = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	Icon[1] = AfxGetApp()->LoadIcon(IDI_ICON_DOCUMENT);

	for (int i = 0; i < 2; i++)
	{
		ImageList->Add(Icon[i]);
	}
	m_treeProjectManagement.SetImageList(ImageList, TVSIL_NORMAL);
	
	CProjectFileMrg::Instance()->InitTestData();
	vector<CString> AreaNames = CProjectFileMrg::Instance()->GetAllAreas();
	for (UINT i = 0; i < AreaNames.size(); i++)
	{
		vector<CProjectData *> vProjectsDatabByArea = CProjectFileMrg::Instance()->GetProjectsByArea(AreaNames[i]);
		HTREEITEM hFolder = m_treeProjectManagement.InsertItem(AreaNames[i], 0, 0);
		for (UINT j = 0; j < vProjectsDatabByArea.size(); j++)
		{
			m_treeProjectManagement.InsertItem(vProjectsDatabByArea[j]->m_prjInfo.m_sName, 1, 1, hFolder);
		}
	}
	return 0;
}

LRESULT DlgLeftBar::OnClickedPopupMenu(WPARAM mID, LPARAM notUsed)
{
	switch (mID)
	{
	case ID_COMMAND_WINDOW:
		CMD_SUNACWINDOW();
		break;

	case ID_COMMAND_WINDOWADVANCE: //��������
		break;

	case ID_COMMAND_WINDOWQUANTITY://�Ŵ�����
		CMD_SUNACSTATISTICS();
		break;
	case ID_COMMAND_WINDOWTABLE:
		CMD_SUNACWINDOWTable();
		break;
	case ID_COMMAND_WINDOWDETAIL:	//�Ŵ���ͼ
		CMD_SUNACWINDOWDetail();
		break;

	case ID_COMMAND_BATHROOM:
		CMD_SUNACBATHROOM();
		break;
	case ID_COMMAND_KITCHEN:
		CMD_SUNACKITCHEN();
		break;
	case ID_COMMAND_KITCHENQUANTITY:
		break;
	case ID_COMMAND_RAILING:
		CMD_SUNACRAILING();
		break;
	case ID_COMMAND_AIRCONDITIONER:
		CMD_SUNACAIRCONDITIONER();
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
	m_tab.SetItemSize(CSize(rect.Width()/2 - 2, 30));
	m_tab.MoveWindow(rect.left, rect.top, rect.Width(), 30);
	rect.top += 30;
	m_treeCtrlMenu.MoveWindow(rect);
	m_treeProjectManagement.MoveWindow(rect);
}

void DlgLeftBar::OnTabSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	if (m_tab.GetCurSel() == 0)
	{
		m_treeCtrlMenu.ShowWindow(SW_SHOW);
		m_treeProjectManagement.ShowWindow(SW_HIDE);
	}
	else
	{
		m_treeCtrlMenu.ShowWindow(SW_HIDE);
		m_treeProjectManagement.ShowWindow(SW_SHOW);
	}
}

BOOL DlgLeftBar::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	m_tab.InsertItem(0, _T("��׼��"));
	m_tab.InsertItem(1, _T("��Ŀ����"));
	m_tab.SetCurSel(0);
	m_treeCtrlMenu.SetItemHeight(30);
	m_treeProjectManagement.SetItemHeight(20);
	FillTreeItem();
	FillProjectManagementTreeItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void DlgLeftBar::OnNMDblclkTreeProjectmanagement(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CPoint CurClkPoint;
	GetCursorPos(&CurClkPoint);
	m_treeProjectManagement.ScreenToClient(&CurClkPoint);
	HTREEITEM CurClkItem;
	/*UINT flag;
	CurClkItem = m_treeProjectManagement.HitTest(CurClkPoint, &flag);*/
	CurClkItem = m_treeProjectManagement.GetSelectedItem();
	if (m_treeProjectManagement.ItemHasChildren(CurClkItem))
	{
		return;
	}
	CString prjName;
	prjName = m_treeProjectManagement.GetItemText(CurClkItem);

	CProjectData* pPrjData = CProjectFileMrg::Instance()->GetProject(prjName);
	if (pPrjData!=NULL)
	{
		CAcModuleResourceOverride resOverride;
		CProjectManagementDlg dlg(pPrjData);
		int ret = dlg.DoModal();
	}
}
