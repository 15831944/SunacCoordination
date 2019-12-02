// UI\KitchenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KitchenDlg.h"
#include "../WebIO/WebIO.h"
#include "../Object/Kitchen/KitchenAutoName.h"
#include "../Common/ComFun_ACad.h"

// CKitchenDlg �Ի���

CKitchenDlg* g_kitchenDlg = NULL;

void OpenKitchenDlg(AcDbBlockReference* pCurEdit /*= NULL*/)
{
	if (g_kitchenDlg == NULL)
	{
		g_kitchenDlg = new CKitchenDlg(acedGetAcadFrame());
		g_kitchenDlg->Create(IDD_DIALOG_KITCHEN);
	}
	g_kitchenDlg->SetEditMode(pCurEdit);
	g_kitchenDlg->ShowWindow(SW_SHOW);
}

BOOL CloseKitchenDlg()
{
	if (g_kitchenDlg == NULL)
		return TRUE;
	BOOL ret = g_kitchenDlg->DestroyWindow();
	if (ret)
	{
		delete g_kitchenDlg;
		g_kitchenDlg = NULL;
	}
	return ret;
}

IMPLEMENT_DYNAMIC(CKitchenDlg, CAcUiDialog)

CKitchenDlg::CKitchenDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CKitchenDlg::IDD, pParent)
	, m_bAutoIndex(FALSE)
	, m_isStd(0)
{
	m_rect.SetLB(AcGePoint3d(0, 0, 0));
	m_rect.SetRT(AcGePoint3d(0, 0, 0));
	m_doorDir = E_DIR_UNKNOWN;
	m_windowDir = E_DIR_UNKNOWN;
	m_pKitchGen = NULL;
	m_pCurEdit = NULL;
	lockUpdate = false;
}

CKitchenDlg::~CKitchenDlg()
{
	if (m_pKitchGen != NULL)
	{
		delete m_pKitchGen;
	}
}

LRESULT CKitchenDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

BOOL CKitchenDlg::PreTranslateMessage(MSG *pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
		return TRUE;
	return CAcUiDialog::PreTranslateMessage(pMsg);
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
	DDX_Control(pDX, IDC_EDIT_OFFSETX, m_offsetX);
	DDX_Control(pDX, IDC_EDIT_OFFSETY, m_offsetY);
	DDX_Control(pDX, IDC_EDIT_X, m_customX);
	DDX_Control(pDX, IDC_EDIT_Y, m_customY);
	DDX_Radio(pDX, IDC_RADIO_STANDARD, m_isStd);
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
	ON_BN_CLICKED(IDC_CHECK_AIROUT, &CKitchenDlg::OnBnClickedNoAirout)
	ON_CBN_SELCHANGE(IDC_COMBO_KITCHENTYPE, &CKitchenDlg::ClearPreviews)
	ON_CBN_SELCHANGE(IDC_COMBO_BASINTYPE, &CKitchenDlg::UpdateAttribute)
	ON_CBN_SELCHANGE(IDC_COMBO_FRIDGETYPE, &CKitchenDlg::UpdateAttribute)
	ON_CBN_SELCHANGE(IDC_COMBO_BENCHWIDTH, &CKitchenDlg::UpdateAttribute)
	ON_CBN_SELCHANGE(IDC_COMBO_FLOORRANGE, &CKitchenDlg::UpdateAttribute)
	ON_BN_CLICKED(IDC_CHECK_IMAGE, &CKitchenDlg::UpdateAttribute)
	ON_BN_CLICKED(IDC_RADIO_STANDARD, &CKitchenDlg::UpdateAttribute)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, &CKitchenDlg::UpdateAttribute)
	ON_EN_CHANGE(IDC_EDIT_OFFSETX, &CKitchenDlg::UpdateAttribute)
	ON_EN_CHANGE(IDC_EDIT_OFFSETY, &CKitchenDlg::UpdateAttribute)
	ON_EN_CHANGE(IDC_EDIT_X, &CKitchenDlg::UpdateAttribute)
	ON_EN_CHANGE(IDC_EDIT_Y, &CKitchenDlg::UpdateAttribute)
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
		AfxMessageBox(_T("��ѡ��ԭ��\n"));
		return;
	}

	CString errMsg;
	if (!CheckValid(errMsg))
	{
		AfxMessageBox(errMsg);
		return;
	}


	CString newInstanceCode = TYUI_GetText(m_number);
	if (!CKitchenAutoName::GetInstance()->IsNameValid(*m_pKitchGen->GetKitchenAtt(), newInstanceCode))
	{
		AfxMessageBox(L"�˱���ѱ�ռ��");
		return;
	}

	UpdateAttribute();

	//�������Ϸ���
	if (!m_pKitchGen->CheckParameter(errMsg))
	{
		AfxMessageBox(errMsg);
		return;
	}

	if (!m_bAutoIndex)
	{
		m_pKitchGen->GetKitchenAtt()->SetInstanceCode(newInstanceCode);
		CKitchenAutoName::GetInstance()->RenameKitchen(*m_pKitchGen->GetKitchenAtt());
	}

	AcGePoint3d origin = m_rect.GetLB();

	//����
	if (m_pCurEdit != NULL)
	{
		JHCOM_DeleteCadObject(m_pCurEdit->objectId());
		m_pCurEdit = NULL;
	}
	m_pKitchGen->GenKitchen(origin, m_angle);
	CKitchenAutoName::GetInstance()->AddKitchenType(*m_pKitchGen->GetKitchenAtt());

	//ShowWindow(TRUE);
	OnOK();
}


