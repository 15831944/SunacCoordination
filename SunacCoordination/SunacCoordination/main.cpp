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
#include <dbidmap.h>
#include <string>
#include "main.h"
#include "AcExtensionModule.h"
#include "accmd.h"
#include "Common/ComFun_Sunac.h"
#include "Common\ComFun_Str.h"
#include "command\CommandWindowDetail.h"
#include "Command\Command.h"
#include "UI\menu\Menu_Def.h"
#include "object\WindowDoor\AttrWindow.h"
#include "object\WindowDoor\WindowGen.h"
#include "object\WindowDoor\WindowSelect.h"
#include "object\AirCondition\AttrAirCon.h"
#include "object\Kitchen\AttrKitchen.h"
#include "object\Bathroom\AttrBathroom.h"
#include "object\Railing\AttrRailing.h"
#include "Object/WindowStatistic/WindowMaterialUsage.h"
#include "Object/WindowStatistic/WindowFormula.h"
#include "Object/WindowStatistic/AluminumSeries.h"
#include "Object/WindowStatistic/DeductedSize.h"
#include "Object/WindowStatistic/WindowStatictic.h"
#include "GlobalSetting.h"
#include "Tool\DoubleClickBlockReference.h"
#include "Tool\MarkupXml\Markup.h"
#include "WebIO\WebIO.h"
#include "webIO\WindowLocalData.h"
#include "WebIO\ConfigDictionary.h"
#include "WebIO\KitchenBathroomWebData.h"
#include "WebIO\RailingWebData.h"
#include "WebIO\AirconditionWebData.h"
#include "WebIO\AirconditionLocalDataFromDB.h"
#include "WebIO\WindowWebData.h"
#include "webIO\WindowLocalDataFromDB.h"
#include "webIO\WebProjectFile.h"
#include "WebIO\SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "ProjectorFileMrg/ProjectFileMrg.h"
#include "ProjectorFileMrg/FileUploadDownload.h"
#include "Src/DocumentData.h"
#include "Src/DocumentDataSerialize.h"
#include "Common/ComFun_RectArray.h"
#include "Object/WindowDoor/WindowAutoName.h"
#include "Src/Reactor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//extern "C" HWND adsw_acadMainWnd(); AutoCAD主窗口
static HANDLE mThreadHandle = 0;

ACRX_DEFINE_MEMBERS(AcDbDoubleClickEdit);
enum Acad::ErrorStatus __stdcall LoadManagedDll(ACHAR const *path);
/////////////////////////////////////////////////////////////////////////////

AC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);


