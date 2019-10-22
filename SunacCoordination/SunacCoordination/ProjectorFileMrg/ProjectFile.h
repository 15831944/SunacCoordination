#pragma once
#include <vector>

using namespace std;

class CProjectorDir;

class CPrjNode
{
public:
	CPrjNode();
	virtual ~CPrjNode();

	virtual bool IsDirectory()const = 0;

public:
	int m_id;	//ID��Ҫ���ڴ�web��̨����������
	CString m_sName; //�ļ������ļ�����
	CProjectorDir* m_parent;  //���ļ���
};

//////////////////////////////////////////////////////////////////////////
class CProjectFile :public CPrjNode
{
public:
	CProjectFile();
	~CProjectFile();
	bool IsDirectory()const { return false; }

public:
	CString m_sSaveName; //ʵ�ʴ洢�����ƣ�ͨ��ΪGUID
	CString m_sFileUrl;  //�ļ����ص�ַ

	CString m_sCreator;		//������
	CString m_sCreateTime;  //����ʱ��
	CString m_sUpdator;		//������Ա
	CString m_sUpdateTime;	//����ʱ��
	CString m_sFileSize;	//�ļ���С
};

//////////////////////////////////////////////////////////////////////////
class CProjectorDir :public CPrjNode
{
public:
	CProjectorDir();
	~CProjectorDir();
	bool IsDirectory()const { return true; }

	bool AddFile(CProjectFile p_file);
	bool FindFile(CString p_fileName, CProjectFile &p_fileOut);
	bool DeleteFile(CString p_fileName);

	bool AddFolder(CString sFolderName);
	bool DeleteFolder(CString p_folder);

	void Rename(CString newName);

protected:
	vector<CProjectFile> m_subFiles; //���ļ�
	vector<CProjectorDir> m_subDirs;//���ļ���
};



