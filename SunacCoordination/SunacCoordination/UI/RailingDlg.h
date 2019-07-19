#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"

// CRailingDlg �Ի���

class CRailingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRailingDlg)

public:
	CRailingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRailingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RAILING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CGsPreviewCtrl m_preRailing; //�·�Ԥ�������ڵ�Ԥ��ͼ
	CGsPreviewCtrl m_preStyle; //����ѡ��Ԥ��ͼ
	CEdit m_railingInfo;
	CEdit m_width; //���ڿ��
	CEdit m_height; //�����ܸ�
	CEdit m_reverse; //�����߶�
};
