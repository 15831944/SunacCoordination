#pragma once
#include <vector>
#include "ProjectFile.h"
#include "FileUploadDownload.h"

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

	//���º���p_sDirPathInProject��ʾ����������Ŀ�е�Ŀ¼,ʹ��\�ָ�
	bool AddFile(CString p_sFilePath, CString  p_sDirPathInProject, FileUpDownCB p_cbFunc, long long n_fileSize); //p_sParentDirPath��ָ�ϴ����ĸ�Ŀ¼��
	bool DeleteFile(CString p_sFileName, CString  p_sDirPathInProject);
	bool DownloadFile(CString p_sDirPathInProject, CString p_fileName, CString p_sFileLocalPath, FileUpDownCB p_cbFunc);

	bool AddFolder(CString  p_sDirPathInProject, CString p_sFolderName);  //p_sFolderNameΪ�ļ���������������·��
	bool DeleteFolder(CString  p_sFolderPath);
	bool RenameFolder(CString p_sFolderPath, CString p_newName);

	//Ŀ¼�㼶��\�ָ���磺A��\ʩ��ͼ ��ʾA���ļ����µ�ʩ��ͼ�ļ���
	CProjectDir* FindDir(CString p_dirPath);
	CProjectFile* GetFileByDirAndName(CString p_sDirPathInProject, CString p_fileName);

	const CProjectDir* GetRootDir() { return &m_rootDir; }
	CProjectInfo GetPrjInfo()const { return m_prjInfo; }
	CString GetProjectId()const;


protected:
	static void FileUpCBFunc(CUpDownFilePara*);
	static void FileDownCBFunc(CUpDownFilePara*);

protected:
	CProjectInfo m_prjInfo;
	CProjectDir m_rootDir;	
};