void CMD_YTest()
{
	int n = 0;
	vector<CSunacObjInCad> winsSelected = CSunacSelect::SelectSunacObjs(S_WINDOW, E_VIEW_ALL, false);
	for (UINT i = 0; i < winsSelected.size(); i++)
	{
		if (winsSelected[i].m_bMxMirror)
		{
			n++;
		}		
	}

	return;


	/*ads_name ename;
	ads_point pt;
	if (acedEntSel(L"\nSelect a dynamic block reference: ", ename, pt) != RTNORM)
	{
		acutPrintf(L"\nError selecting entity.");
		return;
	}
	AcDbObjectId eId;
	acdbGetObjectId(eId, ename);

	vAcDbObjectId ids1,ids2,ids3,ids4;
	AcDbObjectId actionID = TYCOM_GetActionId(eId);
	TYCOM_GetArrayObjects(actionID,ids1);//得到4个对象 每个对象都是一个块 不是arrayid

	TYCOM_CycleBlockReferenceEntites(ids1[0],ids2);//通过这个方法 得到这一个块内部的 arrayid

	TYCOM_IsArray(ids2[0]);
	actionID = TYCOM_GetActionId(ids2[0]);
	TYCOM_GetArrayObjects(actionID,ids3);

	TYCOM_CycleBlockReferenceEntites(ids3[0],ids4);
	TY_IsWindow(ids4[0],E_VIEW_ALL);

	vAcDbObjectId allWindowIds;
	TYCOM_DeepCycleBlockReferences(eId, E_VIEW_FRONT, TY_IsWindow, allWindowIds);

	vector<AttrWindow> winPrototypes = WebIO::GetInstance()->GetWindows(1500, 1700, _T("不限"), 0, _T("不限"));
	if (winPrototypes.size() == 0)
	{
		return;
	}

	AttrWindow winAtt = winPrototypes[0];
	winAtt.SetW(1500);
	winAtt.SetH(1700);

	AcGePoint3d pos;
	bool bSuc = TY_GetPoint(pos);
	if (bSuc == false)
		return;

	AcDbObjectId idOut = CWindowGen::GenerateWindow(winAtt, pos, E_DIR_BOTTOM, false, AcDbObjectId::kNull, GlobalSetting::GetWindowBlockLayer());
	
	//AcGeMatrix3d xform;
	//xform.setTranslation(AcGeVector3d(2500, 0, 0));

	//////////////////////////////////////////////////////////////////////////
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	AcDbObjectId  modelSpaceId = AcDbObjectId::kNull;
	pBlockTable->getAt(ACDB_MODEL_SPACE, modelSpaceId);
	pBlockTable->close();

	AcDbObjectIdArray  objList;
	objList.append(idOut);

	AcDbIdMapping idMap;
	// Step 5: Call deepCloneObj

	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, eId, AcDb::kForRead);

	//这个函数是判断一个实体是不是关联对象的一个好方法  
	bool isarray = AcDbAssocArrayActionBody::isAssociativeArray(pEnt);

	AcDbObjectId actionid = AcDbAssocArrayActionBody::getControllingActionBody(pEnt);
	pEnt->close();ects().
	acdbHostApplicationServices()->workingDatabase()->deepCloneObjects(objList, modelSpaceId, idMap);
	// Now we can go through the ID map and do whatever we'd like to the original and/or clone objects.

	AcDbObjectIdArray  objListCloned;
	AcDbIdMappingIter iter(idMap);
	for (iter.start(); !iter.done(); iter.next()) 
	{
		AcDbIdPair idPair;
		iter.getMap(idPair);
		if (!idPair.isCloned())
			continue;

		objListCloned.append(idPair.value());

		TYCOM_Move(idPair.value(), AcGeVector3d(2500, 0, 0));
		//acutPrintf("\nObjectId is: %Ld",idPair.value().asOldId());
	}
	*/
	return;
}

void CMD_YTest2()
{
	AttrWindow attrwindow;

	attrwindow.m_prototypeCode = _T("Door_TLM3");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("推拉");
	attrwindow.m_openQty = 1;

	CWindowsDimData dimdata1;
	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);
	CWindowsDimData dimdata2;
	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);
	CWindowsDimData dimdata3;

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("(W-2a)/4");
	attrwindow.SetDimData(dimdata1);

	//CWindowsDimData dimdata2;
	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-2a");
	attrwindow.SetDimData(dimdata2);
	
	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(2400);
	attrwindow.SetW(3600);
	attrwindow.SetH1(500);
	attrwindow.SetW1(700);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("ST105AM系列");
	attrwindow.SetInstanceCode(_T("TLM3"));

	vector<AttrWindow> winAtts;
	winAtts.push_back(attrwindow);
	winAtts.push_back(attrwindow);

	attrwindow.m_prototypeCode = _T("Window_NC3");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("外开");
	attrwindow.m_openQty = 1;

	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("(W-2a)/2");
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-2a");
	attrwindow.SetDimData(dimdata2);
	
	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(1400);
	attrwindow.SetW(1300);
	attrwindow.SetH1(500);
	attrwindow.SetW1(700);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("SW55M系列");
	attrwindow.SetInstanceCode(_T("NC3"));

	winAtts.push_back(attrwindow);

	CWindowStatictic winStatictic;

	CString filter = L"参数文件(*.xlsx)|*.xlsx|All Files(*.*)|*.*||";
	CFileDialog dlg(FALSE, L"xlsx", L"*.xlsx", NULL, filter);
	if (dlg.DoModal() == IDOK)
	{
		CString pathName = dlg.GetPathName();
		winStatictic.Statictic(winAtts, pathName);
	}
}

void CMD_YTest3()
{

}

