// ProjectManagementDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProjectManagementDlg.h"
#include "NewDirDlg.h"
#include "afxdialogex.h"
#include "../UI/GridCtrl_src/GridCtrlUtil.h"
#include "..\ProjectorFileMrg\ProjectFileMrg.h"
#include "..\ProjectorFileMrg\ProjectInfo.h"
#include "..\ProjectorFileMrg\FileUploadDownload.h"
#include "..\ProjectorFileMrg\ProjectFile.h"
#include "../Common/ComFun_String.h"
#include "../GlobalSetting.h"

#define WM_FILE_STATE_CHANGE (WM_USER + 100)  


// CProjectManagementDlg 对话框

IMPLEMENT_DYNAMIC(CProjectManagementDlg, CAcUiDialog)

CProjectManagementDlg::CProjectManagementDlg(CProjectData* pPrjData,CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CProjectManagementDlg::IDD, pParent)
	, m_pPrjData(pPrjData)
{

}

CProjectManagementDlg::~CProjectManagementDlg()
{

}

void CProjectManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DELETEALL, m_BtnDeleteAll);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOADALL, m_BtnDownloadAll);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD, m_BtnUpload);
	DDX_Control(pDX, IDC_STATIC_AREA, m_StcArea);
	DDX_Control(pDX, IDC_STATIC_COMPANY, m_StcCompany);
	DDX_Control(pDX, IDC_STATIC_DOCUMENTNAME, m_StcDocName);
	DDX_Control(pDX, IDC_STATIC_DOCUMENTSIZE, m_StcDocSize);
	DDX_Control(pDX, IDC_STATIC_PROJECTNAME, m_StcPrjName);
	DDX_Control(pDX, IDC_STATIC_UPDATERNAME, m_StcUpdaterName);
	DDX_Control(pDX, IDC_STATIC_UPDATETIME, m_StcUpdateTime);
	DDX_Control(pDX, IDC_STATIC_UPLOADERNAME, m_StcUploaderName);
	DDX_Control(pDX, IDC_STATIC_UPLOADTIME, m_StcUploadTime);
	DDX_Control(pDX, IDC_TREE_PRJDIR, m_TreePrjDir);
	DDX_Control(pDX, IDC_GRIDCTRL_PRJMANAGEMENT, m_PjtManagementGridCtrl);
	//DDX_Control(pDX, IDC_STATIC_ROOTNAME, m_StcRootName);
}

void CProjectManagementDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();

	//// 释放非模态对话框的内存
	//delete this;
	//if (g_connectorDlg != NULL)
	//{
	//	g_connectorDlg = NULL;
	//}
}
void CProjectManagementDlg::OnClose()
{
	CAcUiDialog::OnClose();

	// 销毁对话框
	//DestroyWindow();
}
LRESULT CProjectManagementDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

BEGIN_MESSAGE_MAP(CProjectManagementDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CProjectManagementDlg::OnBnClickedButtonUpload)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PRJDIR, &CProjectManagementDlg::OnNMClickTreePrjdir)
	ON_NOTIFY(NM_CLICK, IDC_GRIDCTRL_PRJMANAGEMENT, OnGridClick)
	ON_BN_CLICKED(IDC_BUTTON_NewDir, &CProjectManagementDlg::OnBnClickedButtonNewdir)
	ON_BN_CLICKED(IDC_BUTTON_DeleteDir, &CProjectManagementDlg::OnBnClickedButtonDeletedir)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOADALL, &CProjectManagementDlg::OnBnClickedButtonDownloadall)
	ON_BN_CLICKED(IDC_BUTTON_DELETEALL, &CProjectManagementDlg::OnBnClickedButtonDeleteall)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_FILE_STATE_CHANGE, OnUpdateFileState)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TREE_PRJDIR, &CProjectManagementDlg::OnCustomdraw)
END_MESSAGE_MAP()

