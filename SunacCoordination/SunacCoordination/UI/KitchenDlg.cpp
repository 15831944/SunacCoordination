// UI\KitchenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KitchenDlg.h"
#include "../WebIO/WebIO.h"
#include "../Object/RCKitchen.h"
#include "../KitchenCom.h"
#include "afxdialogex.h"
// CKitchenDlg �Ի���

IMPLEMENT_DYNAMIC(CKitchenDlg, CAcUiDialog)

CKitchenDlg::CKitchenDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CKitchenDlg::IDD, pParent)
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
	DDX_Check(pDX, IDC_CHECK_AUTOINDEX, m_bAutoIndex);
	DDX_Control(pDX, IDC_EDIT_KITCHENNUMBER, m_number);
	DDX_Control(pDX, IDC_CHECK_IMAGE, m_isMirror);
	DDX_Control(pDX, IDC_CHECK_AIROUT, m_noAirOut);
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

	return TRUE;
}


void CKitchenDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAcUiDialog::OnOK();
}

void CKitchenDlg::OnBnClickedButtonInsert()
{
	vector<CCellID> cells = m_preKitchen.GetSelectedCells();
	if (cells.empty())
	{
		acutPrintf(_T("û��ѡ��ԭ�ͣ�������ѡ�����˫��ԭ��\n"));
		return;
	}
	int nSel = cells[0].row;
	
	ShowWindow(FALSE);

	double kaiJian = m_rect.GetWidth();
	double jinShen = m_rect.GetHeight();
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	AcGePoint3d origin = TY_GetPoint();
	acedPostCommandPrompt();

	RCKitchen oneKitchen;
	
	AcDbObjectId id = oneKitchen.Insert(m_allKitchens[nSel]->m_filePathName, origin, 0, L"0", 256);
	oneKitchen.InitParameters();
	oneKitchen.SetParameter(L"����", jinShen);
	oneKitchen.SetParameter(L"����", kaiJian);
	oneKitchen.RunParameters();

	CString basin = TYUI_GetComboBoxText(m_basinType);
	CString fridge = TYUI_GetComboBoxText(m_fridgeType);
	CString bench = TYUI_GetComboBoxText(m_benchWidth);

	Kitchen_SelectShuiPen(id, basin);
	Kitchen_SelectBingXiang(id, fridge);
	Kitchen_SelectZaoTai(id, bench);

	if (m_allKitchens[nSel]->m_kitchenType == _T("Uq"))
	{
		Kitchen_KUq_DuiKai_SetDoorPos(id, kaiJian);
		Kitchen_KUq_DuiKai_SetZaoTaiPos(id, jinShen, bench);
		Kitchen_KUq_DuiKai_SetShuiPenPos(id, kaiJian);
	}
	else if (m_allKitchens[nSel]->m_kitchenType == _T("Uqc"))
	{
		Kitchen_KUq_ChuiZhiKai_SetDoorPos(id, kaiJian);
		Kitchen_KUq_ChuiZhiKai_SetZaoTaiPos(id, kaiJian);
		Kitchen_KUq_ChuiZhiKai_SetShuiPenPos(id, jinShen);
	}
	else if (m_allKitchens[nSel]->m_kitchenType == _T("Us"))
	{
		Kitchen_KUs_DuiKai_SetShuiPenPos(id, kaiJian);
		Kitchen_KUs_DuiKai_SetZaoTaiPos(id, jinShen);
	}
	else if (m_allKitchens[nSel]->m_kitchenType == _T("L"))
	{
		Kitchen_KL_DuiKai_SetShuiPenPos(id, kaiJian);
		Kitchen_KL_DuiKai_SetZaoTaiPos(id, jinShen, bench);
	}
	else
	{
		Kitchen_KI_DuiKai_SetShuiPenPos(id, jinShen, basin);
		Kitchen_KI_DuiKai_SetZaoTaiPos(id, bench);
	}

	if (m_isMirror.GetCheck())
		TYCOM_MirrorOneObject(oneKitchen.m_id, origin, AcGeVector3d(0,1,0));

	//��UI�����ݼ�¼��ͼ�����չ�ֵ���
	AttrKitchen *pAttribute = new AttrKitchen(*m_allKitchens[nSel]);
	oneKitchen.AddAttribute(pAttribute);
	pAttribute->close();

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
		acutPrintf(_T("��ѡ������Χ��Ч\n"));
		return;
	}
	m_rect = rect;

	//���·�Χ�����ԭ�������б�
	m_preKitchen.ClearAllPreviews();
}


void CKitchenDlg::OnBnClickedButtonDoorDir()//�ŷ���
{
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
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

	DIR temp = GetDir(pt);
	if (m_doorDir == temp) //δ�޸ģ�ֱ������
		return;
	if (m_windowDir == temp)
	{
		acutPrintf(_T("�Ŵ���������ͬ\n"));
		return;
	}
	m_doorDir = temp;
	if (m_windowDir == DIR_UNKNOWN)
		return;
	//���·�������ԭ�������б�
	m_preKitchen.ClearAllPreviews();
	if (abs(m_windowDir - m_doorDir) == 2)
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("�Ŵ�λ�ù�ϵ���Ŵ��Կ�"));
	else
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("�Ŵ�λ�ù�ϵ���Ŵ���ֱ��"));
}

