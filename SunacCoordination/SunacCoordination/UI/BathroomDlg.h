#pragma once
#include "GridCtrlWithPreview.h"

// CBathroomDlg �Ի���

class CBathroomDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CBathroomDlg)

public:
	CBathroomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBathroomDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BATHROOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CGridCtrlWithPreview m_preBathroom;
};
