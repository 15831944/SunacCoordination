#pragma once
#include <vector>
#include "ProjectFile.h"

//��Ŀ������Ϣ
class CProjectInfo
{
public:
	CProjectInfo();
	~CProjectInfo();

public:
	int m_id;
	CString m_sName; //��Ŀ����
	CString m_sArea; //����
	CString m_sCityCompany;//���й�˾
};


//////////////////////////////////////////////////////////////////////////
class CProjectData //��Ŀ������
{
public:
	CProjectData();
	~CProjectData();

	static CString GenerateGuid();
	static bool DownloadFile(const CString& strFileURLInServer, const CString & strFileLocalFullPath);
	static bool DownloadFile(CProjectFile p_prjFile);
	static bool UploadFile(CString p_sFileName, CString p_saveName);

	bool UploadFile(CString p_sFileName, CString p_saveName, CString  p_sParentDir); //p_sParentDir Ϊ�����ĸ�Ŀ¼
	bool UploadFile(CString p_sFileName, CString p_saveName, CProjectDir*  p_parentDir);


	bool AddFile(CString p_sFileName, CString  p_sParentDir); //p_sParentDir��ָ�ϴ����ĸ�Ŀ¼��
	void DeleteFile();

public:
	CProjectInfo m_prjInfo;
	CProjectDir m_rootDir;	
};

