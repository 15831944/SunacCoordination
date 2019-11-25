// UI\WindowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WindowDlg.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/WindowDoor/RCWindow.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "../WebIO/WebIO.h"
#include "../GlobalSetting.h"
#include "../Object/WindowDoor/WindowAutoName.h"
#include "../Common/ComFun_ACad.h"
#include "DlgWindowAirCalc.h"

// CWindowDlg �Ի���

IMPLEMENT_DYNAMIC(CWindowDlg, CAcUiDialog)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowDlg::IDD, pParent)
	, m_radioDoor(1)
	, m_radioYes(0)
	, m_autoIndex(FALSE)
	, m_nWidth(0)
	, m_nHeight(0)
{
	m_isMoldless = true;
	m_pCurEdit = NULL;
	m_nWidth = 0;
	m_nHeight = 0;

	m_selectRect = TYRect(AcGePoint3d::kOrigin, AcGePoint3d::kOrigin);

	m_bHasInsert = false;
}

CWindowDlg::~CWindowDlg()
{
}

INT_PTR CWindowDlg::DoModal()
{
	m_isMoldless = false;
	return CAcUiDialog::DoModal();
}

LRESULT CWindowDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

void CWindowDlg::OnOK()
{
	CAcUiDialog::OnOK();
	if (m_isMoldless)
		DestroyWindow();
}

void CWindowDlg::OnCancel()
{
	CAcUiDialog::OnCancel();
	if (m_isMoldless)
	    DestroyWindow();
}

void CWindowDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();
	if (m_isMoldless)
	{
		delete this;
		g_windowDlg = NULL;
	}
}

BOOL CWindowDlg::PreTranslateMessage(MSG *pMsg)
{
	if ((pMsg->message == WM_KEYDOWN))
	{
		bool isCtrlKeyDown = (GetKeyState(VK_CONTROL) & 0x80) != 0;
		const UINT key = (UINT)(pMsg->wParam);
		if (key == VK_RETURN)
		{
			return TRUE;
		}
		else if (key == 'T' && isCtrlKeyDown)
		{
			InsertAllWindows_Test();
			return TRUE;
		}
		else if (key == VK_SPACE && m_bHasInsert) //����Ѿ��в��룬��ɰ��ո���������
		{
			OnBnClickedButtonInsert();
			return TRUE;
		}
	}
	return CAcUiDialog::PreTranslateMessage(pMsg);
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_WINDOW, m_preWindow);
	DDX_Control(pDX, IDC_COMBO_AREATYPE, m_comboAreaType);
	DDX_Control(pDX, IDC_COMBO_OPENTYPE, m_comboOpenType);
	DDX_Control(pDX, IDC_COMBO_OPENAMOUNT, m_comboOpenAmount);
	DDX_Control(pDX, IDC_EDIT_VENTILATION, m_editVentilation);
	DDX_Control(pDX, IDC_COMBO_OPENWIDTH, m_comboW1);
	DDX_Control(pDX, IDC_COMBO_FIXEDVALUE, m_comboH2);
	DDX_Control(pDX, IDC_COMBO_DISTANCE, m_comboOutWallDistance);
	DDX_Control(pDX, IDC_EDIT_NUMBER, m_number);
	DDX_Radio(pDX, IDC_RADIO_DOOR, m_radioDoor);
	DDX_Radio(pDX, IDC_RADIO_YES, m_radioYes);
	//DDX_Control(pDX, IDC_EDIT_AREA, m_area);
	//DDX_Control(pDX, IDC_COMBO_RATE, m_rate);
	DDX_Check(pDX, IDC_CHECK_AUTOINDEX, m_autoIndex);
	DDX_Control(pDX, IDC_COMBO_VIEWDIR, m_comboViewDir);
	DDX_Control(pDX, IDC_CHECK_IMAGE, m_isMirror);
	DDX_Control(pDX, IDC_COMBO_W3, m_comboW3);
	DDX_Control(pDX, IDC_COMBO_H3, m_comboH3);
	DDX_Control(pDX, IDC_COMBO_DIR, m_comboInsertDir);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 100, 20000);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 100, 5000);
}


