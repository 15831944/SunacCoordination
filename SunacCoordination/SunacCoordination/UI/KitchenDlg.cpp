// UI\KitchenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KitchenDlg.h"
#include "afxdialogex.h"


// CKitchenDlg �Ի���

IMPLEMENT_DYNAMIC(CKitchenDlg, CDialogEx)

CKitchenDlg::CKitchenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKitchenDlg::IDD, pParent)
{

}

CKitchenDlg::~CKitchenDlg()
{
}

void CKitchenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_KITCHEN, m_preKitchen);
}

BEGIN_MESSAGE_MAP(CKitchenDlg, CDialogEx)
END_MESSAGE_MAP()


// CKitchenDlg ��Ϣ�������


BOOL CKitchenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}
