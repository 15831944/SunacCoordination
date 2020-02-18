#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "ComFun_MFC.h"
#include "../Common/TYRect.h"

// CWindowDlg �Ի���

class CWindowDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CWindowDlg)

public:
	CWindowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWindowDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();


	virtual void PostNcDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01

	virtual BOOL PreTranslateMessage(MSG *pMsg); //����Ӧ�س�

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonSearchwindow();
	afx_msg void OnBnClickedRadioWindowDoor();
	afx_msg void OnBnClickedCalculate();
	afx_msg void OnBnClickedAutoIndex();
	afx_msg void OnBnClickedSelOnDwg();
	afx_msg void OnBnClickedMirror();
	afx_msg void OnSelChangedPreview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelChangedW1();
	afx_msg void OnSelChangedH2();
	afx_msg void OnSelChangedW3();
	afx_msg void OnSelChangedH3();
	afx_msg void OnSelChangedView();
	afx_msg void OnBnClickedBayWindow();
	afx_msg void OnSelChangedWallDis();

	void ClearPrototypes();
	void WindowDoorChange();
	void UpdateInstanceCode();
	void UpdatePrototypeAirVolume(); //�����Ҳ��ԭ����Ϣ�е�ͨ����
	void UpdateDimDataToComboBox(CComboBox& comboBox, const AttrWindow& attrWindow, CString code);
	void InitDimComboBoxInt(CComboBox& comboBox, vdouble options, double dimValue);

	void InitCombOptions();
	AttrWindow* GetSelWindow();

	void SetEditMode(AcDbObjectId editId);

	double GetArea()const { return (double(m_nWidth*m_nHeight)) / 1e6; }
	bool CheckValueModulo(CComboBox& comboBox, CString p_sType, int p_value, int moduloVale = 50); //��������Ƿ���50��ģ��

	void InitPreviewGridByWindowPrototypes();

	//����ȫ����������
	void InsertAllWindows_Test();
protected:
	CGridCtrlWithPreview m_preWindow;

	int m_radioDoorWindow; //�Ŵ���ѡ��

	int m_nWidth;
	int m_nHeight;
	int m_nThickness;	//ǽ��

	int m_radioBayWindow; //�Ƿ�͹��

	//ɸѡ����
	CComboBox m_comboAreaType; //����������
	CComboBox m_comboOpenType; //��������
	CComboBox m_comboOpenAmount; //����������
	CEdit m_editVentilation; //ͨ����
	BOOL m_isFireproof; //�Ƿ����
	
	BOOL m_bAutoNumber; //�Ƿ��Զ��Ŵ����
	CEdit m_editWinNumber; //�Ŵ����
	CComboBox m_comboW1; //�����ȿ��
	CComboBox m_comboH2; //�¹̶�ֵ
	CComboBox m_comboW3;	//ת�ǿ��
	CComboBox m_comboH3; //����ǽ��
	CComboBox m_comboOutWallDistance; //����ǽ����	

	CComboBox m_comboViewDir;
	CComboBox m_comboInsertDir;
	CButton m_btnMirror;


	//////////////////////////////////////////////////////////////////////////
	TYRect m_selectRect;
	vector<AttrWindow> m_winPrototypes;  //�����������������Ŵ�ԭ��

	bool m_bEditMode;
	AttrWindow m_attBeforeEdit; //�༭ǰ���Ŵ�����
	AcDbObjectId m_curEditWinId;


	bool m_bHasInsert; //�Ƿ���ִ�й��������	
};

extern CWindowDlg* g_windowDlg;
void OpenWindowDlg(AcDbObjectId editId = AcDbObjectId::kNull);
BOOL CloseWindowDlg();
