// DlgSetUp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgSetUp.h"
#include "afxdialogex.h"
#include "../GlobalSetting.h"
#include <algorithm>
#include <vector>
#include "../UI/GridCtrl_src/GridCtrl.h"
#include "../Common/ComFun_Layer.h"
#include "../Tool/DocLock.h"

// CDlgSetUp �Ի���

IMPLEMENT_DYNAMIC(CDlgSetUp, CDialogEx)

CDlgSetUp::CDlgSetUp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetUp::IDD, pParent)
	, m_showLimianNumberDlg(TRUE)
	, m_useAinLimianDlg(TRUE)
	, m_sFrameLayerDlg(_T(""))
	, m_sHardWareLayerDlg(_T(""))
	, m_sLayerDlg(_T(""))
	, m_sNumberLayerLimianDlg(_T(""))
	, m_sNumberLayerPingmianDlg(_T(""))
	, m_sOpenLayerDlg(_T(""))
	, m_sWallLayerDlg(_T(""))
{
	
}

CDlgSetUp::~CDlgSetUp()
{
}

void CDlgSetUp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_SHOWLIMIANNUMBER, m_showLimianNumberDlg);
	DDX_Radio(pDX, IDC_USEAINLIMIAN, m_useAinLimianDlg);
	DDX_Control(pDX, IDC_WINNUMTEXTSIZE, m_winNumberTextSizeDlg);
	DDX_Control(pDX, IDC_WINDETAILDIMRATE, m_winDetailDimRateDlg);
	DDX_Text(pDX, IDC_WINFRAMELAYER, m_sFrameLayerDlg);
	DDX_Text(pDX, IDC_WINHARDWARELAYER, m_sHardWareLayerDlg);
	DDX_Text(pDX, IDC_WINLAYER, m_sLayerDlg);
	DDX_Text(pDX, IDC_WINNUMBERLAYERLIMIAN, m_sNumberLayerLimianDlg);
	DDX_Text(pDX, IDC_WINNUMBERLAYERPINGMIAN, m_sNumberLayerPingmianDlg);
	DDX_Text(pDX, IDC_WINOPENLAYER, m_sOpenLayerDlg);
	DDX_Text(pDX, IDC_WINWALLLAYER, m_sWallLayerDlg);
}


BEGIN_MESSAGE_MAP(CDlgSetUp, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSetUp::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSetUp::OnBnClickedCancel)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()


bool CompareWinSetUpLayer(CString& a, CString& b)
{
	return a < b;
}

BOOL CDlgSetUp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDigPos();

	InitWinSetting();
	
	return TRUE; 
}

void CDlgSetUp::InitWinSetting()
{
	UpdateData(TRUE);

	m_originalWinSetting = GlobalSetting::GetInstance()->m_winSetting;

	m_sFrameLayerDlg = GlobalSetting::GetInstance()->m_winSetting.m_sWinFrameLayer;
	m_sHardWareLayerDlg = GlobalSetting::GetInstance()->m_winSetting.m_sWinHardwareLayer;
	m_sLayerDlg = GlobalSetting::GetInstance()->m_winSetting.m_sWinLayer;
	m_sNumberLayerLimianDlg = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerLimian;
	m_sNumberLayerPingmianDlg = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerPingmian;
	m_sOpenLayerDlg = GlobalSetting::GetInstance()->m_winSetting.m_sWinOpenLayer;
	m_sWallLayerDlg = GlobalSetting::GetInstance()->m_winSetting.m_sWinWallLayer;

	((CComboBox*)GetDlgItem(IDC_WINNUMTEXTSIZE))->SetCurSel(3);//�����Ŵ�������Ĭ����ʾ100
	((CComboBox*)GetDlgItem(IDC_WINDETAILDIMRATE))->SetCurSel(1);//�����Ŵ���ͼ��ע����Ĭ����ʾ50

	UpdateData(FALSE);
}

// CDlgSetUp ��Ϣ�������
void CDlgSetUp::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_sFrameLayerDlg.IsEmpty() || m_sHardWareLayerDlg.IsEmpty() || m_sNumberLayerLimianDlg.IsEmpty() || m_sNumberLayerPingmianDlg.IsEmpty()
		|| m_sOpenLayerDlg.IsEmpty() || m_sWallLayerDlg.IsEmpty() || m_sLayerDlg.IsEmpty())
	{
		AfxMessageBox(_T("ͼ�㲻��Ϊ��"));
		return;
	}

	//��ͼ��push��vector��������
	vector<CString> sLayers;
	sLayers.clear();
	sLayers.push_back(m_sFrameLayerDlg);
	sLayers.push_back(m_sHardWareLayerDlg);
	sLayers.push_back(m_sNumberLayerLimianDlg);
	sLayers.push_back(m_sNumberLayerPingmianDlg);
	sLayers.push_back(m_sOpenLayerDlg);
	sLayers.push_back(m_sWallLayerDlg);
	sLayers.push_back(m_sLayerDlg);

	//����
	std::sort(sLayers.begin(), sLayers.end(), CompareWinSetUpLayer);

	//�жϸ���ͼ�㲻����ͬ
	for (int i = 0; i < sLayers.size() - 1; i++)
	{
		if (sLayers[i].CompareNoCase(sLayers[i + 1]) == 0)
		{
			AfxMessageBox(L"ͼ�����Ʋ�����ͬ");
			return;
		}
	}

	//�Ե�ǰ�ĵ�ͼ��ת��ͼ��
	UpdateLayer();

	//���浽setting
	GlobalSetting::GetInstance()->m_winSetting.m_sWinFrameLayer = m_sFrameLayerDlg;
	GlobalSetting::GetInstance()->m_winSetting.m_sWinWallLayer = m_sHardWareLayerDlg;
	GlobalSetting::GetInstance()->m_winSetting.m_sWinLayer = m_sLayerDlg;
	GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerLimian = m_sNumberLayerLimianDlg;
	GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerPingmian = m_sNumberLayerPingmianDlg;
	GlobalSetting::GetInstance()->m_winSetting.m_sWinOpenLayer = m_sOpenLayerDlg;
	GlobalSetting::GetInstance()->m_winSetting.m_sWinWallLayer = m_sWallLayerDlg;

	//�Ŵ������������
	if (TYUI_GetText(m_winNumberTextSizeDlg).GetLength() > 0)
	{
		GlobalSetting::GetInstance()->m_winSetting.m_numberTextSize = TYUI_GetInt(m_winNumberTextSizeDlg);
	}

	//�Ŵ���ͼ��ע��������
	if (TYUI_GetText(m_winDetailDimRateDlg).GetLength() > 0)
	{
		GlobalSetting::GetInstance()->m_winSetting.m_bWinDetailDimRate = TYUI_GetInt(m_winDetailDimRateDlg);
	}

	//����
	GlobalSetting::GetInstance()->m_winSetting.m_bUseAinLimian = m_useAinLimianDlg ? true : false;
	GlobalSetting::GetInstance()->m_winSetting.m_bShowLimianNumber = m_showLimianNumberDlg ? true : false;
	
	GlobalSetting::GetInstance()->UpdateToXml();

	CDialogEx::OnOK();
}

