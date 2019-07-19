// UI\RailingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RailingDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"


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

	m_preRailing.SubclassDlgItem(IDC_STATIC_PREVIEW, this);
	m_preRailing.Init(theArxDLL.ModuleResourceInstance(), true);
	m_preRailing.SetDwgFile(allRailingFiles[1]);

	m_railingInfo.SetWindowText(_T("������Ϣ˵��\r\n���˼��\r\n��Ԫ�ߴ�\r\n�������ͣ�"));

	return TRUE;
}
