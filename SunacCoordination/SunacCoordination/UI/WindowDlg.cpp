// UI\WindowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WindowDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCWindow.h"
#include "../WebIO/WebIO.h"
#include "../GlobalSetting.h"
#include "../WebIO/WindowLocalData.h"
#include "../Object/AttrWindow.h"
#include "../WebIO/WindowLocalData.h"
// CWindowDlg �Ի���

IMPLEMENT_DYNAMIC(CWindowDlg, CAcUiDialog)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowDlg::IDD, pParent)
	, m_radioDoor(0)
	, m_radioYes(0)
	, m_autoIndex(FALSE)
{

}

CWindowDlg::~CWindowDlg()
{
}

LRESULT CWindowDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	//return FALSE;
	return TRUE;
}

void CWindowDlg::OnOK()
{
	CAcUiDialog::OnOK();
	DestroyWindow();
}

void CWindowDlg::OnCancel()
{
	CAcUiDialog::OnCancel();
	DestroyWindow();
}

void CWindowDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();
	delete this;
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_WINDOW, m_preWindow);
	DDX_Control(pDX, IDC_COMBO_DOORTYPE, m_doorType);
	DDX_Control(pDX, IDC_COMBO_AREATYPE, m_areaType);
	DDX_Control(pDX, IDC_COMBO_OPENTYPE, m_openType);
	DDX_Control(pDX, IDC_COMBO_OPENAMOUNT, m_openAmount);
	DDX_Control(pDX, IDC_EDIT_VENTILATION, m_ventilation);
	DDX_Control(pDX, IDC_COMBO_OPENWIDTH, m_openWidth);
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
}


BEGIN_MESSAGE_MAP(CWindowDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_INSERTWINDOW, &CWindowDlg::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHWINDOW, &CWindowDlg::OnBnClickedButtonSearchwindow)
	ON_BN_CLICKED(IDC_RADIO_DOOR, &CWindowDlg::OnBnClickedRadioDoor)
	ON_BN_CLICKED(IDC_RADIO_WINDOW, &CWindowDlg::OnBnClickedRadioDoor)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_WINDOW, &CWindowDlg::OnSelChanged)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CWindowDlg::OnBnClickedCalculate)
END_MESSAGE_MAP()


// CWindowDlg ��Ϣ�������


BOOL CWindowDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preWindow.LoadDefaltSettings();

	LoadDefaultValue();
	SetRadioDoor(0);
	m_preWindow.SetRowCount(6);
	m_preWindow.SetColumnCount(1);
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);

	return TRUE;
}

void CWindowDlg::OnBnClickedButtonInsert()
{
	ShowWindow(FALSE);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AcGePoint3d origin = TY_GetPoint();

	double width = 0, height = 0;
	CString str;
	m_width.GetWindowText(str);
	width = _wtof(str);
	m_height.GetWindowText(str);
	height = _wtof(str);

	vector<int> sels = m_preWindow.GetSelectedRows();

	m_openWidth.GetWindowText(str);
	double W1 = _wtof(str);

	m_H2.GetWindowText(str);
	double H2 = _wtof(str);

	if (sels.size() > 0)
	{
		RCWindow oneWindow;
			
		oneWindow.Insert(TY_GetLocalFilePath() + m_allWindows[sels[0]].prototypeFile, origin, 0, L"0", 256);

		oneWindow.InitParameters();
		oneWindow.SetParameter(L"H", height);
		oneWindow.SetParameter(L"W", width);

		oneWindow.SetParameter(L"W1", W1);
		oneWindow.SetParameter(L"H2", H2);

		oneWindow.RunParameters();

		str.Format(L"%d_%d",(int)(oneWindow.GetW()), (int)(oneWindow.GetH()));
			
		//��UI�����ݼ�¼��ͼ�����չ�ֵ���
		AttrWindow * pWindow = new AttrWindow(m_allWindows[sels[0]]);
		oneWindow.AddAttribute(pWindow);
		pWindow->close();

		oneWindow.SetBianHao(m_allWindows[sels[0]].prototypeId + str);
	}
	OnOK();
}


void CWindowDlg::OnBnClickedButtonSearchwindow()
{
	double width = _ttof(GetText(m_width));
	double height = _ttof(GetText(m_height));
	CString openType = GetText(m_openType);
	int openNum = _ttoi(GetText(m_openAmount));
	CString areaType = GetText(m_areaType);

	if (m_radioDoor == 0)
	    m_allWindows = CWindowLocalData::GetInstance()->GetDoors(width, openType, openNum, areaType);
	else
		m_allWindows = CWindowLocalData::GetInstance()->GetWindows(width, openType, openNum, areaType);
	m_preWindow.ClearAllPreviews();
	m_preWindow.SetRowCount((int)m_allWindows.size());
	m_preWindow.SetColumnCount(1);
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);
	if (m_allWindows.size() == 0)
		AfxMessageBox(_T("δ�ҵ����������ļ�¼"));
	for (int i = 0; i < m_allWindows.size(); i++)
	{
		CString str;
		str.Format(_T("ԭ�ͱ�ţ�%s\n���������%.2lf\nͨ������0.9\n��̬���ͣ���̬\n���÷�Χ������"), m_allWindows[i].prototypeId, width * height / 1E6);
		m_preWindow.AddPreview(i, 0, TY_GetLocalFilePath() + m_allWindows[i].prototypeFile, str);
	}
}

void CWindowDlg::OnBnClickedRadioDoor()
{
	UpdateData(TRUE);
	SetRadioDoor(m_radioDoor);
}

void CWindowDlg::OnBnClickedCalculate()
{
	double area = _ttof(GetText(m_area));

	CString sRate = GetText(m_rate);
	double rate = _ttof(sRate);
	int pos = sRate.Find(_T('/'));
	if (pos != -1)
		rate /= _ttof(sRate.Mid(pos + 1));

	CString ventilation;
	ventilation.Format(_T("%.2lf"), area * rate);
	m_ventilation.SetWindowTextW(ventilation);
}

void CWindowDlg::OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW* pView = (NM_GRIDVIEW*)pNMHDR;
	int nSel = pView->iRow;
	if (nSel >= 0 && nSel < m_allWindows.size())
	{
		m_number.SetWindowTextW(m_allWindows[nSel].id);
		CString str;
		str.Format(_T("%.0lf"), m_allWindows[nSel].m_openWindowSize);
		m_openWidth.SetWindowTextW(str);
		str.Format(_T("%.0lf"), m_allWindows[nSel].m_windowH2);
		m_H2.SetWindowTextW(str);
		str.Format(_T("%.0lf"), m_allWindows[nSel].m_wallDis);
		m_H2.SetWindowTextW(str);
		m_radioYes = (m_allWindows[nSel].m_isBayWindow ? 0 : 1);
		UpdateData(FALSE);
	}
}

void CWindowDlg::SetRadioDoor(int radioDoor)
{
	if (radioDoor < 0 || radioDoor > 1)
		return;
	m_radioDoor = radioDoor;

	if (radioDoor == 0)
	{
		m_doorType.EnableWindow(TRUE);
		m_openType.EnableWindow(FALSE);
		m_openAmount.EnableWindow(FALSE);
		m_openWidth.EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_YES)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_NO)->EnableWindow(FALSE);
	}
	else
	{
		m_openType.EnableWindow(TRUE);
		m_openAmount.EnableWindow(TRUE);
		m_openWidth.EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_YES)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_NO)->EnableWindow(TRUE);
		m_doorType.EnableWindow(FALSE);
	}
}

void CWindowDlg::SetCombobox(CComboBox& comboBox, const vCString& options, int nSel /*= 0*/)
{
	comboBox.ResetContent();
	for (UINT i = 0; i < options.size(); i++)
	{
		comboBox.InsertString(i,options[i]);
	}
	comboBox.SetCurSel(nSel);
}

void CWindowDlg::LoadDefaultValue()
{
	vCString& doorTypes = WebIO::GetInstance()->m_doorTypes;
	vCString& openTypes = WebIO::GetInstance()->m_windowOpenTypes;
	vCString& areaTypes = WebIO::GetInstance()->m_gongNengQus;
	vCString& openAmount = WebIO::GetInstance()->m_windowOpenAmount;
	vCString& rate = WebIO::GetInstance()->m_rate;

	m_width.SetWindowTextW(_T("1500"));
	m_height.SetWindowTextW(_T("1200"));
	m_ventilation.SetWindowTextW(_T("2"));
	m_area.SetWindowTextW(_T("8"));
	
	SetCombobox(m_doorType, doorTypes);
	SetCombobox(m_openType, openTypes);
	SetCombobox(m_areaType, areaTypes);
	SetCombobox(m_openAmount, openAmount);
	SetCombobox(m_rate, rate);

	m_autoIndex = TRUE;
	m_viewDir.SetCurSel(0);
	UpdateData(FALSE);
}

CString CWindowDlg::GetText(const CWnd& pWnd)
{
	CString str;
	pWnd.GetWindowTextW(str);
	return str;
}