CString SelFilePath()
{
	TCHAR           szFolderPath[MAX_PATH] = {0};  
	CString         strFolderPath = TEXT("");  

	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	
	
	sInfo.pidlRoot   = NULL;  
	sInfo.lpszTitle   = _T("请选择文件存储路径");  
	sInfo.ulFlags   = BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_DONTGOBELOWDOMAIN|BIF_USENEWUI;  
	sInfo.lpfn     = NULL;  

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
		{  
			strFolderPath = szFolderPath;  
		}
		else
		{
			return _T("");
		}
	}  
	if(lpidlBrowse != NULL)  
	{  
		::CoTaskMemFree(lpidlBrowse);  
	}  

	return strFolderPath;  
}

void CProjectManagementDlg::FillPjtMngTreeCtrl()
{
	m_TreePrjDir.DeleteAllItems();
	HICON Icon[3];
	CImageList *ImageList = new CImageList();
	ImageList->Create(16,16,ILC_COLOR24,12,12);
	Icon[1] = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	Icon[2] = AfxGetApp()->LoadIcon(IDI_ICON_DOCUMENT);

	for (int i = 0; i < 3; i++)
	{
		ImageList->Add(Icon[i]);
	}
	m_TreePrjDir.SetImageList(ImageList, TVSIL_NORMAL);

	HTREEITEM hTreeItem;
	hTreeItem = m_TreePrjDir.InsertItem(L"项目文件夹", 3, 3,TVI_ROOT);
	m_TreePrjDir.SetItemHeight(30);

	vector<const CProjectDir*> allDirs;
	vector<HTREEITEM> allItems;
	COLORREF DefaultColor;
	DefaultColor = RGB(0x09, 0x37, 0xF7);
	allDirs.push_back(m_pPrjData->GetRootDir());
	allItems.push_back(hTreeItem);

	for (UINT i = 0; i < allDirs.size(); i++)
	{
		for (UINT j = 0; j < allDirs[i]->m_subDirs.size(); j++)
		{
			HTREEITEM item = m_TreePrjDir.InsertItem(allDirs[i]->m_subDirs[j]->m_sName, 0, 0, allItems[i]);
			m_TreePrjDir.SetItemData(allItems[i], DefaultColor);
			allDirs.push_back(allDirs[i]->m_subDirs[j]);
			allItems.push_back(item);
		}
	}
	m_TreePrjDir.Expand(hTreeItem, TVE_EXPAND);
}

LRESULT CProjectManagementDlg::OnUpdateFileState(WPARAM FileName, LPARAM)
{
	FillPjtGridCtrl(m_selectedDir);

	/*vector<bool> flag;
	for (vector<bool>::iterator it = flag.begin(); it != flag.end(); it++)
	{
		*it = true;
	}*/
	for (int i = 0; i < m_PjtManagementGridCtrl.GetRowCount(); i++)
	{
		if (m_PjtManagementGridCtrl.GetItemText(i, 1) == *(CString *)FileName)
		{
			AfxMessageBox(m_PjtManagementGridCtrl.GetItemText(i, 1) + _T(" ") + m_PjtManagementGridCtrl.GetItemText(i, 7));
		}
		/*if (m_PjtManagementGridCtrl.GetItemText(i, 7) == "下载成功！")
		{
			CString TempString;
			TempString.Format(_T("%s下载成功！"), m_PjtManagementGridCtrl.GetItemText(i, 1));
			AfxMessageBox(TempString);
		}
		if (m_PjtManagementGridCtrl.GetItemText(i, 7) == "上传成功！")
		{
			CString TempString;
			TempString.Format(_T("%s 上传成功！"), m_PjtManagementGridCtrl.GetItemText(i, 1));
			AfxMessageBox(TempString);
		}*/
	}
	return 0;
}
// CProjectManagementDlg 消息处理程序

static void UIFileUpCBFunc(CUpDownFilePara* p_fileUpdownPara)
{
	CString* FileName = &p_fileUpdownPara->sFileName; 

	//更新当前文件的状态
	::SendMessage(g_projectManagementDlg->m_hWnd, WM_FILE_STATE_CHANGE, (WPARAM)FileName, 0);

	//g_projectManagementDlg->FillPjtGridCtrl(g_projectManagementDlg->m_selectedDir); //采用消息机制更新界面，此处注释
}

