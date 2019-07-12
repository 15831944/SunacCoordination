// UI\WindowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WindowDlg.h"
#include "afxdialogex.h"


// CWindowDlg �Ի���

IMPLEMENT_DYNAMIC(CWindowDlg, CDialogEx)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindowDlg::IDD, pParent)
{

}

CWindowDlg::~CWindowDlg()
{
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_WINDOW, m_preWindow);
}


BEGIN_MESSAGE_MAP(CWindowDlg, CDialogEx)
END_MESSAGE_MAP()


// CWindowDlg ��Ϣ�������


BOOL CWindowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_preWindow.LoadDefaltSettings();

	m_preWindow.SetRowCount(4);
	m_preWindow.SetColumnCount(2);
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(2);

	m_preWindow.AddPreview(0, 0, _T("C:\\Daqo\\LogicDwg\\1��������.dwg")); //������dwg�ļ�
	m_preWindow.SetContentItemText(0, 1, _T("�����ͣ�˫�ȵ���\n���������2.1\nͨ������1.6"));
	m_preWindow.ShowPreviews();

	return TRUE;
}