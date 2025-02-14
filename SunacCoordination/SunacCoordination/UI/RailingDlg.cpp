// UI\RailingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RailingDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/Railing/RCRailing.h"
#include "../Object/Railing/RCRailingTieyi.h"
#include "../Object/Railing/RCRailingBoli.h"
#include "../WebIO/WebIO.h"
#include "../GlobalSetting.h"
#include "GridCellWithPicture.h"
#include "../Common/ComFun_ACad.h"
#include "../Common/ComFun_Layer.h"

// CRailingDlg 对话框

CRailingDlg* g_railingDlg = NULL;

void OpenRailingDlg(AcDbBlockReference* pCurEdit/* = NULL*/)
{
	if (g_railingDlg == NULL)
	{
		g_railingDlg = new CRailingDlg(acedGetAcadFrame());
		g_railingDlg->Create(IDD_DIALOG_RAILING);
	}
	g_railingDlg->SetEditMode(pCurEdit);
	g_railingDlg->ShowWindow(SW_SHOW);
}

BOOL CloseRailingDlg()
{
	if (g_railingDlg == NULL)
		return TRUE;
	BOOL ret = g_railingDlg->DestroyWindow();
	if (ret)
	{
		delete g_railingDlg;
		g_railingDlg = NULL;
	}
	return ret;
}


IMPLEMENT_DYNAMIC(CRailingDlg, CAcUiDialog)

CRailingDlg::CRailingDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CRailingDlg::IDD, pParent)
	, m_height(1200)
	, m_width(4200)
	, m_bRailingAutoName(TRUE)
	, m_sRailingId(_T(""))
{
	m_pCurEdit = NULL;
}

CRailingDlg::~CRailingDlg()
{
}

void CRailingDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_RAILING, m_preRailing);
	DDX_Control(pDX, IDC_COMBO_RAILINGTYPE, m_comboRailingType);
	DDX_Text(pDX, IDC_EDIT_RAILINGHEIGHT, m_height);
	DDV_MinMaxInt(pDX, m_height, 100, 10000);
	DDX_Text(pDX, IDC_EDIT_HOLEWIDTH, m_width);
	DDV_MinMaxInt(pDX, m_width, 100, 100000);
	DDX_Check(pDX, IDC_CHECK_AUTOINDEX, m_bRailingAutoName);
	DDX_Text(pDX, IDC_EDIT_RAILINGNUMBER, m_sRailingId);
	DDX_Control(pDX, IDC_EDIT_RAILINGNUMBER, m_editRailingID);
	DDX_Control(pDX, IDC_COMBO_VIEWDIR, m_comboViewDir);
}

BEGIN_MESSAGE_MAP(CRailingDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON_INSERTRAILING, &CRailingDlg::OnBnClickedInsertToCAD)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SELECTLINE, &CRailingDlg::OnBnClickedButtonSelectline)
	ON_CBN_SELCHANGE(IDC_COMBO_RAILINGTYPE, &CRailingDlg::OnCbnSelchangeComboRailingtype)
	ON_BN_CLICKED(IDC_CHECK_AUTOINDEX, &CRailingDlg::OnBnClickedCheckAutoindex)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_RAILING, &CRailingDlg::OnSelChangedPreview)
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO_VIEWDIR, &CRailingDlg::OnCbnSelchangeComboViewdir)
END_MESSAGE_MAP()


// CRailingDlg 消息处理程序


