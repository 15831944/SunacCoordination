#pragma once
#include "PreviewWithDetail.h"

// CAirconditionerDlg �Ի���

class CAirconditionerDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CAirconditionerDlg)

public:
	CAirconditionerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAirconditionerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_AIRCONDITIONER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CPreviewWithDetail m_preAC;
public:
	CComboBox m_pNum;		//ƥ��
	CComboBox m_lNTubePos;	//����ˮ��λ��
	CButton m_hasRainTube;	//�Ƿ�����ˮ���ܴ���
	CComboBox m_rainTubePos;//��ˮ��λ��
};