void CKitchenDlg::OnBnClickedButtonRange()
{
	ShowWindow(false);
	TYRect rect;
	bool bSuc = TY_GetOneRect(rect);
	if (bSuc == false)
	{
		ShowWindow(SW_SHOW);
		return;
	}

	if (IsKitchRectValid(rect)==false)
	{
		AfxMessageBox(_T("��ѡ������Χ��Ч\n"));
		ShowWindow(true);
		return;
	}

	m_rect = rect;

	if (m_doorDir == E_DIR_UNKNOWN)
	{
		ads_point pt;
		acedInitGet(32,NULL);
		if(acedGetPoint(NULL,L"\nѡ���ŵ�λ��\n",pt)!=RTNORM) //��һ�ǵ�ѡ��
		{
			ShowWindow(true);
			return;
		}
		m_doorDir = GetDir(pt);
	}

	if (m_windowDir == E_DIR_UNKNOWN)
	{
		do
		{
			ads_point pt;
			acedInitGet(32,NULL);
			if(acedGetPoint(NULL,L"\nѡ�񴰵�λ��\n",pt)!=RTNORM) //��һ�ǵ�ѡ��
			{
				ShowWindow(true);
				return;
			}
			m_windowDir = GetDir(pt);
			if (m_doorDir == m_windowDir)
				AfxMessageBox(_T("�Ŵ���������ͬ\n"));
		}
		while (m_doorDir == m_windowDir);

		ShowInfo();
	}

	ShowWindow(true);
	ShowInfo();
	ClearPreviews();
}
void CKitchenDlg::ShowInfo()
{
	CString sInfo;
	if (IsKitchRectValid(m_rect))
		sInfo.Format(_T("������Ϣ��%.0lf x %.0lf"), m_rect.GetWidth(), m_rect.GetHeight());
	else
		sInfo = L"������Ϣ��δ֪�ߴ�";

	if ((m_windowDir != E_DIR_UNKNOWN) && (m_doorDir != E_DIR_UNKNOWN))
	{
		if ((abs(m_windowDir - m_doorDir) % 2) == 0)
			sInfo += L"���Ŵ��Կ�";
		else
			sInfo += L"���Ŵ���ֱ��";
	}

	GetDlgItem(IDC_STATIC_DIR)->SetWindowText(sInfo);
}

bool CKitchenDlg::IsKitchRectValid(TYRect rect)
{
	if (rect.GetWidth() < 1000 || rect.GetHeight() < 1000)
	{
		return false;
	}

	if (rect.GetWidth() > 10000 || rect.GetHeight() > 10000)
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

	ShowInfo();
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

	ShowInfo();
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

	m_allKitchens = WebIO::GetInstance()->GetKitchens(kitchenType, width, height, m_doorDir, m_windowDir, m_noAirOut.GetCheck() == FALSE);
	
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
		str.Format(_T("ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ����Ҫ��1.5\n��̬���ͣ�%s\n���÷�Χ������"), m_allKitchens[i].GetPrototypeCode(), m_rect.GetWidth() * m_rect.GetHeight() / 1E6, m_allKitchens[i].m_isDynamic ? _T("��̬") : _T("��̬"));
		CString dwgPath = TY_GetPrototypeFilePath() + m_allKitchens[i].GetFileName();
		CString pngPath = TY_GetPrototypeImagePath_Local() + m_allKitchens[i].GetFileName();
		pngPath.Replace(L".dwg", L".png");
		if (PathFileExists(pngPath))
			m_preKitchen.AddPreview(i, 0, pngPath, str);
		else
			m_preKitchen.AddPreview(i, 0, dwgPath, str);
	}

	m_preKitchen.SelectPreview(0, 0);
}