void AddSubMenu(CAcadPopupMenu&IPopUpMenu, UINT MenuStartIndex)
{
	VARIANT index;

	VariantInit(&index);
	V_VT(&index) = VT_I4;
	V_I4(&index) = MenuStartIndex++;
	IPopUpMenu.AddMenuItem(index, _T("登录"), _T("_SLOGIN ")); //命令的前面的下划线_ 和最后的空格都不能少
	
	VariantInit(&index);
	V_VT(&index) = VT_I4;
	V_I4(&index) = MenuStartIndex++;
	IPopUpMenu.AddMenuItem(index, _T("功能导航栏"), _T("_SPALETTE "));

	//VariantInit(&index);
	//V_VT(&index) = VT_I4;
	//V_I4(&index) = MenuStartIndex++;
	//IPopUpMenu.AddMenuItem(index, _T("设置"), _T("_SSETUP "));
}

void InitMenu()
{
	TRY
	{
		CAcadApplication IAcad(acedGetAcadWinApp()->GetIDispatch(TRUE));
		CAcadMenuBar IMenuBar(IAcad.get_MenuBar());

		long numberOfMenus = IMenuBar.get_Count();

		CAcadMenuGroups IMenuGroups(IAcad.get_MenuGroups());

		VARIANT index;
		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = 0;

		CAcadMenuGroup IMenuGroup(IMenuGroups.Item(index));
		CAcadPopupMenus IPopUpMenus(IMenuGroup.get_Menus());

		CString cstrMenuName = _T("融创产品标准");//自定义主菜单

		VariantInit(&index);
		V_VT(&index) = VT_BSTR;
		V_BSTR(&index) = cstrMenuName.AllocSysString();

		IDispatch* pDisp = NULL;
		TRY{ pDisp = IPopUpMenus.Item(index); pDisp->AddRef(); } CATCH(COleDispatchException, e){}END_CATCH;

		if (pDisp == NULL)
		{
			CAcadPopupMenu IPopUpMenu(IPopUpMenus.Add(cstrMenuName));
			UINT MenuIndex = 0;

			AddSubMenu(IPopUpMenu, MenuIndex);

			pDisp = IPopUpMenu.m_lpDispatch;
			pDisp->AddRef();
		}

		CAcadPopupMenu IPopUpMenu(pDisp);
		if (!IPopUpMenu.get_OnMenuBar())
		{
			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = numberOfMenus;
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
	CATCH(COleDispatchException, e)
	{
		e->ReportError();
		e->Delete();
	}
	END_CATCH;
}

void ZJYTest()
{
	CString localWindowPath = TY_GetPrototypeFilePath();

	vector<AttrWindow> m_winAtts;
	AttrWindow attrwindow;
	//////////////////////////////////////////////////////////////////////////
	attrwindow.m_prototypeCode = _T("Door_WM1");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("外开");
	attrwindow.m_openQty = 1;

	CWindowsDimData dimdata1;
	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);
	CWindowsDimData dimdata2;
	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);
	CWindowsDimData dimdata3;

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("W-2a");
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-2a");
	attrwindow.SetDimData(dimdata2);

	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(1400);
	attrwindow.SetW(1700);
	attrwindow.SetH1(500);
	attrwindow.SetW1(750);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("SW55M系列");
	attrwindow.SetInstanceCode(_T("111"));

	m_winAtts.push_back(attrwindow);
	//////////////////////////////////////////////////////////////////////////
	attrwindow.m_prototypeCode = _T("Door_WM2");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("外开");
	attrwindow.m_openQty = 1;

	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("W-2a-W2");
	attrwindow.SetDimData(dimdata1);

	
	dimdata2.sCodeName = _T("W2");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("450-a");
	attrwindow.SetDimData(dimdata1);

	dimdata1.sCodeName = _T("H1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("H-2a");
	attrwindow.SetDimData(dimdata2);

	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(1400);
	attrwindow.SetW(1700);
	attrwindow.SetH1(500);
	attrwindow.SetW1(750);
	attrwindow.SetW2(600);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("SW55M系列");
	attrwindow.SetInstanceCode(_T("222"));

	m_winAtts.push_back(attrwindow);
	//////////////////////////////////////////////////////////////////////////
	attrwindow.m_prototypeCode = _T("Door_WM3");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("外开");
	attrwindow.m_openQty = 1;

	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("(W-2a)/2");
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-2a");
	attrwindow.SetDimData(dimdata2);

	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(1400);
	attrwindow.SetW(1700);
	attrwindow.SetH1(500);
	attrwindow.SetW1(750);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("SW55M系列");
	attrwindow.SetInstanceCode(_T("333"));

	m_winAtts.push_back(attrwindow);
	//////////////////////////////////////////////////////////////////////////
	attrwindow.m_prototypeCode = _T("Door_WM4");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("外开");
	attrwindow.m_openQty = 1;

	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("(W-2a)/2");
	attrwindow.SetDimData(dimdata1);

	dimdata1.sCodeName = _T("W2");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("W-2a-W1");
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-2a");
	attrwindow.SetDimData(dimdata2);

	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(1400);
	attrwindow.SetW(1700);
	attrwindow.SetH1(500);
	attrwindow.SetW1(600);
	attrwindow.SetH2(400);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("SW55M系列");
	attrwindow.SetInstanceCode(_T("444"));

	m_winAtts.push_back(attrwindow);
	//////////////////////////////////////////////////////////////////////////
	attrwindow.m_prototypeCode = _T("Door_WLC1");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("外开");
	attrwindow.m_openQty = 1;

	//CWindowsDimData dimdata1;
	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);
	//CWindowsDimData dimdata2;
	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);
	//CWindowsDimData dimdata3;

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = UNLIMIT;
	//dimdata1.sFomula = _T("W-2a");
	attrwindow.SetDimData(dimdata1);

	dimdata1.sCodeName = _T("W3");
	dimdata1.type = UNLIMIT;
	//dimdata1.sFomula = _T("W-2a");
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-2a-H2");
	attrwindow.SetDimData(dimdata2);

	dimdata2.sCodeName = _T("H2");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("1000-a");
	attrwindow.SetDimData(dimdata2);

	dimdata2.sCodeName = _T("W2");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("W-2a-W1");
	attrwindow.SetDimData(dimdata2);

	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(2400);
	attrwindow.SetW(2700);
	attrwindow.SetH1(500);
	attrwindow.SetW1(1000);
	attrwindow.SetW2(400);
	attrwindow.SetW3(700);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("SW55M系列");
	attrwindow.SetInstanceCode(_T("555"));

	m_winAtts.push_back(attrwindow);
	//////////////////////////////////////////////////////////////////////////
	attrwindow.m_prototypeCode = _T("Door_WLC2");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("外开");
	attrwindow.m_openQty = 1;

	//CWindowsDimData dimdata1;
	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);
	//CWindowsDimData dimdata2;
	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);
	//CWindowsDimData dimdata3;

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("W-2a-W2");
	attrwindow.SetDimData(dimdata1);

	dimdata1.sCodeName = _T("W2");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-900");
	attrwindow.SetDimData(dimdata2);

	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(2400);
	attrwindow.SetW(1600);
	attrwindow.SetH1(500);
	attrwindow.SetW1(750);
	attrwindow.SetW2(900);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("SW55M系列");
	attrwindow.SetInstanceCode(_T("666"));

	m_winAtts.push_back(attrwindow);
	//////////////////////////////////////////////////////////////////////////
	attrwindow.m_prototypeCode = _T("Door_WLC3");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("外开");
	attrwindow.m_openQty = 1;

	//CWindowsDimData dimdata1;
	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);
	//CWindowsDimData dimdata2;
	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);
	//CWindowsDimData dimdata3;

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("W-2a-W2");
	attrwindow.SetDimData(dimdata1);

	dimdata1.sCodeName = _T("W2");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-2a-H2-H3");
	attrwindow.SetDimData(dimdata2);

	dimdata2.sCodeName = _T("H2");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);

	dimdata2.sCodeName = _T("H3");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);

	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(2400);
	attrwindow.SetW(1600);
	attrwindow.SetH1(500);
	attrwindow.SetH2(400);
	attrwindow.SetH3(600);
	attrwindow.SetW2(900);
	attrwindow.SetW1(700);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("SW55M系列");
	attrwindow.SetInstanceCode(_T("777"));

	m_winAtts.push_back(attrwindow);
	//////////////////////////////////////////////////////////////////////////
	CWindowStatictic winStatictic;

	CString filter=L"参数文件(*.xlsx)|*.xlsx|All Files(*.*)|*.*||";  
	CFileDialog dlg(FALSE, L"xlsx", L"*.xlsx", NULL, filter); 
	if(dlg.DoModal()==IDOK)
	{
		CString pathName = dlg.GetPathName();
		winStatictic.Statictic(m_winAtts, pathName);
	}
}

