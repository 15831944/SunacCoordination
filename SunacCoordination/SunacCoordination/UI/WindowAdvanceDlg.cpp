// UI\WindowAdvanceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WindowAdvanceDlg.h"
#include "afxdialogex.h"


// CWindowAdvanceDlg �Ի���

IMPLEMENT_DYNAMIC(CWindowAdvanceDlg, CAcUiDialog)

CWindowAdvanceDlg::CWindowAdvanceDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowAdvanceDlg::IDD, pParent)
{

}

CWindowAdvanceDlg::~CWindowAdvanceDlg()
{
}

void CWindowAdvanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWindowAdvanceDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CWindowAdvanceDlg ��Ϣ�������
