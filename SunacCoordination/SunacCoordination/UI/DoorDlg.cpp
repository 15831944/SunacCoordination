// UI\DoorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DoorDlg.h"
#include "afxdialogex.h"


// CDoorDlg �Ի���

IMPLEMENT_DYNAMIC(CDoorDlg, CDialogEx)

CDoorDlg::CDoorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDoorDlg::IDD, pParent)
{

}

CDoorDlg::~CDoorDlg()
{
}

void CDoorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDoorDlg, CDialogEx)
END_MESSAGE_MAP()


// CDoorDlg ��Ϣ�������


BOOL CDoorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}