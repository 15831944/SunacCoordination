#pragma once


// CDlgWindowAirCalc �Ի���

class CDlgWindowAirCalc : public CAcUiDialog
{
	DECLARE_DYNAMIC(CDlgWindowAirCalc)

public:
	CDlgWindowAirCalc(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWindowAirCalc();

// �Ի�������
	enum { IDD = IDD_DIALOG_WIN_AIRCALC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	double m_airQuality; //ͨ����
protected:
	CEdit m_area; //���
	CComboBox m_rate; //��ֵ
};
