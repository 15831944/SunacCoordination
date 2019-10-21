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

	static bool DownloadFile(CProjectFile p_prjFile);
	static bool UploadFile(CString p_sFileName, CString p_saveName);

	bool UploadFile(CString p_sFileName, CString p_saveName, CProjectorDir*  p_sParentDir);
		
protected:
	CProjectInfo m_prjInfo;
};

