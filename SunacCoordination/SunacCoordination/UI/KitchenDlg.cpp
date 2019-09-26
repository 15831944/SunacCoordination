// UI\KitchenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KitchenDlg.h"
#include "../WebIO/WebIO.h"
#include "../Object/Kitchen/RCKitchen.h"
#include "../Object/Kitchen/KitchenGen.h"
#include "afxdialogex.h"


// CKitchenDlg �Ի���

IMPLEMENT_DYNAMIC(CKitchenDlg, CAcUiDialog)

CKitchenDlg::CKitchenDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CKitchenDlg::IDD, pParent)
	, m_bAutoIndex(FALSE)
	, m_bHasAirOut(TRUE)
{
	m_rect.SetLB(AcGePoint3d(0, 0, 0));
	m_rect.SetRT(AcGePoint3d(0, 0, 0));
	m_doorDir = E_DIR_UNKNOWN;
	m_windowDir = E_DIR_UNKNOWN;
	m_pKitchGen = NULL;
}

CKitchenDlg::~CKitchenDlg()
{
	for (UINT i = 0; i < m_allKitchens.size(); i++)
	{
		delete m_allKitchens[i];
	}

	if (m_pKitchGen != NULL)
	{
		delete m_pKitchGen;
	}
}

LRESULT CKitchenDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
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
	DDX_Control(pDX, IDC_COMBO_FRIDGETYPE, m_fridgeType);
	DDX_Control(pDX, IDC_COMBO_KITCHENTYPE, m_kitchenType);
	DDX_Check(pDX, IDC_CHECK_AUTOINDEX, m_bAutoIndex);
	DDX_Control(pDX, IDC_EDIT_KITCHENNUMBER, m_number);
	DDX_Control(pDX, IDC_CHECK_IMAGE, m_isMirror);
	DDX_Check(pDX, IDC_CHECK_AIROUT, m_bHasAirOut);
}

BEGIN_MESSAGE_MAP(CKitchenDlg, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CKitchenDlg::OnBnClickedOk)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_INSERTKITCHEN, &CKitchenDlg::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_RANGE, &CKitchenDlg::OnBnClickedButtonRange)
	ON_BN_CLICKED(IDC_BUTTON_DOORDIR, &CKitchenDlg::OnBnClickedButtonDoorDir)
	ON_BN_CLICKED(IDC_BUTTON_WINDOWDIR, &CKitchenDlg::OnBnClickedButtonWindowDir)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CKitchenDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_CHECK_AUTOINDEX, &CKitchenDlg::OnBnClickedAutoIndex)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_KITCHEN, CKitchenDlg::OnSelChanged)
END_MESSAGE_MAP()


// CKitchenDlg ��Ϣ�������


BOOL CKitchenDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preKitchen.LoadDefaltSettings();
	LoadDefaultValue();

	//TODO �ѵײ������������Ŀռ���Ϊ��Ч���Լ����밴ť��Ϊ��ɫ��ֻ��ѡ����ԭ�Ͳ�������ԭ�����ԡ�

	return TRUE;
}


void CKitchenDlg::OnBnClickedOk()
{

	CAcUiDialog::OnOK();
}

void CKitchenDlg::OnBnClickedButtonInsert()
{
	if (m_pKitchGen==NULL)
	{
		MessageBox(_T("��ѡ��ԭ��\n"));
		return;
	}

	//1. ��������ֵ

	m_pKitchGen->GetKitchenAtt()->m_shuiPenType = TYUI_GetComboBoxText(m_basinType);
	m_pKitchGen->GetKitchenAtt()->m_bingXiangType = TYUI_GetComboBoxText(m_fridgeType);
	m_pKitchGen->GetKitchenAtt()->m_zaoTaiType = TYUI_GetComboBoxText(m_benchWidth);
	m_pKitchGen->GetKitchenAtt()->m_isMirror = m_isMirror.GetCheck()? true : false;


	
	//2.ѡ�������
	//ShowWindow(FALSE);
	//AcGePoint3d origin = TY_GetPoint();
	//acedPostCommandPrompt();
	AcGePoint3d origin = m_rect.GetLB();

	//����
	m_pKitchGen->GenKitchen(origin);

	//////////////////////////////////////////////////////////////////////////



	OnOK();
}