void CProjectManagementDlg::OnBnClickedButtonUpload()
{
	CString FileName;
	CString filter = L"参数文件(*.dwg)|*.dwg|All Files(*.*)|*.*||";
    CFileDialog dlg(FALSE, L"dwg", L"*.dwg", NULL, filter);
	if (!m_selectedDir)
	{
		AfxMessageBox(L"请选择有效目录！");
		return;
	}
	if(dlg.DoModal() == IDOK)
	{
		USES_CONVERSION;
		CString PathName = dlg.GetPathName();
	    long long FileSize;
		CFileStatus FileStatus;
		if (CFile::GetStatus(PathName, FileStatus))
		{
			FileSize = FileStatus.m_size;
		}
		
		FileName = FilePathToFileName(PathName);
		CString ParentPath = m_pPrjData->GetDirString(L"", m_selectedDir);//返回文件夹的路径

		m_pPrjData->AddFile(PathName, ParentPath, UIFileUpCBFunc, FileSize);
	}

	FillPjtGridCtrl(m_selectedDir);
}


BOOL CProjectManagementDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	CRect DlgRect,CADRect;
	GetWindowRect(DlgRect);
	GetParent()->GetWindowRect(CADRect);

	int xPos, yPos;
	xPos = (CADRect.Width() - DlgRect.Width())/2;
	yPos = (CADRect.Height() - DlgRect.Height())/2;
	
	SetWindowPos(NULL, xPos, yPos, 0, 0, SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);

	CFont font;
	font.CreatePointFont(160, L"");
	m_StcRootName.SetFont(&font);
	font.Detach();

	m_selectedDir = nullptr;

	InitGridCtrl();	

	FillPjtMngTreeCtrl();

	m_StcPrjName.SetWindowTextW(m_pPrjData->GetPrjInfo().m_sName);
	m_StcArea.SetWindowTextW(m_pPrjData->GetPrjInfo().m_sArea);
	m_StcCompany.SetWindowTextW(m_pPrjData->GetPrjInfo().m_sCityCompany);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CProjectManagementDlg::InitGridCtrl()
{
	CAutoLock a(&m_Lock);

	m_PjtManagementGridCtrl.DeleteAllItems();
	
	m_PjtManagementGridCtrl.SetFixedRowCount(1);
	m_PjtManagementGridCtrl.SetColumnCount(10);
	m_PjtManagementGridCtrl.SetItemText(0, 1, L"文件名称");
	m_PjtManagementGridCtrl.SetItemText(0, 2, L"创建人");
	m_PjtManagementGridCtrl.SetItemText(0, 3, L"创建时间");
	m_PjtManagementGridCtrl.SetItemText(0, 4, L"更新人员");
	m_PjtManagementGridCtrl.SetItemText(0, 5, L"更新时间");
	m_PjtManagementGridCtrl.SetItemText(0, 6, L"文件大小");
	m_PjtManagementGridCtrl.SetItemText(0, 7, L"文件状态");

	m_PjtManagementGridCtrl.SetHeaderWidth(L"3.33;+;8.33;15.56;8.33;15.56;8.89;8.33;6;6");

	/*m_PjtManagementGridCtrl.SetColumnWidth(0, 25);
	m_PjtManagementGridCtrl.SetColumnWidth(1, 205);
	m_PjtManagementGridCtrl.SetColumnWidth(2, 75);
	m_PjtManagementGridCtrl.SetColumnWidth(3, 140);
	m_PjtManagementGridCtrl.SetColumnWidth(4, 75);
	m_PjtManagementGridCtrl.SetColumnWidth(5, 140);
	m_PjtManagementGridCtrl.SetColumnWidth(6, 80);
	m_PjtManagementGridCtrl.SetColumnWidth(7, 75);
	m_PjtManagementGridCtrl.SetColumnWidth(8, 40);
	m_PjtManagementGridCtrl.SetColumnWidth(9, 40);*/

	m_PjtManagementGridCtrl.SetSingleRowSelection(TRUE);

	//COLORREF color=RGB(220,220,220);
}

