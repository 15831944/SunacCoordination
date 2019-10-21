#pragma once
#include <vector>
#include "ProjectFile.h"


class CProjectInfo
{
public:
	CProjectInfo();
	~CProjectInfo();

protected:
	CString m_sName; //��Ŀ����
	CString m_sArea; //����
	CString m_sCityCompany;//���й�˾
};

//////////////////////////////////////////////////////////////////////////
class CProjectData : public CProjectorDir
{
public:
	CProjectData();
	~CProjectData();

	static bool DownloadFile(const CString& strFileURLInServer, const CString & strFileLocalFullPath);
	static bool DownloadFile(CProjectFile p_prjFile);
	static bool UploadFile(CString p_sFileName, CString p_saveName);

	bool UploadFile(CString p_sFileName, CString p_saveName, CString  p_sParentDir); //p_sParentDir Ϊ�����ĸ�Ŀ¼
	bool UploadFile(CString p_sFileName, CString p_saveName, CProjectorDir*  p_parentDir);
		
protected:
	CProjectInfo m_prjInfo;
};

