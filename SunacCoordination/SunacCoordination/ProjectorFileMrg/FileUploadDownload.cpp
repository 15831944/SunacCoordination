#include "StdAfx.h"
#include < afxinet.h>
#include "..\Common\ComFun_String.h"
#include "FileUploadDownload.h"
#include "..\GlobalSetting.h"

CFileUpDownLoadWeb* CFileUpDownLoadWeb::Instance()
{
	static CFileUpDownLoadWeb s_fileUpDown;
	return &s_fileUpDown;
}

CFileUpDownLoadWeb::CFileUpDownLoadWeb()
{
	m_bWaitingForQuit = false;
}

CFileUpDownLoadWeb::~CFileUpDownLoadWeb()
{
	//结束所有线程
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

//strFileURLInServer待下载文件的URL,  strFileLocalFullPath存放到本地的路径
bool CFileUpDownLoadWeb::DownloadFile(const CString& strFileURLInServer, const CString & strFileLocalFullPath)
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
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //重试之间的等待延时
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);   //重试次数
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
				NULL);  //创建本地文件
			if (hFile == INVALID_HANDLE_VALUE)
			{
				pHttpFile->Close();
				pHttpConnection->Close();
				session.Close();
				return false;
			}

			char szInfoBuffer[1000];  //返回消息
			DWORD dwFileSize = 0;   //文件长度
			DWORD dwInfoBufferSize = sizeof(szInfoBuffer);
			BOOL bResult = FALSE;
			bResult = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,
				(void*)szInfoBuffer, &dwInfoBufferSize, NULL);

			dwFileSize = atoi(szInfoBuffer);
			const int BUFFER_LENGTH = 1024 * 10;
			pszBuffer = new char[BUFFER_LENGTH];  //读取文件的缓冲
			DWORD dwWrite, dwTotalWrite;
			dwWrite = dwTotalWrite = 0;
			UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); //读取服务器上数据

			while (nRead > 0)
			{
				WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL);  //写到本地文件
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

void CFileUpDownLoadWeb::DownloadFileByThread(CUpDownFilePara p_upFilePara)
{
	CUpDownFilePara* pFilePara = new CUpDownFilePara(p_upFilePara);
	pFilePara->bUpload = false;

	HANDLE hSampleThread = CreateThread(NULL, 0, CFileUpDownLoadWeb::DownloadFileThreadFunc, pFilePara, 0, NULL);

	m_allUpFileParas.push_back(pFilePara);
	m_allTheadHandle.push_back(hSampleThread);
}