void CMD_test()
{
/*
	ads_name ename;
	ads_point pt;
	if (acedEntSel(L"\nSelect a dynamic block reference: ", ename, pt) != RTNORM)
	{
		acutPrintf(L"\nError selecting entity.");
		return;
	}
	AcDbObjectId eId;
	acdbGetObjectId(eId, ename);*/

	////SetDoorPos(eId, 3600);
	//SelectZaoTai(eId, L"900");
	//SelectShuiPen(eId, L"双盆900");
	//SetShuiPenPos(eId, 3000);
	////SelectShuiPen(eId, L"双盆900");
	////SelectZaoTai(eId, L"800");
	////acedRedraw(eId,);

	double width = 1800;
	double height = 1800;
	CString doorType = "";

	WEBINST->DownloadJPGFile(1423);
	
	std::vector<AttrWindow> WindowFromWebData = CWindowWebData::Instance()->GetAllWindows();
	//std::vector<AttrWindow> WindowFromWebData2 = CWindowWebData::Instance()->GetWindows(1800, 1800, L"外开", 1, L"");
	//std::vector<AttrWindow> WindowFromLocalData = CWindowLocalData::Instance()->GetAllWindows();
	//std::vector<AttrWindow> WindowFromLocalDataFromDB = CWindowLocalDataFromDB::Instance()->GetAllWindows();
	//std::vector<AttrWindow> WindowFromLocalDataFromDB2 = CWindowLocalDataFromDB::Instance()->GetWindows(1500, 1800, L"外开", 1, L"全部");

	//test比较从excel读取的数据和数据库中读取的数据



	//比较windows1, windows2
/*
	for(int i = 0; i < WindowFromWebData.size(); i++ )
	{
		AttrWindow &curWebWin = WindowFromWebData[i];

		//从window1找到相同编号的
		bool bFind = false;
		for (UINT j = 0; j < WindowFromLocalData.size(); j++)
		{
			if (curWebWin.m_prototypeCode == WindowFromLocalData[j].m_prototypeCode)
			{
				bFind = true;
				if(WindowFromLocalData[j].IsWindowPrototypeEqual(WindowFromWebData[i]))
				{
					AfxMessageBox(L"确实相等！");
				}
				else
				{
					AfxMessageBox(L"不完全相等！");
				}
				break;
			}
		}

		assert(bFind==true);
	}*/

	std::vector<AttrWindow> DoorFromWebData = CWindowWebData::Instance()->GetAllDoors();
	//std::vector<AttrWindow> DoorFromLocalData = CWindowLocalData::Instance()->GetAllDoors();
/*
	for(int i = 0; i < DoorFromWebData.size(); i++ )
	{
		AttrWindow &curWebWin = DoorFromWebData[i];

		//从window1找到相同编号的
		bool bFind = false;
		for (UINT j = 0; j < DoorFromLocalData.size(); j++)
		{
			if (curWebWin.m_prototypeCode == DoorFromLocalData[j].m_prototypeCode)
			{
				bFind = true;
				if(DoorFromLocalData[j].IsDoorPrototypeEqual(DoorFromWebData[i]))
				{
					AfxMessageBox(L"确实相等！");
				}
				else
				{
					AfxMessageBox(L"不完全相等！");
				}
				break;
			}
		}

		assert(bFind==true);
	}*/

	CKitchenBathroomWebData KitchenFromWeb;
	CKitchenBathroomWebData BathroomFromWeb;
	//std::vector<AttrKitchen> KitchenFromLocalData = WebIO::GetInstance()->GetAllKitchens();
	std::vector<AttrKitchen> KitchenFromWebData = KitchenFromWeb.GetAllKitchens();
	std::vector<AttrBathroom> BathroomFromWebData = BathroomFromWeb.GetAllBathrooms();
	//std::vector<AttrKitchen> KitchenFromWebData2 = KitchenFromWeb.GetKitchens(1800, 2600, L"门窗对开", L"L型", true);
	//std::vector<AttrBathroom> BathroomFromWebData = BathroomFromWeb.GetBathrooms(1800, 2000, L"门窗垂直开", L"L型");
	/*
	for(int i = 0; i < attrs2.size(); i++ )
	{
		AttrKitchen &curWebAttr = attrs2[i];

		//从attr1找到相同编号的
		bool bFind = false;
		for (UINT j = 0; j < attrs1.size(); j++)
		{
			if (curWebAttr.m_prototypeCode == attrs1[j].m_prototypeCode)
			{
				bFind = true;
				if(attrs1[i].IsKitchenPrototypeEqual(attrs2[j]))
				{
					AfxMessageBox(L"确实相等！");
				}
				else
				{
					AfxMessageBox(L"不完全相等！");
				}
				break;
			}
		}

	}*/



	CRailingWebData RailingFromWeb;
	//std::vector<AttrRailing> RailingFromWebData = RailingFromWeb.GetAllRailings();
	std::vector<AttrRailing> RailingFromWebData = RailingFromWeb.GetRailings(E_RAILING_BOLI);

	//std::vector<AttrAirCon> AirConLocalDataFromDB = CAirConditionLocalDataFromDB::Instance()->GetAllAirCons();
	std::vector<AttrAirCon> AirConWebData = CAirConditionWebData::Instance()->GetAllAirCons();
	//std::vector<AttrAirCon> AirConFromWebData = CAirConditionWebData::Instance()->GetAirCons(1.5, "后面", 1, "侧面");
	return;
}

