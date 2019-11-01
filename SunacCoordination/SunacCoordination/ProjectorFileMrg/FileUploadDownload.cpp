#include "StdAfx.h"
#include < afxinet.h>
#include "..\Common\ComFun_String.h"
#include "FileUploadDownload.h"

CFileUpDownLoad* CFileUpDownLoad::Instance()
{
	static CFileUpDownLoad s_fileUpDown;
	return &s_fileUpDown;
}

CFileUpDownLoad::CFileUpDownLoad()
{
	m_bWaitingForQuit = false;
}

CFileUpDownLoad::~CFileUpDownLoad()
{
	//���������߳�
	for (UINT i = 0; i < m_allTheadHandle.size(); i++)
	{
		TerminateThread(m_allTheadHandle[i],0 );
	}
	m_allTheadHandle.clear();

	//////////////////////////////////////////////////////////////////////////
	for (UINT i = 0; i < m_allUpFileParas.size();i++)
	{
		delete m_allUpFileParas[i];
	}
	m_allUpFileParas.clear();
}

//strFileURLInServer�������ļ���URL,  strFileLocalFullPath��ŵ����ص�·��
bool CFileUpDownLoad::DownloadFile(const CString& strFileURLInServer, const CString & strFileLocalFullPath)
{
	ASSERT(strFileURLInServer != "");
	ASSERT(strFileLocalFullPath != "");
	CInternetSession session;
	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;
	CString strServer, strObject;
	INTERNET_PORT wPort;

	DWORD dwType;
	const int nTimeOut = 2000;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //����֮��ĵȴ���ʱ
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);   //���Դ���
	char* pszBuffer = NULL;

	try
	{
		AfxParseURL(strFileURLInServer, dwType, strServer, strObject, wPort);
		pHttpConnection = session.GetHttpConnection(strServer, wPort);
		pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
		if (pHttpFile->SendRequest() == FALSE)
			return false;
		DWORD dwStateCode;

		pHttpFile->QueryInfoStatusCode(dwStateCode);
		if (dwStateCode == HTTP_STATUS_OK)
		{
			HANDLE hFile = CreateFile(strFileLocalFullPath, GENERIC_WRITE,
				FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
				NULL);  //���������ļ�
			if (hFile == INVALID_HANDLE_VALUE)
			{
				pHttpFile->Close();
				pHttpConnection->Close();
				session.Close();
				return false;
			}

			char szInfoBuffer[1000];  //������Ϣ
			DWORD dwFileSize = 0;   //�ļ�����
			DWORD dwInfoBufferSize = sizeof(szInfoBuffer);
			BOOL bResult = FALSE;
			bResult = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,
				(void*)szInfoBuffer, &dwInfoBufferSize, NULL);

			dwFileSize = atoi(szInfoBuffer);
			const int BUFFER_LENGTH = 1024 * 10;
			pszBuffer = new char[BUFFER_LENGTH];  //��ȡ�ļ��Ļ���
			DWORD dwWrite, dwTotalWrite;
			dwWrite = dwTotalWrite = 0;
			UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); //��ȡ������������

			while (nRead > 0)
			{
				WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL);  //д�������ļ�
				dwTotalWrite += dwWrite;
				nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH);
			}

			delete[]pszBuffer;
			pszBuffer = NULL;
			CloseHandle(hFile);
		}
		else
		{
			delete[]pszBuffer;
			pszBuffer = NULL;
			if (pHttpFile != NULL)
			{
				pHttpFile->Close();
				delete pHttpFile;
				pHttpFile = NULL;
			}
			if (pHttpConnection != NULL)
			{
				pHttpConnection->Close();
				delete pHttpConnection;
				pHttpConnection = NULL;
			}
			session.Close();
			return false;
		}
	}
	catch (...)
	{
		delete[]pszBuffer;
		pszBuffer = NULL;
		if (pHttpFile != NULL)
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		if (pHttpConnection != NULL)
		{
			pHttpConnection->Close();
			delete pHttpConnection;
			pHttpConnection = NULL;
		}
		session.Close();
		return false;
	}

	if (pHttpFile != NULL)
		pHttpFile->Close();
	if (pHttpConnection != NULL)
		pHttpConnection->Close();
	session.Close();
	return true;
}

