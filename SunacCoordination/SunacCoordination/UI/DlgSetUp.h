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
	BOOL m_showLimianNumberDlg;	//�Ƿ���ʾ����ͼ���Ŵ����
	BOOL m_useAinLimianDlg;		//����ͼ�Ƿ�ʹ������ߴ�

	CComboBox m_winNumberTextSizeDlg;//�Ŵ��������
	CComboBox m_winDetailDimRateDlg;//�Ŵ��������

	CString m_sFrameLayerDlg;	//�Ŵ��ͲĽṹ��ͼ��
	CString m_sHardWareLayerDlg;	//�Ŵ�����ͼ��
	CString m_sLayerDlg;			//�Ŵ���ͼ��
	CString m_sNumberLayerLimianDlg;//�Ŵ�����ͼ���ͼ��
	CString m_sNumberLayerPingmianDlg;//�Ŵ�ƽ��ͼ���ͼ��
	CString m_sOpenLayerDlg;			//�Ŵ���������ͼ��
	CString m_sWallLayerDlg;			//�Ŵ���ǽ��ͼ��

	CWinSetting m_originalWinSetting;


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01.

	void InitWinSetting();

	void UpdateLayer();//����ͼ��

	void SetDigPos();

public:
	virtual BOOL OnInitDialog();
};