void CKitchenDlg::OnBnClickedButtonRange()
{
	ShowWindow(false);
	TYRect rect = TY_GetOneRect();
	ShowWindow(true);

	if (m_rect.IsSame(rect, 1E-4))
		return;

	if (IsKitchRectValid(rect)==false)
	{
		acutPrintf(_T("��ѡ������Χ��Ч\n")); //TODO ������������ʾ��Ϊmessagebox������ʾ
		return;
	}

	m_rect = rect;

	//���·�Χ�����ԭ�������б�
	m_preKitchen.ClearAllPreviews();
}

bool CKitchenDlg::IsKitchRectValid(TYRect rect)
{
	if (rect.GetWidth() < 1000 || rect.GetHeight() < 1000)
	{
		return false;
	}

	return true;
}

void CKitchenDlg::OnBnClickedButtonDoorDir()//�ŷ���
{
	if (IsKitchRectValid(m_rect) == false)
	{
		acutPrintf(_T("����ѡ�������Χ\n"));
		return;
	}

	ShowWindow(false);
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\nѡ���ŵ�λ��\n",pt)!=RTNORM) //��һ�ǵ�ѡ��
	{
		ShowWindow(true);
		return;
	}
	ShowWindow(true);

	E_DIRECTION temp = GetDir(pt);
	if (m_doorDir == temp) //δ�޸ģ�ֱ������
		return;

	m_doorDir = temp;
	if (m_windowDir == E_DIR_UNKNOWN)
		return;

	//���·�������ԭ�������б�
	m_preKitchen.ClearAllPreviews();

	if ((abs(m_windowDir - m_doorDir) % 2)==0)
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("�Ŵ�λ�ù�ϵ���Ŵ��Կ�"));
	else
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("�Ŵ�λ�ù�ϵ���Ŵ���ֱ��"));
}

void CKitchenDlg::OnBnClickedButtonWindowDir()//������
{
	if (IsKitchRectValid(m_rect) == false)
	{
		acutPrintf(_T("����ѡ�������Χ\n"));
		return;
	}

	ShowWindow(false);
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\nѡ�񴰵�λ��\n",pt)!=RTNORM) //��һ�ǵ�ѡ��
	{
		ShowWindow(true);
		return;
	}
	ShowWindow(true);

	E_DIRECTION temp = GetDir(pt);
	if (m_windowDir == temp) //δ�޸ģ�ֱ������
		return;

	if (m_doorDir == temp)
	{
		acutPrintf(_T("�Ŵ���������ͬ\n"));
		return;
	}
	m_windowDir = temp;
	if (m_doorDir == E_DIR_UNKNOWN)
		return;
	//���·�������ԭ�������б�
	m_preKitchen.ClearAllPreviews();
	if ((abs(m_windowDir - m_doorDir) % 2) == 0)
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("�Ŵ�λ�ù�ϵ���Ŵ��Կ�"));
	else
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("�Ŵ�λ�ù�ϵ���Ŵ���ֱ��"));
}


