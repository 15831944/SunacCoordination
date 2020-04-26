// DlgLeftBar.cpp : implementation file
//

#include "stdafx.h"
#include "../Command/Command.h"
#include "../Command/CommandWindowDetail.h"
#include "../Command/CommandWindowTable.h"
#include "DlgLeftBar.h"
#include "../res/resource.h"
#include "..\ProjectorFileMrg\ProjectFileMrg.h"
#include "ProjectManagementDlg.h"
#include "../WebIO/WebProjectFile.h"


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

	m_treeCtrlMenu.Expand(hItem, TVE_EXPAND);

	hItem = m_treeCtrlMenu.InsertItem(_T("��׼�ռ�ģ���"));
	m_treeCtrlMenu.InsertItem(_T("����"), hItem);
	m_treeCtrlMenu.InsertItem(_T("�յ�"), hItem);

	m_treeCtrlMenu.Expand(hItem, TVE_EXPAND);

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

	CWebProjectFile::Instance()->GetAllProjectInfo();
	
	//CProjectFileMrg::Instance()->InitTestData();
	vector<CString> AreaNames = CProjectFileMrg::Instance()->GetAllAreas();
	for (UINT i = 0; i < AreaNames.size(); i++)
	{
		vector<CProjectData *> vProjectsDatabByArea = CProjectFileMrg::Instance()->GetProjectsByArea(AreaNames[i]);
		HTREEITEM hFolder = m_treeProjectManagement.InsertItem(AreaNames[i], 0, 0);
		for (UINT j = 0; j < vProjectsDatabByArea.size(); j++)
		{
			m_treeProjectManagement.InsertItem(vProjectsDatabByArea[j]->GetPrjInfo().m_sName, 1, 1, hFolder);
		}
	}
	return 0;
}

LRESULT DlgLeftBar::OnClickedPopupMenu(WPARAM mID, LPARAM notUsed)
{
	switch (mID)
	{
	case ID_COMMAND_WINDOW:
		SendCommandToCAD(_T("SWINDESIGN"));
		break;

	case ID_COMMAND_WINDOWADVANCE: //��������
		SendCommandToCAD(_T("SWINADVANCE"));
		break;
	case ID_COMMAND_WINDOW_FLOORSETTING: //¥������
		SendCommandToCAD(_T("SWINFLOOR"));
		break;
	case ID_COMMAND_WINDOW_TOP2FRONT: //ƽ�浽����
		SendCommandToCAD(_T("SWINFTOP2FRONT"));
		break;
	//case ID_COMMAND_WINDOW_FRONT2TOP: //���浽ƽ��
	//	SendCommandToCAD(_T("SWINFFRONT2TOP"));
	//	break;

	case ID_COMMAND_WINDOWQUANTITY://�Ŵ�����
		SendCommandToCAD(_T("SWINSTATISTICS"));
		break;
	case ID_COMMAND_WINDOWTABLE:
		SendCommandToCAD(_T("SWINTABLE"));
		break;
	case ID_COMMAND_WINDOWDETAIL:	//�Ŵ���ͼ
		SendCommandToCAD(_T("SWINDETAIL"));
		break;
	case ID_COMMAND_WINDOWAUTOID:
		SendCommandToCAD(_T("SWINAUTOID")); //�Զ����
		break;
	case ID_COMMAND_WINSETTING:
		SendCommandToCAD(_T("SSETUP")); //�Ŵ�����
		break;
	case ID_COMMAND_WINDOWNOHIGHLIGHT:
		SendCommandToCAD(_T("SNOHIGHLIGHT")); //ȡ������
		break;
	case ID_COMMAND_WINDOWTABLECHECK:
		SendCommandToCAD(_T("SWINTABLECHECK")); //�Ŵ�����
		break;

	case ID_COMMAND_KITCHEN:
		SendCommandToCAD(_T("SKITCHEN"));
		break;
	case ID_COMMAND_BATHROOM:
		SendCommandToCAD(_T("SBATHROOM"));
		break;
	case ID_COMMAND_KITCHENQUANTITY:
		SendCommandToCAD(_T("SKBSTATISTIC"));
		break;
	case ID_COMMAND_RAILINGQUANTITY:
		SendCommandToCAD(_T("SRAILINGSTATISTICS"));
		break;
	case ID_COMMAND_RAILING:
		SendCommandToCAD(_T("SRAILING"));
		break;
	case ID_COMMAND_RAILINGDETAIL:
		SendCommandToCAD(_T("SRAILINGDETAIL"));
		break;
	case ID_COMMAND_RAILINGFLOORSETTING:
		SendCommandToCAD(_T("SRAILINGFLOORSETTING"));
		break;
	case ID_COMMAND_RAILING_TOP2FRONT:
		SendCommandToCAD(_T("SRAILINGTOP2FRONT"));
		break;
	case ID_COMMAND_AIRCONDITIONER:
		SendCommandToCAD(_T("SAIRCONDITIONER"));
		break;
	case ID_COMMAND_AIRCONDITIONERQUANTITY:
		SendCommandToCAD(_T("SAIRCONSTATISTIC"));
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

	CFont ft;
	ft.CreateFont(10, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("����")); // lpszFac
	//m_treeCtrlMenu.SetFont(&ft);
	m_treeCtrlMenu.SetItemHeight(30);

	m_treeProjectManagement.SetItemHeight(20);
	FillTreeItem();
	FillProjectManagementTreeItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void DlgLeftBar::OnNMDblclkTreeProjectmanagement(NMHDR *pNMHDR, LRESULT *pResult)
{
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
		//if (g_projectManagementDlg == NULL)
		//{
			CAcModuleResourceOverride resOverride;
			g_projectManagementDlg = new CProjectManagementDlg(pPrjData);
			g_projectManagementDlg->Create(IDD_DIALOG_PROJECTMANAGEMENT);
			g_projectManagementDlg->OnInitDialog();
		//}

		g_projectManagementDlg->ShowWindow(SW_SHOW);
	}
}
