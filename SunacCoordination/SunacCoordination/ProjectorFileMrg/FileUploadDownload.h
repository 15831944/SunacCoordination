#pragma once
#include <vector>
#include <queue>
#include "../Common/ThreadUtil.h"
using namespace std;


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



class CFileUpDownLoadWeb //��Ŀ������
{
	CFileUpDownLoadWeb();
public:
	static CFileUpDownLoadWeb* Instance();
	~CFileUpDownLoadWeb();

	void StopAll(); //ֹͣ�����ϴ�����,��Ҫ�ǳ����˳�ʱ
	void SetAppQuit(){ m_bWaitingForQuit = true; }

	//strFileURLInServer�������ļ���URL,  strFileLocalFullPath��ŵ����ص�·��
	static bool DownloadFile(const CString& strFileURLInServer, const CString & strFileLocalFullPath);

	void DownloadFileByThread(CUpDownFilePara p_upFilePara);

protected:
	static DWORD DownloadFileThreadFunc(LPVOID pama);

protected:
	vector<CUpDownFilePara*> m_allUpFileParas;

	vector<HANDLE> m_allTheadHandle;

	bool m_bWaitingForQuit; //�����˳���
};

//////////////////////////////////////////////////////////////////////////

class CFileUpDownLoadFtp //��Ŀ������
{
	CFileUpDownLoadFtp();
public:
	static CFileUpDownLoadFtp* Instance();
	~CFileUpDownLoadFtp();

	void StopAll(); //ֹͣ�����ϴ�����,��Ҫ�ǳ����˳�ʱ
	void SetAppQuit(){ m_bWaitingForQuit = true; }

	static bool UploadFile(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir);

	void UploadFileByThread(CUpDownFilePara p_upFilePara);

protected:
	static DWORD OneUploadFileThreadFunc(LPVOID pama);
	static DWORD FTPThreadFunc(LPVOID pama);
	CUpDownFilePara* GetFrontDownFilePara();

protected:
	//���߳�������
	vector<CUpDownFilePara*> m_allUpFileParas;
	vector<HANDLE> m_allTheadHandle;


	//���߳�������
	HANDLE m_downLoadThread;
	queue<CUpDownFilePara* > m_upFileParasQueue;

	CCritSec m_Lock;
	bool m_bUseMultiThread;
	bool m_bWaitingForQuit; //�����˳���
};