BEGIN_MESSAGE_MAP(CWindowDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_INSERTWINDOW, &CWindowDlg::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHWINDOW, &CWindowDlg::OnBnClickedButtonSearchwindow)
	ON_BN_CLICKED(IDC_RADIO_DOOR, &CWindowDlg::OnBnClickedRadioDoor)
	ON_BN_CLICKED(IDC_RADIO_WINDOW, &CWindowDlg::OnBnClickedRadioDoor)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CWindowDlg::OnBnClickedCalculate)
	ON_BN_CLICKED(IDC_CHECK_AUTOINDEX, &CWindowDlg::OnBnClickedAutoIndex)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CWindowDlg::OnBnClickedSelOnDwg)
	ON_BN_CLICKED(IDC_CHECK_IMAGE, &CWindowDlg::OnBnClickedMirror)
	ON_BN_CLICKED(IDC_RADIO_YES, &CWindowDlg::OnBnClickedBayWindow)
	ON_BN_CLICKED(IDC_RADIO_NO, &CWindowDlg::OnBnClickedBayWindow)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_WINDOW, &CWindowDlg::OnSelChangedPreview)
	ON_CBN_SELCHANGE(IDC_COMBO_OPENWIDTH, &CWindowDlg::OnSelChangedW1)
	ON_CBN_SELCHANGE(IDC_COMBO_FIXEDVALUE, &CWindowDlg::OnSelChangedH2)
	ON_CBN_SELCHANGE(IDC_COMBO_DISTANCE, &CWindowDlg::OnSelChangedWallDis)
	ON_CBN_SELCHANGE(IDC_COMBO_VIEWDIR, &CWindowDlg::OnSelChangedView)
	ON_CBN_SELCHANGE(IDC_COMBO_W3, &CWindowDlg::OnSelChangedW3)
	ON_CBN_SELCHANGE(IDC_COMBO_H3, &CWindowDlg::OnSelChangedH3)
END_MESSAGE_MAP()


// CWindowDlg ��Ϣ�������


BOOL CWindowDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preWindow.LoadDefaltSettings();

	LoadDefaultValue();
	UpdateEnable();

	return TRUE;
}
bool CWindowDlg::CheckValueModulo(CComboBox& comboBox, CString p_sType, int p_value) //��������Ƿ���50��ģ��
{
	if (comboBox.IsWindowEnabled()==FALSE)
	{
		return true;
	}
	if (p_value%10!=0)
	{
		CString str;
		str.Format(_T("%sֵ������10�ı���"), p_sType);
		AfxMessageBox(str);
		return false;
	}

	return true;
}
void CWindowDlg::OnBnClickedButtonInsert()
{
	UpdateData();

	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
	{
		AfxMessageBox(L"��ѡ��ԭ��");
		return;
	}

	if (pSelWinAttr->GetTongFengQty(false) + TOL < TYUI_GetDouble(m_editVentilation))
	{
		AfxMessageBox(L"��ԭ�Ͳ�����ͨ����Ҫ��");
		return;
	}

	int W1 = TYUI_GetInt(m_comboW1);
	int H2 = TYUI_GetInt(m_comboH2);
	int W3 = TYUI_GetInt(m_comboW3);
	int H3 = TYUI_GetInt(m_comboH3);
	////�������ֵ
	//if (CheckValueModulo(m_comboW1, _T("�����ȿ��"), W1) == false ||
	//	CheckValueModulo(m_comboH2, _T("�¹̶��߶�"), H2) == false ||
	//	CheckValueModulo(m_comboW3, _T("ת�ǿ��"), W3) == false ||
	//	CheckValueModulo(m_comboH3, _T("����ǽ�߶�"), H3) == false)
	//{
	//	return;
	//}

	//�ڼ��ԭ�ͱ��ǰ��������ֵ
	pSelWinAttr->SetW1(W1);
	pSelWinAttr->SetH2(H2);
	pSelWinAttr->SetW3(W3);
	pSelWinAttr->SetH3(H3);

	//�Զ�����¸���ԭ�ͱ��
	UpdateInstanceCode();

	CString sNumber = TYUI_GetText(m_number);
	if (!m_autoIndex && !CWindowAutoName::GetInstance()->IsNameValid(*pSelWinAttr,sNumber))
	{
		AfxMessageBox(L"�˱���ѱ�ռ��,������������");
		return;
	}

	pSelWinAttr->SetInstanceCode(sNumber);
	CWindowAutoName::GetInstance()->RenameWindow(*pSelWinAttr);

	//////////////////////////////////////////////////////////////////////////
	const eViewDir viewDir = (eViewDir)m_comboViewDir.GetCurSel();
	E_DIRECTION winDir = E_DIR_BOTTOM;
	if (viewDir == E_VIEW_TOP)
	{
		CString sDir = TYUI_GetComboBoxText(m_comboInsertDir);
		winDir = String2Direction(sDir);
	}

	ShowWindow(FALSE);

	//ѡ������
	AcGePoint3d origin;
	if (m_pCurEdit == NULL)
	{
		bool bSuc = TY_GetPoint(origin, L"��ѡ������(����ǽλ��)"); 
		acedPostCommandPrompt();
		if (bSuc == false)
		{
			ShowWindow(SW_SHOW);
			return;
		}
	}
	else
	{
		AcDbExtents ext;
		m_pCurEdit->getGeomExtents(ext);
		origin = ext.minPoint();
		JHCOM_DeleteCadObject(m_pCurEdit->objectId());
	}

	//////////////////////////////////////////////////////////////////////////
	AcDbObjectId idOut = GenerateWindow(*pSelWinAttr, origin, viewDir, winDir, true, L"0");
	if (idOut==AcDbObjectId::kNull)
	{
		assert(false);
		ShowWindow(TRUE);
		return;
	}
	

	CWindowAutoName::GetInstance()->AddWindowType(*pSelWinAttr);

	ShowWindow(TRUE);

	m_bHasInsert = true;
	//OnOK();
}

void CWindowDlg::OnBnClickedButtonSearchwindow()
{
	UpdateData();

	CString openType = TYUI_GetComboBoxText(m_comboOpenType);
	int openNum = _ttoi(TYUI_GetComboBoxText(m_comboOpenAmount));
	CString areaType = TYUI_GetComboBoxText(m_comboAreaType);

	if (m_radioDoor == 0)
		m_allWindows = WebIO::GetInstance()->GetDoors(m_nWidth, m_nHeight, openType, openNum, areaType);
	else
		m_allWindows = WebIO::GetInstance()->GetWindows(m_nWidth, m_nHeight, openType, openNum, areaType);

	m_preWindow.ClearAllPreviews();

	for (UINT i = 0; i < m_allWindows.size(); i++)
	{
		CString dwgPath = TY_GetPrototypeFilePath() + m_allWindows[i].GetFileName();
		if (!PathFileExists(dwgPath))
		{
			acutPrintf(L"\nԭ���ļ�" + m_allWindows[i].GetFileName() + L"δ�ҵ�\n");
			m_allWindows.erase(m_allWindows.begin() + i--);
		}
	}
	if (m_allWindows.empty())
	{
		AfxMessageBox(L"δ�ҵ����������ļ�¼\n");
		return;
	}

	m_preWindow.SetRowCount((int)m_allWindows.size());
	m_preWindow.SetColumnCount(1);
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);

	for (UINT i = 0; i < m_allWindows.size(); i++)
	{
		m_allWindows[i].SetW(m_nWidth);
		m_allWindows[i].SetH(m_nHeight);
		CString str;
		str.Format(L"ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ������%.2lf\n��̬���ͣ�%s\n���÷�Χ������", 
			m_allWindows[i].m_prototypeCode, GetArea(), m_allWindows[i].GetTongFengQty(false), m_allWindows[i].m_isDynamic ? L"��̬" : L"��̬");

		CString dwgPath = TY_GetPrototypeFilePath() + m_allWindows[i].GetFileName();
		CString pngPath = TY_GetPrototypeImagePath_Local() + m_allWindows[i].GetFileName(); //�Ŵ�ԭ������ʹ���ڲ���ͼƬ
		pngPath.Replace(L".dwg", L".png");
		if (PathFileExists(pngPath))
			m_preWindow.AddPreview(i, 0, pngPath, str);
		else
			m_preWindow.AddPreview(i, 0, dwgPath, str);
	}

	m_preWindow.SelectPreview(0, 0);
}