void CMD_TEST2()
{
	/*vector<CAluminumFormula> vAlFormula;
	vector<CGlassFormula> vGlassFormula;
	vector<CHardwareData> vHardwareFormula;
	vector<CString> vAlSeries;
	CAluminumData AlData;
	CString AlSeries;
	double DeductedSizeData;
	vAlFormula = CWindowFormula::Instance()->GetAluminumFormulas(L"Window_NC1");
	vGlassFormula = CWindowFormula::Instance()->GetGlassFormulas(L"Window_NC1");
	vHardwareFormula = CWindowFormula::Instance()->GetHardwareData(L"Window_NC1");
	CDeductedSize::Instance()->GetDeductedSizeBySeriesAndName(E_WindowAluminum_NC, L"SW55M系列", L"M1", DeductedSizeData);
	CDeductedSize::Instance()->GetDeductedSizeBySeriesAndName(E_WindowAluminum_WC, L"SW55M系列", L"M1", DeductedSizeData);
	CAluminumSeries::Instance()->GetAluminumDataBySeriesAndName(E_WindowAluminum_NC, L"SW55M系列", L"假中梃", AlData);
	CAluminumSeries::Instance()->GetAluminumSerialByCode(L"SN60T002", AlSeries);
	vAlSeries = CAluminumSeries::Instance()->GetAluminumSerialsByWindowType(E_WindowAluminum_NC);*/
	//CFileUpDownLoad::UploadFile(L"D:\\Drawing1.dwg", L"1234567778.dwg", _T("20191030"));
	//CFileUpDownLoad::DownloadFile(L"http://fastsoft.onlinedown.net/down/idm_ald.exe", L"F:\\FTPServer\\Test.exe");
	/*if(CWebProjectFile::Instance()->GetAllProjectInfo())
	{
		vector<CProjectData *> vProjects;
		vProjects.swap(CProjectFileMrg::Instance()->m_projects);
	}*/

	//CInstanceCodeTextMrg Ref;
	//Ref.GetAllInstanceCodeIds();
	CInstanceCodeTextMrg::GetAllInstanceCodeIds();


	TYRect rect;
	bool bSuc = TY_GetOneRect(rect);

	CInstanceCodeTextMrg::GetInstanceCodeIdsInRect(rect);
	//Ref.GetInstanceCodeIdsInRect(rect);
}