//https://blog.csdn.net/the_king_cloud/article/details/8090699
//https://www.cnblogs.com/linuxws/p/11006293.html
bool CFileUpDownLoad::UploadFile(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir)
{
	BOOL bSuc = false; 
	CFtpConnection *pFtpConnection = NULL;
	CInternetSession *m_pInetsession = new CInternetSession(NULL, 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		//TODO ��������ַ���û���������������
//#ifdef WORK_LOCAL
//		pFtpConnection = m_pInetsession->GetFtpConnection(L"192.168.13.13", L"test", L"1234", 21);
//#else
		pFtpConnection = m_pInetsession->GetFtpConnection(L"10.4.64.91", L"caduploader", L"ty19@scad", 2121, TRUE); //���Ի���, ���ڱ���ʹ��ͨ���Ǵ����صľ������������ñ���ģʽ
//#endif // WORK_LOCAL

		if (pFtpConnection ==  NULL)
		{
			assert(false);
			m_pInetsession->Close();
			delete m_pInetsession;
			return false; 
		}

		BOOL bDirExsit = pFtpConnection->SetCurrentDirectory(p_ftpDir);
		if (bDirExsit == FALSE)
		{
			BOOL bCreateSuc = pFtpConnection->CreateDirectory(p_ftpDir);
			bDirExsit = pFtpConnection->SetCurrentDirectory(p_ftpDir);
			if (bCreateSuc == FALSE || bDirExsit ==FALSE)
			{
				return false;
			}
		}

		bSuc = pFtpConnection->PutFile(p_sFilePath, p_ftpSaveName, FTP_TRANSFER_TYPE_BINARY, 1);
		if (bSuc==FALSE)
		{
			DWORD lError = GetLastError();
			DWORD lError2 = 0;
			TCHAR errorMsg[256] = { 0 };
			DWORD len = 256;

			InternetGetLastResponseInfo(&lError2, errorMsg, &len);
			AfxMessageBox(errorMsg);
		}
	}
	catch (CInternetException *pEx)
	{
		TCHAR szError[1024];
		if (pEx->GetErrorMessage(szError, 1024))
			AfxMessageBox(szError);
		else
			AfxMessageBox(L"There was an exception");
		pEx->Delete();

		if (pFtpConnection != NULL)
		{
			pFtpConnection->Close();
			//delete pFtpConnection;
		}
		m_pInetsession->Close();
		delete m_pInetsession;
		return false;
	}

	if (pFtpConnection)
	{
		pFtpConnection->Close();
		//delete pFtpConnection;
	}

	m_pInetsession->Close();
	delete m_pInetsession;

	return bSuc ?  true :false;
}

void CFileUpDownLoad::UploadFileByThread(CUpDownFilePara p_upFilePara)
{
	CUpDownFilePara* pFilePara = new CUpDownFilePara(p_upFilePara);
	pFilePara->bUpload = true;

	HANDLE hSampleThread = CreateThread(NULL, 0, CFileUpDownLoad::UploadFileThreadFunc, pFilePara, 0, NULL);


	m_allUpFileParas.push_back(pFilePara);
	m_allTheadHandle.push_back(hSampleThread);
}

DWORD CFileUpDownLoad::UploadFileThreadFunc(LPVOID pama)
{
	CUpDownFilePara* pFilePara = (CUpDownFilePara*)pama;

	bool bSuc = UploadFile(pFilePara->sFileLocalPath, pFilePara->ftpSaveName, pFilePara->ftpDir);

	if (CFileUpDownLoad::Instance()->m_bWaitingForQuit) //���������˳����������Ļص�
		return 0;

	pFilePara->progress = bSuc ? 100 : -1;

	//�ϴ���ɺ�֪ͨ������ʾ����һ������
	if (pFilePara->cbFunc!=NULL)
	{
		pFilePara->cbFunc(pFilePara);
	}
	if (pFilePara->uiCBFunc != NULL)
	{
		pFilePara->uiCBFunc(pFilePara);
	}

	return 0;
}
void CFileUpDownLoad::DownloadFileByThread(CUpDownFilePara p_upFilePara)
{
	CUpDownFilePara* pFilePara = new CUpDownFilePara(p_upFilePara);
	pFilePara->bUpload = false;

	HANDLE hSampleThread = CreateThread(NULL, 0, CFileUpDownLoad::DownloadFileThreadFunc, pFilePara, 0, NULL);

	m_allUpFileParas.push_back(pFilePara);
	m_allTheadHandle.push_back(hSampleThread);
}

DWORD CFileUpDownLoad::DownloadFileThreadFunc(LPVOID pama)
{
	CUpDownFilePara* pFilePara = (CUpDownFilePara*)pama;

	bool bSuc = DownloadFile(pFilePara->sFileUrl, pFilePara->sFileLocalPath);

	if (CFileUpDownLoad::Instance()->m_bWaitingForQuit) //���������˳����������Ļص�
		return 0;

	pFilePara->progress = bSuc ? 100 : -1;

	//�ϴ���ɺ�֪ͨ������ʾ����һ������
	if (pFilePara->cbFunc != NULL)
	{
		pFilePara->cbFunc(pFilePara);
	}
	if (pFilePara->uiCBFunc != NULL)
	{
		pFilePara->uiCBFunc(pFilePara);
	}

	return 0;
}