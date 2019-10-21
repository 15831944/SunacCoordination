// UI\RailingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RailingDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
//#include "RailingBaseDlg.h"
#include "../Object/Railing/RCRailing.h"
#include "../Object/Railing/RCRailingTieyi.h"
#include "../Object/Railing/RCRailingBoli.h"
#include "../WebIO/WebIO.h"
#include "../GlobalSetting.h"
#include "GridCellWithPicture.h"

// CRailingDlg �Ի���

CRailingDlg* g_railingDlg = NULL;

BOOL CloseRailingDlg()
{
	if (g_railingDlg == NULL)
		return TRUE;
	BOOL ret = g_railingDlg->DestroyWindow();
	if (ret)
		g_railingDlg = NULL;
	return ret;
}


IMPLEMENT_DYNAMIC(CRailingDlg, CAcUiDialog)

CRailingDlg::CRailingDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CRailingDlg::IDD, pParent)
	, m_height(1200)
	, m_width(5400)
{
	m_isMoldless = true;
}

CRailingDlg::~CRailingDlg()
{
}

INT_PTR CRailingDlg::DoModal()
{
	m_isMoldless = false;
	return CAcUiDialog::DoModal();
}

void CRailingDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_RAILING, m_preRailing);
	DDX_Control(pDX, IDC_COMBO_RAILINGTYPE, m_type);
	DDX_Text(pDX, IDC_EDIT_RAILINGHEIGHT, m_height);
	DDV_MinMaxDouble(pDX, m_height, 100, 10000);
	DDX_Text(pDX, IDC_EDIT_HOLEWIDTH, m_width);
	DDV_MinMaxDouble(pDX, m_width, 100, 100000);
	DDX_Control(pDX, IDC_EDIT_RAILINGNUMBER, m_railingNumber);
}

BEGIN_MESSAGE_MAP(CRailingDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON_INSERTRAILING, &CRailingDlg::OnBnClickedInsertToCAD)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_SELECTLINE, &CRailingDlg::OnBnClickedButtonSelectline)
	ON_CBN_SELCHANGE(IDC_COMBO_RAILINGTYPE, &CRailingDlg::OnCbnSelchangeComboRailingtype)
	ON_BN_CLICKED(IDC_CHECK_AUTOINDEX, &CRailingDlg::OnBnClickedCheckAutoindex)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_RAILING, &CRailingDlg::OnSelChangedPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CRailingDlg ��Ϣ�������


BOOL CRailingDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	vCString allRailingFiles;
	TY_GetAllTieYiLanGanFiles(allRailingFiles);
	m_selectedFile = allRailingFiles[0];

	//m_preStyle.SubclassDlgItem(IDC_STATIC_STYLE, this);
	//m_preStyle.Init(theArxDLL.ModuleResourceInstance(), true);
	//m_preStyle.SetDwgFile(m_selectedFile);

	//m_railingInfo.SetWindowText(_T("������Ϣ˵��:\r\n���˼��:\r\n��Ԫ�ߴ�:\r\n�������ͣ�"));
	m_type.AddString(_T("����"));
	m_type.AddString(_T("��������"));
	m_type.AddString(_T("��������"));
	m_type.SetCurSel(0);

	UpdateAll();

	//((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_LIB))->SetImage(IDB_BITMAP37);
	//((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_CANCEL))->SetImage(IDB_BITMAP37);
	return TRUE;
}

/*
void CRailingDlg::UpdateSelectFile(CString selectFile)
{
	if (selectFile.GetLength() > 0)
	{
		m_preStyle.SetDwgFile(selectFile);
		m_selectedFile = selectFile;
	}
}
*/

void Test(AttrRailing& railingAtt)
{
	static int n = 0; 
	
	railingAtt.m_railingType = E_RAILING_BOLI;
	if (n==0)
		railingAtt.m_prototypeCode = _T("Railing_B1");
	else if (n == 1)
		railingAtt.m_prototypeCode = _T("Railing_B2");
	else if (n == 2)
		railingAtt.m_prototypeCode = _T("Railing_B3_1");
	else if (n == 3)
		railingAtt.m_prototypeCode = _T("Railing_B3_2");
	else if (n == 4)
		railingAtt.m_prototypeCode = _T("Railing_B4");
	else if (n == 5)
		railingAtt.m_prototypeCode = _T("Railing_B5");
	else if (n == 6)
		railingAtt.m_prototypeCode = _T("Railing_B6");

	n = (n + 1) % 7;
}

