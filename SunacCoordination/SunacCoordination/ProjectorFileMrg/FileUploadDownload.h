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
	FileUpDownCB  uiCBFunc; //UI�ص�����

	int progress; //���ȣ�0-100, -1��ʾʧ�ܻ��ж�

	void* pCaller; //������

public:
	CUpDownFilePara()
	{ 
		bUpload = true;
		cbFunc = NULL; 
		uiCBFunc = NULL;
		progress = 0; 
		pCaller = NULL;
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
	static bool UploadFile(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir);

	void DownloadFileByThread(CUpDownFilePara p_upFilePara);
	void UploadFileByThread(CUpDownFilePara p_upFilePara);

	void SetAppQuit(){ m_bWaitingForQuit = true; }

protected:
	static DWORD UploadFileThreadFunc(LPVOID pama);
	static DWORD DownloadFileThreadFunc(LPVOID pama);

protected:
	vector<CUpDownFilePara*> m_allUpFileParas;

	vector<HANDLE> m_allTheadHandle;


	bool m_bWaitingForQuit; //�����˳���
};