void CWindowDlg::OnBnClickedRadioDoor()
{
	int preVal = m_radioDoor;
	UpdateData(TRUE);

	if (m_radioDoor != preVal)
		UpdateEnable();
}

void CWindowDlg::OnBnClickedCalculate()
{
#if 1
	UpdateData(TRUE);
	CDlgWindowAirCalc dlg;
	if (dlg.DoModal()==IDOK)
	{
		TYUI_SetDouble(m_editVentilation, dlg.m_airQuality);
		UpdateData(FALSE);
	}
#else
	double area = TYUI_GetDouble(m_area);
	if (area <= 0)
	{
		AfxMessageBox(L"��Ч�ķ������");
		return;
	}

	CString sRate = TYUI_GetText(m_rate);
	double rate = _ttof(sRate);
	int pos = sRate.Find(L'/');
	if (pos != -1)
		rate /= _ttof(sRate.Mid(pos + 1));

	TYUI_SetDouble(m_editVentilation, area * rate);
#endif
}

void CWindowDlg::OnBnClickedAutoIndex()
{
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;

	UpdateData(TRUE);
	if (m_autoIndex)
	{
		m_number.SetReadOnly(TRUE);
		CString newName = CWindowAutoName::GetInstance()->GetWindowName(*pSelWinAttr);
		TYUI_SetText(m_number, newName);
	}
	else
		m_number.SetReadOnly(FALSE);
}

void CWindowDlg::OnBnClickedSelOnDwg()
{
	ShowWindow(false);
	TYRect rect;
	bool bSuc = TY_GetOneRect(rect);
	if (bSuc == false)
	{
		ShowWindow(SW_SHOW);
		return;
	}

	ShowWindow(true);
	
	int width = int(rect.GetWidth() + 0.5);
	int height = int(rect.GetHeight() + 0.5);

	//���ƽ��ͼ�¶����ര�Զ������
	if (width<400 && width<height)
	{
		swap(width, height);
	}

	if (width<100 || width>1000000)
	{
		AfxMessageBox(_T("�Ŷ��ߴ����䲻����"));
		return;
	}

	m_selectRect = rect;

	m_nWidth = width;
	if (height>=400 && height<3000) //����ƽ��ͼ�ĸ߶��򲻸ı�ԭ���ĸ߶�ֵ
	{
		m_nHeight = height;
	}

	UpdateData(FALSE);
}

void CWindowDlg::OnBnClickedMirror()
{
	UpdateData(TRUE);
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;
	pSelWinAttr->m_isMirror = (m_isMirror.GetCheck() != FALSE);
	UpdateInstanceCode();
}

