// UI\KitchenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KitchenDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCKitchen.h"
#include "../Object/AttrKitchen.h"
// CKitchenDlg �Ի���

IMPLEMENT_DYNAMIC(CKitchenDlg, CAcUiDialog)

CKitchenDlg::CKitchenDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CKitchenDlg::IDD, pParent)
{

}

CKitchenDlg::~CKitchenDlg()
{
}

LRESULT CKitchenDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	//return FALSE;
	return TRUE;
}

void CKitchenDlg::OnOK()
{
	CAcUiDialog::OnOK();
	DestroyWindow();
}

void CKitchenDlg::OnCancel()
{
	CAcUiDialog::OnCancel();
	DestroyWindow();
}

void CKitchenDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();
	delete this;
}

void CKitchenDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_KITCHEN, m_preKitchen);
	DDX_Control(pDX, IDC_COMBO_FLOORRANGE, m_floorRange);
	DDX_Control(pDX, IDC_COMBO_BASINTYPE, m_basinType);
	DDX_Control(pDX, IDC_COMBO_BENCHWIDTH, m_benchWidth);
	DDX_Control(pDX, IDC_COMBO_FRIDGETYPE, m_frigeType);
}

BEGIN_MESSAGE_MAP(CKitchenDlg, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CKitchenDlg::OnBnClickedOk)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_INSERTKITCHEN, &CKitchenDlg::OnBnClickedMfcbuttonInsert)
	ON_BN_CLICKED(IDC_BUTTON_RANGE, &CKitchenDlg::OnBnClickedMfcbuttonRange)
	ON_BN_CLICKED(IDC_BUTTON_DOORDIR, &CKitchenDlg::OnBnClickedMfcbuttonDoorDir)
	ON_BN_CLICKED(IDC_BUTTON_WINDOWDIR, &CKitchenDlg::OnBnClickedMfcbuttonWindowDir)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CKitchenDlg::OnBnClickedButtonSearch)
END_MESSAGE_MAP()


// CKitchenDlg ��Ϣ�������


BOOL CKitchenDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preKitchen.LoadDefaltSettings();

	m_preKitchen.SetRowCount(8);
	m_preKitchen.SetColumnCount(2);

	//����Ԥ��ͼ��ʾ����Ϊ3��1��
	m_preKitchen.SetDisplayRows(3);
	m_preKitchen.SetDisplayColumns(1);

	//���õ�һ��ռ60%�����ಿ��Ϊ�ڶ���
	m_preKitchen.SetHeaderWidth(_T("60;+"));

	
	m_floorRange.SetCurSel(0);
	m_basinType.SetCurSel(0);
	m_benchWidth.SetCurSel(0);
	m_frigeType.SetCurSel(0);
	m_projectLevel.SetCurSel(0);

	return TRUE;
}


void CKitchenDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAcUiDialog::OnOK();
}

TYRect rect;

void CKitchenDlg::OnBnClickedMfcbuttonInsert()
{
	ShowWindow(FALSE);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vCString allFiles;
	TY_GetAllKitchenFiles(allFiles);

	double width = rect.GetWidth(), height = rect.GetHeight();
	AcGePoint3d origin = rect.GetLB();

	vector<int> sels = m_preKitchen.GetSelectedRows();
	if (sels.size() > 0)
	{
		RCKitchen oneKitchen;
		if (sels[0] == 0)
			oneKitchen.Insert(allFiles[0], origin, 0, L"0", 256);
		else
			oneKitchen.Insert(allFiles[1], origin, 0, L"0", 256);
		oneKitchen.InitParameters();
		oneKitchen.SetParameter(L"����", height);
		oneKitchen.SetParameter(L"����", width);
		oneKitchen.RunParameters();

		//��UI�����ݼ�¼��ͼ�����չ�ֵ���
		AttrKitchen *pAttribute = new AttrKitchen();
		if (sels[0] == 0)
			pAttribute->m_yxid = L"KUA";
		else if (sels[0] == 1)
			pAttribute->m_yxid = L"KUB";
		else
			pAttribute->m_yxid = L"KUC";

		oneKitchen.AddAttribute(pAttribute);
		pAttribute->close();
	}
	ShowWindow(true);
	OnOK();
}


void CKitchenDlg::OnBnClickedMfcbuttonRange()
{
	ShowWindow(false);
	rect = TY_GetOneRect();
	ShowWindow(true);
}


void CKitchenDlg::OnBnClickedMfcbuttonDoorDir()//�ŷ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(false);
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\nѡ���ŵ�λ��\n",pt)!=RTNORM) //��һ�ǵ�ѡ��
	{
		return;
	}
	ShowWindow(true);
}

void CKitchenDlg::OnBnClickedMfcbuttonWindowDir()//������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(false);
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\nѡ�񴰵�λ��\n",pt)!=RTNORM) //��һ�ǵ�ѡ��
	{
		return;
	}
	ShowWindow(true);
}


void CKitchenDlg::OnBnClickedButtonSearch()
{
	vCString allFiles;
	TY_GetAllKitchenFiles(allFiles);
	for (int i = 0; i < allFiles.size(); i++)
		m_preKitchen.AddPreview(i, 0, allFiles[i], _T("�������ͣ�KU\n���������2.1\nͨ������1.6"));

	//m_preKitchen.ShowPreviews();

}