void CKitchenDlg::OnBnClickedButtonWindowDir()//������
{
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
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

	DIR temp = GetDir(pt);
	if (m_windowDir == temp) //δ�޸ģ�ֱ������
		return;
	if (m_doorDir == temp)
	{
		acutPrintf(_T("�Ŵ���������ͬ\n"));
		return;
	}
	m_windowDir = temp;
	if (m_doorDir == DIR_UNKNOWN)
		return;
	//���·�������ԭ�������б�
	m_preKitchen.ClearAllPreviews();
	if (abs(m_windowDir - m_doorDir) == 2)
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("�Ŵ�λ�ù�ϵ���Ŵ��Կ�"));
	else
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("�Ŵ�λ�ù�ϵ���Ŵ���ֱ��"));
}


void CKitchenDlg::OnBnClickedButtonSearch()
{
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("����ѡ�������Χ\n"));
		return;
	}
	if (m_doorDir == DIR_UNKNOWN || m_windowDir == DIR_UNKNOWN)
	{
		acutPrintf(_T("����ѡ���Ŵ�����\n"));
		return;
	}
	CString type = TYUI_GetText(m_kitchenType);
	if (type == _T("U��"))
		m_allKitchens = FilterKU();
	else if (type == _T("L��"))
		m_allKitchens = FilterKL();
	else if (type == _T("I��"))
		m_allKitchens = FilterKI();
	
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
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	if (nSel >= 0 && nSel < m_allKitchens.size())
	{
		CString type = m_allKitchens[nSel]->m_kitchenType;
		CString pos = m_allKitchens[nSel]->m_windowDoorPos;

		TYUI_SetText(m_number, m_allKitchens[nSel]->m_yxid);
		if (type == _T("Uq") && pos == _T("�Կ�"))
		{
			if (kaiJian < 2900)
				TYUI_InitComboBox(m_basinType, _T("����600;����800"), _T("����600"));
			else
				TYUI_InitComboBox(m_basinType, _T("˫��900;˫��1000;˫��1200"), _T("˫��900"));

			TYUI_InitComboBox(m_fridgeType, _T("�Կ���800;�Կ���1000"), _T("�Կ���800"));
			TYUI_InitComboBox(m_benchWidth, _T("800;900"), _T("800"));
		}
		else if (type == _T("Uq"))
		{
			if (jinShen < 2150)
				TYUI_InitComboBox(m_basinType, _T("����600;����800"), _T("����600"));
			else
				TYUI_InitComboBox(m_basinType, _T("˫��900;˫��1000;˫��1200"), _T("˫��900"));

			TYUI_InitComboBox(m_fridgeType, _T("�Կ���800;�Կ���1000"), _T("�Կ���800"));
			TYUI_InitComboBox(m_benchWidth, _T("800;900"), _T("800"));
		}
		else if (type == _T("Us"))
		{
			TYUI_InitComboBox(m_basinType, _T("����600;����800;˫��900;˫��1000;˫��1200"), _T("����600"));
			TYUI_InitComboBox(m_fridgeType, _T("�Կ���800;�Կ���1000"), _T("�Կ���800"));
			TYUI_InitComboBox(m_benchWidth, _T("800;900"), _T("800"));
		}
		else if (type == _T("L"))
		{
			TYUI_InitComboBox(m_basinType, _T("����600;����800;˫��900;˫��1000;˫��1200"), _T("����600"));

			if (jinShen < 3200)
				TYUI_InitComboBox(m_fridgeType, _T("������700"), _T("������700"));
			else
				TYUI_InitComboBox(m_fridgeType, _T("�Կ���800;�Կ���1000"), _T("�Կ���800"));

			TYUI_InitComboBox(m_benchWidth, _T("800;900"), _T("800"));
		}
		else if (type == _T("I"))
		{
			if (jinShen < 3350)
				TYUI_InitComboBox(m_basinType, _T("����600;����800"), _T("����600"));
			else
				TYUI_InitComboBox(m_basinType, _T("˫��900;˫��1000;˫��1200"), _T("˫��900"));

			TYUI_InitComboBox(m_fridgeType, _T("������700;�Կ���800;�Կ���1000"), _T("������700"));

			if (jinShen <= 3350)
				TYUI_InitComboBox(m_benchWidth, _T("800"), _T("800"));
			else
				TYUI_InitComboBox(m_benchWidth, _T("900"), _T("900"));
		}
	}
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

	if ((kaiJian > jinShen) && (abs(m_windowDir - m_doorDir) == 2))
		return FilterKUq();
	else if (kaiJian > jinShen)
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

	//�޶����������ķ�Χ
	if (kaiJian < 2450 || kaiJian > 3500)
		return attrKitchen;
	if (jinShen < 1700 || jinShen > 2600)
		return attrKitchen;
	if (kaiJian <= jinShen)
		return attrKitchen;

	//��֧��2450*1700
	if (kaiJian == 2450 && jinShen == 1700)
		return attrKitchen;

	//��150�����ߴ�
	if ((kaiJian - 2450) % 150 != 0)
		return attrKitchen;
	if ((jinShen - 1700) % 150 != 0)
		return attrKitchen;

	angle = (m_doorDir - DIR_BOTTOM) * 90;
	return WebIO::GetInstance()->GetKitchens(kaiJian, jinShen, _T("�Կ�"), _T("Uq"), (m_noAirOut.GetCheck() == 0));
}

