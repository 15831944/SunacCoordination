#pragma once
#include "gridctrlwithpreview.h"


// CMoldingsDlg �Ի���

class CMoldingsDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CMoldingsDlg)

public:
	CMoldingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMoldingsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MOLDINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlWithPreview m_preMoldings;
};
