#pragma once
#include "gridctrlwithpreview.h"


// CRailingBaseDlg �Ի���

class CRailingBaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRailingBaseDlg)

public:
	CRailingBaseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRailingBaseDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RAILINGBASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlWithPreview m_preRailingBase;
};
