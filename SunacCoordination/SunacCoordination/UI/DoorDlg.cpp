// UI\DoorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DoorDlg.h"
#include "afxdialogex.h"


// CDoorDlg �Ի���

IMPLEMENT_DYNAMIC(CDoorDlg, CAcUiDialog)

CDoorDlg::CDoorDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CDoorDlg::IDD, pParent)
{

}

CDoorDlg::~CDoorDlg()
{
}

void CDoorDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDoorDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CDoorDlg ��Ϣ�������


BOOL CDoorDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	return TRUE;
}