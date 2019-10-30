#pragma once
#include <vector>

class CFileUpDownLoad //��Ŀ������
{
	CFileUpDownLoad();
public:
	static CFileUpDownLoad* Instance();
	~CFileUpDownLoad();

	void StopAll(); //ֹͣ�����ϴ�����,��Ҫ�ǳ����˳�ʱ

	//strFileURLInServer�������ļ���URL,  strFileLocalFullPath��ŵ����ص�·��
	static bool DownloadFile(const CString& strFileURLInServer, const CString & strFileLocalFullPath);
	//static bool DownloadFile(CProjectFile p_prjFile);

	static bool UploadFile(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir);


	void UploadFileByThread(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir);

protected:


};

