// UI\KitchenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KitchenDlg.h"
#include "../WebIO/WebIO.h"

// CKitchenDlg �Ի���

CKitchenDlg* g_kitchenDlg = NULL;

BOOL CloseKitchenDlg()
{
	if (g_kitchenDlg == NULL)
		return TRUE;
	BOOL ret = g_kitchenDlg->DestroyWindow();
	if (ret)
		g_kitchenDlg = NULL;
	return ret;
}

IMPLEMENT_DYNAMIC(CKitchenDlg, CAcUiDialog)

CKitchenDlg::CKitchenDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CKitchenDlg::IDD, pParent)
	, m_bAutoIndex(FALSE)
	, m_bNoAirout(FALSE)
	, m_isStd(0)
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
	g_kitchenDlg = NULL;
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
	DDX_Check(pDX, IDC_CHECK_AIROUT, m_bNoAirout);
	DDX_Control(pDX, IDC_EDIT_OFFSETX, m_offsetX);
	DDX_Control(pDX, IDC_EDIT_OFFSETY, m_offsetY);
	DDX_Control(pDX, IDC_EDIT_X, m_customX);
	DDX_Control(pDX, IDC_EDIT_Y, m_customY);
	DDX_Radio(pDX, IDC_RADIO_STANDARD, m_isStd);
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
	ON_BN_CLICKED(IDC_CHECK_AIROUT, &CKitchenDlg::OnBnClickedNoAirout)
	ON_CBN_SELCHANGE(IDC_COMBO_KITCHENTYPE, &CKitchenDlg::ClearPreviews)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_KITCHEN, &CKitchenDlg::OnSelChanged)
END_MESSAGE_MAP()


// CKitchenDlg ��Ϣ�������


BOOL CKitchenDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preKitchen.LoadDefaltSettings();
	LoadDefaultValue();

	EnableSetProperty(false);

	return TRUE;
}


void CKitchenDlg::OnBnClickedOk()
{
	CAcUiDialog::OnOK();
	DestroyWindow();
}

void CKitchenDlg::OnBnClickedButtonInsert()
{
	UpdateData(TRUE);
	if (m_pKitchGen==NULL)
	{
		MessageBox(_T("��ѡ��ԭ��\n"));
		return;
	}

	CString errMsg;
	if (!CheckValid(errMsg))
	{
		AfxMessageBox(errMsg);
		return;
	}

	//1. ��������ֵ

	m_pKitchGen->GetKitchenAtt()->m_width = m_rect.GetWidth();
	m_pKitchGen->GetKitchenAtt()->m_height = m_rect.GetHeight();
	if (m_doorDir == E_DIR_LEFT || m_doorDir == E_DIR_RIGHT)
		swap(m_pKitchGen->GetKitchenAtt()->m_width, m_pKitchGen->GetKitchenAtt()->m_height);
	m_pKitchGen->SetDoorDir(m_doorDir);
	m_pKitchGen->SetWindowDir(m_windowDir);

	m_pKitchGen->GetKitchenAtt()->m_shuiPenType = TYUI_GetComboBoxText(m_basinType);
	m_pKitchGen->GetKitchenAtt()->m_bingXiangType = TYUI_GetComboBoxText(m_fridgeType);
	m_pKitchGen->GetKitchenAtt()->m_zaoTaiType = TYUI_GetComboBoxText(m_benchWidth);

	m_pKitchGen->GetKitchenAtt()->m_hasPaiQiDao = !m_bNoAirout;
	m_pKitchGen->GetKitchenAtt()->m_isGuoBiao = (m_isStd == 0);
	m_pKitchGen->GetKitchenAtt()->m_floorRange = (E_FLOOR_RANGE)m_floorRange.GetCurSel();
	m_pKitchGen->GetKitchenAtt()->m_airVentOffsetX = TYUI_GetInt(m_offsetX);
	m_pKitchGen->GetKitchenAtt()->m_airVentOffsetY = TYUI_GetInt(m_offsetY);
	m_pKitchGen->GetKitchenAtt()->m_airVentW = TYUI_GetInt(m_customX);
	m_pKitchGen->GetKitchenAtt()->m_airVentH = TYUI_GetInt(m_customY);

	m_pKitchGen->GetKitchenAtt()->m_isMirror = m_isMirror.GetCheck()? true : false;

	AcGePoint3d origin = m_rect.GetLB();

	//����
	m_pKitchGen->GenKitchen(origin);

	OnOK();
}