void CRailingDlg::OnBnClickedInsertToCAD()
{	
	UpdateData();

	vector<CCellID> selCells = m_preRailing.GetSelectedCells();
	if (selCells.empty())
		return;

	int selectedRow = selCells[0].row;
	int selectedColoum = selCells[0].col;

	CGridCellForPreview* pCell = m_preRailing.GetPreviewCell(selectedRow, selectedColoum);
	if (pCell==NULL)
	{
		AfxMessageBox(_T("û��ѡ��ԭ�ͣ�������ѡ�����˫��ԭ��\n"));
		return;
	}

	CString sPrototypeName = pCell->GetName();
	
	CString path;
	AttrRailing railingAtt;
	railingAtt.m_height = m_height;
	railingAtt.m_length = m_width;
	railingAtt.m_prototypeCode = sPrototypeName;
	railingAtt.m_railingType = sPrototypeName.Find(_T("_T"))>0 ? E_RAILING_TIEYI : E_RAILING_BOLI;

	//����
	CRCRailing* pRailing = CreateRailing(railingAtt);
	//�������
	if (pRailing->CheckLengthWidth()==false)
	{
		delete pRailing;
		AfxMessageBox(_T("���˳��Ȼ�߶Ȳ�����Ҫ��"));
		//ShowWindow(TRUE);
		return;
	}

	//ѡ������
	ShowWindow(FALSE);
	AcGePoint3d pnt;
	pnt = TY_GetPoint();

	AcDbObjectId railingId;
	int nRet = pRailing->GenerateRailing(pnt, railingId);
	
	OnOK();
}

LRESULT CRailingDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}
void CRailingDlg::OnOK()
{
	CAcUiDialog::OnOK();
	if (m_isMoldless)
		DestroyWindow();
}

void CRailingDlg::OnCancel()
{
	CAcUiDialog::OnCancel();
	if (m_isMoldless)
		DestroyWindow();
}

void CRailingDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();
	if (m_isMoldless)
	{
		delete this;
		g_railingDlg = NULL;
	}
}

void CRailingDlg::OnBnClickedButtonSelectline()
{
	ShowWindow(FALSE);
	AcGePoint3d pnt1, pnt2;
	TY_GetTwoPoints(pnt1, pnt2);

	//TODO ѡ��

	ShowWindow(TRUE);

	double len = pnt2.x - pnt1.x;
	m_width = len;
	UpdateData(FALSE);
}


void CRailingDlg::OnCbnSelchangeComboRailingtype()
{
	UpdateData(TRUE);

	//��ȡ�ؼ�ComboBox��ѡ��ֵ
	CString type = TYUI_GetComboBoxText(m_type);

	if (type == _T("����"))
	{
		UpdateAll();
	}

	if (type == _T("��������"))
	{
		UpdateTY();
	}

	if (type == _T("��������"))
	{
		UpdateBL();
	}

	UpdateData(FALSE);
}

void CRailingDlg::UpdateTY()
{
	m_preRailing.ClearAllPreviews();
	m_preRailing.SetRowCount(4);
	m_preRailing.SetColumnCount(2);
	m_preRailing.SetDisplayRows(2);
	m_preRailing.SetDisplayColumns(2);

	for (UINT i = 0; i < 7; i++)
	{
		CString str;
		CString path;
		sPrototypeName.Format(_T("Railing_T%d"),i+1);
		CString standradRailingSize = RailingSize(i);
		str.Format(_T("ԭ�ͱ�ţ�%s\n�������ͣ���������\n��׼���˳���%s\n"),sPrototypeName, standradRailingSize);
		path.Format(TY_GetLocalImagePath() + ("Railing_T%d.png"),i+1);
		CGridCellForPreview* pCell = m_preRailing.AddPreview(i/2, i%2, path, str);
		pCell->SetName(sPrototypeName);
	}

	m_preRailing.SelectPreview(0, 0);
	m_preRailing.SetDefCellMargin(20);
	m_preRailing.SetBkColor(RGB(128, 128, 128));
}

void CRailingDlg::UpdateBL()
{
	m_preRailing.ClearAllPreviews();
	m_preRailing.SetRowCount(4);
	m_preRailing.SetColumnCount(2);
	m_preRailing.SetDisplayRows(2);
	m_preRailing.SetDisplayColumns(2);

	for (UINT i = 0; i < 7; i++)
	{
		CString str;
		CString path;
		if (i <= 1)
		{
			sPrototypeName.Format(_T("Railing_B%d"),i+1);
			str.Format(_T("ԭ�ͱ�ţ�%s\n�������ͣ���������\n"),sPrototypeName);
			path.Format(TY_GetLocalImagePath() + ("Railing_B%d.png"),i+1);
			CGridCellForPreview* pCell = m_preRailing.AddPreview(i/2, i%2, path, str);
			pCell->SetName(sPrototypeName);
		}
		else if (i > 1 && i < 4)
		{
			sPrototypeName.Format(_T("Railing_B3_%d"),i-1);
			str.Format(_T("ԭ�ͱ�ţ�%s\n�������ͣ���������"),sPrototypeName);
			path.Format(TY_GetLocalImagePath() + ("Railing_B3_%d.png"),i-1);
			CGridCellForPreview* pCell = m_preRailing.AddPreview(i/2, i%2, path, str);	
			pCell->SetName(sPrototypeName);
		}
		else
		{
			sPrototypeName.Format(_T("Railing_B%d"),i);
			str.Format(_T("ԭ�ͱ�ţ�%s\n�������ͣ���������\n"),sPrototypeName);
			path.Format(TY_GetLocalImagePath() + ("Railing_B%d.png"),i);
			CGridCellForPreview* pCell = m_preRailing.AddPreview(i/2, i%2, path, str);
			pCell->SetName(sPrototypeName);
		}
	}

	m_preRailing.SelectPreview(0, 0);
	m_preRailing.SetDefCellMargin(20);
	m_preRailing.SetBkColor(RGB(128, 128, 128));
}