void CWindowDlg::OnSelChangedPreview(NMHDR *pNMHDR, LRESULT *pResult)
{
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;

	UpdateDimDataToComboBox(m_comboW1, *pSelWinAttr, L"W1");
	UpdateDimDataToComboBox(m_comboH2, *pSelWinAttr, L"H2");
	UpdateDimDataToComboBox(m_comboW3, *pSelWinAttr, L"W3");

	//UpdateDimDataToComboBox(m_comboH3, *pSelWinAttr, L"H3");
	vint nH3Optins;
	nH3Optins.push_back(600);
	nH3Optins.push_back(900);
	TYUI_InitComboBox(m_comboH3, nH3Optins, (int)(pSelWinAttr->GetHeightUnderWindow()));

	if (pSelWinAttr->m_isDynamic)
	{
		TYUI_Enable(m_isMirror);
		m_isMirror.SetCheck(pSelWinAttr->m_isMirror);
	}
	else
		TYUI_Disable(m_isMirror);

	pSelWinAttr->m_instanceCode = CWindowAutoName::GetInstance()->GetWindowName(*pSelWinAttr);
	TYUI_SetText(m_number, pSelWinAttr->m_instanceCode);
	m_radioYes = (pSelWinAttr->m_isBayWindow ? 0 : 1);
	TYUI_SetInt(m_comboOutWallDistance, int(pSelWinAttr->m_wallDis));

	//������ͼ����
	CString sView = TYUI_GetComboBoxText(m_comboViewDir);
	m_comboViewDir.ResetContent();
	if ((!pSelWinAttr->m_frontViewFile.fileName.IsEmpty()) && PathFileExists(TY_GetPrototypeFilePath() + pSelWinAttr->m_frontViewFile.fileName))
		m_comboViewDir.AddString(L"����");
	if ((!pSelWinAttr->m_topViewFile.fileName.IsEmpty()) && PathFileExists(TY_GetPrototypeFilePath() + pSelWinAttr->m_topViewFile.fileName))
		m_comboViewDir.AddString(L"ƽ��");
	if ((!pSelWinAttr->m_leftViewFile.fileName.IsEmpty()) && PathFileExists(TY_GetPrototypeFilePath() + pSelWinAttr->m_leftViewFile.fileName))
		m_comboViewDir.AddString(L"����");

	//����ԭ������ͼ����
	if (sView.IsEmpty()==FALSE)
	{
		m_comboViewDir.SelectString(0, sView);
	}

	OnSelChangedView();

	UpdateData(FALSE);
}

void CWindowDlg::OnSelChangedW1()
{
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;

	CString sSel = TYUI_GetComboBoxText(m_comboW1);
	pSelWinAttr->SetW1(_ttoi(sSel));
	//���Ĳ���������ʵ����ű仯�������
	UpdateInstanceCode();
	UpdateVent();
}

void CWindowDlg::OnSelChangedH2()
{
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;

	CString sSel = TYUI_GetComboBoxText(m_comboH2);
	pSelWinAttr->SetH2(_ttoi(sSel));
	//���Ĳ���������ʵ����ű仯�������
	UpdateInstanceCode();
	UpdateVent();
}

void CWindowDlg::OnSelChangedW3()
{
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;

	CString sSel = TYUI_GetComboBoxText(m_comboW3);
	pSelWinAttr->SetW3(_ttoi(sSel));
	//���Ĳ���������ʵ����ű仯�������
	UpdateInstanceCode();
	UpdateVent();
}

void CWindowDlg::OnSelChangedH3()
{
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;

	CString sSel = TYUI_GetComboBoxText(m_comboH3);
	pSelWinAttr->SetH3(_ttoi(sSel));

	//���Ĳ���������ʵ����ű仯�������
	UpdateInstanceCode();
	UpdateVent();
}

void CWindowDlg::OnSelChangedView()
{
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;

	CString sView = TYUI_GetComboBoxText(m_comboViewDir);
	if (sView == L"ƽ��")
		pSelWinAttr->m_viewDir = E_VIEW_TOP;
	else if (sView == L"����")
		pSelWinAttr->m_viewDir = E_VIEW_FRONT;
	else if (sView == L"����")
		pSelWinAttr->m_viewDir = E_VIEW_LEFT;

	if (sView == L"ƽ��")
	{
		TYUI_Show(*GetDlgItem(IDC_STATIC_DIR));
		TYUI_Show(m_comboInsertDir);
	}
	else
	{
		TYUI_Hide(*GetDlgItem(IDC_STATIC_DIR));
		TYUI_Hide(m_comboInsertDir);
	}
}