void CProjectManagementDlg::FillPjtGridCtrl(CProjectDir* SelectedDir)
{
	CAutoLock a(&m_Lock);

	m_PjtManagementGridCtrl.DeleteAllItems();
	InitGridCtrl();
	m_PjtManagementGridCtrl.SetRowCount((int)SelectedDir->m_subFiles.size() + 1);
	for (UINT i = 1; i <= SelectedDir->m_subFiles.size(); i++)
	{
		m_PjtManagementGridCtrl.SetCellType(i, 0, RUNTIME_CLASS(CGridCellCheck));
		m_PjtManagementGridCtrl.SetItemText(i, 1, SelectedDir->m_subFiles[i - 1].m_sName);
		m_PjtManagementGridCtrl.SetItemState(i, 1, GVIS_READONLY);

		m_PjtManagementGridCtrl.SetItemText(i, 2, SelectedDir->m_subFiles[i - 1].m_sCreator);
		m_PjtManagementGridCtrl.SetItemState(i, 2, GVIS_READONLY);

		m_PjtManagementGridCtrl.SetItemText(i, 3, SelectedDir->m_subFiles[i - 1].m_sCreateTime);
		m_PjtManagementGridCtrl.SetItemState(i, 3, GVIS_READONLY);

		m_PjtManagementGridCtrl.SetItemText(i, 4, SelectedDir->m_subFiles[i - 1].m_sUpdator);
		m_PjtManagementGridCtrl.SetItemState(i, 4, GVIS_READONLY);

		m_PjtManagementGridCtrl.SetItemText(i, 5, SelectedDir->m_subFiles[i - 1].m_sUpdateTime);
		m_PjtManagementGridCtrl.SetItemState(i, 5, GVIS_READONLY);

		m_PjtManagementGridCtrl.SetItemText(i, 6, SelectedDir->m_subFiles[i - 1].m_sFileSize);
		m_PjtManagementGridCtrl.SetItemState(i, 6, GVIS_READONLY);

		m_PjtManagementGridCtrl.SetItemText(i, 7, EProjectFileStateToCString(SelectedDir->m_subFiles[i - 1].m_fileState));
		m_PjtManagementGridCtrl.SetItemState(i, 7, GVIS_READONLY);

		m_PjtManagementGridCtrl.SetItemText(i, 8, L"下载");
		CGridCtrlUtil::SetCellButtonType(m_PjtManagementGridCtrl, i, 8);
		m_PjtManagementGridCtrl.SetItemState(i, 8, GVIS_READONLY);

		m_PjtManagementGridCtrl.SetItemText(i, 9, L"删除");
		CGridCtrlUtil::SetCellButtonType(m_PjtManagementGridCtrl, i, 9);
		m_PjtManagementGridCtrl.SetItemState(i, 9, GVIS_READONLY);
	}
	UpdateData();
}
void CProjectManagementDlg::UpdateGridCtrlState(CProjectDir* SelectedDir)
{
	CAutoLock a(&m_Lock);
	
	UINT rowCount = (UINT)(m_PjtManagementGridCtrl.GetRowCount());
	for (UINT i = 1; i <= SelectedDir->m_subFiles.size() && i<rowCount; i++)
	{
		m_PjtManagementGridCtrl.SetItemText(i, 7, EProjectFileStateToCString(SelectedDir->m_subFiles[i - 1].m_fileState));	
	}

	UpdateData();
}

CProjectDir* CProjectManagementDlg::FindClkDir(HTREEITEM CurClkItem)
{
	CProjectDir* RootDir = (CProjectDir *)m_pPrjData->GetRootDir();
	if (CurClkItem == NULL)
	{
		return RootDir;
	}
	if (CurClkItem == m_TreePrjDir.GetRootItem())
	{
		return RootDir;
	}
	CProjectDir* pParentDir = FindClkDir(m_TreePrjDir.GetParentItem(CurClkItem));
	assert(pParentDir);
	CProjectDir* pDir = pParentDir->GetSubFolder(m_TreePrjDir.GetItemText(CurClkItem));
	//assert(pDir);
	return pDir;
}