void CKitchenDlg::OnBnClickedButtonRange()
{
	ShowWindow(false);
	TYRect rect = TY_GetOneRect();
	ShowWindow(true);

	if (IsKitchRectValid(rect)==false)
	{
		AfxMessageBox(_T("��ѡ������Χ��Ч\n"));
		return;
	}

	m_rect = rect;

	ClearPreviews();
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
		AfxMessageBox(_T("����ѡ�������Χ\n"));
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

	if (m_windowDir == temp)
	{
		AfxMessageBox(_T("�Ŵ���������ͬ\n"));
		return;
	}

	m_doorDir = temp;
	if (m_windowDir == E_DIR_UNKNOWN)
		return;

	ClearPreviews();

	if ((abs(m_windowDir - m_doorDir) % 2)==0)
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("�Ŵ�λ�ù�ϵ���Ŵ��Կ�"));
	else
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("�Ŵ�λ�ù�ϵ���Ŵ���ֱ��"));
}

void CKitchenDlg::OnBnClickedButtonWindowDir()//������
{
	if (IsKitchRectValid(m_rect) == false)
	{
		AfxMessageBox(_T("����ѡ�������Χ\n"));
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
		AfxMessageBox(_T("�Ŵ���������ͬ\n"));
		return;
	}
	m_windowDir = temp;
	if (m_doorDir == E_DIR_UNKNOWN)
		return;

	ClearPreviews();

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
		AfxMessageBox(_T("����ѡ�������Χ\n"));
		return;
	}
	if (m_doorDir == E_DIR_UNKNOWN || m_windowDir == E_DIR_UNKNOWN)
	{
		AfxMessageBox(_T("����ѡ���Ŵ�����\n"));
		return;
	}
	if (m_windowDir == m_doorDir)
	{
		AfxMessageBox(_T("�Ŵ���������ͬ\n"));
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
	m_allKitchens = CKitchMrg::GetInstance()->FilterKitch(kitchenType, width, height, m_doorDir, m_windowDir, m_bNoAirout == FALSE);
	
	//////////////////////////////////////////////////////////////////////////
	//3. ��ʾԭ��
	m_preKitchen.ClearAllPreviews();
	if (m_allKitchens.empty())
	{
		AfxMessageBox(_T("δ�ҵ����������ļ�¼\n"));
		return;
	}
	m_preKitchen.SetRowCount((int)m_allKitchens.size());
	m_preKitchen.SetColumnCount(1);
	m_preKitchen.SetDisplayRows(3);
	m_preKitchen.SetDisplayColumns(1);
	for (UINT i = 0; i < m_allKitchens.size(); i++)
	{
		CString str;
		str.Format(_T("ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ����Ҫ��1.5\n��̬���ͣ���̬\n���÷�Χ������"), m_allKitchens[i]->m_prototypeCode, m_rect.GetWidth() * m_rect.GetHeight() / 1E6);
		m_preKitchen.AddPreview(i, 0, TY_GetLocalFilePath() + m_allKitchens[i]->m_fileName, str);
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

	if (nSel < 0 || nSel >= (int)m_allKitchens.size())
	{
		EnableSetProperty(false);
		return;
	}

	AttrKitchen* curSecKitchen = m_allKitchens[nSel];

	if (m_pKitchGen != NULL)
	{
		delete m_pKitchGen;
	}

	m_pKitchGen = CKitchMrg::GetInstance()->CreateKitchGenByKitchType(curSecKitchen);
	if (m_pKitchGen==NULL)
	{
		MessageBox(_T("ԭ���޷�����KitchGen"));
		EnableSetProperty(false);
		return;
	}

	//����������ѡ��
	TYUI_SetText(m_number, curSecKitchen->m_prototypeCode);

	TYUI_InitComboBox(m_basinType, m_pKitchGen->GetShuipenOptions(), m_pKitchGen->GetShuipenDefault());
	TYUI_InitComboBox(m_fridgeType, m_pKitchGen->GetBinxiangOptions(), m_pKitchGen->GetBinxiangDefault());
	TYUI_InitComboBox(m_benchWidth, m_pKitchGen->GetZhaotaiOptions(), m_pKitchGen->GetZhaotaiDefault());
	EnableSetProperty(true);

	if (curSecKitchen->m_windowDoorPos == _T("�Ŵ���ֱ"))
	{
		if (m_doorDir == (m_windowDir + 1) % 4) //��λ���ŵ�˳ʱ�뷽��
		{
			curSecKitchen->m_isMirror = true;
			m_isMirror.SetCheck(TRUE);
		}
		else
		{
			curSecKitchen->m_isMirror = false;
			m_isMirror.SetCheck(FALSE);
		}
		TYUI_Disable(m_isMirror);
	}
	else
	{
		m_isMirror.SetCheck(false);
		TYUI_Enable(m_isMirror);
	}
}

void CKitchenDlg::OnBnClickedAutoIndex()
{
	UpdateData(TRUE);
	m_number.SetReadOnly(m_bAutoIndex);
}

void CKitchenDlg::OnBnClickedNoAirout()
{
	UpdateData(TRUE);
	if (m_preKitchen.GetSelectedCount() > 0)
		EnableSetAirout(m_bNoAirout == FALSE);

	//�޸����������ú���������
	if (m_allKitchens.size() > 0)
		OnBnClickedButtonSearch();
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
	m_floorRange.SetCurSel(0);
	TYUI_SetInt(m_offsetX, 0);
	TYUI_SetInt(m_offsetY, 0);
	TYUI_SetInt(m_customX, 450);
	TYUI_SetInt(m_customY, 350);
	UpdateData(FALSE);
}

void CKitchenDlg::EnableSetProperty(bool bEnable)
{
	m_number.EnableWindow(bEnable);
	m_basinType.EnableWindow(bEnable);
	m_fridgeType.EnableWindow(bEnable);
	m_benchWidth.EnableWindow(bEnable);
	m_isMirror.EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_AUTOINDEX)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_INSERTKITCHEN)->EnableWindow(bEnable);
	EnableSetAirout(bEnable && (m_bNoAirout == FALSE));
}

