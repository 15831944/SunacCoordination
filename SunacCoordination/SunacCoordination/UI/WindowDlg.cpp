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

// CWindowDlg �Ի���

IMPLEMENT_DYNAMIC(CWindowDlg, CAcUiDialog)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowDlg::IDD, pParent)
	, m_radioDoor(1)
	, m_radioYes(0)
	, m_autoIndex(FALSE)
{
	m_isMoldless = true;
	m_pCurEdit = NULL;
	m_selWidth = 0;
	m_selHeight = 0;
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
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		InsertAllWindows();
		return TRUE;
	}
	return CAcUiDialog::PreTranslateMessage(pMsg);
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_WINDOW, m_preWindow);
	DDX_Control(pDX, IDC_COMBO_AREATYPE, m_areaType);
	DDX_Control(pDX, IDC_COMBO_OPENTYPE, m_openType);
	DDX_Control(pDX, IDC_COMBO_OPENAMOUNT, m_openAmount);
	DDX_Control(pDX, IDC_EDIT_VENTILATION, m_ventilation);
	DDX_Control(pDX, IDC_COMBO_OPENWIDTH, m_W1);
	DDX_Control(pDX, IDC_COMBO_FIXEDVALUE, m_H2);
	DDX_Control(pDX, IDC_COMBO_DISTANCE, m_distance);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_width);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_height);
	DDX_Control(pDX, IDC_EDIT_NUMBER, m_number);
	DDX_Radio(pDX, IDC_RADIO_DOOR, m_radioDoor);
	DDX_Radio(pDX, IDC_RADIO_YES, m_radioYes);
	DDX_Control(pDX, IDC_EDIT_AREA, m_area);
	DDX_Control(pDX, IDC_COMBO_RATE, m_rate);
	DDX_Check(pDX, IDC_CHECK_AUTOINDEX, m_autoIndex);
	DDX_Control(pDX, IDC_COMBO_VIEWDIR, m_viewDir);
	DDX_Control(pDX, IDC_CHECK_IMAGE, m_isMirror);
	DDX_Control(pDX, IDC_COMBO_W3, m_W3);
	DDX_Control(pDX, IDC_COMBO_H3, m_H3);
	DDX_Control(pDX, IDC_COMBO_DIR, m_insertDir);
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

void CWindowDlg::OnBnClickedButtonInsert()
{
	UpdateData();

	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
	{
		AfxMessageBox(L"��ѡ��ԭ��");
		return;
	}

	if (pSel->GetTongFengQty(false) + TOL < TYUI_GetDouble(m_ventilation))
	{
		AfxMessageBox(L"��ԭ�Ͳ�����ͨ����Ҫ��");
		return;
	}

	CString sNumber = TYUI_GetText(m_number);
	if (!m_autoIndex && !CWindowAutoName::GetInstance()->IsNameValid(*pSel,sNumber))
	{
		AfxMessageBox(L"�˱���ѱ�ռ��");
		return;
	}
	else
	{
		pSel->SetInstanceCode(sNumber);
		CWindowAutoName::GetInstance()->RenameWindow(*pSel);
	}

	ShowWindow(FALSE);

	AcGePoint3d origin;
	if (m_pCurEdit == NULL)
	{
		origin = TY_GetPoint();
		acedPostCommandPrompt();
	}
	else
	{
		AcDbExtents ext;
		m_pCurEdit->getGeomExtents(ext);
		origin = ext.minPoint();
		JHCOM_DeleteCadObject(m_pCurEdit->objectId());
	}

	double W1 = TYUI_GetDouble(m_W1);
	double H2 = TYUI_GetDouble(m_H2);
	double W3 = TYUI_GetDouble(m_W3);
	double H3 = TYUI_GetDouble(m_H3);

	RCWindow oneWindow;

	int sel = m_viewDir.GetCurSel();
	if (sel == 0)
		oneWindow.Insert(TY_GetPrototypeFilePath() + pSel->m_frontViewFile.fileName, origin, 0, L"0", 256);
	else if (sel == 1)
	{
		double rotateAngle = 0;
		AcGeVector3d offsetXY(0, 0, 0);
		CString sDir = TYUI_GetComboBoxText(m_insertDir);
		if (sDir == L"��")
		{
			rotateAngle = PI;
			offsetXY.x += m_selWidth;
		}
		if (sDir == L"��")
			rotateAngle = PI / 2;
		if (sDir == L"��")
		{
			rotateAngle = -PI / 2;
			offsetXY.y += m_selWidth;
		}
		oneWindow.Insert(TY_GetPrototypeFilePath() + pSel->m_topViewFile.fileName, origin + offsetXY, rotateAngle, L"0", 256);
	}
	else
		oneWindow.Insert(TY_GetPrototypeFilePath() + pSel->m_leftViewFile.fileName, origin, 0, L"0", 256);

	oneWindow.InitParameters();

	oneWindow.SetParameter(L"H", m_selHeight);
	oneWindow.SetParameter(L"W", m_selWidth);
	oneWindow.SetParameter(L"W1", W1);
	oneWindow.SetParameter(L"H2", H2);
	oneWindow.SetParameter(L"W3", W3);
	oneWindow.SetParameter(L"H3", H3);

	oneWindow.RunParameters();

	if (m_isMirror.GetCheck())
	{
		AcGePoint3d basePt(origin.x + m_selWidth / 2, 0, 0);
		TYCOM_Mirror(oneWindow.m_id, basePt, AcGeVector3d(0, 1, 0));
	}

	CWindowAutoName::GetInstance()->AddWindowType(*pSel);

	//��UI�����ݼ�¼��ͼ�����չ�ֵ���
	AttrWindow * pWindow = new AttrWindow(*pSel);
	oneWindow.AddAttribute(pWindow);
	pWindow->close();

	ShowWindow(TRUE);
	//OnOK();
}