static void initApp()
{
	CAcModuleResourceOverride resOverride;

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("ytest"),
		_T("ytest"),
		ACRX_CMD_MODAL,
		CMD_YTest,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());
	acedRegCmds->addCommand(_T("SUNAC"),
		_T("tes"),
		_T("tes"),
		ACRX_CMD_MODAL,
		CMD_test,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("test2"),
		_T("test2"),
		ACRX_CMD_SESSION,
		CMD_TEST2,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("zjy"),
		_T("zjy"),
		ACRX_CMD_MODAL,
		ZJYTest,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	//////////////////////////////////////////////////////////////////////////

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SLOGIN"),
		_T("SLOGIN"),
		ACRX_CMD_MODAL,
		CMD_Login,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SPALETTE"),
		_T("SPALETTE"),
		ACRX_CMD_MODAL,
		CMD_ShowCADPalette,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SSETUP"),
		_T("SSETUP"),
		ACRX_CMD_MODAL,
		CMD_SetUp,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINDESIGN"),
		_T("SWINDESIGN"),
		ACRX_CMD_MODAL,
		CMD_SunacWindow,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINADVANCE"),
		_T("SWINADVANCE"),
		ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET,
		CMD_SunacWindowAdvanceDesign,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINSTATISTICS"),
		_T("SWINSTATISTICS"),
		ACRX_CMD_MODAL,
		CMD_SunacWindowsStatistics,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINFLOOR"),
		_T("SWINFLOOR"),
		ACRX_CMD_MODAL,
		CMD_SunacFloorSetting,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINFTOP2FRONT"),
		_T("SWINFTOP2FRONT"),
		ACRX_CMD_MODAL,
		CMD_SunacWindowTop2Front,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINFFRONT2TOP"),
		_T("SWINFFRONT2TOP"),
		ACRX_CMD_MODAL,
		CMD_SunacWindowFront2Top,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINTABLE"),
		_T("SWINTABLE"),
		ACRX_CMD_MODAL,
		CMD_SunacWindowsTable,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINDETAIL"),
		_T("SWINDETAIL"),
		ACRX_CMD_MODAL,
		CMD_SunacWindowDetail,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINAUTOID"),
		_T("SWINAUTOID"),
		ACRX_CMD_MODAL,
		CMD_SunacWinAutoId,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SNOHIGHLIGHT"),
		_T("SNOHIGHLIGHT"),
		ACRX_CMD_MODAL,
		CMD_SunacNoHighlight,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINTABLECHECK"),
		_T("SWINTABLECHECK"),
		ACRX_CMD_MODAL,
		CMD_SunacWinTableCheck,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SKITCHEN"),
		_T("SKITCHEN"),
		ACRX_CMD_MODAL,
		CMD_SunacKitchen,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SBATHROOM"),
		_T("SBATHROOM"),
		ACRX_CMD_MODAL,
		CMD_SunacBathroom,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SKBSTATISTIC"),
		_T("SKBSTATISTIC"),
		ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET,
		CMD_SunacKitchenBathroomStatistic,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SRAILING"),
		_T("SRAILING"),
		ACRX_CMD_MODAL,
		CMD_SunacRailing,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());
	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SRAILINGDETAIL"),
		_T("SRAILINGDETAIL"),
		ACRX_CMD_MODAL,
		CMD_SunacRailingDetail,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SRAILINGSTATISTICS"),
		_T("SRAILINGSTATISTICS"),
		ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET,
		CMD_SunacRailingStatistic,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SRAILINGFLOORSETTING"),
		_T("SRAILINGFLOORSETTING"),
		ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET,
		CMD_SunacFloorSetting,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SRAILINGTOP2FRONT"),
		_T("SRAILINGTOP2FRONT"),
		ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET,
		CMD_SunacRailingTop2Front,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SAIRCONDITIONER"),
		_T("SAIRCONDITIONER"),
		ACRX_CMD_MODAL,
		CMD_SunacAirconditioner,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SAIRCONSTATISTIC"),
		_T("SAIRCONSTATISTIC"),
		ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET,
		CMD_SunacAirconditionerStatistic,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SMOLDINGS"),
		_T("SMOLDINGS"),
		ACRX_CMD_MODAL,
		CMD_SunacMoldings,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SFILLING"),
		_T("SFILLING"),
		ACRX_CMD_MODAL,
		CMD_SunacFilling,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());


	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SFACADE"),
		_T("SFACADE"),
		ACRX_CMD_MODAL,
		CMD_SunacFacade,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWATERPROOF"),
		_T("SWATERPROOF"),
		ACRX_CMD_MODAL,
		CMD_SunacWaterproof,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	//添加命令取消高亮
	acedRegCmds->addCommand(_T("SUNAC"),
		_T("NOHIGHLIGHT"),
		_T("NOHIGHLIGHT"),
		ACRX_CMD_MODAL,
		CMD_SunacNoHighlight,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());



	AttrObject::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT));

	AttrWindow::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW));

	AttrAirCon::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_AIRCON));

	AttrKitchen::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN));

	AttrBathroom::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom));

	AttrRailing::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_RAILING));

	CDocumentDataSerialize::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(cDocmentDataSerialize_DBXSERVICE);

	// 为AcDbCircle类添加协议扩展
	CDoubleClickBlockReference *pCircleEdit = new CDoubleClickBlockReference;
	AcDbBlockReference::desc()->addX(AcDbDoubleClickEdit::desc(), pCircleEdit);	