void CDlgSetUp::UpdateLayer()
{
	//TODO Ҷ��Զ
	CDocLock cLock;
	UpdateData(TRUE);
	if (m_originalWinSetting.m_sWinFrameLayer != m_sFrameLayerDlg)
	{
		ChangeLayer(m_originalWinSetting.m_sWinFrameLayer, m_sFrameLayerDlg);
		ChangeLayer(GSINST->m_winSetting.GetWinFrameLayerDefault(), m_sFrameLayerDlg);
	}
	if (m_originalWinSetting.m_sWinHardwareLayer != m_sHardWareLayerDlg)
	{
		ChangeLayer(m_originalWinSetting.m_sWinHardwareLayer, m_sHardWareLayerDlg);
		ChangeLayer(GSINST->m_winSetting.GetWinHardwareLayerDefault(), m_sHardWareLayerDlg);
	}
	if (m_originalWinSetting.m_sWinLayer != m_sLayerDlg)
	{
		ChangeLayer(m_originalWinSetting.m_sWinLayer, m_sLayerDlg);
		ChangeLayer(GSINST->m_winSetting.GetWinLayerDefault(), m_sLayerDlg);
	}
	if (m_originalWinSetting.m_sWinNumberLayerLimian != m_sNumberLayerLimianDlg)
	{
		ChangeLayer(m_originalWinSetting.m_sWinNumberLayerLimian, m_sNumberLayerLimianDlg);
		ChangeLayer(GSINST->m_winSetting.GetWinNumberLayerLimianDefault(), m_sNumberLayerLimianDlg);
	}
	if (m_originalWinSetting.m_sWinNumberLayerPingmian != m_sNumberLayerPingmianDlg)
	{
		ChangeLayer(m_originalWinSetting.m_sWinNumberLayerPingmian, m_sNumberLayerPingmianDlg);
		ChangeLayer(GSINST->m_winSetting.GetWinNumberLayerPingmianDefault(), m_sNumberLayerPingmianDlg);
	}
	if (m_originalWinSetting.m_sWinOpenLayer != m_sOpenLayerDlg)
	{
		ChangeLayer(m_originalWinSetting.m_sWinOpenLayer, m_sOpenLayerDlg);
		ChangeLayer(GSINST->m_winSetting.GetWinOpenLayerDefault(), m_sOpenLayerDlg);
	}
	if (m_originalWinSetting.m_sWinWallLayer != m_sWallLayerDlg)
	{
		ChangeLayer(m_originalWinSetting.m_sWinWallLayer, m_sWallLayerDlg);
		ChangeLayer(GSINST->m_winSetting.GetWallLayerDefault(), m_sWallLayerDlg);
	}
}

void CDlgSetUp::SetDigPos()
{
	CRect DlgRect, CADRect;
	GetWindowRect(DlgRect);
	GetParent()->GetWindowRect(CADRect);
	int xPos, yPos;
	xPos = (CADRect.Width() - DlgRect.Width()) / 2;
	yPos = (CADRect.Height() - DlgRect.Height()) / 2;

	SetWindowPos(NULL, xPos, yPos, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}

void CDlgSetUp::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

LRESULT CDlgSetUp::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

CDlgSetUp* g_winSetupDlg = NULL;

void OpenWindowSetUpDlg()
{
	if (g_winSetupDlg == NULL)
	{
		CAcModuleResourceOverride resOverride;
		g_winSetupDlg = new CDlgSetUp(acedGetAcadFrame());
		g_winSetupDlg->Create(IDD_DIALOG_SETUP);
	}
	g_winSetupDlg->ShowWindow(SW_SHOW);
}

BOOL CloseWindowSetUpDlg()
{
	if (g_winSetupDlg == NULL)
		return TRUE;
	BOOL ret = g_winSetupDlg->DestroyWindow();
	if (ret)
	{
		delete g_winSetupDlg;
		g_winSetupDlg = NULL;
	}
	return ret;
}