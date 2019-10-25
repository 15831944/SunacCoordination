#pragma once

#include "ProjectorFileMrg\ProjectInfo.h"
#include "GridCtrlEx.h"


// CProjectManagementDlg �Ի���

class CProjectManagementDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CProjectManagementDlg)

public:
	CProjectManagementDlg(CProjectData* pPrjData, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProjectManagementDlg();
	void FillPjtMngTreeCtrl(); //������ؼ�
	void InitGridCtrl(); //��ʼ��GridCtrl
	void FillPjtGridCtrl(CProjectDir* SelectedDir); //���GridCtrl
	CProjectDir* FindClkDir(HTREEITEM CurClkItem); //ͨ���ݹ��ҵ����ؼ���Ӧ���ļ���λ��

// �Ի�������
	enum { IDD = IDD_DIALOG_PROJECTMANAGEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProjectData* m_pPrjData;
	CProjectDir* m_selectedDir;

	CButton m_BtnDeleteAll;
	CButton m_BtnDownloadAll;
	CButton m_BtnUpload;
	CStatic m_StcArea;
	CStatic m_StcCompany;
	CStatic m_StcDocName;
	CStatic m_StcDocSize;
	CStatic m_StcPrjName;
	CStatic m_StcUpdaterName;
	CStatic m_StcUpdateTime;
	CStatic m_StcUploaderName;
	CStatic m_StcUploadTime;
	CTreeCtrl m_TreePrjDir;
	CGridCtrl m_PjtManagementGridCtrl;
	afx_msg void OnBnClickedButtonUpload();
	virtual BOOL OnInitDialog();
	CStatic m_StcRootName;
	afx_msg void OnNMClickTreePrjdir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonNewdir();
};
