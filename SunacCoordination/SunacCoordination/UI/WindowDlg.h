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
	void UpdateEnable();
	void UpdateInstanceCode();
	void LoadDefaultValue();
	AttrWindow* GetSelWindow();

protected:

	CGridCtrlWithPreview m_preWindow;

	CEdit m_width; //���
	CEdit m_height; //�߶�
	CComboBox m_doorType; //������
	CComboBox m_areaType; //����������
	CComboBox m_openType; //��������
	CComboBox m_openAmount; //����������
	CEdit m_ventilation; //ͨ����
	CEdit m_area; //���
	CComboBox m_rate; //��ֵ

	CEdit m_number; //�Ŵ����
	CComboBox m_W1; //�����ȿ��
	CComboBox m_H2; //�¹̶�ֵ
	CComboBox m_distance; //����ǽ����
	
	int m_radioDoor; //�Ŵ���ѡ
	int m_radioYes; //�Ƿ�͹��

	vector<AttrWindow> m_allWindows;
public:
	BOOL m_autoIndex;
	CComboBox m_viewDir;
	CButton m_isMirror;

	bool m_isMoldless;//�Ƿ��ģ̬�Ի���
};

extern CWindowDlg* g_windowDlg;
BOOL CloseWindowDlg();
