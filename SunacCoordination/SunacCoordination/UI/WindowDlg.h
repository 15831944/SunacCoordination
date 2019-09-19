#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"
#include "../Object/AttrWindow.h"
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
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //�ͷŷ�ģ̬�Ի����ڴ�

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonSearchwindow();
	afx_msg void OnBnClickedRadioDoor();

	void SetRadioDoor(int radioDoor);
	void SetCombobox(CComboBox& comboBox, const vCString& options, int nSel = 0);
	void LoadDefaultValue();

	CString GetText(const CWnd& pWnd);

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
	CComboBox m_openWidth; //�����ȿ��
	CComboBox m_H2; //�¹̶�ֵ
	CComboBox m_distance; //����ǽ����
	
	int m_radioDoor; //�Ŵ���ѡ
	int m_radioYes; //�Ƿ�͹��

	vector<AttrWindow> m_allWindows;
};