void CKitchenDlg::OnBnClickedButtonSearch()
{
	//1.�������
	if (IsKitchRectValid(m_rect) == false)
	{
		//acutPrintf(_T("����ѡ�������Χ\n"));
		::MessageBox(NULL, _T("����ѡ�������Χ\n"), NULL, 0);
		return;
	}
	if (m_doorDir == E_DIR_UNKNOWN || m_windowDir == E_DIR_UNKNOWN)
	{
		::MessageBox(NULL, _T("����ѡ���Ŵ�����\n"), NULL, 0);
		//acutPrintf(_T("����ѡ���Ŵ�����\n"));
		return;
	}
	if (m_windowDir == m_doorDir)
	{
		::MessageBox(NULL, _T("�Ŵ���������ͬ\n"), NULL, 0);
		//acutPrintf(_T("�Ŵ���������ͬ\n"));
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//2. ����ԭ��
	EKitchType kitchenType = E_KITCH_ALL;
	CString type = TYUI_GetText(m_kitchenType);
	if (type == _T("U��"))
		kitchenType = E_KITCH_U;
	else if (type == _T("L��"))
		kitchenType = E_KITCH_L;
	else if (type == _T("I��"))
		kitchenType = E_KITCH_I;

	double width = m_rect.GetWidth();
	double height = m_rect.GetHeight();
	m_allKitchens = CKitchMrg::GetInstance()->FilterKitch(kitchenType, width, height, m_doorDir, m_windowDir, m_bHasAirOut == TRUE);
	
	//////////////////////////////////////////////////////////////////////////
	//3. ��ʾԭ��
	m_preKitchen.ClearAllPreviews();
	if (m_allKitchens.empty())
	{
		acutPrintf(_T("δ�ҵ����������ļ�¼\n"));
		return;
	}
	m_preKitchen.SetRowCount((int)m_allKitchens.size());
	m_preKitchen.SetColumnCount(1);
	m_preKitchen.SetDisplayRows(3);
	m_preKitchen.SetDisplayColumns(1);
	for (int i = 0; i < m_allKitchens.size(); i++)
	{
		CString str;
		str.Format(_T("ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ����Ҫ��1.5\n��̬���ͣ���̬\n���÷�Χ������"), m_allKitchens[i]->m_yxid, m_rect.GetWidth() * m_rect.GetHeight() / 1E6);
		m_preKitchen.AddPreview(i, 0, TY_GetLocalFilePath() + m_allKitchens[i]->m_name, str);
	}

	m_preKitchen.SelectPreview(0, 0);
}

void CKitchenDlg::OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW* pView = (NM_GRIDVIEW*)pNMHDR;
	int nSel = pView->iRow;

	int kaiJian = int(m_rect.GetWidth() + 0.5);
	int jinShen = int(m_rect.GetHeight() + 0.5);
	if (m_doorDir == E_DIR_LEFT || m_doorDir == E_DIR_RIGHT)
		swap(kaiJian, jinShen);

	if (nSel < 0 || nSel >= m_allKitchens.size())
		return;

	AttrKitchen* curSecKitchen = m_allKitchens[nSel];

	if (m_pKitchGen != NULL)
	{
		delete m_pKitchGen;
	}

	m_pKitchGen = CKitchMrg::GetInstance()->CreateKitchGenByKitchType(curSecKitchen);
	if (m_pKitchGen==NULL)
	{
		MessageBox(_T("ԭ���޷�����KitchGen"));
		return;
	}

	//����������ѡ��
	TYUI_SetText(m_number, curSecKitchen->m_yxid);


	TYUI_InitComboBox(m_basinType, m_pKitchGen->GetShuipenOptions(), m_pKitchGen->GetShuipenDefault());
	TYUI_InitComboBox(m_fridgeType, m_pKitchGen->GetBinxiangOptions(), m_pKitchGen->GetBinxiangDefault());
	TYUI_InitComboBox(m_benchWidth, m_pKitchGen->GetZhaotaiOptions(), m_pKitchGen->GetZhaotaiDefault());

	//TODO ��������������Ϊ�ɱ༭״̬
}

void CKitchenDlg::OnBnClickedAutoIndex()
{
	UpdateData(TRUE);
	m_number.SetReadOnly(m_bAutoIndex);
}

E_DIRECTION CKitchenDlg::GetDir(ads_point pt)
{
	double minDis = abs(pt[X] - m_rect.GetLB().x);
	E_DIRECTION dir = E_DIR_LEFT;
	if (abs(pt[X] - m_rect.GetRT().x) < minDis)
	{
		minDis = abs(pt[X] - m_rect.GetRT().x);
		dir = E_DIR_RIGHT;
	}
	if (abs(pt[Y] - m_rect.GetLB().y) < minDis)
	{
		minDis = abs(pt[Y] - m_rect.GetLB().y);
		dir = E_DIR_BOTTOM;
	}
	if (abs(pt[Y] - m_rect.GetRT().y) < minDis)
	{
		minDis = abs(pt[Y] - m_rect.GetRT().y);
		dir = E_DIR_TOP;
	}
	return dir;
}

void CKitchenDlg::LoadDefaultValue()
{
	const vCString& kitchenTypes = WebIO::GetConfigDict()->Kitchen_GetTypes();
	TYUI_InitComboBox(m_kitchenType, kitchenTypes, kitchenTypes.empty() ? _T("") : kitchenTypes[0]);
	m_bAutoIndex = TRUE;
	m_number.SetReadOnly(TRUE);
	UpdateData(FALSE);
}

