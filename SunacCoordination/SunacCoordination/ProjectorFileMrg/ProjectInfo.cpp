#include "StdAfx.h"
#include < afxinet.h>
#include "ProjectInfo.h"


CProjectInfo::CProjectInfo()
{
}

CProjectInfo::~CProjectInfo()
{
}

//////////////////////////////////////////////////////////////////////////

CProjectData::CProjectData()
{
}

CProjectData::~CProjectData()
{
}

bool CProjectData::DownloadFile(const CString& strFileURLInServer, //�������ļ���URL
	const CString & strFileLocalFullPath)//��ŵ����ص�·��
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

bool CProjectData::UploadFile(CString p_sFileName, CString p_saveName)
{
	CFtpConnection *m_pFtpConnection = NULL;
	CInternetSession *m_pInetsession = new CInternetSession(NULL, 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		//TODO ��������ַ���û���������������
		m_pFtpConnection = m_pInetsession->GetFtpConnection(L"127.0.0.1", NULL, NULL, 38);
		//MessageBox("���ӳɹ�");

		m_pFtpConnection->PutFile(p_sFileName, p_saveName, FTP_TRANSFER_TYPE_BINARY, 1);
	}
	catch (CInternetException *pEx)
	{
		TCHAR szError[1024];
		if (pEx->GetErrorMessage(szError, 1024))
			AfxMessageBox(szError);
		else
			AfxMessageBox(L"There was an exception");
		pEx->Delete();
		m_pFtpConnection = NULL;
		return false;
	}

	return true;
}