#if (defined ARX_2010) || (defined ARX_2011) || (defined ARX_2012)
	LoadManagedDll(MD2010_GetAppPath() + L"\\Sunac2019\\External_DLL\\RemoveCuiDoubleClick-vs2008.dll");
#elif (defined ARX_2013) || (defined ARX_2014)
	LoadManagedDll(MD2010_GetAppPath() + L"\\Sunac2019\\External_DLL\\RemoveCuiDoubleClick-vs2010.dll");
#elif (defined ARX_2015) || (defined ARX_2016)
	LoadManagedDll(MD2010_GetAppPath() + L"\\Sunac2019\\External_DLL\\RemoveCuiDoubleClick-vs2012.dll");
#elif (defined ARX_2017) || (defined ARX_2018) || (defined ARX_2019)
	LoadManagedDll(MD2010_GetAppPath() + L"\\Sunac2019\\External_DLL\\RemoveCuiDoubleClick-vs2015.dll");
#endif
	

	//添加反应器
	g_editorReactor = new CMyEditReactor;
	acedEditor->addReactor(g_editorReactor);
	g_docReactor = new CMyDocReactor;
	acDocManager->addReactor(g_docReactor);
	g_dbReactor = new CMyDbReactor;
	AcApDocumentIterator* it = acDocManager->newAcApDocumentIterator();
	for (; !it->done(); it->step())
	{
		it->document()->database()->addReactor(g_dbReactor);
	}
	delete it;

	//WEBINST;
}


