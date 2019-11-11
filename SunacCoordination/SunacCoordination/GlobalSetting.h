#pragma once
#include "Sunac_def.h"
#include <utility>

using namespace std;

class GlobalSetting
{
public:
	static GlobalSetting *GetInstance()
	{
		static GlobalSetting instance;
		return &instance;
	}

	CString GetFtpIp()const;
	const char*  GetCADServiceUrl()const;

private:
	GlobalSetting();
	~GlobalSetting();

protected:
	bool m_bTestMode; //���Ի���
}; 

#define GSINST (GlobalSetting::GetInstance())