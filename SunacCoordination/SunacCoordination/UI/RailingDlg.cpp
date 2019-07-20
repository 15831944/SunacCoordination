// UI\RailingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RailingDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "RailingBaseDlg.h"

// CRailingDlg �Ի���

IMPLEMENT_DYNAMIC(CRailingDlg, CDialogEx)

CRailingDlg::CRailingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRailingDlg::IDD, pParent)
{

}

CRailingDlg::~CRailingDlg()
{
}

void CRailingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RAILINGINFO, m_railingInfo);
	DDX_Control(pDX, IDC_EDIT_HOLEWIDTH, m_width);
	DDX_Control(pDX, IDC_EDIT_RAILINGHEIGHT, m_height);
	DDX_Control(pDX, IDC_EDIT_REVERSERIDGE_HEIGHT, m_reverse);
}

BEGIN_MESSAGE_MAP(CRailingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON_LIB, &CRailingDlg::OnBnClickedMfcbuttonLib)
	ON_BN_CLICKED(IDC_MFCBUTTON_SELECTLINE, &CRailingDlg::OnBnClickedMfcbuttonSelectline)
	ON_BN_CLICKED(IDC_MFCBUTTON_CLOSE, &CRailingDlg::OnBnClickedMfcbuttonClose)
END_MESSAGE_MAP()


// CRailingDlg ��Ϣ�������


BOOL CRailingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	vCString allRailingFiles;
	TY_GetAllKitchenFiles(allRailingFiles); //��ʱ�ó���ͼֽ����

	m_preStyle.SubclassDlgItem(IDC_STATIC_STYLE, this);
	m_preStyle.Init(theArxDLL.ModuleResourceInstance(), true);
	m_preStyle.SetDwgFile(allRailingFiles[0]);

	m_railingInfo.SetWindowText(_T("������Ϣ˵��\r\n���˼��\r\n��Ԫ�ߴ�\r\n�������ͣ�"));

	return TRUE;
}

void CRailingDlg::OnBnClickedMfcbuttonLib()
{
	CRailingBaseDlg dlg;
	dlg.DoModal();
}


void CRailingDlg::OnBnClickedMfcbuttonSelectline()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CRailingDlg::OnBnClickedMfcbuttonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
