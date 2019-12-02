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
	// ��õ�ǰ�ĵ�
	AcApDocument *pDoc = acDocManager->curDocument();
	
	// ��AcDbEntityָ��ת��ΪAcDbLineָ��
	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference *pBlockReference = AcDbBlockReference::cast(pEnt);

		// �����ĵ�
		acDocManager->lockDocument(pDoc);
		// ��ʵ��Ĵ�״̬����Ϊ��д״̬
		//pBlockReference->upgradeOpen();
		pBlockReference->close();
		// �����ĵ�
		acDocManager->unlockDocument(pDoc);

		eRCType rcType = TY_GetType(pBlockReference);

		CAcModuleResourceOverride resOverride;
		switch (rcType)
		{
		case WINDOW:
			{
				OpenWindowDlg(pBlockReference);
				break;
			}
			
		case KITCHEN:
			{
				OpenKitchenDlg(pBlockReference);
				break;
			}
			
		case Bathroom:
			{
				OpenBathroomDlg(pBlockReference);
				break;
			}
		case RAILING:
			{
				OpenRailingDlg(pBlockReference);
				break;
			}
		case AIRCON:
			{
				OpenAirconditionerDlg(pBlockReference);
				break;
			}
		}
	}
	else
	{
		acutPrintf(L"\n��Ч��AcDbBlockReference����...");
		pEnt->close();
		return;
	}
}

void CDoubleClickBlockReference::finishEdit( void )
{
	// �˴���ʹ��
}