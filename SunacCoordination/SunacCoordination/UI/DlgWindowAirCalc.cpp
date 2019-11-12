// DlgWindowAirCalc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgWindowAirCalc.h"
#include "afxdialogex.h"
#include "../WebIO/WebIO.h"
#include "ComFun_MFC.h"


// CDlgWindowAirCalc �Ի���

IMPLEMENT_DYNAMIC(CDlgWindowAirCalc, CAcUiDialog)

CDlgWindowAirCalc::CDlgWindowAirCalc(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CDlgWindowAirCalc::IDD, pParent)
{
	m_airQuality = 0;
}

CDlgWindowAirCalc::~CDlgWindowAirCalc()
{
}

void CDlgWindowAirCalc::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_AREA, m_area);
	DDX_Control(pDX, IDC_COMBO_RATE, m_rate);
}


BEGIN_MESSAGE_MAP(CDlgWindowAirCalc, CAcUiDialog)
END_MESSAGE_MAP()


// CDlgWindowAirCalc ��Ϣ�������


BOOL CDlgWindowAirCalc::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	TYUI_SetInt(m_area, 0);
	const vCString& rate = WebIO::GetInstance()->GetConfigDict()->Window_GetRate();
	TYUI_InitComboBox(m_rate, rate, rate.empty() ? L"" : rate[0]);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgWindowAirCalc::OnOK()
{
	double area = TYUI_GetDouble(m_area);
	if (area <= 0)
	{
		AfxMessageBox(L"��Ч�ķ������");
		return;
	}

	CString sRate = TYUI_GetText(m_rate);
	double rate = _ttof(sRate);
	int pos = sRate.Find(L'/');
	if (pos != -1)
		rate /= _ttof(sRate.Mid(pos + 1));

	m_airQuality = area * rate;

	CAcUiDialog::OnOK();
}
