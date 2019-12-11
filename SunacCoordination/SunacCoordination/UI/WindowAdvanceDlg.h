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

// �Ի�������
	enum { IDD = IDD_DIALOG_WINDOW_ADVANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();


	virtual void PostNcDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);

	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG *pMsg); //����Ӧ�س�

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedSelectOnDwg();
	afx_msg void OnBnClickedAuxiliaryFrame();
	void InitHeatCoeffCtrl(); //��ʼ������ϵ��
	void InitGlassSeries(); //����ѡ��
	void InitAluminumSeries(); //�Ͳ�ϵ��
	void InitMaterialType(); //��������

	void InitPlugSlotSize(); //����ߴ�
	void InitAuxiliaryFrame(); //����
	void InitAuxiliaryFrameSeries();

	void InitAllOption();//������ϵ�����������еĳ�ʼ���ڴ˺����е���

	vector<AttrWindow*> m_selAttrWindows;

	CString m_sCode;

	//CComboBox m_jieNeng;
	CEdit m_eidtJieneng;
	double m_fJieNeng;// ����ϵ��

	CComboBox m_comboCaizhi;
	CComboBox m_comboXingcai;
	CComboBox m_comboBoli;

	int m_bFuKuang;
	CComboBox m_comboFuKuangType;
	CComboBox m_comboSaifeng; //����ߴ�
	CButton m_checkFukuang;
};

extern CWindowAdvanceDlg* g_windowAdvanceDlg;
void OpenWindowAdvanceDlg();
BOOL CloseWindowAdvanceDlg();
