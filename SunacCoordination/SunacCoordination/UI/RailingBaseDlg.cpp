// UI\RailingBaseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RailingBaseDlg.h"
#include "afxdialogex.h"


// CRailingBaseDlg �Ի���

IMPLEMENT_DYNAMIC(CRailingBaseDlg, CDialogEx)

CRailingBaseDlg::CRailingBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRailingBaseDlg::IDD, pParent)
{

}

CRailingBaseDlg::~CRailingBaseDlg()
{
}

void CRailingBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_RAILINGBASE, m_preRailingBase);
}


BEGIN_MESSAGE_MAP(CRailingBaseDlg, CDialogEx)
END_MESSAGE_MAP()


// CRailingBaseDlg ��Ϣ�������
