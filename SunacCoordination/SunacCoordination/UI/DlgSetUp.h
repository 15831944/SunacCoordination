#pragma once

#include "afxwin.h"
#include "ComFun_MFC.h"
#include "GridCtrlWithPreview.h"
#include "../GlobalSetting.h"

// CDlgSetUp �Ի���

class CDlgSetUp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetUp)

public:
	CDlgSetUp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetUp();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CEdit m_winFrameLayer;		//�Ŵ��ͲĽṹ��ͼ��
	CEdit m_winHardwareLayer;	//�Ŵ�����ͼ��
	CEdit m_winLayer;			 //�Ŵ���ͼ��
	CEdit m_winNumberLayerLimian;//�Ŵ�����ͼ���ͼ��
	CEdit m_winNumberLayerPingmian;//�Ŵ�ƽ��ͼ���ͼ��
	CEdit m_winOpenLayer;		//�Ŵ���������ͼ��
	CEdit m_winWallLayer;		//�Ŵ���ǽ��ͼ��
	BOOL m_showLimianNumber;	//�Ƿ���ʾ����ͼ���Ŵ����
	BOOL m_useAinLimian;		//����ͼ�Ƿ�ʹ������ߴ�
	CComboBox m_winNumberTextSize;//�Ŵ��������
	CComboBox m_winDetailDimRate;//�Ŵ��������

	CString m_sFrameLayerDlg;

	CWinSetting m_originalWinSetting;


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01

	//�ж������Ƿ�Ϊ�գ���Ϊ�ղŽ����öԻ����е�ֵ����GlobalSetting,Ϊ����ʹ��Ĭ��ֵ
	void WinSetUp(CString& p_winSetting, CString p_winSetUp);

	void InitWinSetting();

	void UpdateLayer();//����ͼ��

private:
	vector<CString> m_winSetUpLayer;

public:
	virtual BOOL OnInitDialog();
};

extern CDlgSetUp* g_winSetupDlg;
void OpenWindowSetUpDlg();
BOOL CloseWindowSetUpDlg();