// UI\WaterproofDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WaterproofDlg.h"
#include "afxdialogex.h"


// CWaterproofDlg �Ի���

IMPLEMENT_DYNAMIC(CWaterproofDlg, CAcUiDialog)

CWaterproofDlg::CWaterproofDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWaterproofDlg::IDD, pParent)
{

}

CWaterproofDlg::~CWaterproofDlg()
{
}

void CWaterproofDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaterproofDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CWaterproofDlg ��Ϣ�������


BOOL CWaterproofDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	return TRUE;
}