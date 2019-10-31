#pragma once
#include <vector>

class CUpDownFilePara;

typedef void(*FileUpDownCB)(CUpDownFilePara*);

class CUpDownFilePara
{
public:
	bool bUpload;
	CString sFileLocalPath; //�����ļ���
	CString sFileName;		//������׺

	CString sDirInProject; //����Ŀ�е�Ŀ¼

	CString sFileUrl;
	CString ftpSaveName;
	CString ftpDir;

	FileUpDownCB  cbFunc; //�ص�����

	int progress; //���ȣ�0-100

public:
	CUpDownFilePara()
	{ 
		bUpload = true;
		cbFunc = NULL; 
		progress = 0; 
	}
};



class CFileUpDownLoad //��Ŀ������
{
	CFileUpDownLoad();
public:
	static CFileUpDownLoad* Instance();
	~CFileUpDownLoad();

	void StopAll(); //ֹͣ�����ϴ�����,��Ҫ�ǳ����˳�ʱ

	//strFileURLInServer�������ļ���URL,  strFileLocalFullPath��ŵ����ص�·��
	static bool DownloadFile(const CString& strFileURLInServer, const CString & strFileLocalFullPath);
	void DownloadFileByThread(CUpDownFilePara p_upFilePara);

	static bool UploadFile(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir);
	
	void UploadFileByThread(CUpDownFilePara p_upFilePara);

protected:
	static DWORD UploadFileThreadFunc(LPVOID pama);
	static DWORD DownloadFileThreadFunc(LPVOID pama);

protected:
	vector<CUpDownFilePara*> m_allUpFileParas;

	vector<HANDLE> m_allTheadHandle;

};

