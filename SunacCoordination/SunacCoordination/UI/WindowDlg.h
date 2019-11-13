#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "ComFun_MFC.h"

// CWindowDlg �Ի���

class CWindowDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CWindowDlg)

public:
	CWindowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWindowDlg();

	virtual INT_PTR DoModal();

// �Ի�������
	enum { IDD = IDD_DIALOG_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //�ͷŷ�ģ̬�Ի����ڴ�
	virtual BOOL PreTranslateMessage(MSG *pMsg); //����Ӧ�س�

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonSearchwindow();
	afx_msg void OnBnClickedRadioDoor();
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
	void UpdateEnable();
	void UpdateInstanceCode();
	void UpdateVent();
	void UpdateDimDataToComboBox(CComboBox& comboBox, const AttrWindow& attrWindow, CString code);
	void InitDimComboBoxInt(CComboBox& comboBox, vdouble options, double dimValue);
	void LoadDefaultValue();
	AttrWindow* GetSelWindow();

	bool CheckValueModulo(CComboBox& comboBox, CString p_sType, int p_value); //��������Ƿ���50��ģ��

	void SetEditMode(AcDbBlockReference* pBlock);

	//����ȫ����������
	void InsertAllWindows();

	double GetArea()const { return (double(m_nWidth*m_nHeight)) / 1e6; }

protected:

	CGridCtrlWithPreview m_preWindow;

	int m_nWidth;
	int m_nHeight;

	CComboBox m_areaType; //����������
	CComboBox m_openType; //��������
	CComboBox m_openAmount; //����������
	CEdit m_ventilation; //ͨ����

	//CEdit m_area; //���
	//CComboBox m_rate; //��ֵ

	CEdit m_number; //�Ŵ����
	CComboBox m_comboW1; //�����ȿ��
	CComboBox m_comboH2; //�¹̶�ֵ
	CComboBox m_comboW3;	//ת�ǿ��
	CComboBox m_comboH3; //����ǽ��
	CComboBox m_distance; //����ǽ����
	CComboBox m_insertDir;
	
	int m_radioDoor; //�Ŵ���ѡ
	int m_radioYes; //�Ƿ�͹��

	BOOL m_autoIndex;
	CComboBox m_viewDir;
	CButton m_isMirror;

	vector<AttrWindow> m_allWindows;

	AcDbBlockReference* m_pCurEdit;

	bool m_isMoldless;//�Ƿ��ģ̬�Ի���
public:
};

extern CWindowDlg* g_windowDlg;
BOOL CloseWindowDlg();
