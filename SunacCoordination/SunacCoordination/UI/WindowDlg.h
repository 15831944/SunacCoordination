#pragma once
#include "GridCtrlWithPreview.h"

// CWindowDlg �Ի���

class CWindowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWindowDlg)

public:
	CWindowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWindowDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlWithPreview m_preWindow;
};
