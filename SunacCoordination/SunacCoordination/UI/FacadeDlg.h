#pragma once
#include "GridCtrl_src\GridCtrl.h"

// CFacadeDlg �Ի���

class CFacadeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFacadeDlg)

public:
	CFacadeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFacadeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FACADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrl m_tFacade;
};