void CRailingDlg::UpdateAll()
{
	m_preRailing.ClearAllPreviews();
	m_preRailing.SetRowCount(7);
	m_preRailing.SetColumnCount(2);
	m_preRailing.SetDisplayRows(2);
	m_preRailing.SetDisplayColumns(2);

	for (UINT i = 0; i < 14; i++)
	{
		CString str;
		CString path;
		if (i < 7)
		{
			if (i <= 1)
			{
				CString standradRailingSize = RailingSize(i);
				sPrototypeName.Format(_T("Railing_B%d"),i+1);
				str.Format(_T("ԭ�ͱ�ţ�%s\n�������ͣ���������\n"),sPrototypeName);
				path.Format(TY_GetLocalImagePath() + ("Railing_B%d.png"),i+1);
				CGridCellForPreview* pCell = m_preRailing.AddPreview(i/2, i%2, path, str);
				pCell->SetName(sPrototypeName);
			}
			else if (i > 1 && i < 4)
			{
				sPrototypeName.Format(_T("Railing_B3_%d"),i-1);
				str.Format(_T("ԭ�ͱ�ţ�%s\n�������ͣ���������"),sPrototypeName);
				path.Format(TY_GetLocalImagePath() + ("Railing_B3_%d.png"),i-1);
				CGridCellForPreview* pCell = m_preRailing.AddPreview(i/2, i%2, path, str);	
				pCell->SetName(sPrototypeName);
			}
			else
			{
				sPrototypeName.Format(_T("Railing_B%d"),i);
				str.Format(_T("ԭ�ͱ�ţ�%s\n�������ͣ���������\n"),sPrototypeName);
				path.Format(TY_GetLocalImagePath() + ("Railing_B%d.png"),i);
				CGridCellForPreview* pCell = m_preRailing.AddPreview(i/2, i%2, path, str);
				pCell->SetName(sPrototypeName);
			}
		}
		else
		{
			sPrototypeName.Format(_T("Railing_T%d"),i-6);
			CString standradRailingSize = RailingSize(i-7);
			str.Format(_T("ԭ�ͱ�ţ�%s\n�������ͣ���������\n��׼���˳���%s\n"),sPrototypeName, standradRailingSize);
			path.Format(TY_GetLocalImagePath() + ("Railing_T%d.png"),i-6);
			CGridCellForPreview* pCell = m_preRailing.AddPreview(i/2, i%2, path, str);
			pCell->SetName(sPrototypeName);
		}
	}

	m_preRailing.SelectPreview(0, 0);
	m_preRailing.SetDefCellMargin(20);
	m_preRailing.SetBkColor(RGB(128, 128, 128));
}

CString CRailingDlg::RailingSize(int i)
{
	CString str;
	if (i < 3)
		str = _T("1260,1380");
	else if(i == 3)
		str = _T("1206,1320");
	else if(i == 4)
		str = _T("1220,1430");
	else if(i == 5)
		str = _T("1355,1570");
	else if(i == 6)
		str = _T("1510,1716");
	return str;
}

void CRailingDlg::OnBnClickedCheckAutoindex()
{
	int state =((CButton *)GetDlgItem(IDC_CHECK_AUTOINDEX))->GetCheck(); 
	if (state == BST_CHECKED ) 
	{
		m_railingNumber.SetWindowTextW(_T(""));
		TYUI_Disable(m_railingNumber);
	}
	else
	{
		TYUI_Enable(m_railingNumber);
	}
}


void CRailingDlg::OnSelChangedPreview(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData();

	vector<CCellID> selCells = m_preRailing.GetSelectedCells();
	if (selCells.empty())
		return;

	int selectedRow = selCells[0].row;
	int selectedColoum = selCells[0].col;
	CGridCellForPreview* pCell = m_preRailing.GetPreviewCell(selectedRow, selectedColoum);
	if (pCell != NULL)
	{
		AttrRailing railingAtt;
		railingAtt.m_height = m_height;
		railingAtt.m_length = m_width;
		CString sPrototypeName = pCell->GetName();
		railingAtt.m_prototypeCode = sPrototypeName;
		CString sRailingDefName;
		sRailingDefName.Format(_T("%s_%d_%d"), railingAtt.m_prototypeCode, (int)(railingAtt.m_length), (int)(railingAtt.m_height));
		m_railingNumber.SetWindowTextW(sRailingDefName);
	}
}
