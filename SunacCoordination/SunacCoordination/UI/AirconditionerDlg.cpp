m_airConPrototypeFile
m_airConPrototypeFile
m_airConPrototypeFile
m_airConHorseNumber
m_airConHorseNumber
m_airConPrototypeId
m_airConPrototypeId
m_airConPrototypeFile
// UI\AirconditionerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AirconditionerDlg.h"
#include "afxdialogex.h"
#include "../WebIO/WebIO.h"
#include "../GlobalSetting.h"
#include "../WebIO/LocalData.h"
#include "../Object/AttrAirCon.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCAirCondition.h"
#include "../GlobalSetting.h"
#include "../Common/ComFun_Str.h"


// CAirconditionerDlg �Ի���

IMPLEMENT_DYNAMIC(CAirconditionerDlg, CAcUiDialog)

CAirconditionerDlg::CAirconditionerDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CAirconditionerDlg::IDD, pParent)
{

}

CAirconditionerDlg::~CAirconditionerDlg()
{
}

LRESULT CAirconditionerDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	//return FALSE;
	return TRUE;
}

void CAirconditionerDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PNUM, m_pNum);
	DDX_Control(pDX, IDC_COMBO_LNTUBEPOS, m_lNTubePos);
	DDX_Control(pDX, IDC_CHECK_HASRAINTUBE, m_hasRainTube);
	DDX_Control(pDX, IDC_COMBO_RAINTUBEPOS, m_rainTubePos);
	//DDX_Control(pDX, IDC_STATIC_AC, m_preAirCon);
	DDX_Control(pDX, IDC_BUTTON_INSERTAC, m_btnInbsert);
	DDX_Control(pDX, IDC_CHECK_UPDOWNIMAGE, m_upDownImage);
	DDX_Control(pDX, IDC_CHECK_LEFTRIGHTIMAGE, m_leftRightImage);
}


BEGIN_MESSAGE_MAP(CAirconditionerDlg, CAcUiDialog)
//	ON_CBN_SELCHANGE(IDC_COMBO_PNUM, &CAirconditionerDlg::OnCbnSelchangeComboPnum)
	ON_BN_CLICKED(IDC_CHECK_HASRAINTUBE, &CAirconditionerDlg::OnBnClickedCheckHasraintube)
	ON_CBN_SELCHANGE(IDC_COMBO_PNUM, &CAirconditionerDlg::OnCbnSelchangeComboPnum)
	ON_CBN_SELCHANGE(IDC_COMBO_LNTUBEPOS, &CAirconditionerDlg::OnCbnSelchangeComboLntubepos)
	ON_CBN_SELCHANGE(IDC_COMBO_RAINTUBEPOS, &CAirconditionerDlg::OnCbnSelchangeComboRaintubepos)
	ON_BN_CLICKED(IDC_BUTTON_INSERTAC, &CAirconditionerDlg::OnBnClickedButtonInsertac)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()


// CAirconditionerDlg ��Ϣ�������


BOOL CAirconditionerDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	//������
	//m_preAC.SubclassDlgItem(IDC_STATIC_AC, this);
	//m_preAC.SetLayoutMode(PREVIEW_LAYOUT_HORIZONTAL);
	//m_preAC.SetPreview(_T("D:\\Projects\\Sunac\\Support\\Sunac2019\\LocalMode\\Window_N_7_0.dwg"));
	//m_preAC.SetText(_T("Ԥ��ͼ"));
	//m_preAC.Invalidate();

	TYUI_Disable(m_rainTubePos); //Ĭ�ϻҵ���ˮ��λ��������

	LoadDefaultValue(); //����������Ĭ��ֵ

	m_preAirCon.SubclassDlgItem(IDC_STATIC_AC, this);		// �ؼ�����Դ�Ĺ���
	m_preAirCon.Init(theArxDLL.ModuleResourceInstance(), true);

	UpdatePreview();
   // Air_GetPiShus();
	return TRUE;
}

//���ÿյ��Ի����пؼ���Ĭ��ֵ
void CAirconditionerDlg::LoadDefaultValue()
{
	const vCString& pNum = WebIO::GetConfigDict()->Air_GetPiShus();
	const vCString& lNTubePos = WebIO::GetConfigDict()->Air_GetLengNingShuiGuanPos();
	const vCString& rainTubePos = WebIO::GetConfigDict()->Air_GetYuShuiGuanPos();

	TYUI_InitComboBox(m_pNum, pNum, pNum.empty() ? _T("") : pNum[0]);
	TYUI_InitComboBox(m_lNTubePos, lNTubePos, lNTubePos.empty()? _T("") : lNTubePos[0]);
	TYUI_InitComboBox(m_rainTubePos, rainTubePos, rainTubePos.empty() ? _T("") : rainTubePos[0]);

	UpdateData(FALSE);
}

