// UI\RailingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RailingDlg.h"
#include "afxdialogex.h"


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
}

BEGIN_MESSAGE_MAP(CRailingDlg, CDialogEx)
END_MESSAGE_MAP()


// CRailingDlg ��Ϣ�������


BOOL CRailingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}