DWORD CFileUpDownLoadWeb::DownloadFileThreadFunc(LPVOID pama)
{
	CUpDownFilePara* pFilePara = (CUpDownFilePara*)pama;

	bool bSuc = DownloadFile(pFilePara->sFileUrl, pFilePara->sFileLocalPath);

	if (CFileUpDownLoadWeb::Instance()->m_bWaitingForQuit) //程序正在退出，无需后面的回调
		return 0;

	pFilePara->progress = bSuc ? 100 : -1;

	//上传完成后通知界面显示和下一步动作
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

//////////////////////////////////////////////////////////////////////////
CFileUpDownLoadFtp* CFileUpDownLoadFtp::Instance()
{
	static CFileUpDownLoadFtp s_fileUpDown;
	return &s_fileUpDown;
}

CFileUpDownLoadFtp::CFileUpDownLoadFtp()
{
	m_bUseMultiThread = false;
	m_bWaitingForQuit = false;

	m_downLoadThread = CreateThread(NULL, 0, CFileUpDownLoadFtp::FTPThreadFunc, this, 0, NULL);;
}

CFileUpDownLoadFtp::~CFileUpDownLoadFtp()
{
	//结束所有线程
	for (UINT i = 0; i < m_allTheadHandle.size(); i++)
	{
		TerminateThread(m_allTheadHandle[i], 0);
	}
	m_allTheadHandle.clear();

	for (UINT i = 0; i < m_allUpFileParas.size(); i++)
	{
		delete m_allUpFileParas[i];
	}
	m_allUpFileParas.clear();

	//////////////////////////////////////////////////////////////////////////
	if (m_downLoadThread!=NULL)
	{
		TerminateThread(m_downLoadThread, 0);
	}

	while (m_upFileParasQueue.empty()==false)
	{
		delete m_upFileParasQueue.front();
		m_upFileParasQueue.pop();
	}
}

void CFileUpDownLoadFtp::UploadFileByThread(CUpDownFilePara p_upFilePara)
{
	CUpDownFilePara* pFilePara = new CUpDownFilePara(p_upFilePara);
	pFilePara->bUpload = true;

	if (m_bUseMultiThread)
	{
		HANDLE hSampleThread = CreateThread(NULL, 0, CFileUpDownLoadFtp::OneUploadFileThreadFunc, pFilePara, 0, NULL);
		m_allUpFileParas.push_back(pFilePara);
		m_allTheadHandle.push_back(hSampleThread);
	}
	else
	{
		CAutoLock a(&m_Lock);
		m_upFileParasQueue.push(pFilePara);
	}
}

DWORD CFileUpDownLoadFtp::OneUploadFileThreadFunc(LPVOID pama)
{
	CUpDownFilePara* pFilePara = (CUpDownFilePara*)pama;

	bool bSuc = UploadFile(pFilePara->sFileLocalPath, pFilePara->ftpSaveName, pFilePara->ftpDir);

	if (CFileUpDownLoadFtp::Instance()->m_bWaitingForQuit) //程序正在退出，无需后面的回调
		return 0;

	pFilePara->progress = bSuc ? 100 : -1;

	//上传完成后通知界面显示和下一步动作
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

CUpDownFilePara* CFileUpDownLoadFtp::GetFrontDownFilePara()
{
	CAutoLock a(&m_Lock);

	CUpDownFilePara* pPara = NULL;
	if (m_upFileParasQueue.empty()==false)
	{
		pPara = m_upFileParasQueue.front();
		m_upFileParasQueue.pop();
	}
	return pPara;
}

DWORD CFileUpDownLoadFtp::FTPThreadFunc(LPVOID pama)
{
	while (CFileUpDownLoadFtp::Instance()->m_bWaitingForQuit==false)
	{
		CUpDownFilePara* pFilePara = CFileUpDownLoadFtp::Instance()->GetFrontDownFilePara();
		if (pFilePara!=NULL)
		{
			bool bSuc = UploadFile(pFilePara->sFileLocalPath, pFilePara->ftpSaveName, pFilePara->ftpDir);

			if (CFileUpDownLoadFtp::Instance()->m_bWaitingForQuit) //程序正在退出，无需后面的回调
				return 0;

			pFilePara->progress = bSuc ? 100 : -1;

			//上传完成后通知界面显示和下一步动作
			if (pFilePara->cbFunc != NULL)
			{
				pFilePara->cbFunc(pFilePara);
			}
			if (pFilePara->uiCBFunc != NULL)
			{
				pFilePara->uiCBFunc(pFilePara);
			}

			delete pFilePara; //下载完成后删除参数
		}
		else
		{
			Sleep(50);
		}
	}

	return 0;
}


//https://blog.csdn.net/the_king_cloud/article/details/8090699
//https://www.cnblogs.com/linuxws/p/11006293.html
bool CFileUpDownLoadFtp::UploadFile(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir)
{
	BOOL bSuc = false;
	CFtpConnection *pFtpConnection = NULL;
	CInternetSession *m_pInetsession = new CInternetSession(NULL, 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		//pFtpConnection = m_pInetsession->GetFtpConnection(L"192.168.13.13", L"test", L"1234", 21); //本地测试
		pFtpConnection = m_pInetsession->GetFtpConnection(GSINST->GetFtpIp(), L"caduploader", L"ty19@scad", 2121, TRUE); //测试环境10.4.64.91, 由于本地使用通常是带网关的局域网，需设置被动模式
		//pFtpConnection = m_pInetsession->GetFtpConnection(L"1.203.161.246", L"caduploader", L"ty19@scad", 2121, TRUE); //生产环境, 由于本地使用通常是带网关的局域网，需设置被动模式

		if (pFtpConnection == NULL)
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
			if (bCreateSuc == FALSE || bDirExsit == FALSE)
			{
				return false;
			}
		}

		bSuc = pFtpConnection->PutFile(p_sFilePath, p_ftpSaveName, FTP_TRANSFER_TYPE_BINARY, 1);
		if (bSuc == FALSE)
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

	return bSuc ? true : false;
}