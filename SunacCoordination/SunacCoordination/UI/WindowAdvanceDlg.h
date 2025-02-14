#pragma once
#include "afxwin.h"
#include "../Object/WindowDoor/AttrWindow.h"

// CWindowAdvanceDlg 对话框

class CWindowAdvanceDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CWindowAdvanceDlg)

public:
	CWindowAdvanceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWindowAdvanceDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WINDOW_ADVANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();


	virtual void PostNcDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);

	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG *pMsg); //不响应回车

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedSelectOnDwg();
	afx_msg void OnBnClickedAuxiliaryFrame();
	void InitHeatCoeffCtrl(); //初始化传热系数
	void InitGlassSeries(); //玻璃选型
	void InitAluminumSeries(); //型材系列
	void InitMaterialType(); //材质类型

	void InitPlugSlotSize(); //塞缝尺寸
	void InitAuxiliaryFrame(); //附框
	void InitAuxiliaryFrameSeries();

	void InitAllOption();//将传热系数到附框所有的初始化在此函数中调用

	vector<AttrWindow*> m_selAttrWindows;

	CString m_sCode;

	//CComboBox m_jieNeng;
	CEdit m_eidtJieneng;
	double m_fJieNeng;// 节能系数

	CComboBox m_comboCaizhi;
	CComboBox m_comboXingcai;
	CComboBox m_comboBoli;

	int m_bFuKuang;
	CComboBox m_comboFuKuangType;
	CComboBox m_comboSaifeng; //塞缝尺寸
	CButton m_checkFukuang;
};

extern CWindowAdvanceDlg* g_windowAdvanceDlg;
void OpenWindowAdvanceDlg();
BOOL CloseWindowAdvanceDlg();

