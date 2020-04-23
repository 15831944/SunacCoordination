#pragma once


// CWindowTableCheckDlg �Ի���

class CWindowTableCheckDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWindowTableCheckDlg)

public:
	CWindowTableCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWindowTableCheckDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WINTABLECHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCancel();
	afx_msg void OnBnClickedButtonWintableselect();

public:
	void GetWinIdFromWinTableXData(AcDbObjectId p_tableId, vector<AcDbObjectId> &vWinIds);
	void CreateBrightBox(vector<AcDbObjectId> vWinIds); //Ϊ�Ŵ���Ŵ�������
	void DeleteBrightBox();

protected:
	vector<AcDbObjectId> m_polyBrightBoxId;
};

extern CWindowTableCheckDlg* g_winTableCheckDlg;
BOOL CloseWindowTableCheckDlg();
