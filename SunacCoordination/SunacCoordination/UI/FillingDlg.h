#pragma once
#include "gridctrlwithpreview.h"


// CFillingDlg �Ի���

class CFillingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFillingDlg)

public:
	CFillingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFillingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FILLING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlWithPreview m_preFilling;
};