BOOL CRailingDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	m_preRailing.LoadDefaltSettings();

	m_comboRailingType.AddString(_T("不限"));
	m_comboRailingType.AddString(_T("铁艺栏杆"));
	m_comboRailingType.AddString(_T("玻璃栏杆"));
	m_comboRailingType.SetCurSel(0);
	m_comboViewDir.SetCurSel(0);

	UpdateRailingToGrid(E_RAILING_ALL);

	return TRUE;
}

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
		AfxMessageBox(_T("没有选择原型，请重新选择或者双击原型\n"));
		return;
	}

	if (m_sRailingId.IsEmpty())
	{
		AfxMessageBox(_T("请设置原型编号\n"));
		return;
	}

	AttrRailing railingAtt;
	railingAtt.m_height = m_height;
	railingAtt.SetRLength(m_width);
	railingAtt.m_prototypeCode = pCell->GetName();
	railingAtt.m_railingType = railingAtt.m_prototypeCode.Find(_T("_T"))>0 ? E_RAILING_TIEYI : E_RAILING_BOLI;

	if (m_bRailingAutoName)
	{
		m_sRailingId = railingAtt.AutoInstanceCode();
	}
	else
	{
		railingAtt.SetInstanceCode(m_sRailingId);
	}

	//平面图的栏杆长度为选择的插入点的间距
	const eViewDir viewDir = (eViewDir)m_comboViewDir.GetCurSel();
	railingAtt.SetViewDir(viewDir);
	vector<AcGePoint3d> pts;
	if (viewDir == E_VIEW_TOP)
	{
		if (m_pCurEdit != NULL)
		{
			JHCOM_DeleteCadObject(m_pCurEdit->objectId());
		}

		ShowWindow(SW_HIDE);
		bool bSuc = TY_GetPoints(pts);
		if (bSuc == false)
		{
			ShowWindow(SW_SHOW);
			return;
		}

		railingAtt.SetRailingPath(pts);
	}

	//生成
	CRCRailing* pRailing = CreateRailing(railingAtt);

	//检查数据
	CString str;
	if (pRailing->CheckLength() == false )
	{
		str.Format(_T("栏杆长度必须大于等于%d"), (int)(pRailing->GetMinWidth()));
		AfxMessageBox(str);
		delete pRailing;
		return;
	}
	if (pRailing->CheckHeight() == false)
	{
		str.Format(_T("栏杆高度必须大于等于%d"), (int)(pRailing->GetMinHeight()));
		AfxMessageBox(str);
		delete pRailing;
		return;
	}
	if (pRailing->GenRailing() == false)
	{
		AfxMessageBox(_T("此栏杆长度无法生成"));
		delete pRailing;
		return;
	}

	//栏杆平面图生成
	if (viewDir == E_VIEW_TOP)
	{
		pRailing->CreateRailingTop(pts);
		ShowWindow(SW_SHOW);
		return;
	}
	else
	{
		//选择插入点
		ShowWindow(SW_HIDE);

		AcGePoint3d pnt;
		if (m_pCurEdit == NULL)
		{
			bool bSuc = TY_GetPoint(pnt);
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
			pnt = ext.minPoint();
			JHCOM_DeleteCadObject(m_pCurEdit->objectId());
		}

		AcDbObjectId railingId;
		int nRet = pRailing->GenerateRailing(pnt, railingId);
	}

	delete pRailing;

	OnOK();
}

void CRailingDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();

	//// 释放非模态对话框的内存
	//delete this;
	//if (g_connectorDlg != NULL)
	//{
	//	g_connectorDlg = NULL;
	//}
}

void CRailingDlg::OnClose()
{
	CAcUiDialog::OnClose();

	// 销毁对话框
	//DestroyWindow();
}

LRESULT CRailingDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

BOOL CRailingDlg::PreTranslateMessage(MSG *pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
		return TRUE;
	return CAcUiDialog::PreTranslateMessage(pMsg);
}

void CRailingDlg::OnBnClickedButtonSelectline()
{
	ShowWindow(FALSE);
	AcGePoint3d pnt1, pnt2;
	bool bSuc = TY_GetTwoPoints(pnt1, pnt2);
	ShowWindow(TRUE);
	if (bSuc==false)
	{
		return;
	}

	double len = pnt2.x - pnt1.x;
	m_width = (int)(len+0.4); //四舍五入
	UpdateData(FALSE);
}

void CRailingDlg::OnCbnSelchangeComboRailingtype()
{
	UpdateData(TRUE);

	CString type = TYUI_GetComboBoxText(m_comboRailingType);
	if (type == _T("不限"))
	{
		UpdateRailingToGrid(E_RAILING_ALL);
	}
	else if (type == _T("铁艺栏杆"))
	{
		UpdateRailingToGrid(E_RAILING_TIEYI);
	}
	else if (type == _T("玻璃栏杆"))
	{
		UpdateRailingToGrid(E_RAILING_BOLI);
	}

	UpdateData(FALSE);
}

