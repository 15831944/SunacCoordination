// UI\AirconditionerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AirconditionerDlg.h"
#include "afxdialogex.h"


// CAirconditionerDlg �Ի���

IMPLEMENT_DYNAMIC(CAirconditionerDlg, CAcUiDialog)

CAirconditionerDlg::CAirconditionerDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CAirconditionerDlg::IDD, pParent)
{

}

CAirconditionerDlg::~CAirconditionerDlg()
{
}

void CAirconditionerDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAirconditionerDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CAirconditionerDlg ��Ϣ�������


BOOL CAirconditionerDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	return TRUE;
}