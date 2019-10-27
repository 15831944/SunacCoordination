#pragma once
#include <vector>

using namespace std;

class CProjectDir;

class CPrjNode
{
public:
	CPrjNode();
	virtual ~CPrjNode();

	virtual bool IsDirectory()const = 0;

public:
	int m_id;	//ID��Ҫ���ڴ�web��̨����������
	CString m_sName; //�ļ������ļ�����
	CProjectDir* m_parent;  //���ļ���
};

//////////////////////////////////////////////////////////////////////////
class CProjectFile :public CPrjNode
{
public:
	CProjectFile();
	~CProjectFile();
	bool IsDirectory()const { return false; }

protected:
	static CString FileSizeToString(long long p_size);

public:
	CString m_sSaveName; //ʵ�ʴ洢�����ƣ�ͨ��ΪGUID
	CString m_sFileUrl;  //�ļ����ص�ַ

	CString m_sCreator;		//������
	CString m_sCreateTime;  //����ʱ��
	CString m_sUpdator;		//������Ա
	CString m_sUpdateTime;	//����ʱ��
	CString m_sFileSize;	//�ļ���С
	long long m_fileSize;
};

//////////////////////////////////////////////////////////////////////////
class CProjectDir :public CPrjNode
{
public:
	CProjectDir();
	~CProjectDir();
	bool IsDirectory()const { return true; }

	bool AddFile(CProjectFile p_file);
	bool DeleteFile(CString p_fileName);
	bool FindFile(CString p_fileName, CProjectFile &p_fileOut);

	CProjectDir*  AddFolder(CString p_sFolderName);
	bool DeleteFolder(CString p_sFolderName);
	CProjectDir* GetFolder(CString p_folder);

	void Rename(CString newName);

public:
	vector<CProjectFile> m_subFiles; //���ļ�
	vector<CProjectDir*> m_subDirs;//���ļ���

};



