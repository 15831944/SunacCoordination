#pragma once
#include "Sunac_def.h"
#include <utility>

using namespace std;

const int cPasswordMaxlength = 30;
const int cPasswordBufferSize = (cPasswordMaxlength*sizeof(wchar_t)/16+1)*16;


//�Ŵ�����
struct CWinSetting
{
public:
	CWinSetting();
	CString GetWinLayerDefault() { return L"Sunac_Window"; }

	CString GetWinFrameLayerDefault() { return L"Sunac_Gridline_Glass"; }
	CString GetWinWallLayerDefault() { return L"Sunac_Wall"; }
	CString GetWinHardwareLayerDefault() { return L"Sunac_HardWare"; }
	CString GetWinNumberLayerPingmianDefault() { return L"Sunac_WinNumber_Pingmian"; }

	CString GetWinOpenLayerDefault() { return L"Sunac_Gridline_Glass_Open"; }
	CString GetWinNumberLayerLimianDefault() { return L"Sunac_WinNumber_Limian";}



	CString m_sWinLayer;		//�Ŵ���ͼ��

	CString m_sWinFrameLayer;	//�Ŵ���ͼ��
	CString m_sWinWallLayer;	//��������ͼ��
	CString m_sWinHardwareLayer; //�Ŵ�����ͼ��
	CString m_sWinOpenLayer; //�Ŵ���������ͼ��

	CString m_sWinNumberLayerLimian;	//�Ŵ�����ͼ���ͼ��
	CString m_sWinNumberLayerPingmian;	//�Ŵ�ƽ��ͼ���ͼ��

	bool m_bUseAinLimian;  //����ͼ�Ƿ�ʹ������ߴ�

	int m_numberTextSize; //�Ŵ��������

	bool m_bShowLimianNumber; //�Ƿ���ʾ����ͼ���Ŵ����

	int m_bWinDetailDimRate; //�Ŵ���ͼ��ע����
};

class GlobalSetting
{
public:
	~GlobalSetting();
	static GlobalSetting *GetInstance()
	{
		static GlobalSetting instance;
		return &instance;
	}

	static CWinSetting* GetWinSetting(){return &(GetInstance()->m_winSetting);}

	CString GetFtpIp()const;
	const char*  GetCADServiceUrl()const;

	static CString GetWindowBlockLayer() { return GetInstance()->m_winSetting.m_sWinLayer; }

	bool UpdateToXml();

private:
	GlobalSetting();

	CString GetXmlFilePath();
	bool LoadFromXml();
	bool SaveToXml();

public:
	bool m_bTestMode; //���Ի���

	bool m_bRememberPwd; //��ס����
	CString m_userName;
	CString m_password;

	CWinSetting m_winSetting;
}; 

#define GSINST (GlobalSetting::GetInstance())