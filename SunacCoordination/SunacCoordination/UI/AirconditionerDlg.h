#pragma once
#include "PreviewWithDetail.h"
#include "GsPreviewCtrl.h"
#include "afxwin.h"
#include "../Object/AttrAirCon.h"
#include "ComFun_MFC.h"

// CAirconditionerDlg �Ի���

class CAirconditionerDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CAirconditionerDlg)

public:
	CAirconditionerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAirconditionerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_AIRCONDITIONER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CPreviewWithDetail m_preAC;

	CComboBox m_pNum;		//ƥ��
	CComboBox m_lNTubePos;	//����ˮ��λ��
	CButton m_hasRainTube;	//�Ƿ�����ˮ���ܴ���
	CComboBox m_rainTubePos;//��ˮ��λ��

	vector<AttrAirCon> m_allAirCons;

public:

	CGsPreviewCtrl m_preAirCon;
	CButton m_btnInbsert;

	void LoadDefaultValue();
//	afx_msg void OnCbnSelchangeComboPnum();
	afx_msg void OnBnClickedCheckHasraintube();
	afx_msg void OnCbnSelchangeComboPnum();
	void UpdatePreview();
	
	afx_msg void OnCbnSelchangeComboLntubepos();
	afx_msg void OnCbnSelchangeComboRaintubepos();
	afx_msg void OnBnClickedButtonInsertac();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);

	CString m_filePathName;
	CButton m_upDownImage;
	CButton m_leftRightImage;
	CStatic m_room;	//�������(��̬�ı���)
	afx_msg void OnBnClickedButtonCalculate(); 
	CEdit m_roomSize; //�������(�༭��)
	bool m_flag;  //�ж��Ƿ���ʾ�յ��༭��
	double m_rSize; //������������ڼ���յ�ƥ��
};

extern CAirconditionerDlg* g_airconditionerDlg;
BOOL CloseAirconditionerDlg();