// UI\RailingBaseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RailingBaseDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"

// CRailingBaseDlg �Ի���

IMPLEMENT_DYNAMIC(CRailingBaseDlg, CDialogEx)

CRailingBaseDlg::CRailingBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRailingBaseDlg::IDD, pParent)
{

}

CRailingBaseDlg::~CRailingBaseDlg()
{
}

void CRailingBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_RAILINGBASE, m_libPreview);
	DDX_Control(pDX, IDC_TREE_RAILINGBASE, m_category);
}


BEGIN_MESSAGE_MAP(CRailingBaseDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRailingBaseDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRailingBaseDlg ��Ϣ�������


BOOL CRailingBaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitTree();
	FillTreeItem();
	InitLib();
	return TRUE;
}

int CRailingBaseDlg::InitLib()
{
	m_libPreview.LoadDefaltSettings();

	m_libPreview.SetRowCount(3);
	m_libPreview.SetColumnCount(2);

	//����Ԥ��ͼ��ʾ����Ϊ3��1��
	m_libPreview.SetDisplayRows(3);
	m_libPreview.SetDisplayColumns(2);

	//���õ�һ��ռ60%�����ಿ��Ϊ�ڶ���
	//m_libPreview.SetHeaderWidth(_T("60;+"));

	vCString allFiles;
	TY_GetAllLanGanFiles(allFiles);
	m_libPreview.AddPreview(0, 0, allFiles[0]); 
	m_libPreview.AddPreview(0, 1, allFiles[1]); 
	m_libPreview.AddPreview(1, 0, allFiles[2]); 
	m_libPreview.AddPreview(1, 1, allFiles[3]); 
	m_libPreview.AddPreview(2, 0, allFiles[4]); 
	m_libPreview.AddPreview(2, 1, allFiles[5]); 
	m_libPreview.AddPreview(3, 0, allFiles[6]); 

	m_libPreview.ShowPreviews();

	return 0;
}

int CRailingBaseDlg::InitTree()
{
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
	m_category.ModifyStyle(1,dwViewStyle);

	UINT uiBmpId = IDB_BITMAP1;
	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return false;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	m_treeImages.Create(16, bmpObj.bmHeight, ILC_MASK | ILC_COLOR24, 0, 0);
	m_treeImages.Add(&bmp, RGB(255, 0, 255));
	m_category.SetImageList(&m_treeImages, TVSIL_NORMAL);

	return true;
}

int CRailingBaseDlg::FillTreeItem()
{
	m_hroot = m_category.InsertItem(_T("���˿�"), 0, 0);
	m_category.SetItemState(m_hroot, TVIS_BOLD, TVIS_BOLD);

	m_hlibTieYi = m_category.InsertItem(_T("��������"), 0, 0, m_hroot);
	m_hlibBoLi = m_category.InsertItem(_T("��������"), 0, 0, m_hroot);

	m_category.InsertItem(_T("TY-11B-10"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-11"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-12"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-13"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-14"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-15"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-16"), 1, 1, m_hlibTieYi);

	m_category.InsertItem(_T("������1"), 1, 1, m_hlibBoLi);
	m_category.InsertItem(_T("������2"), 1, 1, m_hlibBoLi);
	m_category.InsertItem(_T("������3"), 1, 1, m_hlibBoLi);
	m_category.InsertItem(_T("������4"), 1, 1, m_hlibBoLi);
	m_category.InsertItem(_T("������5"), 1, 1, m_hlibBoLi);
	m_category.InsertItem(_T("������6"), 1, 1, m_hlibBoLi);

	m_category.Expand(m_hroot, TVE_EXPAND);
	m_category.Expand(m_hlibBoLi, TVE_EXPAND);
	m_category.Expand(m_hlibTieYi, TVE_EXPAND);

	return 0;
}


void CRailingBaseDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