void CWindowDlg::OnBnClickedButtonSearchwindow()
{
	m_selWidth = TYUI_GetDouble(m_width);
	m_selHeight = TYUI_GetDouble(m_height);
	CString openType = TYUI_GetComboBoxText(m_openType);
	int openNum = _ttoi(TYUI_GetComboBoxText(m_openAmount));
	CString areaType = TYUI_GetComboBoxText(m_areaType);

	if (m_radioDoor == 0)
	    m_allWindows = WebIO::GetInstance()->GetDoors(m_selWidth, m_selHeight, openType, openNum, areaType);
	else
		m_allWindows = WebIO::GetInstance()->GetWindows(m_selWidth, m_selHeight, openType, openNum, areaType);

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
		m_allWindows[i].SetW(m_selWidth);
		m_allWindows[i].SetH(m_selHeight);
		CString str;
		str.Format(L"ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ������%.2lf\n��̬���ͣ�%s\n���÷�Χ������", m_allWindows[i].m_prototypeCode, m_selWidth * m_selHeight / 1E6, m_allWindows[i].GetTongFengQty(false), m_allWindows[i].m_isDynamic ? L"��̬" : L"��̬");

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

	TYUI_SetDouble(m_ventilation, area * rate);
}

void CWindowDlg::OnBnClickedAutoIndex()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	UpdateData(TRUE);
	if (m_autoIndex)
	{
		m_number.SetReadOnly(TRUE);
		CString newName = CWindowAutoName::GetInstance()->GetWindowName(*pSel);
		TYUI_SetText(m_number, newName);
	}
	else
		m_number.SetReadOnly(FALSE);
}

void CWindowDlg::OnBnClickedSelOnDwg()
{
	ShowWindow(false);
	TYRect rect = TY_GetOneRect();
	ShowWindow(true);
	
	int width = int(rect.GetWidth() + 0.5);
	int height = int(rect.GetHeight() + 0.5);

	TYUI_SetInt(m_width, width);
	TYUI_SetInt(m_height, height);
}

void CWindowDlg::OnBnClickedMirror()
{
	UpdateData(TRUE);
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;
	pSel->m_isMirror = (m_isMirror.GetCheck() != FALSE);
	UpdateInstanceCode();
}

void CWindowDlg::OnSelChangedPreview(NMHDR *pNMHDR, LRESULT *pResult)
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	UpdateDimDataToComboBox(m_W1, *pSel, L"W1");
	UpdateDimDataToComboBox(m_H2, *pSel, L"H2");
	UpdateDimDataToComboBox(m_W3, *pSel, L"W3");
	UpdateDimDataToComboBox(m_H3, *pSel, L"H3");

	if (pSel->m_isDynamic)
	{
		TYUI_Enable(m_isMirror);
		m_isMirror.SetCheck(pSel->m_isMirror);
	}
	else
		TYUI_Disable(m_isMirror);

	pSel->m_instanceCode = CWindowAutoName::GetInstance()->GetWindowName(*pSel);
	TYUI_SetText(m_number, pSel->m_instanceCode);
	m_radioYes = (pSel->m_isBayWindow ? 0 : 1);
	TYUI_SetInt(m_distance, int(pSel->m_wallDis));

	m_viewDir.ResetContent();
	if ((!pSel->m_frontViewFile.fileName.IsEmpty()) && PathFileExists(TY_GetPrototypeFilePath() + pSel->m_frontViewFile.fileName))
		m_viewDir.AddString(L"����");
	if ((!pSel->m_topViewFile.fileName.IsEmpty()) && PathFileExists(TY_GetPrototypeFilePath() + pSel->m_topViewFile.fileName))
		m_viewDir.AddString(L"ƽ��");
	if ((!pSel->m_leftViewFile.fileName.IsEmpty()) && PathFileExists(TY_GetPrototypeFilePath() + pSel->m_leftViewFile.fileName))
		m_viewDir.AddString(L"����");
	if (pSel->m_viewDir < 3)
		m_viewDir.SetCurSel(pSel->m_viewDir);
	OnSelChangedView();

	UpdateData(FALSE);
}

