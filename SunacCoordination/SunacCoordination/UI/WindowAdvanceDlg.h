#pragma once
#include "afxwin.h"
#include "../Object/WindowDoor/AttrWindow.h"

// CWindowAdvanceDlg �Ի���

class CWindowAdvanceDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CWindowAdvanceDlg)

public:
	CWindowAdvanceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWindowAdvanceDlg();

	virtual INT_PTR DoModal();

// �Ի�������
	enum { IDD = IDD_DIALOG_WINDOW_ADVANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //�ͷŷ�ģ̬�Ի����ڴ�

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedSelectOnDwg();
	afx_msg void OnBnClickedAuxiliaryFrame();
	void InitGlassSeries(); //����ѡ��
	void InitAluminumSeries(); //�Ͳ�ϵ��
	void InitMaterialType(); //��������
	void InitPlugSlotSize(); //����ߴ�
	void InitAuxiliaryFrame(); //����
	void InitAuxiliaryFrameSeries();

	bool m_isMoldless;	//�Ƿ��ģ̬�Ի���

	vector<AttrWindow*> m_selAttrWindows;

	CString m_sCode;
	CComboBox m_jieNeng;
	CComboBox m_caiZhi;
	CComboBox m_xingCai;
	CComboBox m_boLi;
	int m_bFuKuang;
	CComboBox m_fuKuangType;
	CComboBox m_saiFeng;
};

extern CWindowAdvanceDlg* g_windowAdvanceDlg;
BOOL CloseWindowAdvanceDlg();
