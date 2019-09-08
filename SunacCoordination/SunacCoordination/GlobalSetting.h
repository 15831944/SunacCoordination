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

	
	//�����Ƿ�������ļ�,������ڸ������ص�ȫ·����ַ��filePathName
	bool GetLocalFile(CString fileName,CString &filePathName);

private:
	GlobalSetting();
	~GlobalSetting();
	int InitLocalFiles();

public:
	bool m_syncOK;
	vector<pair<CString,CString>> m_localFiles;
}; 

#define GSINST (GlobalSetting::GetInstance())