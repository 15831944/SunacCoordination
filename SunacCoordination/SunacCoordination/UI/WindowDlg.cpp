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

	m_preWindow.SetRowCount(8);
	m_preWindow.SetColumnCount(2);

	//����Ԥ��ͼ��ʾ����Ϊ3��1��
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);

	//���õ�һ��ռ60%�����ಿ��Ϊ�ڶ���
	m_preWindow.SetHeaderWidth(_T("60;+"));

	m_preWindow.AddPreview(0, 0, _T("E:\\Sunac_Kitchen_KUq_10_1.dwg")); //������dwg�ļ�
	m_preWindow.AddPreview(1, 0, _T("E:\\1.dwg")); //������dwg�ļ�
	m_preWindow.AddPreview(2, 0, _T("E:\\2.dwg")); //������dwg�ļ�
	m_preWindow.AddPreview(3, 0, _T("E:\\3.dwg")); //������dwg�ļ�
	m_preWindow.AddPreview(4, 0, _T("E:\\4.dwg")); //������dwg�ļ�
	m_preWindow.AddPreview(5, 0, _T("E:\\5.dwg")); //������dwg�ļ�
	m_preWindow.AddPreview(6, 0, _T("E:\\6.dwg")); //������dwg�ļ�
	m_preWindow.AddPreview(7, 0, _T("E:\\7.dwg")); //������dwg�ļ�
	m_preWindow.AddPreview(8, 0, _T("E:\\8.dwg")); //������dwg�ļ�
	m_preWindow.SetContentItemText(0, 1, _T("�����ͣ�˫�ȵ���\n���������2.1\nͨ������1.6"));
	m_preWindow.ShowPreviews();

	return TRUE;
}