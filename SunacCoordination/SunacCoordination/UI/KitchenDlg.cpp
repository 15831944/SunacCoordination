// UI\KitchenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KitchenDlg.h"
#include "../WebIO/WebIO.h"
#include "../Object/RCKitchen.h"
#include "afxdialogex.h"
// CKitchenDlg 对话框

IMPLEMENT_DYNAMIC(CKitchenDlg, CAcUiDialog)

CKitchenDlg::CKitchenDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CKitchenDlg::IDD, pParent)
	, m_bHasAirOut(FALSE)
	, m_bAutoIndex(FALSE)
{
	m_rect.SetLB(AcGePoint3d(0, 0, 0));
	m_rect.SetRT(AcGePoint3d(0, 0, 0));
	m_doorDir = DIR_UNKNOWN;
	m_windowDir = DIR_UNKNOWN;
}

CKitchenDlg::~CKitchenDlg()
{
	for (UINT i = 0; i < m_allKitchens.size(); i++)
	{
		delete m_allKitchens[i];
	}
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
	DDX_Control(pDX, IDC_COMBO_FRIDGETYPE, m_fridgeType);
	DDX_Control(pDX, IDC_COMBO_KITCHENTYPE, m_kitchenType);
	DDX_Check(pDX, IDC_CHECK_AIROUT, m_bHasAirOut);
	DDX_Check(pDX, IDC_CHECK_AUTOINDEX, m_bAutoIndex);
	DDX_Control(pDX, IDC_EDIT_KITCHENNUMBER, m_number);
}

BEGIN_MESSAGE_MAP(CKitchenDlg, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CKitchenDlg::OnBnClickedOk)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_INSERTKITCHEN, &CKitchenDlg::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_RANGE, &CKitchenDlg::OnBnClickedButtonRange)
	ON_BN_CLICKED(IDC_BUTTON_DOORDIR, &CKitchenDlg::OnBnClickedButtonDoorDir)
	ON_BN_CLICKED(IDC_BUTTON_WINDOWDIR, &CKitchenDlg::OnBnClickedButtonWindowDir)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CKitchenDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_CHECK_AIROUT, &CKitchenDlg::OnBnClickedAirOut)
	ON_BN_CLICKED(IDC_CHECK_AUTOINDEX, &CKitchenDlg::OnBnClickedAutoIndex)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_KITCHEN, CKitchenDlg::OnSelChanged)
END_MESSAGE_MAP()


// CKitchenDlg 消息处理程序


BOOL CKitchenDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preKitchen.LoadDefaltSettings();
	LoadDefaultValue();

	return TRUE;
}


void CKitchenDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CAcUiDialog::OnOK();
}

void CKitchenDlg::OnBnClickedButtonInsert()
{
	ShowWindow(FALSE);

	// TODO: 在此添加控件通知处理程序代码
	vCString allFiles;
	TY_GetAllKitchenFiles(allFiles);

	double width = m_rect.GetWidth(), height = m_rect.GetHeight();
	AcGePoint3d origin = m_rect.GetLB();

	vector<int> sels = m_preKitchen.GetSelectedRows();
	if (sels.size() > 0)
	{
		RCKitchen oneKitchen;
		if (sels[0] == 0)
			oneKitchen.Insert(allFiles[0], origin, 0, L"0", 256);
		else
			oneKitchen.Insert(allFiles[1], origin, 0, L"0", 256);
		oneKitchen.InitParameters();
		oneKitchen.SetParameter(L"进深", height);
		oneKitchen.SetParameter(L"开间", width);
		oneKitchen.RunParameters();

		//把UI的数据记录在图框的扩展字典中
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


void CKitchenDlg::OnBnClickedButtonRange()
{
	ShowWindow(false);
	TYRect rect = TY_GetOneRect();
	ShowWindow(true);

	if (m_rect.IsSame(rect, 1E-4))
		return;
	if (rect.GetWidth() < 1E-4 || rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("所选厨房范围无效\n"));
		return;
	}
	m_rect = rect;

	//更新范围后清空原有搜索列表
	m_preKitchen.ClearAllPreviews();
}


void CKitchenDlg::OnBnClickedButtonDoorDir()//门方向
{
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("请先选择厨房范围\n"));
		return;
	}

	ShowWindow(false);
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\n选择门的位置\n",pt)!=RTNORM) //第一角点选择
	{
		ShowWindow(true);
		return;
	}
	ShowWindow(true);

	DIR temp = GetDir(pt);
	if (m_doorDir == temp) //未修改，直接跳过
		return;
	if (m_windowDir == temp)
	{
		acutPrintf(_T("门窗方向不能相同\n"));
		return;
	}
	m_doorDir = temp;
	if (m_windowDir == DIR_UNKNOWN)
		return;
	//更新方向后清空原有搜索列表
	m_preKitchen.ClearAllPreviews();
	if (abs(m_windowDir - m_doorDir) == 2)
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗对开"));
	else
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗垂直开"));
}

void CKitchenDlg::OnBnClickedButtonWindowDir()//窗方向
{
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("请先选择厨房范围\n"));
		return;
	}

	ShowWindow(false);
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\n选择窗的位置\n",pt)!=RTNORM) //第一角点选择
	{
		ShowWindow(true);
		return;
	}
	ShowWindow(true);

	DIR temp = GetDir(pt);
	if (m_windowDir == temp) //未修改，直接跳过
		return;
	if (m_doorDir == temp)
	{
		acutPrintf(_T("门窗方向不能相同\n"));
		return;
	}
	m_windowDir = temp;
	if (m_doorDir == DIR_UNKNOWN)
		return;
	//更新方向后清空原有搜索列表
	m_preKitchen.ClearAllPreviews();
	if (abs(m_windowDir - m_doorDir) == 2)
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗对开"));
	else
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗垂直开"));
}


void CKitchenDlg::OnBnClickedButtonSearch()
{
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("请先选择厨房范围\n"));
		return;
	}
	if (m_doorDir == DIR_UNKNOWN || m_windowDir == DIR_UNKNOWN)
	{
		acutPrintf(_T("请先选择门窗方向\n"));
		return;
	}
	CString type = TYUI_GetText(m_kitchenType);
	if (type == _T("U型"))
		m_allKitchens = FilterKU();
	else if (type == _T("L型"))
		m_allKitchens = FilterKL();
	else if (type == _T("I型"))
		m_allKitchens = FilterKI();
	
	m_preKitchen.ClearAllPreviews();
	if (m_allKitchens.empty())
	{
		acutPrintf(_T("未找到符合条件的记录\n"));
		return;
	}
	m_preKitchen.SetRowCount((int)m_allKitchens.size());
	m_preKitchen.SetColumnCount(1);
	m_preKitchen.SetDisplayRows(3);
	m_preKitchen.SetDisplayColumns(1);
	for (int i = 0; i < m_allKitchens.size(); i++)
	{
		CString str;
		str.Format(_T("原型编号：%s\n厨房面积：%.2lf\n通风量要求：1.5\n动态类型：动态\n适用范围：集团"), m_allKitchens[i]->m_yxid, m_rect.GetWidth() * m_rect.GetHeight() / 1E6);
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
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	if (nSel >= 0 && nSel < m_allKitchens.size())
	{
		CString type = m_allKitchens[nSel]->m_kitchenType;
		CString pos = m_allKitchens[nSel]->m_windowDoorPos;

		TYUI_SetText(m_number, m_allKitchens[nSel]->m_yxid);
		if (type == _T("Uq") && pos == _T("对开"))
		{
			if (kaiJian < 2900)
				TYUI_InitComboBox(m_basinType, _T("单盆600;单盆800"), _T("单盆600"));
			else
				TYUI_InitComboBox(m_basinType, _T("双盆900;双盆1000;双盆1200"), _T("双盆900"));

			TYUI_InitComboBox(m_fridgeType, _T("对开门800;对开门1000"), _T("对开门800"));
			TYUI_InitComboBox(m_benchWidth, _T("800;900"), _T("800"));
		}
		else if (type == _T("Uq"))
		{
			if (jinShen < 2150)
				TYUI_InitComboBox(m_basinType, _T("单盆600;单盆800"), _T("单盆600"));
			else
				TYUI_InitComboBox(m_basinType, _T("双盆900;双盆1000;双盆1200"), _T("双盆900"));

			TYUI_InitComboBox(m_fridgeType, _T("对开门800;对开门1000"), _T("对开门800"));
			TYUI_InitComboBox(m_benchWidth, _T("800;900"), _T("800"));
		}
		else if (type == _T("Us"))
		{
			TYUI_InitComboBox(m_basinType, _T("单盆600;单盆800;双盆900;双盆1000;双盆1200"), _T("单盆600"));
			TYUI_InitComboBox(m_fridgeType, _T("对开门800;对开门1000"), _T("对开门800"));
			TYUI_InitComboBox(m_benchWidth, _T("800;900"), _T("800"));
		}
		else if (type == _T("L"))
		{
			TYUI_InitComboBox(m_basinType, _T("单盆600;单盆800;双盆900;双盆1000;双盆1200"), _T("单盆600"));

			if (jinShen < 3200)
				TYUI_InitComboBox(m_fridgeType, _T("单开门700"), _T("单开门700"));
			else
				TYUI_InitComboBox(m_fridgeType, _T("对开门800;对开门1000"), _T("对开门800"));

			TYUI_InitComboBox(m_benchWidth, _T("800;900"), _T("800"));
		}
		else if (type == _T("I"))
		{
			if (kaiJian < 3350)
				TYUI_InitComboBox(m_basinType, _T("单盆600;单盆800"), _T("单盆600"));
			else
				TYUI_InitComboBox(m_basinType, _T("双盆900;双盆1000;双盆1200"), _T("双盆900"));

			TYUI_InitComboBox(m_fridgeType, _T("单开门700;对开门800;对开门1000"), _T("单开门700"));

			if (kaiJian <= 3350)
				TYUI_InitComboBox(m_benchWidth, _T("800"), _T("800"));
			else
				TYUI_InitComboBox(m_benchWidth, _T("900"), _T("900"));
		}
	}
}

void CKitchenDlg::OnBnClickedAirOut()
{
	UpdateData(TRUE);
}

void CKitchenDlg::OnBnClickedAutoIndex()
{
	UpdateData(TRUE);
	m_number.SetReadOnly(m_bAutoIndex);
}

DIR CKitchenDlg::GetDir(ads_point pt)
{
	double minDis = abs(pt[X] - m_rect.GetLB().x);
	DIR dir = DIR_LEFT;
	if (abs(pt[X] - m_rect.GetRT().x) < minDis)
	{
		minDis = abs(pt[X] - m_rect.GetRT().x);
		dir = DIR_RIGHT;
	}
	if (abs(pt[Y] - m_rect.GetLB().y) < minDis)
	{
		minDis = abs(pt[Y] - m_rect.GetLB().y);
		dir = DIR_BOTTOM;
	}
	if (abs(pt[Y] - m_rect.GetRT().y) < minDis)
	{
		minDis = abs(pt[Y] - m_rect.GetRT().y);
		dir = DIR_TOP;
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

vector<AttrKitchen*> CKitchenDlg::FilterKU()
{
	vector<AttrKitchen*> attrKitchen;
	int kaiJian = int(m_rect.GetWidth() + 0.5);
	int jinShen = int(m_rect.GetHeight() + 0.5);
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	if ((kaiJian <= jinShen) && (abs(m_windowDir - m_doorDir) == 2))
		return FilterKUq();
	else if (kaiJian <= jinShen)
		return FilterKUqc();
	else
		return FilterKUs();
}

vector<AttrKitchen*> CKitchenDlg::FilterKUq()
{
	vector<AttrKitchen*> attrKitchen;
	int kaiJian = int(m_rect.GetWidth() + 0.5);
	int jinShen = int(m_rect.GetHeight() + 0.5);
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	//限定开间与进深的范围
	if (kaiJian < 2450 || kaiJian > 3500)
		return attrKitchen;
	if (jinShen < 1700 || jinShen > 2600)
		return attrKitchen;
	if (kaiJian <= jinShen)
		return attrKitchen;

	//不支持2450*1700
	if (kaiJian == 2450 && jinShen == 1700)
		return attrKitchen;

	//以150递增尺寸
	if ((kaiJian - 2450) % 150 != 0)
		return attrKitchen;
	if ((jinShen - 1700) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(kaiJian, jinShen, _T("对开"), _T("Uq"), (m_bHasAirOut == FALSE));
}

vector<AttrKitchen*> CKitchenDlg::FilterKUqc()
{
	vector<AttrKitchen*> attrKitchen;
	int kaiJian = int(m_rect.GetWidth() + 0.5);
	int jinShen = int(m_rect.GetHeight() + 0.5);
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	//限定开间与进深的范围
	if (kaiJian < 2450 || kaiJian > 3500)
		return attrKitchen;
	if (jinShen < 1700 || jinShen > 2450)
		return attrKitchen;
	if (kaiJian <= jinShen)
		return attrKitchen;

	//不支持2450*1700
	if (kaiJian == 2450 && jinShen == 1700)
		return attrKitchen;

	//以150递增尺寸
	if ((kaiJian - 2450) % 150 != 0)
		return attrKitchen;
	if ((jinShen - 1700) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(kaiJian, jinShen, _T("垂直开"), _T("Uq"), (m_bHasAirOut == FALSE));
}

vector<AttrKitchen*> CKitchenDlg::FilterKUs()
{
	vector<AttrKitchen*> attrKitchen;
	int kaiJian = int(m_rect.GetWidth() + 0.5);
	int jinShen = int(m_rect.GetHeight() + 0.5);
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	//限定开间与进深的范围
	if (kaiJian < 2300 || kaiJian > 2450)
		return attrKitchen;
	if (jinShen < 2450 || jinShen > 3200)
		return attrKitchen;
	if (kaiJian > jinShen)
		return attrKitchen;

	//以150递增尺寸
	if ((kaiJian - 2300) % 150 != 0)
		return attrKitchen;
	if ((jinShen - 2450) % 150 != 0)
		return attrKitchen;

	//只能对开
	if (abs(m_windowDir - m_doorDir) != 2)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(kaiJian, jinShen, _T("对开"), _T("Us"), (m_bHasAirOut == FALSE));
}

vector<AttrKitchen*> CKitchenDlg::FilterKL()
{
	vector<AttrKitchen*> attrKitchen;
	int kaiJian = int(m_rect.GetWidth() + 0.5);
	int jinShen = int(m_rect.GetHeight() + 0.5);
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	//限定开间与进深的范围
	if (kaiJian < 1700 || kaiJian > 2000)
		return attrKitchen;
	if (jinShen < 2600 || jinShen > 3950)
		return attrKitchen;

	//以150递增尺寸
	if ((kaiJian - 1700) % 150 != 0)
		return attrKitchen;
	if ((jinShen - 2600) % 150 != 0)
		return attrKitchen;

	//只能对开
	if (abs(m_windowDir - m_doorDir) != 2)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(kaiJian, jinShen, _T("对开"), _T("L"), (m_bHasAirOut == FALSE));
}

vector<AttrKitchen*> CKitchenDlg::FilterKI()
{
	vector<AttrKitchen*> attrKitchen;
	int kaiJian = int(m_rect.GetWidth() + 0.5);
	int jinShen = int(m_rect.GetHeight() + 0.5);
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	//限定开间与进深的范围
	if (kaiJian != 1700)
		return attrKitchen;
	if (jinShen < 3050 || jinShen > 4100)
		return attrKitchen;

	//以150递增尺寸
	if ((jinShen - 3050) % 150 != 0)
		return attrKitchen;

	//只能对开
	if (abs(m_windowDir - m_doorDir) != 2)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(kaiJian, jinShen, _T("对开"), _T("I"), (m_bHasAirOut == FALSE));
}