vector<AttrKitchen*> CKitchenDlg::FilterKUqc()
{
	vector<AttrKitchen*> attrKitchen;
	int kaiJian = int(m_rect.GetWidth() + 0.5);
	int jinShen = int(m_rect.GetHeight() + 0.5);
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	//�޶����������ķ�Χ
	if (kaiJian < 2450 || kaiJian > 3500)
		return attrKitchen;
	if (jinShen < 1700 || jinShen > 2450)
		return attrKitchen;
	if (kaiJian <= jinShen)
		return attrKitchen;

	//��֧��2450*1700
	if (kaiJian == 2450 && jinShen == 1700)
		return attrKitchen;

	//��150�����ߴ�
	if ((kaiJian - 2450) % 150 != 0)
		return attrKitchen;
	if ((jinShen - 1700) % 150 != 0)
		return attrKitchen;

	angle = (m_doorDir - DIR_BOTTOM) * 90;
	return WebIO::GetInstance()->GetKitchens(kaiJian, jinShen, _T("��ֱ��"), _T("Uq"), (m_noAirOut.GetCheck() == 0));
}

vector<AttrKitchen*> CKitchenDlg::FilterKUs()
{
	vector<AttrKitchen*> attrKitchen;
	int kaiJian = int(m_rect.GetWidth() + 0.5);
	int jinShen = int(m_rect.GetHeight() + 0.5);
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	//�޶����������ķ�Χ
	if (kaiJian < 2300 || kaiJian > 2450)
		return attrKitchen;
	if (jinShen < 2450 || jinShen > 3200)
		return attrKitchen;
	if (kaiJian > jinShen)
		return attrKitchen;

	//��150�����ߴ�
	if ((kaiJian - 2300) % 150 != 0)
		return attrKitchen;
	if ((jinShen - 2450) % 150 != 0)
		return attrKitchen;

	//ֻ�ܶԿ�
	if (abs(m_windowDir - m_doorDir) != 2)
		return attrKitchen;

	angle = (m_doorDir - DIR_BOTTOM) * 90;
	return WebIO::GetInstance()->GetKitchens(kaiJian, jinShen, _T("�Կ�"), _T("Us"), (m_noAirOut.GetCheck() == 0));
}

vector<AttrKitchen*> CKitchenDlg::FilterKL()
{
	vector<AttrKitchen*> attrKitchen;
	int kaiJian = int(m_rect.GetWidth() + 0.5);
	int jinShen = int(m_rect.GetHeight() + 0.5);
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	//�޶����������ķ�Χ
	if (kaiJian < 1700 || kaiJian > 2000)
		return attrKitchen;
	if (jinShen < 2600 || jinShen > 3950)
		return attrKitchen;

	//��150�����ߴ�
	if ((kaiJian - 1700) % 150 != 0)
		return attrKitchen;
	if ((jinShen - 2600) % 150 != 0)
		return attrKitchen;

	//ֻ�ܶԿ�
	if (abs(m_windowDir - m_doorDir) != 2)
		return attrKitchen;

	angle = (m_doorDir - DIR_LEFT) * 90;
	return WebIO::GetInstance()->GetKitchens(kaiJian, jinShen, _T("�Կ�"), _T("L"), (m_noAirOut.GetCheck() == 0));
}

vector<AttrKitchen*> CKitchenDlg::FilterKI()
{
	vector<AttrKitchen*> attrKitchen;
	int kaiJian = int(m_rect.GetWidth() + 0.5);
	int jinShen = int(m_rect.GetHeight() + 0.5);
	if (m_doorDir == DIR_LEFT || m_doorDir == DIR_RIGHT)
		swap(kaiJian, jinShen);

	//�޶����������ķ�Χ
	if (kaiJian != 1700)
		return attrKitchen;
	if (jinShen < 3050 || jinShen > 4100)
		return attrKitchen;

	//��150�����ߴ�
	if ((jinShen - 3050) % 150 != 0)
		return attrKitchen;

	//ֻ�ܶԿ�
	if (abs(m_windowDir - m_doorDir) != 2)
		return attrKitchen;

	angle = (m_doorDir - DIR_LEFT) * 90;
	return WebIO::GetInstance()->GetKitchens(kaiJian, jinShen, _T("�Կ�"), _T("I"), (m_noAirOut.GetCheck() == 0));
}