void CKitchenDlg::OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW* pView = (NM_GRIDVIEW*)pNMHDR;
	int nSel = pView->iRow;

	if (nSel < 0 || nSel >= (int)m_allKitchens.size())
	{
		EnableSetProperty(false);
		return;
	}

	if (m_pKitchGen != NULL)
	{
		delete m_pKitchGen;
	}

	m_pKitchGen = CKitchMrg::CreateKitchGenByKitchType(&m_allKitchens[nSel]);
	if (m_pKitchGen==NULL)
	{
		AfxMessageBox(_T("ԭ���޷�����KitchGen"));
		EnableSetProperty(false);
		return;
	}

	AttrKitchen* pCurSelKitchen = m_pKitchGen->GetKitchenAtt();
	CKitchenBathroomProp* pCurSelPrototype = &pCurSelKitchen->m_prop;

	lockUpdate = true;
	bool bNeedMirror;
	pCurSelPrototype->GetRotateAngle(m_doorDir, m_windowDir, m_angle, bNeedMirror);
	pCurSelKitchen->m_angle = m_angle;
	if (pCurSelPrototype->GetWindowDoorPos() == CHUIZHIKAI)
	{
		TYUI_Disable(m_isMirror);
		pCurSelKitchen->m_isMirror = bNeedMirror;
	}
	else
		TYUI_Enable(m_isMirror);
	m_isMirror.SetCheck(pCurSelKitchen->m_isMirror);

	//����������ѡ��
	EnableSetProperty(true);
	m_pKitchGen->InitKitchenByDefault();
	TYUI_InitComboBox(m_basinType, m_pKitchGen->GetShuipenOptions(), pCurSelKitchen->m_shuiPenType, true);
	TYUI_InitComboBox(m_fridgeType, m_pKitchGen->GetBinxiangOptions(), pCurSelKitchen->m_bingXiangType, true);
	TYUI_InitComboBox(m_benchWidth, m_pKitchGen->GetZhaotaiOptions(), pCurSelKitchen->m_zaoTaiType, true);
	m_isStd = pCurSelKitchen->m_isGuoBiao ? 0 : 1;
	m_floorRange.SetCurSel(int(pCurSelKitchen->m_floorRange));
	TYUI_SetInt(m_offsetX, (int)pCurSelKitchen->m_airVentOffsetX);
	TYUI_SetInt(m_offsetY, (int)pCurSelKitchen->m_airVentOffsetY);
	TYUI_SetInt(m_customX, (int)pCurSelKitchen->m_airVentW);
	TYUI_SetInt(m_customY, (int)pCurSelKitchen->m_airVentH);

	m_bAutoIndex = TRUE;
	TYUI_SetText(m_number, pCurSelKitchen->m_instanceCode);
	
	UpdateData(FALSE);
	lockUpdate = false;
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
		EnableSetAirout(m_noAirOut.GetCheck() == FALSE);

	//�޸����������ú���������
	if (m_allKitchens.size() > 0)
		OnBnClickedButtonSearch();
}

