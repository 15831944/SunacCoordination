// UI\FillingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FillingDlg.h"
#include "afxdialogex.h"


// CFillingDlg �Ի���

IMPLEMENT_DYNAMIC(CFillingDlg, CDialogEx)

CFillingDlg::CFillingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFillingDlg::IDD, pParent)
{

}

CFillingDlg::~CFillingDlg()
{
}

void CFillingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_FILLING, m_preFilling);
}


BEGIN_MESSAGE_MAP(CFillingDlg, CDialogEx)
END_MESSAGE_MAP()


// CFillingDlg ��Ϣ�������


BOOL CFillingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}