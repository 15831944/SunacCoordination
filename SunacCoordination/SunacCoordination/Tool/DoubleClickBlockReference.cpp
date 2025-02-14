// DoubleClickCircle.cpp: implementation of the CDoubleClickCircle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DoubleClickBlockReference.h"
#include <dbents.h>
#include <actrans.h>
#include "../Common/ComFun_Sunac.h"
#include "../UI/WindowDlg.h"
#include "../UI/KitchenDlg.h"
#include "../UI/BathroomDlg.h"
#include "../UI/RailingDlg.h"
#include "../UI/AirconditionerDlg.h"
#include "../UI/FacadeDlg.h"
#include "../UI/FillingDlg.h"
#include "../UI/MoldingsDlg.h"
#include "../UI/WaterproofDlg.h"
#include "../ui/MyPalette.h"
#include "../ui/MyPaletteSet.h"
#include "../ui/DlgLogin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoubleClickBlockReference::CDoubleClickBlockReference()
{

}

CDoubleClickBlockReference::~CDoubleClickBlockReference()
{

}

void CDoubleClickBlockReference::startEdit( AcDbEntity *pEnt, AcGePoint3d pt )
{
	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference *pBlockReference = AcDbBlockReference::cast(pEnt);

		// 将实体的打开状态升级为可写状态
		//acDocManager->lockDocument(acDocManager->curDocument());
		//pBlockReference->upgradeOpen();
		//acDocManager->unlockDocument(acDocManager->curDocument());

		eRCType rcType = TY_GetType(pBlockReference);
		pBlockReference->close();

		CAcModuleResourceOverride resOverride;
		switch (rcType)
		{
		case S_WINDOW:
			{
				OpenWindowDlg(pBlockReference->objectId());
				break;
			}
			
		case S_KITCHEN:
			{
				OpenKitchenDlg(pBlockReference);
				break;
			}
			
		case S_BATHROOM:
			{
				OpenBathroomDlg(pBlockReference);
				break;
			}
		case S_RAILING:
			{
				OpenRailingDlg(pBlockReference);
				break;
			}
		case S_AIRCON:
			{
				OpenAirconditionerDlg(pBlockReference);
				break;
			}
		}
	}
	else
	{
		acutPrintf(L"\n无效的AcDbBlockReference对象...");
		pEnt->close();
		return;
	}
}

void CDoubleClickBlockReference::finishEdit( void )
{
	// 此处不使用
}