void CKitchenDlg::EnableSetAirout(bool bEnable)
{
	GetDlgItem(IDC_RADIO_STANDARD)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_CUSTOM)->EnableWindow(bEnable);
	m_floorRange.EnableWindow(bEnable);
	m_offsetX.EnableWindow(bEnable);
	m_offsetY.EnableWindow(bEnable);
	m_customX.EnableWindow(bEnable);
	m_customY.EnableWindow(bEnable);
}

void CKitchenDlg::ClearPreviews()
{
	m_allKitchens.clear();
	m_preKitchen.ClearAllPreviews();
	EnableSetProperty(false);
}

bool CKitchenDlg::CheckValid(CString& errMsg)
{
	errMsg = _T("");

	//Ŀǰֻ�����������������100-1000��Ϊ��Чֵ
	if (m_bNoAirout)
		return true;
	double ventX, ventY;
	int nSel = m_floorRange.GetCurSel();
	if (m_isStd == 0)
	{
		ventX = TYUI_GetDouble(m_offsetX) + c_airVentSize[nSel];
		ventY = TYUI_GetDouble(m_offsetY) + c_airVentSize[nSel];
	}
	else
	{
		ventX = TYUI_GetDouble(m_customX);
		ventY = TYUI_GetDouble(m_customY);
	}
	if (ventX >= m_rect.GetWidth() || ventX <= 0 || ventY >= m_rect.GetHeight() || ventY <= 0)
	{
		errMsg = _T("�������ߴ�Ϊ��Чֵ");
		return false;
	}
	return true;
}