void CWindowDlg::OnSelChangedW1()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	CString sSel = TYUI_GetComboBoxText(m_W1);
	pSel->SetW1(_ttoi(sSel));
	//���Ĳ���������ʵ����ű仯�������
	UpdateInstanceCode();
	UpdateVent();
}

void CWindowDlg::OnSelChangedH2()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	CString sSel = TYUI_GetComboBoxText(m_H2);
	pSel->SetH2(_ttoi(sSel));
	//���Ĳ���������ʵ����ű仯�������
	UpdateInstanceCode();
	UpdateVent();
}

void CWindowDlg::OnSelChangedW3()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	CString sSel = TYUI_GetComboBoxText(m_W3);
	pSel->SetW3(_ttoi(sSel));
	//���Ĳ���������ʵ����ű仯�������
	UpdateInstanceCode();
	UpdateVent();
}

void CWindowDlg::OnSelChangedH3()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	CString sSel = TYUI_GetComboBoxText(m_H3);
	pSel->SetH3(_ttoi(sSel));
	//���Ĳ���������ʵ����ű仯�������
	UpdateInstanceCode();
	UpdateVent();
}

void CWindowDlg::OnSelChangedView()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	CString sView = TYUI_GetComboBoxText(m_viewDir);
	
	if (sView == L"ƽ��")
	{
		pSel->m_viewDir = E_VIEW_TOP;
		TYUI_Show(*GetDlgItem(IDC_STATIC_DIR));
		TYUI_Show(m_insertDir);
	}
	else
	{
		if (sView == L"����")
			pSel->m_viewDir = E_VIEW_FRONT;
		else if (sView == L"����")
			pSel->m_viewDir = E_VIEW_LEFT;
		TYUI_Hide(*GetDlgItem(IDC_STATIC_DIR));
		TYUI_Hide(m_insertDir);
	}
}

void CWindowDlg::OnBnClickedBayWindow()
{
	UpdateData(TRUE);
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	pSel->m_isBayWindow = (m_radioYes == 0);
}

void CWindowDlg::OnSelChangedWallDis()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	pSel->m_wallDis = _ttof(TYUI_GetComboBoxText(m_distance));
}

void CWindowDlg::UpdateEnable()
{
	//�л�������б��������ʱ����
	m_allWindows.clear();
	m_preWindow.ClearAllPreviews();

	if (m_radioDoor == 0)
	{
		TYUI_Disable(m_openAmount);
		TYUI_Disable(m_W1);
		TYUI_Disable(*GetDlgItem(IDC_RADIO_YES));
		TYUI_Disable(*GetDlgItem(IDC_RADIO_NO));
	}
	else
	{
		TYUI_Enable(m_openAmount);
		TYUI_Enable(m_W1);
		TYUI_Enable(*GetDlgItem(IDC_RADIO_YES));
		TYUI_Enable(*GetDlgItem(IDC_RADIO_NO));
	}
	LoadDefaultValue();
}

void CWindowDlg::UpdateInstanceCode()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;
	if (m_autoIndex)
	{
		pSel->m_instanceCode = CWindowAutoName::GetInstance()->GetWindowName(*pSel);
		TYUI_SetText(m_number, pSel->m_instanceCode);
	}
}

void CWindowDlg::UpdateVent()
{
	CCellRange selRange = m_preWindow.GetSelectedCellRange();
	int nRow = selRange.GetMinRow();
	CGridCellForPreview* pCell = m_preWindow.GetPreviewCell(nRow, 0);
	if (pCell == NULL)
		return;
	AttrWindow* pSel = &m_allWindows[nRow];
	CString str;
	str.Format(L"ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ������%.2lf\n��̬���ͣ�%s\n���÷�Χ������", pSel->m_prototypeCode, m_selWidth * m_selHeight / 1E6, pSel->GetTongFengQty(false), pSel->m_isDynamic ? L"��̬" : L"��̬");
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
	if (pDimData->type == MULTI)
	{
		TYUI_Enable(comboBox);
		TYUI_InitComboBox(comboBox, pDimData->valueOptions, pDimData->value);
	}
	else if ((pDimData->type == SINGLE) || (pDimData->type == CALC))
	{
		double dimValue = attrWindow.GetValue(code);
		vdouble options(1, dimValue);
		TYUI_InitComboBox(comboBox, options, dimValue);
		TYUI_Disable(comboBox);
	}
	else
	{
		comboBox.ResetContent();
		TYUI_Disable(comboBox);
	}
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
	const vCString& rate = WebIO::GetInstance()->GetConfigDict()->Window_GetRate();
	const vCString& wallDis = WebIO::GetInstance()->GetConfigDict()->Window_GetWallDis();

	TYUI_SetInt(m_width, 1500);
	TYUI_SetInt(m_height, 1800);
	TYUI_SetInt(m_ventilation, 0);
	TYUI_SetInt(m_area, 0);
	TYUI_SetText(m_number, L"");
	
	if (m_radioDoor == 0)
		TYUI_InitComboBox(m_openType, doorTypes, doorTypes.empty() ? L"" : doorTypes[0]);
	else
		TYUI_InitComboBox(m_openType, openTypes, openTypes.empty()? L"" : openTypes[0]);
	TYUI_InitComboBox(m_areaType, areaTypes, areaTypes.empty() ? L"" : areaTypes[0]);
	TYUI_InitComboBox(m_openAmount, openAmount, openAmount.empty() ? L"" : openAmount[0]);
	TYUI_InitComboBox(m_rate, rate, rate.empty() ? L"" : rate[0]);
	TYUI_InitComboBox(m_distance, wallDis, wallDis.empty() ? L"" : wallDis[0]);

	m_viewDir.SetCurSel(0);
	m_insertDir.SetCurSel(0);
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
	m_selWidth = pWindow->GetW();
	m_selHeight =  pWindow->GetH();
	TYUI_SetInt(m_width, int(m_selWidth));
	TYUI_SetInt(m_height, int(m_selHeight));

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
	str.Format(L"ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ������%.2lf\n��̬���ͣ�%s\n���÷�Χ������", m_allWindows[0].m_prototypeCode, m_selWidth * m_selHeight / 1E6, m_allWindows[0].GetTongFengQty(false), m_allWindows[0].m_isDynamic ? L"��̬" : L"��̬");

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

void CWindowDlg::InsertAllWindows()
{
	ShowWindow(SW_HIDE);
	AcGePoint3d origin = TY_GetPoint();

	for (UINT i = 0; i < m_allWindows.size(); i++)
	{
		AttrWindow* pSel = &m_allWindows[i];
		RCWindow oneWindow;

		AcGePoint3d insertPt;

		insertPt.x = origin.x + (m_selWidth + 100) * (i % 5);
		insertPt.y = origin.y + (m_selHeight + 100) * (i / 5);

		int sel = m_viewDir.GetCurSel();
		if (sel == 0)
			oneWindow.Insert(TY_GetPrototypeFilePath() + pSel->m_frontViewFile.fileName, insertPt, 0, L"0", 256);
		else if (sel == 1)
		{
			double rotateAngle = 0;
			AcGeVector3d offsetXY(0, 0, 0);
			CString sDir = TYUI_GetComboBoxText(m_insertDir);
			if (sDir == L"��")
			{
				rotateAngle = PI;
				offsetXY.x += m_selWidth;
			}
			if (sDir == L"��")
				rotateAngle = PI / 2;
			if (sDir == L"��")
			{
				rotateAngle = -PI / 2;
				offsetXY.y += m_selWidth;
			}
			oneWindow.Insert(TY_GetPrototypeFilePath() + pSel->m_topViewFile.fileName, origin + offsetXY, rotateAngle, L"0", 256);
		}
		else
			oneWindow.Insert(TY_GetPrototypeFilePath() + pSel->m_leftViewFile.fileName, insertPt, 0, L"0", 256);

		oneWindow.InitParameters();

		oneWindow.SetParameter(L"H", m_selHeight);
		oneWindow.SetParameter(L"W", m_selWidth);

		oneWindow.RunParameters();

		if (m_isMirror.GetCheck())
		{
			AcGePoint3d basePt(insertPt.x + m_selWidth / 2, 0, 0);
			TYCOM_Mirror(oneWindow.m_id, basePt, AcGeVector3d(0, 1, 0));
		}

		CWindowAutoName::GetInstance()->AddWindowType(*pSel);

		//��UI�����ݼ�¼��ͼ�����չ�ֵ���
		AttrWindow * pWindow = new AttrWindow(*pSel);
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
