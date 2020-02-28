#pragma once
#include "afxwin.h"
#include "ComFun_MFC.h"
#include "GridCtrlWithPreview.h"
#include "../Object/Railing/AttrRailing.h"
#include "D:\vs2010\VC\atlmfc\include\afxwin.h"
#include "D:\vs2010\VC\atlmfc\include\afxwin.h"

// CRailingDlg �Ի���

class CRailingDlg : public CAcUiDialog
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

	//��ģ̬�Ի�����غ���
	virtual void PostNcDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01

	virtual BOOL PreTranslateMessage(MSG *pMsg); //����Ӧ�س�

public:
	afx_msg void OnBnClickedInsertToCAD();
	afx_msg void OnBnClickedButtonSelectline();
	afx_msg void OnCbnSelchangeComboRailingtype();
	afx_msg void OnBnClickedCheckAutoindex();
	afx_msg void OnSelChangedPreview(NMHDR *pNMHDR, LRESULT *pResult);
	void UpdateRailingToGrid(eRailingType p_railetype);
	CString RailingSize(int i);

	void SetEditMode(AcDbBlockReference* pBlock);

	CGridCtrlWithPreview m_preRailing;
	CComboBox m_comboRailingType;
	int m_height;// ���˸߶�	
	int m_width;// ���˳���
	CEdit m_editRailingID;
	CString m_sRailingId;
	BOOL m_bRailingAutoName;// �Զ����

	AcDbBlockReference* m_pCurEdit;
	CComboBox m_comboViewDir;

	afx_msg void OnCbnSelchangeComboViewdir();
};

extern CRailingDlg* g_railingDlg;
void OpenRailingDlg(AcDbBlockReference* pCurEdit = NULL);
BOOL CloseRailingDlg();