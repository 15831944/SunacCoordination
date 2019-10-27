#pragma once
#include "..\ProjectorFileMrg\ProjectFileMrg.h"


class CWebProjectFile
{
	CWebProjectFile();
public:
	static CWebProjectFile* Instance();
	~CWebProjectFile();

	//��ȡ��ǰ�û��µ���Ŀ
	bool GetAllProjectInfo();
	
	bool UpdateFile(CString p_prjectId, CString p_dir, CString p_fileSaveName, CString p_fileName);
	bool DeleteFile(CString p_prjectId, CString p_dir, CString p_fileName);

	bool NewFolder(CString p_prjectId, CString p_dir);
	bool RenameFolder(CString p_prjectId, CString p_dir, CString p_sNewDirName);
	bool DeleteFolder(CString p_prjectId, CString p_dir);


protected:

};