void CRailingDlg::UpdateRailingToGrid(eRailingType p_railetype)
{
	int nRowCount = 4;
	if (p_railetype == E_RAILING_ALL)
		nRowCount = 7;

	m_preRailing.ClearAllPreviews();
	m_preRailing.SetRowCount(nRowCount);
	m_preRailing.SetColumnCount(2);
	m_preRailing.SetDisplayRows(2);
	m_preRailing.SetDisplayColumns(2);
	m_preRailing.SetRowsPerWheelNotch(2);

	CString str;
	CString path;
	CString sPrototypeName;

	int nCount = 0;
	//铁艺
	if (p_railetype==E_RAILING_TIEYI || p_railetype==E_RAILING_ALL)
	{
		for (UINT i = 0; i < 7; i++, nCount++)
		{
			sPrototypeName.Format(_T("Railing_T%d"), i + 1);
			CString standradRailingSize = RailingSize(i);
			str.Format(_T("原型编号：%s\n栏杆类型：铁艺栏杆\n标准栏杆长：%s\n"), sPrototypeName, standradRailingSize);
			path.Format(TY_GetPrototypeImagePath_Local() + ("Railing_T%d.png"), i + 1);

			CGridCellForPreview* pCell = m_preRailing.AddPreview(nCount / 2, nCount % 2, path, str);
			pCell->SetName(sPrototypeName);
		}
	}

	//玻璃
	if (p_railetype == E_RAILING_BOLI || p_railetype == E_RAILING_ALL)
	{
		for (UINT i = 0; i < 7; i++, nCount++)
		{
			if (i <= 1)
			{
				sPrototypeName.Format(_T("Railing_B%d"), i + 1);
				str.Format(_T("原型编号：%s\n栏杆类型：玻璃栏杆\n"), sPrototypeName);
				path.Format(TY_GetPrototypeImagePath_Local() + ("Railing_B%d.png"), i + 1);
			}
			else if (i > 1 && i < 4)
			{
				sPrototypeName.Format(_T("Railing_B3_%d"), i - 1);
				str.Format(_T("原型编号：%s\n栏杆类型：玻璃栏杆"), sPrototypeName);
				path.Format(TY_GetPrototypeImagePath_Local() + ("Railing_B3_%d.png"), i - 1);
			}
			else
			{
				sPrototypeName.Format(_T("Railing_B%d"), i);
				str.Format(_T("原型编号：%s\n栏杆类型：玻璃栏杆\n"), sPrototypeName);
				path.Format(TY_GetPrototypeImagePath_Local() + ("Railing_B%d.png"), i);
			}

			CGridCellForPreview* pCell = m_preRailing.AddPreview(nCount / 2, nCount % 2, path, str);
			pCell->SetName(sPrototypeName);
		}
	}

	m_preRailing.SelectPreview(0, 0);
	//m_preRailing.SetDefCellMargin(20);
	//m_preRailing.SetBkColor(RGB(128, 128, 128));
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

void CRailingDlg::SetEditMode(AcDbBlockReference* pBlock)
{
	m_pCurEdit = pBlock;
	if (m_pCurEdit == NULL)
		return;

	AcDbObject* pAtt = NULL;
	TY_GetAttributeData(pBlock->objectId(), pAtt, true);
	AttrRailing *pRailing = dynamic_cast<AttrRailing *>(pAtt);
	if (pRailing == NULL)
		return;

	m_width = (int)(pRailing->GetRLength());
	m_height = (int)(pRailing->m_height);
	UpdateData(FALSE);

	for (int i = 0; i < m_preRailing.GetRowCount(); i++)
	{
		for (int j = 0; j < m_preRailing.GetColumnCount(); j++)
		{
			CGridCellForPreview* pCell = m_preRailing.GetPreviewCell(i, j);
			if (pCell == NULL)
				continue;
			if (pRailing->m_prototypeCode == pCell->GetName())
			{
				m_preRailing.SelectPreview(i, j);
				break;
			}
		}
	}

	TYUI_SetText(*GetDlgItem(IDC_BUTTON_INSERTRAILING), L"确定");
}

void CRailingDlg::OnBnClickedCheckAutoindex()
{
	int state =((CButton *)GetDlgItem(IDC_CHECK_AUTOINDEX))->GetCheck(); 
	BOOL bEnable = state == BST_CHECKED ? FALSE : TRUE;
	m_editRailingID.EnableWindow(bEnable);
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

	//自动编号
	if (pCell != NULL && m_bRailingAutoName)
	{
		AttrRailing railingAtt;
		railingAtt.m_height = m_height;
		railingAtt.SetRLength(m_width);
		railingAtt.m_prototypeCode = pCell->GetName();

		m_sRailingId =railingAtt.AutoInstanceCode();
		UpdateData(FALSE);
	}
}

//改变视图会调用此函数
void CRailingDlg::OnCbnSelchangeComboViewdir()
{
	CString sView = TYUI_GetComboBoxText(m_comboViewDir);

	if (sView == L"平面")
	{
		TYUI_Disable(*GetDlgItem(IDC_EDIT_HOLEWIDTH));
		TYUI_Disable(*GetDlgItem(IDC_BUTTON_SELECTLINE));		
	}
	else
	{
		TYUI_Enable(*GetDlgItem(IDC_EDIT_HOLEWIDTH));
		TYUI_Enable(*GetDlgItem(IDC_BUTTON_SELECTLINE));
	}
}