void CProjectManagementDlg::OnNMClickTreePrjdir(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	CPoint CurClkPoint;
	GetCursorPos(&CurClkPoint);
	m_TreePrjDir.ScreenToClient(&CurClkPoint);
	HTREEITEM CurClkItem;
	/*UINT flag;
	CurClkItem = m_treeProjectManagement.HitTest(CurClkPoint, &flag);*/
	CurClkItem = m_TreePrjDir.GetSelectedItem();
	/*if (m_TreePrjDir.ItemHasChildren(CurClkItem))
	{
		InitGridCtrl();
		return;
	}*/
	m_selectedDir = FindClkDir(CurClkItem);
	if (m_selectedDir == nullptr)
	{
		return;
	}

	if (m_selectedDir->m_subFiles.empty())
	{
		InitGridCtrl();
		return;
	}

	FillPjtGridCtrl(m_selectedDir);
}


void CProjectManagementDlg::OnBnClickedButtonNewdir()
{
	if (m_TreePrjDir.GetSelectedCount() == 0)
	{
		AfxMessageBox(L"请选择正确的目录！");
		return;
	}

	CNewDirDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}

	CString sNewDir = dlg.m_sNewDir;
	if (sNewDir == L"")
	{
		AfxMessageBox(L"请输入目录名！");
		return;
	}
	

	CPoint CurClkPoint;
	GetCursorPos(&CurClkPoint);
	m_TreePrjDir.ScreenToClient(&CurClkPoint);
	HTREEITEM CurClkItem;
	CurClkItem = m_TreePrjDir.GetSelectedItem();
	m_selectedDir = FindClkDir(CurClkItem);
	m_selectedDir->AddFolder(sNewDir);
	m_pPrjData->AddFolder(m_pPrjData->GetDirString(L"", m_selectedDir), sNewDir);
	m_TreePrjDir.InsertItem(sNewDir, 0, 0, CurClkItem);
	m_TreePrjDir.Expand(CurClkItem, TVE_EXPAND);
}

void CProjectManagementDlg::OnGridClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNMHDR;
	m_nClkRow = pItem->iRow;
	m_nClkCol = pItem->iColumn;
	CString sSelectedFileName = m_PjtManagementGridCtrl.GetItemText(m_nClkRow, 1);
	CString sUserName = GlobalSetting::GetInstance()->m_userName;
	if (m_selectedDir == NULL)
	{
		return;
	}

	CString sSelectedFileParentPath = m_pPrjData->GetDirString(L"", m_selectedDir);
	CProjectFile SelectedFile;

	m_selectedDir->FindFile(sSelectedFileName, SelectedFile);
	m_StcDocName.SetWindowTextW(SelectedFile.m_sName);
	//m_StcDocSize.SetWindowTextW(SelectedFile.GetFileSize()); 
	m_StcUpdaterName.SetWindowTextW(SelectedFile.m_sCreator); 
	m_StcUpdateTime.SetWindowTextW(SelectedFile.m_sCreateTime); 
	m_StcUploaderName.SetWindowTextW(SelectedFile.m_sUpdator); 
	m_StcUploadTime.SetWindowTextW(SelectedFile.m_sUpdateTime);

	if (m_nClkCol == 8 && m_nClkRow != 0 && m_nClkRow <= (int)(m_selectedDir->m_subFiles.size()))//下载
	{
		CString sFolderPath = SelFilePath();
		if (sFolderPath == _T(""))
			return;

		if (sFolderPath.ReverseFind(L'\\') != sFolderPath.GetLength() - 1)
		{
			sFolderPath += L"\\";
		}
		

		CString FileFullPath;
		FileFullPath = sFolderPath + sSelectedFileName;
		if(JHCom_FileExist(FileFullPath))
		{
			if(IDNO == AfxMessageBox(_T("文件已存在，是否覆盖?"), MB_YESNO))
			{
				return;
			}
		}
		m_pPrjData->DownloadFile(sSelectedFileParentPath, sSelectedFileName, FileFullPath, UIFileUpCBFunc);
		FillPjtGridCtrl(m_selectedDir);
	}

	if (m_nClkCol == 9 && m_nClkRow != 0 && m_nClkRow <= (int)(m_selectedDir->m_subFiles.size()))//删除
	{
		if (IDYES == AfxMessageBox(L"确定删除?", MB_YESNO))
		{
			if (SelectedFile.m_sCreator == sUserName)//只有创建者才能删除该文件
			{
				m_pPrjData->DeleteFile(sSelectedFileName,sSelectedFileParentPath);
				FillPjtGridCtrl(m_selectedDir);
			}
			else AfxMessageBox(L"该文件并非你创建，所以无法删除！");
		}
	}
}

