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
	//������
	m_preAC.SubclassDlgItem(IDC_STATIC_AC, this);
	m_preAC.SetLayoutMode(PREVIEW_LAYOUT_HORIZONTAL);
	m_preAC.SetPreview(_T("D:\\Projects\\Sunac\\Support\\Sunac2019\\LocalMode\\Window_N_7_0.dwg"));
	m_preAC.SetText(_T("Ԥ��ͼ"));
	m_preAC.Invalidate();
	return TRUE;
}