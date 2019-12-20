// UI\WindowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WindowDlg.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/WindowDoor/RCWindow.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "../Object/WindowDoor/WindowGen.h"
#include "../WebIO/WebIO.h"
#include "../GlobalSetting.h"
#include "../Object/WindowDoor/WindowAutoName.h"
#include "../Common/ComFun_ACad.h"
#include "DlgWindowAirCalc.h"
#include "..\Src\DocumentData.h"

// CWindowDlg �Ի���

IMPLEMENT_DYNAMIC(CWindowDlg, CAcUiDialog)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowDlg::IDD, pParent)
	, m_radioDoorWindow(1)
	, m_radioBayWindow(0)
	, m_bAutoNumber(TRUE)
	, m_nWidth(1500)
	, m_nHeight(1700)
	, m_nThickness(200)
{
	m_bEditMode = false;
	m_pCurEditWinRef = NULL;
	
	m_selectRect = TYRect(AcGePoint3d::kOrigin, AcGePoint3d::kOrigin);

	m_bHasInsert = false;
}

CWindowDlg::~CWindowDlg()
{
}

void CWindowDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();

	//// �ͷŷ�ģ̬�Ի�����ڴ�
	//delete this;
	//if (g_connectorDlg != NULL)
	//{
	//	g_connectorDlg = NULL;
	//}
}

void CWindowDlg::OnClose()
{
	CAcUiDialog::OnClose();

	// ���ٶԻ���
	//DestroyWindow();
}

