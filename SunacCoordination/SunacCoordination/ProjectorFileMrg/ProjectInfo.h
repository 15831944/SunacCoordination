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

class CProjectFileMrg;
class CWebProjectFile;
//////////////////////////////////////////////////////////////////////////
class CProjectData //��Ŀ������
{
	friend CProjectFileMrg;
	friend CWebProjectFile;
public:
	CProjectData();
	~CProjectData();

	CString GetDirString(CString sName, CProjectDir* p_parentDir);

	static CString GenerateGuid();

	//����Ŀ¼����������������Ŀ¼
	bool AddFile(CString p_sFilePath, CString  p_sParentFolderPath); //p_sParentDirPath��ָ�ϴ����ĸ�Ŀ¼��
	//bool AddFile(CString p_sFilePath, CProjectDir* p_pParentDir); //p_pParentDir��ָ�ϴ����ĸ�Ŀ¼��
	bool DeleteFile(CString p_sFileName, CString  p_sParentFolderPath);

	bool AddFolder(CString  p_sParentFolderPath, CString p_sFolderName);
	bool DeleteFolder(CString  p_sFolderPath);
	bool RenameFolder(CString p_sFolderPath, CString p_newName);

	//Ŀ¼�㼶��\�ָ���磺A��\ʩ��ͼ ��ʾA���ļ����µ�ʩ��ͼ�ļ���
	CProjectDir* FindDir(CString p_dirName);

	const CProjectDir* GetRootDir() { return &m_rootDir; }
	CProjectInfo GetPrjInfo()const { return m_prjInfo; }
	CString GetProjectId()const;

protected:
	CProjectInfo m_prjInfo;
	CProjectDir m_rootDir;	
};