static void unloadApp()
{
	// Do other cleanup tasks here  
	acedRegCmds->removeGroup(_T("SUNAC"));  

	deleteAcRxClass(CDocumentDataSerialize::desc());
	delete acrxServiceDictionary->remove(cDocmentDataSerialize_DBXSERVICE);

	deleteAcRxClass(AttrWindow::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW));

	deleteAcRxClass(AttrAirCon::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_AIRCON));

	deleteAcRxClass(AttrRailing::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_RAILING));
	
	deleteAcRxClass(AttrKitchen::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN));

	deleteAcRxClass(AttrBathroom::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom));

	deleteAcRxClass(AttrObject::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT));

	AcDbBlockReference::desc()->delX(AcDbDoubleClickEdit::desc());

	//删除反应器
	AcApDocumentIterator* it = acDocManager->newAcApDocumentIterator();
	for (; !it->done(); it->step())
	{
		it->document()->database()->removeReactor(g_dbReactor);
	}
	delete it;
	delete g_dbReactor;
	acDocManager->removeReactor(g_docReactor);
	delete g_docReactor;
	acedEditor->removeReactor(g_editorReactor);
	delete g_editorReactor;

	//WaitForSingleObject(mThreadHandle, 1000);
	//TerminateThread(mThreadHandle,0);
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
#ifndef _WIN64
		SetDllDirectory(MD2010_GetAppPath() + L"\\Sunac2019\\External_DLL\\x86");
#else
		SetDllDirectory(MD2010_GetAppPath() + L"\\Sunac2019\\External_DLL\\x64");
#endif
		acrxDynamicLinker->unlockApplication(appId);
		acrxDynamicLinker->registerAppMDIAware(appId);
		initApp(); 
		InitMenu();
		break;
	case AcRx::kUnloadAppMsg: 
		CloseModelessDialogs();
		CADPalette_RemoveP();
		unloadApp();
		if (acDocManager->documentCount() > 0)
			InitMenu();
		break;
	case AcRx::kQuitMsg:
		CFileUpDownLoadWeb::Instance()->SetAppQuit();
		CFileUpDownLoadFtp::Instance()->SetAppQuit();
		break;
	case  AcRx::kLoadDwgMsg: //加载完成后才进入此消息
		CDocumentFactory::Instance().AddDocument(::acDocManager->curDocument());
		CDocumentDataSerialize::InitSerialize();  //YUAN DocSerialize
		break;
	case  AcRx::kUnloadDwgMsg:
		CloseModelessDialogs(); //预览控件依赖于当前文档，关闭文档时退出对话框以防止崩溃
		CDocumentFactory::Instance().RemoveCurDoc();
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



