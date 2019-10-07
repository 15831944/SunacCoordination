#pragma once
#include "GridCtrlWithPreview.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/Kitchen/KitchenGen.h"
#include "ComFun_MFC.h"

// CKitchenDlg �Ի���



class CKitchenDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CKitchenDlg)

public:
	CKitchenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKitchenDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_KITCHEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //�ͷŷ�ģ̬�Ի����ڴ�

	DECLARE_MESSAGE_MAP()

	bool IsKitchRectValid(TYRect rect);
public:
	CGridCtrlWithPreview m_preKitchen;
	CComboBox m_kitchenType;
	CComboBox m_floorRange;
	CComboBox m_basinType;
	CComboBox m_benchWidth;
	CComboBox m_fridgeType;
	CButton m_isMirror;
	CEdit m_number;
	CEdit m_offsetX;
	CEdit m_offsetY;
	CEdit m_customX;
	CEdit m_customY;
	BOOL m_bAutoIndex;

	//////////////////////////////////////////////////////////////////////////
	TYRect m_rect;
	E_DIRECTION m_doorDir;
	E_DIRECTION m_windowDir;
	BOOL m_bNoAirout;// �Ƿ�������
	int m_isStd; //����/�Զ���

	vector<AttrKitchen*> m_allKitchens;

	CKitchGen* m_pKitchGen;


	afx_msg void OnBnClickedOk();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonRange();
	afx_msg void OnBnClickedButtonDoorDir();
	afx_msg void OnBnClickedButtonWindowDir();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAutoIndex();
	afx_msg void OnBnClickedNoAirout();

	E_DIRECTION GetDir(ads_point pt);
	void LoadDefaultValue();
	void EnableSetProperty(bool bEnable);
	void EnableSetAirout(bool bEnable);
	void ClearPreviews();
	bool CheckValid(CString& errMsg); //����ǰ��������Ϸ���
};

extern CKitchenDlg* g_kitchenDlg;
BOOL CloseKitchenDlg();