void CWindowDlg::OnBnClickedBayWindow()
{
	UpdateData(TRUE);
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;

	pSelWinAttr->m_isBayWindow = (m_radioYes == 0);
}

void CWindowDlg::OnSelChangedWallDis()
{
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;

	pSelWinAttr->m_wallDis = _ttof(TYUI_GetComboBoxText(m_comboOutWallDistance));
}

void CWindowDlg::UpdateEnable()
{
	//�л�������б��������ʱ����
	m_allWindows.clear();
	m_preWindow.ClearAllPreviews();

	if (m_radioDoor == 0)
	{
		TYUI_Disable(m_comboOpenAmount);
		TYUI_Disable(m_comboW1);
		TYUI_Disable(*GetDlgItem(IDC_RADIO_YES));
		TYUI_Disable(*GetDlgItem(IDC_RADIO_NO));
	}
	else
	{
		TYUI_Enable(m_comboOpenAmount);
		TYUI_Enable(m_comboW1);
		TYUI_Enable(*GetDlgItem(IDC_RADIO_YES));
		TYUI_Enable(*GetDlgItem(IDC_RADIO_NO));
	}
	LoadDefaultValue();
}

void CWindowDlg::UpdateInstanceCode()
{
	if (m_autoIndex)
	{
		AttrWindow* pSelWinAttr = GetSelWindow();
		if (pSelWinAttr == NULL)
			return;

		UpdateData(TRUE);
		pSelWinAttr->m_instanceCode = CWindowAutoName::GetInstance()->GetWindowName(*pSelWinAttr);
		TYUI_SetText(m_number, pSelWinAttr->m_instanceCode);
		UpdateData(FALSE);
	}
}

void CWindowDlg::UpdateVent()
{
	CCellRange selRange = m_preWindow.GetSelectedCellRange();
	int nRow = selRange.GetMinRow();
	CGridCellForPreview* pCell = m_preWindow.GetPreviewCell(nRow, 0);
	if (pCell == NULL)
		return;
	AttrWindow* pSelWinAttr = &m_allWindows[nRow];
	CString str;
	str.Format(L"ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ������%.2lf\n��̬���ͣ�%s\n���÷�Χ������", 
		pSelWinAttr->m_prototypeCode, GetArea(), pSelWinAttr->GetTongFengQty(false), pSelWinAttr->m_isDynamic ? L"��̬" : L"��̬");
	pCell->GetPreview()->SetText(str);
	pCell->GetPreview()->Invalidate();
}

void CWindowDlg::UpdateDimDataToComboBox(CComboBox& comboBox, const AttrWindow& attrWindow, CString code)
{
	if (!attrWindow.m_isDynamic)
		return;
	const CWindowsDimData* pDimData = attrWindow.GetDimData(code);
	if (pDimData == NULL)
		return;

	switch (pDimData->type)
	{
	case SINGLE://�̶�ֵ
	{
					TYUI_Enable(comboBox);
					double dimValue = attrWindow.GetValue(code);
					vdouble options(1, dimValue);
					InitDimComboBoxInt(comboBox, options, dimValue);
					break;
	}
	case MULTI://ֵϵ��
	{
		TYUI_Enable(comboBox);
		InitDimComboBoxInt(comboBox, pDimData->valueOptions, pDimData->value);
		break;
	}
	case SCOPE://��Χ
	case UNLIMIT://����
	{
				   TYUI_Enable(comboBox);
				   double dimValue = attrWindow.GetValue(code, true);
				   vdouble options(1, dimValue);
				   InitDimComboBoxInt(comboBox, options, dimValue);
				   break;
	}
	case CALC://��ʽ
	{
				  TYUI_Disable(comboBox);
				  double dimValue = attrWindow.GetValue(code);
				  vdouble options(1, dimValue);
				  InitDimComboBoxInt(comboBox, options, dimValue);
				  break;
	}
	case NOVALUE: //��
	default:
	{
			   comboBox.ResetContent();
			   TYUI_Disable(comboBox);
			   break;

	}
	}
}

