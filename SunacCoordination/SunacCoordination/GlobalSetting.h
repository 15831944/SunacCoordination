#pragma once
#include "Sunac_def.h"
#include <utility>

using namespace std;

const int cPasswordMaxlength = 30;
const int cPasswordBufferSize = (cPasswordMaxlength*sizeof(wchar_t)/16+1)*16;


//�Ŵ�����
struct CWinSetting
{
	CString m_sWinLayer; //�Ŵ��ṹͼ��
	CString m_sWinWallLayer; //�Ŵ����ǽ��ͼ��
	CString m_sWinHardwareLayer; //�Ŵ�����ͼ��
	CString m_sWinOpenLayer; //�Ŵ���������ͼ��
	CString m_sWinNumberLayerLimian; //�Ŵ�����ͼ���ͼ��
	CString m_sWinNumberLayerPingmian; //�Ŵ�����ͼ���ͼ��

	int m_winNumberTextSize; //�Ŵ��������
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