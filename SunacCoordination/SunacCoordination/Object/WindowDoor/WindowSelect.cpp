#include "StdAfx.h"
#include <dbeval.h>
#include <AcValue.h>
#include <AcDbAssocArrayRectangularParameters.h>
#include <AcDbAssocArrayActionBody.h>
#include <AcDbAssocManager.h>
#include "dbobjptr2.h"
#include <AcDbAssocArrayPathParameters.h>
#include <AcDbAssocArrayPolarParameters.h>

#include "WindowSelect.h"
#include "..\..\Common\ComFun_Def.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_RectArray.h"



CWinInCad::CWinInCad()
{
	m_winId = AcDbObjectId::kNull;
	m_bMirror = false;
	m_mx = AcGeMatrix3d::kIdentity;
}


vector<CWinInCad> CWindowSelect::SelectWindows(eViewDir p_view, bool p_bAllWindow)
{
	Acad::ErrorStatus es;
	vector<CWinInCad> winsOut;//��ǰѡ���ids

	acutPrintf(L"\n��ѡ���Ŵ�");

	ads_name sset;
	//if (p_bAllWindow)
	//{
	//	//TODO ѡ��ȫ�����Զ�����
	//}
	//else
	{
		acedSSGet(NULL, NULL, NULL, NULL, sset);
	}


	Adesk::Int32 length = 0;
	acedSSLength(sset, &length);
	for (int i = 0; i < length; i++)
	{
		ads_name ent;
		acedSSName(sset, i, ent);

		AcDbObjectId objId = 0;
		es = acdbGetObjectId(objId, ent);
		if (es != Acad::eOk || objId == AcDbObjectId::kNull)
		{
			continue;
		}

		vector<CWinInCad> winsTemp;
		AcGeMatrix3d mxTemp = AcGeMatrix3d::kIdentity;
		FindWindowsDeep(objId, p_view, mxTemp, winsTemp);
		if (winsTemp.size() > 0)
		{
			winsOut.insert(winsOut.end(), winsTemp.begin(), winsTemp.end());
		}
	}
	acedSSFree(sset);

	if (winsOut.size() == 0)
	{
		acutPrintf(L"\nδѡ���Ŵ�\n");
	}
	else
	{
		CString info;
		info.Format(L"\n��ѡ����%d���Ŵ�\n", winsOut.size());
		acutPrintf(info);
	}
	return winsOut;
}

int CWindowSelect::FindWindowsDeep(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds)
{
	if (inputId == AcDbObjectId::kNull)
		return 0;

	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, inputId, AcDb::kForRead);
	if (es!=Acad::eOk || pEnt==NULL)
		return 0;


	AcDbBlockReference * pBRef = AcDbBlockReference::cast(pEnt);
	const bool bBlock = (pBRef != NULL);
	const bool bArray = AcDbAssocArrayActionBody::isAssociativeArray(pEnt);
	pEnt->close();

	if (bArray) 
	{
		FindWindowInArray(inputId, viewDir, p_parentMx, outputIds);
	}
	else if (bBlock)
	{
		FindWindowInBlock(inputId, viewDir, p_parentMx, outputIds);
	}
	 

	return 0;
}

int CWindowSelect::FindWindowInBlock(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds)
{
	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, inputId, AcDb::kForRead);
	if (es != Acad::eOk || pEnt == NULL)
	{
		return 0;
	}

	AcGeMatrix3d curReferenceMx = AcGeMatrix3d::kIdentity;
	pEnt->getCompoundObjectTransform(curReferenceMx);
	pEnt->close();

	const AcGeMatrix3d curMx = p_parentMx*curReferenceMx;

	if (TY_IsWindow(inputId, viewDir))
	{
		CWinInCad winInCad;
		winInCad.m_winId = inputId;
		winInCad.m_bMirror = IsWindowMirror(inputId, viewDir, curMx);
		winInCad.m_mx = curMx;
		outputIds.push_back(winInCad);
		return 1;
	}


	ACHAR *blockName = NULL;
	vAcDbObjectId vidsToCheck;
	int ret = MD2010_GetBlockReference_Record_name(inputId, blockName);
	if (ret == 0)
	{
		MD2010_CycleBlockEntites(blockName, vidsToCheck);
		for (int i = 0; i < vidsToCheck.size(); i++)
		{
			FindWindowsDeep(vidsToCheck[i], viewDir, curMx, outputIds);
		}
	}

	if (blockName!=NULL)
	{
		delete blockName;
	}

	return 0;
}

int CWindowSelect::FindWindowInArray(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds)
{
	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, inputId, AcDb::kForRead);
	if (es != Acad::eOk || pEnt == NULL)
	{
		return 0;
	}

	AcGeMatrix3d curReferenceMx = AcGeMatrix3d::kIdentity;
	pEnt->getCompoundObjectTransform(curReferenceMx);
	pEnt->close();

	const AcGeMatrix3d curMx = p_parentMx*curReferenceMx;

	vAcDbObjectId ids;
	AcDbObjectId actionID = TYCOM_GetActionId(inputId);
	TYCOM_GetArrayObjects(actionID, ids);

	vAcDbObjectId ids2;
	for (int i = 0; i < ids.size(); i++)
	{
		//TYCOM_CycleBlockReferenceEntites(ids[0], ids2);//ͨ��������� �õ���һ�����ڲ��� arrayid

		//TYCOM_IsArray(ids2[0]);
		//actionID = TYCOM_GetActionId(ids2[0]);
		//TYCOM_GetArrayObjects(actionID, ids3);//�����12���ڲ���ͼ�� ������window

		//TYCOM_CycleBlockReferenceEntites(ids3[0], ids4);//ͨ��ͼ��õ���Ӧ��window
		//TY_IsWindow(ids4[0], eViewDir::E_VIEW_ALL);

		FindWindowsDeep(ids[i], viewDir, curMx, outputIds);
	}

	return 0;
}

bool CWindowSelect::IsWindowMirror(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx)
{
	double value = p_parentMx.entry[0][0] * p_parentMx.entry[1][1] * p_parentMx.entry[2][2];
	
	return value<0;
}