#pragma once
#include <vector>
#include "ProjectFile.h"
#include "ProjectInfo.h"


//��Ŀ�����࣬����������Ŀ
class CProjectFileMrg
{
	CProjectFileMrg();
public:
	static CProjectFileMrg* Instance();
	~CProjectFileMrg();

	void InitTestData();

	CProjectData* GetProject(CString p_sPrjName);
	vector<CProjectData*> GetProjectsByArea(CString p_sArea); //��ѯ����������Щ��Ŀ

	vector<CString> GetAllAreas()const;

protected:
	void Clear();

public:
	vector<CProjectData*> m_projects;
};

