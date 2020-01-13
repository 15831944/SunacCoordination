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

	CString GetWallLayerDefault() const { return L"Sunnc_Wall";	}


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