void CProjectManagementDlg::OnBnClickedButtonDeletedir()
{
	if (!m_selectedDir)
	{
		AfxMessageBox(L"请选择正确的目录！");
		return;
	}
	
	CString sDeleteDir;
	sDeleteDir = m_pPrjData->GetDirString(L"", m_selectedDir);
	if (sDeleteDir == "\0")
	{
		AfxMessageBox(L"此目录不能删除！");
		return;
	}
	m_pPrjData->DeleteFolder(sDeleteDir);
	HTREEITEM DeleteTreeItem;
	DeleteTreeItem = m_TreePrjDir.GetSelectedItem();
	m_TreePrjDir.DeleteItem(DeleteTreeItem);
}


void CProjectManagementDlg::OnBnClickedButtonDownloadall()
{
	bool flag = false;
	for(int i = 1; i < m_PjtManagementGridCtrl.GetRowCount(); i++)
	{
		CGridCellBase* pCell = m_PjtManagementGridCtrl.GetCell(i, 0);
		if(((CGridCellCheck* )pCell)->GetCheck())
		{
			flag = true;
			break;
		}
	}
	if (flag == false)
	{
		AfxMessageBox(L"请至少选择一个文件!");
		return;
	}


	CString sFolderPath = SelFilePath();
	if (sFolderPath == _T(""))
	{
		return;
	}

	CString sDirPathInProject = m_selectedDir->GetDirFullPath();
	
	vector<CString> fileNames;
	for(int i = 1; i < m_PjtManagementGridCtrl.GetRowCount(); i++)
	{
		CGridCellBase* pCell = m_PjtManagementGridCtrl.GetCell(i, 0);
		if(((CGridCellCheck* )pCell)->GetCheck())
		{
			CString sCheckedFileName = m_PjtManagementGridCtrl.GetItemText(i, 1);
			CString sFileSavePath = sFolderPath + L"\\" + sCheckedFileName;

			//判断文件是否存在，若存在，则提示并退出
			if(JHCom_FileExist(sFileSavePath))
			{
				CString sTips;
				sTips.Format(_T("文件%s已存在，是否覆盖?"), sCheckedFileName);
				if(IDNO == AfxMessageBox(sTips, MB_YESNO))
				{
					continue;
				}
			}
			fileNames.push_back(sCheckedFileName);
		}
	}



	for (UINT i = 0; i < fileNames.size(); i++)
	{
		CString sFileSavePath = sFolderPath + L"\\" + fileNames[i];
		m_pPrjData->DownloadFile(sDirPathInProject, fileNames[i], sFileSavePath, UIFileUpCBFunc);
	}
}