void CKitchenDlg::SetEditMode(AcDbBlockReference* pBlock)
{
	m_pCurEdit = pBlock;
	if (m_pCurEdit == NULL)
		return;

	AcDbObject* pAtt = NULL;
	TY_GetAttributeData(pBlock->objectId(), pAtt);
	AttrKitchen *pKitchen = dynamic_cast<AttrKitchen *>(pAtt);
	if (pKitchen == NULL)
		return;

	AcDbExtents ext;
	pBlock->getGeomExtents(ext);
	AcGePoint3d ptLB = ext.minPoint();

	//��ԭ������Χ���Ŵ�λ��
	m_doorDir = E_DIRECTION((pKitchen->m_angle / 90 + pKitchen->m_prop.m_doorPos) % 4);
	m_windowDir = E_DIRECTION((pKitchen->m_angle / 90 + pKitchen->m_prop.m_windowPos) % 4);
	AcGePoint3d ptRT = ptLB;
	if (m_doorDir == E_DIR_BOTTOM || m_doorDir == E_DIR_TOP)
	{
		ptRT.x += pKitchen->m_width;
		ptRT.y += pKitchen->m_height;
	}
	else
	{
		ptRT.x += pKitchen->m_height;
		ptRT.y += pKitchen->m_width;
	}
	m_rect.SetLB(ptLB);
	m_rect.SetRT(ptRT);

	ShowInfo();
	m_noAirOut.SetCheck(!pKitchen->m_hasPaiQiDao);

	m_allKitchens.clear();
	m_allKitchens.push_back(*pKitchen);

	//////////////////////////////////////////////////////////////////////////
	//3. ��ʾԭ��
	m_preKitchen.ClearAllPreviews();
	m_preKitchen.SetRowCount(1);
	m_preKitchen.SetColumnCount(1);
	m_preKitchen.SetDisplayRows(3);
	m_preKitchen.SetDisplayColumns(1);
	
	CString str;
	str.Format(_T("ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ����Ҫ��1.5\n��̬���ͣ�%s\n���÷�Χ������"), m_allKitchens[0].m_prototypeCode, m_rect.GetWidth() * m_rect.GetHeight() / 1E6, m_allKitchens[0].m_isDynamic ? _T("��̬") : _T("��̬"));
	m_preKitchen.AddPreview(0, 0, TY_GetPrototypeFilePath() + m_allKitchens[0].GetFileName(), str);

	m_preKitchen.SelectPreview(0, 0);

	TYUI_SetText(*GetDlgItem(IDC_BUTTON_INSERTKITCHEN), L"ȷ��");
}

void CKitchenDlg::UpdateAttribute()
{
	if (lockUpdate)
		return;

	UpdateData(TRUE);

	if (m_pKitchGen == NULL)
		return;

	AttrKitchen* pAtt = m_pKitchGen->GetKitchenAtt();

	pAtt->m_shuiPenType = TYUI_GetComboBoxText(m_basinType);
	pAtt->m_bingXiangType = TYUI_GetComboBoxText(m_fridgeType);
	pAtt->m_zaoTaiType = TYUI_GetComboBoxText(m_benchWidth);

	pAtt->m_isGuoBiao = (m_isStd == 0);
	pAtt->m_floorRange = (E_KITCHEN_FLOOR_RANGE)m_floorRange.GetCurSel();
	pAtt->m_airVentOffsetX = TYUI_GetInt(m_offsetX);
	pAtt->m_airVentOffsetY = TYUI_GetInt(m_offsetY);
	pAtt->m_airVentW = TYUI_GetInt(m_customX);
	pAtt->m_airVentH = TYUI_GetInt(m_customY);

	pAtt->m_isMirror = m_isMirror.GetCheck() ? true : false;

	if (m_bAutoIndex)
	{
		pAtt->m_instanceCode = CKitchenAutoName::GetInstance()->GetKitchenName(*pAtt);
		TYUI_SetText(m_number, pAtt->m_instanceCode);
	}
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
	lockUpdate = true;
	const vCString& kitchenTypes = WebIO::GetInstance()->GetConfigDict()->Kitchen_GetTypes();
	TYUI_InitComboBox(m_kitchenType, kitchenTypes, kitchenTypes.empty() ? _T("") : kitchenTypes[0]);
	m_bAutoIndex = TRUE;
	m_number.SetReadOnly(TRUE);
	m_floorRange.SetCurSel(0);
	TYUI_SetInt(m_offsetX, 0);
	TYUI_SetInt(m_offsetY, 0);
	TYUI_SetInt(m_customX, 450);
	TYUI_SetInt(m_customY, 350);
	UpdateData(FALSE);
	lockUpdate = false;
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
	EnableSetAirout(bEnable && (m_noAirOut.GetCheck() == FALSE));
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
	if (m_noAirOut.GetCheck() == TRUE)
		return true;
	double ventX, ventY;
	int nSel = m_floorRange.GetCurSel();
	if (m_isStd == 0)
	{
		ventX = TYUI_GetDouble(m_offsetX) + c_kitchenAirVentSize[nSel];
		ventY = TYUI_GetDouble(m_offsetY) + c_kitchenAirVentSize[nSel];
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