void CWindowDlg::InitDimComboBoxInt(CComboBox& comboBox, vdouble options, double dimValue)
{
	vint nOptins;
	for (UINT i = 0; i < options.size(); i++)
	{
		nOptins.push_back((int)options[i]);
	}

	TYUI_InitComboBox(comboBox, nOptins, (int)dimValue);
}

void CWindowDlg::LoadDefaultValue()
{
	vCString openAmount;
	openAmount.push_back(L"����");
	openAmount.push_back(L"1");
	openAmount.push_back(L"2");
	const vCString& doorTypes = WebIO::GetInstance()->GetConfigDict()->Door_GetTypes();
	const vCString& openTypes = WebIO::GetInstance()->GetConfigDict()->Window_GetOpenTypes();
	const vCString& areaTypes = WebIO::GetInstance()->GetConfigDict()->GetGongNengQus();
	//const vCString& openAmount = WebIO::GetInstance()->GetConfigDict()->Window_GetOpenAmount();
	const vCString& wallDis = WebIO::GetInstance()->GetConfigDict()->Window_GetWallDis();

	m_nWidth = 1500;
	m_nHeight = 1700;

	TYUI_SetText(m_number, L"");
	
	if (m_radioDoor == 0)
		TYUI_InitComboBox(m_comboOpenType, doorTypes, doorTypes.empty() ? L"" : doorTypes[0]);
	else
		TYUI_InitComboBox(m_comboOpenType, openTypes, openTypes.empty()? L"" : openTypes[0]);
	TYUI_InitComboBox(m_comboAreaType, areaTypes, areaTypes.empty() ? L"" : areaTypes[0]);
	TYUI_InitComboBox(m_comboOpenAmount, openAmount, openAmount.empty() ? L"" : openAmount[0]);
	TYUI_InitComboBox(m_comboOutWallDistance, wallDis, wallDis.empty() ? L"" : wallDis[0]);

	//TYUI_SetInt(m_area, 0);
	//const vCString& rate = WebIO::GetInstance()->GetConfigDict()->Window_GetRate();
	//TYUI_InitComboBox(m_rate, rate, rate.empty() ? L"" : rate[0]);

	m_comboViewDir.SetCurSel(0);
	m_comboInsertDir.SetCurSel(0);
	OnSelChangedView();
	m_autoIndex = TRUE;
	m_number.SetReadOnly(TRUE);
	UpdateData(FALSE);
}

AttrWindow* CWindowDlg::GetSelWindow()
{
	CCellRange sels = m_preWindow.GetSelectedCellRange();
	if (sels.Count() == 0)
		return NULL;
	return &m_allWindows[sels.GetMinRow()];
}

