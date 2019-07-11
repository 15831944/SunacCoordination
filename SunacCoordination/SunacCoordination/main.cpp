//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2013 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//
// AsdkAcUiSample.cpp : Defines the initialization routines for the DLL.
//
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include <rxmfcapi.h>
#include <dbgroup.h>
#include <geassign.h>
#include "main.h"
#include "AcExtensionModule.h"
#include "accmd.h"
#include "UI\menu\Menu_Def.h"
#include "UI\WindowDlg.h"
#include "UI\KitchenDlg.h"
#include "UI\BathroomDlg.h"
#include "UI\RailingDlg.h"
#include "UI\AirconditionerDlg.h"
#include "UI\DoorDlg.h"
#include "UI\FacadeDlg.h"
#include "UI\FillingDlg.h"
#include "UI\MoldingsDlg.h"
#include "UI\WaterproofDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern "C" HWND adsw_acadMainWnd();

/////////////////////////////////////////////////////////////////////////////
// Define the sole extension module object.

AC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);

//��
void CMD_SUNACWINDOW()
{
	CWindowDlg dlg;
	dlg.DoModal();
}

//����
void CMD_SUNACKITCHEN()
{
	CKitchenDlg dlg;
	dlg.DoModal();
}

//������
void CMD_SUNACBATHROOM()
{
	CBathroomDlg dlg;
	dlg.DoModal();
}

//��
void CMD_SUNACDOOR()
{
	CDoorDlg dlg;
	dlg.DoModal();
}

//����
void CMD_SUNACRAILING()
{
	CRailingDlg dlg;
	dlg.DoModal();
}

//�߽�
void CMD_SUNACMOLDINGS()
{
	CMoldingsDlg dlg;
	dlg.DoModal();
}

//������
void CMD_SUNACFILLING()
{
	CFillingDlg dlg;
	dlg.DoModal();
}

//�յ�
void CMD_SUNACAIRCONDITIONER()
{
	CAirconditionerDlg dlg;
	dlg.DoModal();
}

//��׼����
void CMD_SUNACFACADE()
{
	CFacadeDlg dlg;
	dlg.DoModal();
}

//��ˮ����
void CMD_SUNACWATERPROOF()
{
	CWaterproofDlg dlg;
	dlg.DoModal();
}

//ͳ������
void CMD_SUNACSTATISTICS()
{

}

void InitMenu()
{
	TRY
	{
		CAcadApplication IAcad(acedGetAcadWinApp()->GetIDispatch(TRUE));
		CAcadMenuBar IMenuBar(IAcad.get_MenuBar());

		long numberOfMenus;
		numberOfMenus = IMenuBar.get_Count();

		CAcadMenuGroups IMenuGroups(IAcad.get_MenuGroups());

		VARIANT index;
		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = 0;

		CAcadMenuGroup IMenuGroup(IMenuGroups.Item(index));
		CAcadPopupMenus IPopUpMenus(IMenuGroup.get_Menus());

		CString cstrMenuName = _T("�ڴ����");//�Զ������˵�

		VariantInit(&index);
		V_VT(&index) = VT_BSTR;
		V_BSTR(&index) = cstrMenuName.AllocSysString();

		IDispatch* pDisp=NULL;
		TRY{pDisp = IPopUpMenus.Item(index); pDisp->AddRef();} CATCH(COleDispatchException,e){}END_CATCH;

		if (pDisp==NULL) 
		{
			CAcadPopupMenu IPopUpMenu(IPopUpMenus.Add(cstrMenuName));
			UINT MenuIndex=0;

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;


			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;
			IPopUpMenu.AddMenuItem(index, _T("��"), _T("_SUNACWINDOW ")); //�����ǰ����»���_ �����Ŀո񶼲�����


			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;
			IPopUpMenu.AddMenuItem(index, _T("����"), _T("_SUNACKITCHEN "));

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;
			IPopUpMenu.AddMenuItem(index, _T("������"), _T("_SUNACBATHROOM "));

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;
			IPopUpMenu.AddMenuItem(index, _T("��"), _T("_SUNACDOOR "));

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;
			IPopUpMenu.AddMenuItem(index, _T("����"), _T("_SUNACRAILING "));

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;
			IPopUpMenu.AddMenuItem(index, _T("�߽�"), _T("_SUNACMOLDINGS "));

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;
			IPopUpMenu.AddMenuItem(index, _T("������"), _T("_SUNACFILLING "));

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;
			IPopUpMenu.AddMenuItem(index, _T("�յ�"), _T("_SUNACAIRCONDITIONER "));

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;
			IPopUpMenu.AddMenuItem(index, _T("��׼����"), _T("_SUNACFACADE "));

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;
			IPopUpMenu.AddMenuItem(index, _T("��ˮ����"), _T("_SUNACWATERPROOF "));

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = MenuIndex++;
			IPopUpMenu.AddMenuItem(index, _T("ͳ������"), _T("_SUNACSTATISTICS "));

			pDisp = IPopUpMenu.m_lpDispatch;
			pDisp->AddRef();
		}

		CAcadPopupMenu IPopUpMenu(pDisp);
		if (!IPopUpMenu.get_OnMenuBar())
		{
			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = numberOfMenus ;
			IPopUpMenu.InsertInMenuBar(index);
		}
		else
		{
			VariantInit(&index);
			V_VT(&index) = VT_BSTR;
			V_BSTR(&index) = cstrMenuName.AllocSysString();
			IPopUpMenus.RemoveMenuFromMenuBar(index);
			VariantClear(&index);
		}
		pDisp->Release();
	}
	CATCH(COleDispatchException,e)
	{
		e->ReportError();
		e->Delete();
	}
	END_CATCH;
}

