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
	afx_msg void OnBnClickedMfcbuttonInsert();
	afx_msg void OnBnClickedButtonSearchwindow();
	afx_msg void OnBnClickedRadioDoor();

	void SetRadioDoor(int radioDoor);

	CGridCtrlWithPreview m_preWindow;

	CComboBox m_doorType;
	CComboBox m_areaType;
	CComboBox m_openType;
	CComboBox m_openAmount;
	CComboBox m_openWidth;
	CComboBox m_H2;
	CComboBox m_distance;
	CEdit m_width;
	CEdit m_height;
	CEdit m_ventilation;
	CEdit m_number;
	
	vector<AttrWindow *> m_allWindws;

	int m_radioDoor;
	int m_radioYes;
};
