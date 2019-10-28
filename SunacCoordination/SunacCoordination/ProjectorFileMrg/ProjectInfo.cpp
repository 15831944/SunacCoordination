#include "StdAfx.h"
#include < afxinet.h>
#include "ProjectInfo.h"
#include "..\Common\ComFun_String.h"
#include "..\WebIO\WebProjectFile.h"


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

CString CProjectData::GenerateGuid()
{
	GUID guid;
	CoCreateGuid(&guid);

	CString sGuid;
	sGuid.Format(_T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		guid.Data1, guid.Data2,
		guid.Data3, guid.Data4[0],
		guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4],
		guid.Data4[5], guid.Data4[6],
		guid.Data4[7]);
	return sGuid;
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

bool CProjectData::UploadFile(CString p_sFilePath, CString p_saveName)
{
	CFtpConnection *m_pFtpConnection = NULL;
	CInternetSession *m_pInetsession = new CInternetSession(NULL, 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		//TODO ��������ַ���û���������������
		m_pFtpConnection = m_pInetsession->GetFtpConnection(L"192.168.13.13", L"test", L"1234", 21);
		//MessageBox("���ӳɹ�");

		m_pFtpConnection->PutFile(p_sFilePath, p_saveName, FTP_TRANSFER_TYPE_BINARY, 1);
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

//Ŀ¼�㼶��\�ָ���磺A��\ʩ��ͼ ��ʾA���ļ����µ�ʩ��ͼ�ļ���
CProjectDir* CProjectData::FindDir(CString p_dirName)
{
	p_dirName.Trim(_T('\\'));

	int nPos1 = 0;
	int npos2 = 0; 
	
	CProjectDir* pParentDir = &m_rootDir; //��Ŀ¼�ڵ�

	do 
	{
		npos2 = p_dirName.Find(_T('\\'), npos2);
		if (npos2<0)
		{
			break;
		}
		else
		{
			CString sFolderName = p_dirName.Mid(nPos1, npos2 - nPos1);
			pParentDir = pParentDir->GetSubFolder(sFolderName);
			if (pParentDir==NULL)
			{
				return NULL;
			}
			nPos1 = npos2 + 1;
			npos2++;
		}
	} while (npos2>0);


	CString lastFolderName = p_dirName.Mid(nPos1);
	return pParentDir->GetSubFolder(lastFolderName);	
}

bool CProjectData::AddFile(CString p_sFilePath, CString  p_sParentFolderPath) //p_sParentFolderPath��ָ�ϴ����ĸ�Ŀ¼��,Ŀ¼�㼶��\�ָ���磺A��\ʩ��ͼ ��ʾA���ļ����µ�ʩ��ͼ�ļ���
{
	//�ҵ�Ŀ¼
	CProjectDir* p_pParentDir = FindDir(p_sParentFolderPath);
	if (p_pParentDir == NULL)
		return false;

	//1�ϴ��ļ���ftp��
	CString sFileName = FilePathToFileName(p_sFilePath);
	int nPos = sFileName.ReverseFind(_T('.'));
	if (nPos<0)
	{
		return false;
	}

	CString sExtensionName = sFileName.Mid(nPos);
	CString sSaveName = GenerateGuid() + sExtensionName; //ʵ�ʴ洢������
	sSaveName.Delete(sSaveName.Find(_T("{")));
	sSaveName.Delete(sSaveName.Find(_T("}")));

	//�ϴ���ftp��
	UploadFile(p_sFilePath, sSaveName);

	//2.�ӵ����ص���Ŀ�ļ���
	CProjectFile prjfile;
	prjfile.m_sName = sFileName;
	prjfile.m_sSaveName = sSaveName;

	p_pParentDir->AddFile(prjfile);
	

	//3.����web�ӿ���ӵ�web���ݿ���
	CWebProjectFile::Instance()->UpdateFile(GetProjectId(), p_sParentFolderPath, sSaveName, sFileName);

	return true;
}
CString CProjectData::GetProjectId()const
{
	CString sId;
	sId.Format(_T("%d"), m_prjInfo.m_id);
	return sId;
}
bool CProjectData::DeleteFile(CString p_sFileName, CString  p_sParentFolderPath)
{
	//�ҵ�Ŀ¼
	CProjectDir* pDir = FindDir(p_sParentFolderPath);
	if (pDir == NULL)
		return false;

	pDir->DeleteFile(p_sFileName);

	//web�˸��� 
	CWebProjectFile::Instance()->DeleteFile(GetProjectId(), p_sParentFolderPath, p_sFileName);

	return true;
}
bool CProjectData::AddFolder(CString  p_sParentFolderPath, CString p_sFolderName)
{
	//�ҵ�Ŀ¼
	CProjectDir* pDir = NULL;
	if (p_sParentFolderPath.IsEmpty() == false)
	{
		pDir = FindDir(p_sParentFolderPath);
	}
	else
	{
		pDir = &m_rootDir;
	}

	if (pDir == NULL)
		return false;

	pDir->AddFolder(p_sFolderName);

	return true;
}
bool CProjectData::DeleteFolder(CString  p_sFolderPath)
{
	//�ҵ�Ŀ¼
	CProjectDir* pDir = FindDir(p_sFolderPath);
	if (pDir == NULL)
		return false;

	if (pDir->m_parent==NULL)
	{
		assert(false);
		return false;
	}

	bool bSuc = pDir->m_parent->DeleteSubFolder(pDir->m_sName);
	if (bSuc==false)
	{
		return false;
	}

	//web�˸��� 
	CWebProjectFile::Instance()->DeleteFolder(GetProjectId(), p_sFolderPath);

	return bSuc; 
}

bool CProjectData::RenameFolder(CString p_sFolderPath, CString p_newName)
{
	//�ҵ�Ŀ¼
	CProjectDir* pDir = FindDir(p_sFolderPath);
	if (pDir == NULL)
		return false;

	pDir->Rename(p_newName);

	//web�˸��� 
	CWebProjectFile::Instance()->RenameFolder(GetProjectId(), p_sFolderPath, p_newName);

	return true; 
}

CString CProjectData::GetDirString(CString sName, CProjectDir* p_parentDir)
{
	CString temp = sName;
	CProjectDir* tempDir = p_parentDir;
	while (tempDir != &m_rootDir)
	{
		CString sDirString = tempDir->m_sName;
		temp = sDirString + L"\\" + temp;
		tempDir = tempDir->m_parent;
	}
	return temp;
}