static void initApp()
{
	CAcModuleResourceOverride resOverride;
	////----------------��ȡ����-------------------//

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SUNACWINDOW"),
		_T("SUNACWINDOW"),
		ACRX_CMD_MODAL,
		CMD_SUNACWINDOW,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SUNACKITCHEN"),
		_T("SUNACKITCHEN"),
		ACRX_CMD_MODAL,
		CMD_SUNACKITCHEN,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SUNACBATHROOM"),
		_T("SUNACBATHROOM"),
		ACRX_CMD_MODAL,
		CMD_SUNACBATHROOM,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SUNACDOOR"),
		_T("SUNACDOOR"),
		ACRX_CMD_MODAL,
		CMD_SUNACDOOR,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SUNACRAILING"),
		_T("SUNACRAILING"),
		ACRX_CMD_MODAL,
		CMD_SUNACRAILING,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SUNACMOLDINGS"),
		_T("SUNACMOLDINGS"),
		ACRX_CMD_MODAL,
		CMD_SUNACMOLDINGS,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SUNACFILLING"),
		_T("SUNACFILLING"),
		ACRX_CMD_MODAL,
		CMD_SUNACFILLING,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SUNACAIRCONDITIONER"),
		_T("SUNACAIRCONDITIONER"),
		ACRX_CMD_MODAL,
		CMD_SUNACAIRCONDITIONER,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SUNACFACADE"),
		_T("SUNACFACADE"),
		ACRX_CMD_MODAL,
		CMD_SUNACFACADE,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SUNACWATERPROOF"),
		_T("SUNACWATERPROOF"),
		ACRX_CMD_MODAL,
		CMD_SUNACWATERPROOF,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SUNACSTATISTICS"),
		_T("SUNACSTATISTICS"),
		ACRX_CMD_MODAL,
		CMD_SUNACSTATISTICS,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());
}


static void unloadApp()
{
	// Do other cleanup tasks here  
	acedRegCmds->removeGroup(_T("SUNAC"));  
	InitMenu();
}



//////////////////////////////////////////////////////////////
//
// Entry points
//
//////////////////////////////////////////////////////////////

extern "C" int APIENTRY
	DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		theArxDLL.AttachInstance(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		theArxDLL.DetachInstance();  
	}
	return 1;   // ok
}

extern "C" AcRx::AppRetCode acrxEntryPoint( AcRx::AppMsgCode msg, void* appId)
{
	switch( msg ) 
	{
	case AcRx::kInitAppMsg: 
		acrxDynamicLinker->unlockApplication(appId);
		acrxDynamicLinker->registerAppMDIAware(appId);
		initApp(); 
		InitMenu();
		break;
	case AcRx::kUnloadAppMsg: 
		unloadApp();
		break;
	case  AcRx::kLoadDwgMsg:
		break;
	case  AcRx::kUnloadDwgMsg:
		break;
	case  AcRx::kSaveMsg:
		break;
	case AcRx::kInitDialogMsg:
		break;
	default:
		break;
	}
	return AcRx::kRetOK;
}

