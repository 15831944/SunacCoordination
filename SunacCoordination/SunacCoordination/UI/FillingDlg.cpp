// UI\FillingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FillingDlg.h"
#include "afxdialogex.h"


// CFillingDlg �Ի���

IMPLEMENT_DYNAMIC(CFillingDlg, CAcUiDialog)

CFillingDlg::CFillingDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CFillingDlg::IDD, pParent)
{

}

CFillingDlg::~CFillingDlg()
{
}

void CFillingDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_FILLING, m_preFilling);
}


BEGIN_MESSAGE_MAP(CFillingDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CFillingDlg ��Ϣ�������


BOOL CFillingDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	return TRUE;
}