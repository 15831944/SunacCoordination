// UI\MoldingsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MoldingsDlg.h"
#include "afxdialogex.h"


// CMoldingsDlg �Ի���

IMPLEMENT_DYNAMIC(CMoldingsDlg, CAcUiDialog)

CMoldingsDlg::CMoldingsDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CMoldingsDlg::IDD, pParent)
{

}

CMoldingsDlg::~CMoldingsDlg()
{
}

void CMoldingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_MOLDINGS, m_preMoldings);
}


BEGIN_MESSAGE_MAP(CMoldingsDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CMoldingsDlg ��Ϣ�������


BOOL CMoldingsDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	return TRUE;
}