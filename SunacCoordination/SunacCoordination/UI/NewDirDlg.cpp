// NewDirDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewDirDlg.h"
#include "afxdialogex.h"


// CNewDirDlg �Ի���

IMPLEMENT_DYNAMIC(CNewDirDlg, CAcUiDialog)

CNewDirDlg::CNewDirDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CNewDirDlg::IDD, pParent)
{

}

CNewDirDlg::~CNewDirDlg()
{
}

void CNewDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NewDir, m_NewDirEdit);
}


BEGIN_MESSAGE_MAP(CNewDirDlg, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CNewDirDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewDirDlg ��Ϣ�������


void CNewDirDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_NewDirEdit.GetWindowTextW(m_sNewDir);
	CNewDirDlg::OnOK();
}