void CAirconditionerDlg::OnBnClickedCheckHasraintube()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int state =((CButton *)GetDlgItem(IDC_CHECK_HASRAINTUBE))->GetCheck(); 

	/*�����˸�ѡ���״̬��ֵ��state
	��state == BST_UNCHECKED ʱ��ʾ�ø�ѡ��û�б�ѡ�У�
	��state == BST_CHECKED ʱ��ʾ�ø�ѡ��ѡ�У�
	��state == BST_INDETERMINATE ʱ��ʾ��ȷ��*/


	if (state == BST_CHECKED )  //������˸�ѡ����ѡ��״̬����ô��ͨ���������ڶ��������ɸѡ
	{
		TYUI_Enable(m_rainTubePos);
	}
	else
	{
		TYUI_Disable(m_rainTubePos);
	}

	UpdatePreview();
}

void CAirconditionerDlg::UpdatePreview() //���յ��Ի����еĿؼ���ֵ�����仯ʱ��ͼ��ҲҪ���ݿؼ���ֵ��ɸѡ���仯
{
	UpdateData(FALSE);

	//��ȡ�ؼ�ComboBox��ѡ��ֵ
	CString pNum = TYUI_GetComboBoxText(m_pNum);
	CString lNTubePos = TYUI_GetComboBoxText(m_lNTubePos);
	CString rainTubePos = TYUI_GetComboBoxText(m_rainTubePos);
	int hasRainTube = m_hasRainTube.GetCheck();
	CString strHasTube = hasRainTube > 0 ? L"��" : L"��";

	//����GetAirCon()������ɸѡ���������Ŀյ�
	m_allAirCons = CLocalData::GetInstance()->GetAirCon(pNum, lNTubePos, strHasTube, rainTubePos);

	//��δ���ҵ����������Ŀյ�ʱ���Ի����Ҳ�ͼ��Ϊ�գ����Ҳ��밴ť���
	if (m_allAirCons.empty())
	{
		acutPrintf(_T("δ�ҵ����������ļ�¼\n"));
		m_preAirCon.SetDatabase(NULL);
		m_btnInbsert.EnableWindow(FALSE);
		return;
	}

	Acad::ErrorStatus es=acDocManager->lockDocument(curDoc());
	AcDbDatabase *pDatabase = new AcDbDatabase();
	//ͼ���ļ�·��
	m_filePathName = MD2010_GetAppPath() + L"\\support\\Sunac2019\\LocalMode\\" + m_allAirCons[0].m_airConPrototypeFile;
	es = pDatabase->readDwgFile(m_filePathName);
	//DrawSolid(zhu, pDatabase,false);
	m_preAirCon.SetDatabase(pDatabase);
	acDocManager->unlockDocument(curDoc());
	m_btnInbsert.EnableWindow(TRUE);
}

void CAirconditionerDlg::OnCbnSelchangeComboPnum()
{
	UpdatePreview();
}

void CAirconditionerDlg::OnCbnSelchangeComboLntubepos()
{
	UpdatePreview();
}

void CAirconditionerDlg::OnCbnSelchangeComboRaintubepos()
{
	UpdatePreview();
}


void CAirconditionerDlg::OnBnClickedButtonInsertac()
{
	ShowWindow(FALSE);
	
	//��ȡ�����
	AcGePoint3d pnt = TY_GetPoint();

	RCAirCondition blockAirCon;
	//�������ͼ�οռ�
	blockAirCon.Insert(m_filePathName, pnt, 0, L"0", 256);

	//���¾���
	if (m_upDownImage.GetCheck())
		TYCOM_MirrorOneObject(blockAirCon.m_id, pnt, AcGeVector3d(1,0,0));
	//���Ҿ���
	if (m_leftRightImage.GetCheck())
		TYCOM_MirrorOneObject(blockAirCon.m_id, pnt, AcGeVector3d(0,1,0));
	//�������Ҿ���
	if (m_upDownImage.GetCheck() && m_leftRightImage.GetCheck())
	{
		TYCOM_MirrorRotate(blockAirCon.m_id, pnt, 3.1415926);
	}

	//��UI�����ݼ�¼��ͼ�����չ�ֵ���
	AttrAirCon * pAirCon = new AttrAirCon(m_allAirCons[0]);
	blockAirCon.AddAttribute(pAirCon);

	pAirCon->close();
	  
	ShowWindow(TRUE);
}