void CProjectManagementDlg::OnBnClickedButtonDeleteall()
{
	bool flag = false;
	CString sSelectedFileName = m_PjtManagementGridCtrl.GetItemText(m_nClkRow, 1);
	CProjectFile SelectedFile;
	m_selectedDir->FindFile(sSelectedFileName, SelectedFile);
	CString sUserName = GlobalSetting::GetInstance()->m_userName;

	for(int i = 1; i < m_PjtManagementGridCtrl.GetRowCount(); i++)
	{
		CGridCellBase* pCell = m_PjtManagementGridCtrl.GetCell(i, 0);
		if(((CGridCellCheck* )pCell)->GetCheck())
		{
			flag = true;
			break;
		}
	}
	if (flag == false)
	{
		AfxMessageBox(L"请至少选择一个文件!");
		return;
	}


	if (IDYES == AfxMessageBox(L"确定删除?", MB_YESNO))
	{
		for(int i = 1; i < m_PjtManagementGridCtrl.GetRowCount(); i++)
		{
			CGridCellBase* pCell = m_PjtManagementGridCtrl.GetCell(i, 0);
			if(((CGridCellCheck* )pCell)->GetCheck())
			{
				CString sCheckedFileName = m_PjtManagementGridCtrl.GetItemText(i, 1);
				CString sCheckedParentPath = m_pPrjData->GetDirString(L"", m_selectedDir);		
				if (SelectedFile.m_sCreator == sUserName)//只有创建者才能删除该文件
				{
					m_pPrjData->DeleteFile(sCheckedFileName, sCheckedParentPath);
				}
				else AfxMessageBox(L"该文件并非你创建，所以无法删除！");
			}
		}
		FillPjtGridCtrl(m_selectedDir);
	}
}

CProjectManagementDlg* g_projectManagementDlg = NULL;

BOOL CloseProjectManagementDlg()
{
	if (g_projectManagementDlg == NULL)
		return TRUE;
	BOOL ret = g_projectManagementDlg->DestroyWindow();
	if (ret)
	{
		delete g_projectManagementDlg;
		g_projectManagementDlg = NULL;
	}
	return ret;
}

void CProjectManagementDlg::OnCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	/*if (this->m_hWnd != pNMHDR->hwndFrom)
		return;*/

	LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)pNMHDR;
	switch (pCustomDraw->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		// Need to process this case and set pResult to CDRF_NOTIFYITEMDRAW, 
		// otherwise parent will never receive CDDS_ITEMPREPAINT notification. (GGH) 
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;

	case CDDS_ITEMPREPAINT:  //失去焦点后被选项各种颜色
		if (this->IsWindowEnabled() == 1)
		{
			if ((pCustomDraw->nmcd.uItemState & (CDIS_FOCUS)) == 0
				&& (pCustomDraw->nmcd.uItemState & (CDIS_SELECTED)) == CDIS_SELECTED) // selected
			{
				pCustomDraw->clrTextBk = RGB(0, 128, 220);  //失去焦点后被选项背景颜色
				pCustomDraw->clrText = RGB(255, 255, 255);  //失去焦点后被选项文本颜色
			}
			*pResult = CDRF_NOTIFYPOSTPAINT;
			return;
		}
		else {
			*pResult = CDRF_DODEFAULT;
			return;
		}
	case CDDS_ITEMPOSTPAINT:  //失去焦点后给被选项画一个外框，这个case根据需要可有可无
		if (this->IsWindowEnabled() == 1)
		{
			if ((pCustomDraw->nmcd.uItemState & (CDIS_FOCUS)) == 0
				&& (pCustomDraw->nmcd.uItemState & (CDIS_SELECTED)) == CDIS_SELECTED) // selected
			{
				CRect   rcText;
				HTREEITEM hItem = (HTREEITEM)pCustomDraw->nmcd.dwItemSpec;
				m_TreePrjDir.GetItemRect(hItem, &rcText, true);
				CPen penBlue(PS_SOLID, 1, RGB(255, 0, 0));  //失去焦点后被选项的外框颜色
				CDC* pDC = CDC::FromHandle(pCustomDraw->nmcd.hdc);
				CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
				CBrush* pOldBrush = pDC->SelectObject(pBrush);
				CPen* pOldPen = pDC->SelectObject(&penBlue);
				pDC->Rectangle(&rcText);
				pDC->SelectObject(pOldBrush);
				pDC->SelectObject(pOldPen);
			}
			*pResult = CDRF_SKIPDEFAULT;
			return;
		}
		else {
			*pResult = CDRF_DODEFAULT;
			return;
		}
	}
}