LRESULT CWindowDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
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
	DDX_Control(pDX, IDC_EDIT_NUMBER, m_editWinNumber);
	DDX_Radio(pDX, IDC_RADIO_DOOR, m_radioDoorWindow);
	DDX_Radio(pDX, IDC_RADIO_RAYWINDOW, m_radioBayWindow);
	DDX_Check(pDX, IDC_CHECK_AUTOINDEX, m_bAutoNumber);
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
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_INSERTWINDOW, &CWindowDlg::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHWINDOW, &CWindowDlg::OnBnClickedButtonSearchwindow)
	ON_BN_CLICKED(IDC_RADIO_DOOR, &CWindowDlg::OnBnClickedRadioWindowDoor)
	ON_BN_CLICKED(IDC_RADIO_WINDOW, &CWindowDlg::OnBnClickedRadioWindowDoor)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CWindowDlg::OnBnClickedCalculate)
	ON_BN_CLICKED(IDC_CHECK_AUTOINDEX, &CWindowDlg::OnBnClickedAutoIndex)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CWindowDlg::OnBnClickedSelOnDwg)
	ON_BN_CLICKED(IDC_CHECK_IMAGE, &CWindowDlg::OnBnClickedMirror)
	ON_BN_CLICKED(IDC_RADIO_RAYWINDOW, &CWindowDlg::OnBnClickedBayWindow)
	ON_BN_CLICKED(IDC_RADIO_NOTRAYWINDOW, &CWindowDlg::OnBnClickedBayWindow)
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

	m_bHasInsert = false;

	m_preWindow.LoadDefaltSettings();

	InitCombOptions();

	m_comboViewDir.SetCurSel(0);
	m_comboInsertDir.SetCurSel(0);
	m_bAutoNumber = TRUE;
	m_editWinNumber.SetReadOnly(TRUE);
	
	return TRUE;
}
bool CWindowDlg::CheckValueModulo(CComboBox& comboBox, CString p_sType, int p_value, int moduloVale) //��������Ƿ���50��ģ��
{
	if (comboBox.IsWindowEnabled()==FALSE)
	{
		return true;
	}
	if (p_value%moduloVale != 0)
	{
		CString str;
		str.Format(_T("%sֵ������%d�ı���"), p_sType, moduloVale);
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

	//TODO ��鵱ǰ���������ѡ��ԭ�͵Ŀ�߷�Χ��


	int W1 = TYUI_GetInt(m_comboW1);
	int H2 = TYUI_GetInt(m_comboH2);
	int W3 = TYUI_GetInt(m_comboW3);
	int H3 = TYUI_GetInt(m_comboH3);
	////�������ֵ
	//if ((pSelWinAttr->IsValueCanbeSet(_T("W1")) && CheckValueModulo(m_comboW1, _T("�����ȿ��"), W1) == false) ||
	//	(pSelWinAttr->IsValueCanbeSet(_T("H2")) && CheckValueModulo(m_comboH2, _T("�¹̶��߶�"), H2) == false) ||
	//	(pSelWinAttr->IsValueCanbeSet(_T("H2")) && CheckValueModulo(m_comboW3, _T("ת�ǿ��"), W3)) == false ||
	//	CheckValueModulo(m_comboH3, _T("����ǽ�߶�"), H3) == false)
	//{
	//	return;
	//}

	//�ڼ��ԭ�ͱ��ǰ��������ֵ
	pSelWinAttr->SetW1(W1);
	pSelWinAttr->SetH2(H2);
	pSelWinAttr->SetW3(W3);
	pSelWinAttr->SetH3(H3);
	pSelWinAttr->SetD(m_nThickness);

	//�Զ�����¸���ԭ�ͱ��
	UpdateInstanceCode();

	CString sNumber = TYUI_GetText(m_editWinNumber);
	if (!m_bAutoNumber && !GetWindowAutoName()->IsNameValid(*pSelWinAttr,sNumber))
	{
		AfxMessageBox(L"�˱���ѱ�ռ��,������������");
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	const eViewDir viewDir = (eViewDir)m_comboViewDir.GetCurSel();
	pSelWinAttr->m_viewDir = viewDir;
	E_DIRECTION winDir = E_DIR_BOTTOM;
	if (viewDir == E_VIEW_TOP)
	{
		CString sDir = TYUI_GetComboBoxText(m_comboInsertDir);
		winDir = String2Direction(sDir);
	}

	ShowWindow(FALSE);

	if (m_bEditMode == false)
	{
		//ѡ������
		AcGePoint3d origin;
		bool bSuc = TY_GetPoint(origin, L"��ѡ������(����ǽλ��)"); 
		acedPostCommandPrompt();
		if (bSuc == false)
		{
			ShowWindow(SW_SHOW);
			return;
		}

		pSelWinAttr->m_relatedWinIds.removeAll();
		pSelWinAttr->m_fromWinId = AcDbObjectId::kNull;

		AcDbObjectId idOut = CWindowGen::GenerateWindow(*pSelWinAttr, origin, winDir, false, AcDbObjectId::kNull, GlobalSetting::GetWindowBlockLayer());
		assert(idOut != AcDbObjectId::kNull);

		m_bHasInsert = true;

		ShowWindow(TRUE);
		//OnOK();
	}
	else
	{
		bool bUpdateAll = (IDYES == AfxMessageBox(_T("�Ƿ����ͬ��ŵĶ������Ŵ��޸ģ�"), MB_YESNO));

		AcDbObjectId idOut = CWindowGen::UpdateWindow(m_pCurEditWinRef->objectId(), *pSelWinAttr, true, bUpdateAll);
		assert(idOut != AcDbObjectId::kNull);

		m_pCurEditWinRef = NULL;

		OnOK(); //�޸�ģʽֱ�ӹرմ���
	}
}

void CWindowDlg::OnBnClickedButtonSearchwindow()
{
	UpdateData();

	CString openType = TYUI_GetComboBoxText(m_comboOpenType);
	int openNum = _ttoi(TYUI_GetComboBoxText(m_comboOpenAmount));
	CString areaType = TYUI_GetComboBoxText(m_comboAreaType);

	if (m_radioDoorWindow == 0)
		m_winPrototypes = WebIO::GetInstance()->GetDoors(m_nWidth, m_nHeight, openType, openNum, areaType);
	else
		m_winPrototypes = WebIO::GetInstance()->GetWindows(m_nWidth, m_nHeight, openType, openNum, areaType);
	
	for (UINT i = 0; i < m_winPrototypes.size(); i++)
	{
		CString dwgPath = TY_GetPrototypeFilePath() + m_winPrototypes[i].GetFileName();
		if (!PathFileExists(dwgPath))
		{
			acutPrintf(L"\nԭ���ļ�" + m_winPrototypes[i].GetFileName() + L"δ�ҵ�\n");
			m_winPrototypes.erase(m_winPrototypes.begin() + i--);
		}

		m_winPrototypes[i].SetW(m_nWidth);
		m_winPrototypes[i].SetH(m_nHeight);
	}

	InitPreviewGridByWindowPrototypes();

	if (m_winPrototypes.empty())
	{
		AfxMessageBox(L"δ�ҵ����������ļ�¼\n");
		return;
	}
}

void CWindowDlg::InitPreviewGridByWindowPrototypes()
{
	m_preWindow.ClearAllPreviews();

	m_preWindow.SetRowCount((int)m_winPrototypes.size());
	m_preWindow.SetColumnCount(1);
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);

	for (UINT i = 0; i < m_winPrototypes.size(); i++)
	{
		CString str;
		str.Format(L"ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ������%.2lf\n��̬���ͣ�%s\n���÷�Χ������",
			m_winPrototypes[i].m_prototypeCode, GetArea(), m_winPrototypes[i].GetTongFengQty(false), m_winPrototypes[i].m_isDynamic ? L"��̬" : L"��̬");

		CString dwgPath = TY_GetPrototypeFilePath() + m_winPrototypes[i].GetFileName();
		CString pngPath = TY_GetPrototypeImagePath_Local() + m_winPrototypes[i].GetFileName(); //�Ŵ�ԭ������ʹ���ڲ���ͼƬ
		CString jpgPath = TY_GetPrototypeImagePath_Web() + m_winPrototypes[i].GetFileName(); //�Ŵ�ԭ������ʹ���ڲ���ͼƬ
		pngPath.Replace(L".dwg", L".png");
		jpgPath.Replace(L".dwg", L".jpg");
		if (PathFileExists(pngPath))
			m_preWindow.AddPreview(i, 0, pngPath, str);
		else if (PathFileExists(jpgPath))
			m_preWindow.AddPreview(i, 0, jpgPath, str);
		else
			m_preWindow.AddPreview(i, 0, dwgPath, str);
	}

	m_preWindow.SelectPreview(0, 0);
}

void CWindowDlg::OnBnClickedRadioWindowDoor()
{
	int preVal = m_radioDoorWindow;
	UpdateData(TRUE);

	if (m_radioDoorWindow != preVal)
	{
		ClearPrototypes();
		WindowDoorChange();
	}
}

void CWindowDlg::OnBnClickedCalculate()
{
	UpdateData(TRUE);
	CDlgWindowAirCalc dlg;
	if (dlg.DoModal()==IDOK)
	{
		TYUI_SetDouble(m_editVentilation, dlg.m_airQuality);
		UpdateData(FALSE);
	}
}

void CWindowDlg::OnBnClickedAutoIndex()
{
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;

	UpdateData(TRUE);
	if (m_bAutoNumber)
	{
		m_editWinNumber.SetReadOnly(TRUE);
		CString newName = GetWindowAutoName()->GetWindowName(*pSelWinAttr);
		TYUI_SetText(m_editWinNumber, newName);
	}
	else
		m_editWinNumber.SetReadOnly(FALSE);
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
	if (height>=400 && height<3000) //����ƽ��ͼ�򽫸߶���Ϊǽ���
	{
		m_nHeight = height;
	}
	else
	{
		m_nThickness = height;
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
	{
		TYUI_Disable(m_isMirror);
	}

	pSelWinAttr->m_instanceCode = GetWindowAutoName()->GetWindowName(*pSelWinAttr);
	TYUI_SetText(m_editWinNumber, pSelWinAttr->m_instanceCode);
	m_radioBayWindow = (pSelWinAttr->m_isBayWindow ? 0 : 1);
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

	//���Ĳ���������ʵ����ź�ͨ�����仯�������
	UpdateInstanceCode();
	UpdatePrototypeAirVolume();
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
	UpdatePrototypeAirVolume();
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
	UpdatePrototypeAirVolume();
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
	UpdatePrototypeAirVolume();
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

	pSelWinAttr->m_isBayWindow = (m_radioBayWindow == 0);
}

void CWindowDlg::OnSelChangedWallDis()
{
	AttrWindow* pSelWinAttr = GetSelWindow();
	if (pSelWinAttr == NULL)
		return;

	pSelWinAttr->m_wallDis = _ttof(TYUI_GetComboBoxText(m_comboOutWallDistance));
}

void CWindowDlg::ClearPrototypes()
{
	m_winPrototypes.clear();
	m_preWindow.ClearAllPreviews();
}
void CWindowDlg::WindowDoorChange()
{
	if (m_radioDoorWindow == 0)
	{
		TYUI_Disable(m_comboOpenAmount);
		TYUI_Disable(m_comboW1);
		TYUI_Disable(*GetDlgItem(IDC_RADIO_RAYWINDOW));
		TYUI_Disable(*GetDlgItem(IDC_RADIO_NOTRAYWINDOW));
	}
	else
	{
		TYUI_Enable(m_comboOpenAmount);
		TYUI_Enable(m_comboW1);
		TYUI_Enable(*GetDlgItem(IDC_RADIO_RAYWINDOW));
		TYUI_Enable(*GetDlgItem(IDC_RADIO_NOTRAYWINDOW));
	}

	ClearPrototypes();
}

void CWindowDlg::UpdateInstanceCode()
{
	if (m_bAutoNumber)
	{
		AttrWindow* pSelWinAttr = GetSelWindow();
		if (pSelWinAttr == NULL)
			return;

		UpdateData(TRUE);
		pSelWinAttr->m_instanceCode = GetWindowAutoName()->GetWindowName(*pSelWinAttr);
		TYUI_SetText(m_editWinNumber, pSelWinAttr->m_instanceCode);
		UpdateData(FALSE);
	}
}

void CWindowDlg::UpdatePrototypeAirVolume()
{
	CCellRange selRange = m_preWindow.GetSelectedCellRange();
	int nRow = selRange.GetMinRow();
	CGridCellForPreview* pCell = m_preWindow.GetPreviewCell(nRow, 0);
	if (pCell == NULL)
		return;

	AttrWindow* pSelWinAttr = &m_winPrototypes[nRow];
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
	{
		TYUI_Disable(comboBox);
		TYUI_SetText(comboBox, _T(""));
		return;
	}

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
			   TYUI_SetText(comboBox, _T(""));
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


void CWindowDlg::InitCombOptions()
{
	vCString openAmount;
	openAmount.push_back(L"����");
	openAmount.push_back(L"1");
	openAmount.push_back(L"2");
	//const vCString& openAmount = WebIO::GetInstance()->GetConfigDict()->Window_GetOpenAmount();

	const vCString& doorTypes = WebIO::GetInstance()->GetConfigDict()->Door_GetTypes();
	const vCString& openTypes = WebIO::GetInstance()->GetConfigDict()->Window_GetOpenTypes();
	const vCString& areaTypes = WebIO::GetInstance()->GetConfigDict()->GetGongNengQus();
	const vCString& wallDis = WebIO::GetInstance()->GetConfigDict()->Window_GetWallDis();

	TYUI_SetText(m_editWinNumber, L"");
	
	if (m_radioDoorWindow == 0)
		TYUI_InitComboBox(m_comboOpenType, doorTypes, doorTypes.empty() ? L"" : doorTypes[0]);
	else
		TYUI_InitComboBox(m_comboOpenType, openTypes, openTypes.empty()? L"" : openTypes[0]);
	TYUI_InitComboBox(m_comboAreaType, areaTypes, areaTypes.empty() ? L"" : areaTypes[0]);
	TYUI_InitComboBox(m_comboOpenAmount, openAmount, openAmount.empty() ? L"" : openAmount[0]);
	TYUI_InitComboBox(m_comboOutWallDistance, wallDis, wallDis.empty() ? L"" : wallDis[0]);	
}

AttrWindow* CWindowDlg::GetSelWindow()
{
	CCellRange sels = m_preWindow.GetSelectedCellRange();
	if (sels.Count() == 0)
		return NULL;
	return &m_winPrototypes[sels.GetMinRow()];
}

void CWindowDlg::SetEditMode(AcDbBlockReference* pBlock)
{
	m_pCurEditWinRef = pBlock;
	m_bEditMode = pBlock != NULL;
	if (m_bEditMode==false)
	{
		TYUI_SetText(*GetDlgItem(IDC_BUTTON_INSERTWINDOW), L"����");
		TYUI_Enable(m_comboViewDir);
		TYUI_Enable(m_comboInsertDir);
		TYUI_Enable(*GetDlgItem(IDC_RADIO_DOOR));
		TYUI_Enable(*GetDlgItem(IDC_RADIO_WINDOW));
		return;
	}
	else
	{
		TYUI_Disable(m_comboViewDir); //�޸�ģʽ�²����޸���ͼ
		TYUI_Disable(m_comboInsertDir); //�޸�ģʽ�²����޸���ͼ
		TYUI_Disable(*GetDlgItem(IDC_RADIO_DOOR));
		TYUI_Disable(*GetDlgItem(IDC_RADIO_WINDOW));

		AcDbObject* pAtt = NULL;
		TY_GetAttributeData(pBlock->objectId(), pAtt);
		const AttrWindow *pWinAtt = dynamic_cast<AttrWindow *>(pAtt);
		if (pWinAtt == NULL)
		{
			assert(false);
			return;
		}

		m_attBeforeEdit = *pWinAtt;

		//////////////////////////////////////////////////////////////////////////
		//��ʼ�Ŵ���������
		m_radioDoorWindow = (pWinAtt->GetType() == DOOR) ? 0 : 1;
		WindowDoorChange();
		m_nWidth = (int)pWinAtt->GetW();
		m_nHeight = (int)pWinAtt->GetH();
		m_radioBayWindow = pWinAtt->m_isBayWindow ? 0 : 1;
		m_bAutoNumber = TRUE;
		UpdateData(FALSE);

		TYUI_SetText(m_editWinNumber, pWinAtt->GetInstanceCode());

		UpdateDimDataToComboBox(m_comboW1, *pWinAtt, L"W1");
		UpdateDimDataToComboBox(m_comboH2, *pWinAtt, L"H2");
		UpdateDimDataToComboBox(m_comboW3, *pWinAtt, L"W3");

		CString str; 
		str.Format(_T("%d"), (int)pWinAtt->m_heightUnderWindow);
		TYUI_SetText(m_comboH3, str);
		str.Format(_T("%d"), (int)pWinAtt->m_wallDis);//����ǽ����	
		TYUI_SetText(m_comboOutWallDistance, str);

		str = ViewDir2String(pWinAtt->m_viewDir);
		TYUI_SetText(m_comboViewDir, str);
		m_isMirror.SetCheck(pWinAtt->m_isMirror ? TRUE :FALSE);


		//////////////////////////////////////////////////////////////////////////
		//����ԭ���б�Ϊ��ǰԭ��
		m_winPrototypes.clear();
		m_winPrototypes.push_back(*pWinAtt);

		//////////////////////////////////////////////////////////////////////////
		//3. ��ʾԭ����Ϣ
		InitPreviewGridByWindowPrototypes();
	
		//////////////////////////////////////////////////////////////////////////
		//�����밴ť���Ƹ�Ϊȷ����ť
		TYUI_SetText(*GetDlgItem(IDC_BUTTON_INSERTWINDOW), L"ȷ��");
	}
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

	for (UINT i = 0; i < m_winPrototypes.size(); i++)
	{
		AttrWindow* pSelWinAttr = &m_winPrototypes[i];

		int W1 = TYUI_GetInt(m_comboW1);
		int H2 = TYUI_GetInt(m_comboH2);
		int W3 = TYUI_GetInt(m_comboW3);
		int H3 = TYUI_GetInt(m_comboH3);
		pSelWinAttr->SetW1(W1);
		pSelWinAttr->SetH2(H2);
		pSelWinAttr->SetW3(W3);
		pSelWinAttr->SetH3(H3);
		pSelWinAttr->SetD(m_nThickness);

		pSelWinAttr->m_instanceCode = GetWindowAutoName()->GetWindowName(*pSelWinAttr);

		//////////////////////////////////////////////////////////////////////////
		const eViewDir viewDir = (eViewDir)m_comboViewDir.GetCurSel();
		pSelWinAttr->m_viewDir = viewDir;
		E_DIRECTION winDir = E_DIR_BOTTOM;
		if (viewDir == E_VIEW_TOP)
		{
			CString sDir = TYUI_GetComboBoxText(m_comboInsertDir);
			winDir = String2Direction(sDir);
		}

		AcGePoint3d insertPt;
		insertPt.x = origin.x + (m_nWidth + 100) * (i % 5);
		insertPt.y = origin.y + (m_nHeight + 100) * (i / 5);

		AcDbObjectId idOut = CWindowGen::GenerateWindow(*pSelWinAttr, origin, winDir, false, AcDbObjectId::kNull, GlobalSetting::GetWindowBlockLayer());
		assert(idOut != AcDbObjectId::kNull);
	}
	ShowWindow(SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////
CWindowDlg* g_windowDlg = NULL;

void OpenWindowDlg(AcDbBlockReference* pCurEdit/* = NULL*/)
{
	if (g_windowDlg == NULL)
	{
		g_windowDlg = new CWindowDlg(acedGetAcadFrame());
		g_windowDlg->Create(IDD_DIALOG_WINDOW);
	}
	g_windowDlg->SetEditMode(pCurEdit);
	g_windowDlg->ShowWindow(SW_SHOW);
}

BOOL CloseWindowDlg()
{
	if (g_windowDlg == NULL)
		return TRUE;

	BOOL ret = g_windowDlg->DestroyWindow();
	if (ret)
	{
		delete g_windowDlg;
		g_windowDlg = NULL;
	}
	return ret;
}