void CWindowDlg::SetEditMode(AcDbBlockReference* pBlock)
{
	m_pCurEdit = pBlock;
	if (m_pCurEdit == NULL)
		return;

	AcDbObject* pAtt = NULL;
	TY_GetAttributeData(pBlock->objectId(), pAtt);
	AttrWindow *pWindow = dynamic_cast<AttrWindow *>(pAtt);
	if (pWindow == NULL)
		return;

	m_radioDoor = (pWindow->GetType() == DOOR) ? 0 : 1;
	UpdateEnable();

	//��ԭ�Ŵ��ߴ�
	m_nWidth = (int)pWindow->GetW();
	m_nHeight = (int)pWindow->GetH();


	m_allWindows.clear();
	m_allWindows.push_back(*pWindow);

	//////////////////////////////////////////////////////////////////////////
	//3. ��ʾԭ��
	m_preWindow.ClearAllPreviews();
	m_preWindow.SetRowCount(1);
	m_preWindow.SetColumnCount(1);
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);

	CString str;
	str.Format(L"ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ������%.2lf\n��̬���ͣ�%s\n���÷�Χ������", 
		m_allWindows[0].m_prototypeCode, GetArea(), m_allWindows[0].GetTongFengQty(false), m_allWindows[0].m_isDynamic ? L"��̬" : L"��̬");

	CString dwgPath = TY_GetPrototypeFilePath() + m_allWindows[0].GetFileName();
	CString pngPath = TY_GetPrototypeImagePath_Local() + m_allWindows[0].GetFileName(); //�Ŵ�ԭ������ʹ���ڲ���ͼƬ
	pngPath.Replace(L".dwg", L".png");
	if (PathFileExists(pngPath))
		m_preWindow.AddPreview(0, 0, pngPath, str);
	else
		m_preWindow.AddPreview(0, 0, dwgPath, str);

	m_preWindow.SelectPreview(0, 0);

	TYUI_SetText(*GetDlgItem(IDC_BUTTON_INSERTWINDOW), L"ȷ��");
}

void CWindowDlg::InsertAllWindows_Test()
{
	ShowWindow(SW_HIDE);

	AcGePoint3d origin;
	bool bSuc = TY_GetPoint(origin);
	if (bSuc == false)
	{
		ShowWindow(SW_SHOW);
		return;
	}

	for (UINT i = 0; i < m_allWindows.size(); i++)
	{
		AttrWindow* pSelWinAttr = &m_allWindows[i];
		RCWindow oneWindow;

		AcGePoint3d insertPt;

		insertPt.x = origin.x + (m_nWidth + 100) * (i % 5);
		insertPt.y = origin.y + (m_nHeight + 100) * (i / 5);

		int sel = m_comboViewDir.GetCurSel();
		if (sel == 0)
			oneWindow.Insert(TY_GetPrototypeFilePath() + pSelWinAttr->m_frontViewFile.fileName, insertPt, 0, L"0", 256);
		else if (sel == 1)
		{
			double rotateAngle = 0;
			AcGeVector3d offsetXY(0, 0, 0);
			CString sDir = TYUI_GetComboBoxText(m_comboInsertDir);
			if (sDir == L"��")
			{
				rotateAngle = PI;
				offsetXY.x += m_nWidth;
			}
			if (sDir == L"��")
				rotateAngle = PI / 2;
			if (sDir == L"��")
			{
				rotateAngle = -PI / 2;
				offsetXY.y += m_nWidth;
			}
			oneWindow.Insert(TY_GetPrototypeFilePath() + pSelWinAttr->m_topViewFile.fileName, origin + offsetXY, rotateAngle, L"0", 256);
		}
		else
			oneWindow.Insert(TY_GetPrototypeFilePath() + pSelWinAttr->m_leftViewFile.fileName, insertPt, 0, L"0", 256);

		oneWindow.InitParameters();

		oneWindow.SetParameter(L"H", m_nHeight);
		oneWindow.SetParameter(L"W", m_nWidth);

		oneWindow.RunParameters();

		if (m_isMirror.GetCheck())
		{
			AcGePoint3d basePt(insertPt.x + m_nWidth / 2, 0, 0);
			TYCOM_Mirror(oneWindow.m_id, basePt, AcGeVector3d(0, 1, 0));
		}

		CWindowAutoName::GetInstance()->AddWindowType(*pSelWinAttr);

		//��UI�����ݼ�¼��ͼ�����չ�ֵ���
		AttrWindow * pWindow = new AttrWindow(*pSelWinAttr);
		oneWindow.AddAttribute(pWindow);
		pWindow->close();
	}
	ShowWindow(SW_SHOW);
}

CWindowDlg* g_windowDlg = NULL;

BOOL CloseWindowDlg()
{
	if (g_windowDlg == NULL)
		return TRUE;
	BOOL ret = g_windowDlg->DestroyWindow();
	if (ret)
		g_windowDlg = NULL;
	return ret;
}
