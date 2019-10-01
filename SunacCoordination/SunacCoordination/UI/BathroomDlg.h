#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"
#include "../Object/Bathroom/AttrBathroom.h"
#include "../Object/PrototypeCommonDef.h"
#include "../Common/ComFun_Sunac.h"
#include "ComFun_MFC.h"

// CBathroomDlg �Ի���


class CBathroomDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CBathroomDlg)

public:
	CBathroomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBathroomDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BATHROOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //�ͷŷ�ģ̬�Ի����ڴ�

	DECLARE_MESSAGE_MAP()

	CGridCtrlWithPreview m_preBathroom;
	TYRect m_rect;
	E_DIRECTION m_doorDir;
	E_DIRECTION m_windowDir;

	vector<AttrBathroom*> m_allBathrooms;

public:
	CComboBox m_bathroomType;
	CButton m_noAirOut;
	CEdit m_number;
	CComboBox m_basinWidth;
	CComboBox m_BathroomWidth;
	CComboBox m_washWidth;
	CButton m_isMirror;
	CButton m_autoIndex;

	E_DIRECTION GetDir(ads_point pt);
	void LoadDefaultValue();
	vector<AttrBathroom*> FilterTI();
	vector<AttrBathroom*> FilterTL();
	vector<AttrBathroom*> FilterTU();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	afx_msg void OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonRange();
	afx_msg void OnBnClickedButtonDoorDir();
	afx_msg void OnBnClickedButtonWindowDir();
	afx_msg void OnBnClickedButtonSearch();
};

extern CBathroomDlg* g_bathroomDlg;
BOOL CloseBathroomDlg();