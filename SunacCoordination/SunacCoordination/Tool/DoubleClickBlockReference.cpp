// DoubleClickCircle.cpp: implementation of the CDoubleClickCircle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DoubleClickBlockReference.h"
#include <dbents.h>
#include <actrans.h>
#include "../Common/ComFun_Sunac.h"

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
		pBlockReference->upgradeOpen();
		
		bool isWindow = TY_IsWindow(pBlockReference->objectId());

		pBlockReference->close();
		
		// �����ĵ�
		acDocManager->unlockDocument(pDoc);
		
		// ����PickFirstѡ���е�����
		//acedSSSetFirst(NULL, NULL);
		
		// ����ͼ�ε���ʾ
		pBlockReference->draw();		// Ϊʲô���ڹر�֮����ʹ��������Ҳ��̫���
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();		
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