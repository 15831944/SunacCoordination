#include "StdAfx.h"
#include <dbeval.h>
#include <AcValue.h>

#if (!defined ARX_2010) && (!defined ARX_2011)
#include <AcDbAssocArrayRectangularParameters.h>
#include <AcDbAssocArrayActionBody.h>
#include <AcDbAssocArrayPathParameters.h>
#include <AcDbAssocArrayPolarParameters.h>
#endif

#include <AcDbAssocManager.h>
#include <dbobjptr2.h>
#include "WindowSelect.h"
#include "WindowAutoName.h"
#include "..\..\Common\ComFun_Def.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_RectArray.h"
#include "..\..\Src\DocumentData.h"



CSunacObjInCad::CSunacObjInCad()
{
	m_winId = AcDbObjectId::kNull;
	m_rootId = AcDbObjectId::kNull;
	m_bMxMirror = false;
	m_rotateAngle = 0;
	m_mx = AcGeMatrix3d::kIdentity;
}


vector<CSunacObjInCad> CSunacSelect::SelectSunacObjs(const eRCType p_rcType, eViewDir p_view, bool p_bAllWindow)
{
	Acad::ErrorStatus es;	

	CString sType = ToCstring(p_rcType);
	CString sViewDir = _T("ͼ");
	if (E_VIEW_TOP == p_view)
		sViewDir = _T("ƽ��ͼ");
	else if (E_VIEW_FRONT == p_view)
		sViewDir = _T("����ͼ");
	acutPrintf(L"\n����" + sViewDir + L"��ѡ��" + sType);

	ads_name sset;
	if (p_bAllWindow)
	{
		int rt = acedSSGet(_T("A"), NULL, NULL, NULL, sset); // ��ʾ�û�ѡ�����
	}
	else
	{
		acedSSGet(NULL, NULL, NULL, NULL, sset);
	}


	Adesk::Int32 length = 0;
	acedSSLength(sset, &length);
	vector<AcDbObjectId> ids;
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

		ids.push_back(objId);
	}
	acedSSFree(sset);
	//////////////////////////////////////////////////////////////////////////

	vector<CSunacObjInCad> winsOut = GetSunacObjsInObjectIds(ids, p_rcType, p_view);//��ǰѡ���ids

	if (winsOut.size() == 0)
	{
		acutPrintf(L"\nδѡ���ڴ�����\n");
	}
	else
	{
		CString info;
		info.Format(L"\n��ѡ����%d���ڴ�����\n", winsOut.size());
		acutPrintf(info);
	}
	return winsOut;
}


vector<CSunacObjInCad> CSunacSelect::GetSunacObjsInObjectIds(const vector<AcDbObjectId>& p_ids, const eRCType p_rcType, eViewDir p_view)
{
	vector<CSunacObjInCad> winsOut;
	for (UINT i = 0; i < p_ids.size(); i++)
	{
		AcDbObjectId objId = p_ids[i];

		vector<CSunacObjInCad> winsTemp;
		AcGeMatrix3d mxTemp = AcGeMatrix3d::kIdentity;
		FindSunacObjsDeep(objId, p_rcType, p_view, mxTemp, winsTemp);
		if (winsTemp.size() > 0)
		{
			for (UINT n = 0; n < winsTemp.size(); n++)
			{
				winsTemp[n].m_rootId = objId;
			}
			winsOut.insert(winsOut.end(), winsTemp.begin(), winsTemp.end());
		}
	}

	return winsOut;
}

vector<CSunacObjInCad> CSunacSelect::SelectSunacObjsByRect(const eRCType p_rcType, eViewDir p_view, TYRect p_rect)
{
	ads_point pt1, pt2;
	pt1[X] = p_rect.GetLT().x;
	pt1[Y] = p_rect.GetLT().y;
	pt1[Z] = p_rect.GetLT().z;
	pt2[X] = p_rect.GetRB().x;
	pt2[Y] = p_rect.GetRB().y;
	pt2[Z] = p_rect.GetRB().z;

	Acad::ErrorStatus es;

	acutPrintf(L"\n��ѡ���Ŵ�");

	ads_name sset;
	acedSSGet(TEXT("W"), pt1, pt2, NULL, sset);//ɸѡ��rect��Χ�ڵĽ��

	Adesk::Int32 length = 0;
	acedSSLength(sset, &length);
	vector<AcDbObjectId> ids;
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

		ids.push_back(objId);
	}
	acedSSFree(sset);
	//////////////////////////////////////////////////////////////////////////

	vector<CSunacObjInCad> winsOut = GetSunacObjsInObjectIds(ids, p_rcType, p_view);//��ǰѡ���ids

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

int CSunacSelect::FindSunacObjsDeep(const AcDbObjectId inputId, const eRCType p_rcType, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CSunacObjInCad> &outputIds)
{
	if (inputId == AcDbObjectId::kNull)
		return 0;

	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, inputId, AcDb::kForRead);
	if (es!=Acad::eOk || pEnt==NULL)
		return 0;


	bool bArray = false;

	AcDbBlockReference * pBRef = AcDbBlockReference::cast(pEnt);
    const bool bBlock = (pBRef != NULL);
#if (!defined ARX_2010) && (!defined ARX_2011)
    bArray  = AcDbAssocArrayActionBody::isAssociativeArray(pEnt);
#endif
	pEnt->close();

	if (bArray) 
	{
		FindSunacObjsInArray(inputId, p_rcType, viewDir, p_parentMx, outputIds);
	}
	else if (bBlock)
	{
		FindSunacObjsInBlock(inputId, p_rcType, viewDir, p_parentMx, outputIds);
	}
	//group��� �����ض�������ѡ���ʱ���Ƕ��������ʵ�壬ֻ��һ��ѡ��
	 

	return 0;
}

int CSunacSelect::FindSunacObjsInBlock(const AcDbObjectId inputId, const eRCType p_rcType, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CSunacObjInCad> &outputIds)
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

	if (TY_IsSunacObj(inputId, p_rcType, viewDir))
	{
		CSunacObjInCad winInCad;
		winInCad.m_winId = inputId;
		winInCad.m_rootId = inputId;
		winInCad.m_bMxMirror = IsMxMirror(curMx);
		winInCad.m_mx = curMx;

		AcDbBlockReference * pBRef = AcDbBlockReference::cast(pEnt);
		if (pBRef!=NULL)
		{
			winInCad.m_rotateAngle = pBRef->rotation();
		}

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
			FindSunacObjsDeep(vidsToCheck[i], p_rcType, viewDir, curMx, outputIds);
		}
	}

	if (blockName!=NULL)
	{
		delete blockName;
	}

	return 0;
}

int CSunacSelect::FindSunacObjsInArray(const AcDbObjectId inputId, const eRCType p_rcType, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CSunacObjInCad> &outputIds)
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
#if (defined ARX_2010) || (defined ARX_2011)
	ids.push_back(inputId);
#else
	AcDbObjectId actionID = TYCOM_GetActionId(inputId);
	TYCOM_GetArrayObjects(actionID, ids);
#endif

	vAcDbObjectId ids2;
	for (int i = 0; i < ids.size(); i++)
	{
		FindSunacObjsDeep(ids[i], p_rcType, viewDir, curMx, outputIds);
	}

	return 0;
}

bool CSunacSelect::IsMxMirror(const AcGeMatrix3d p_parentMx)
{
	double value = p_parentMx.entry[0][0] * p_parentMx.entry[1][1] * p_parentMx.entry[2][2];
	
	return value<0;
}

bool CSunacSelect::IsReferenctMirror(const AcDbObjectId refId)
{
	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, refId, AcDb::kForRead);
	if (es != Acad::eOk || pEnt == NULL)
	{
		return false;
	}

	AcGeMatrix3d curReferenceMx = AcGeMatrix3d::kIdentity;
	pEnt->getCompoundObjectTransform(curReferenceMx);
	pEnt->close();

	return IsMxMirror(curReferenceMx);
}

AcGeMatrix3d CSunacSelect::GetReferenctWorldMatrix(const AcDbObjectId refId) //�õ�ʵ�����������
{
	AcGeMatrix3d mx = AcGeMatrix3d::kIdentity;

	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, refId, AcDb::kForRead);
	if (es != Acad::eOk || pEnt == NULL)
		return mx;

	pEnt->getCompoundObjectTransform(mx);

	AcDbObjectId parentId = pEnt->ownerId();
	while (parentId!=AcDbObjectId::kNull)
	{
		AcDbEntity * pEntParent = 0;
		es = acdbOpenObject(pEntParent, parentId, AcDb::kForRead);
		if (es != Acad::eOk || pEntParent == NULL)
			continue;

		AcGeMatrix3d mxParent = AcGeMatrix3d::kIdentity;
		pEntParent->getCompoundObjectTransform(mxParent);
		mx = mxParent*mx;

		pEntParent->close();

		parentId = pEntParent->ownerId();
	}

	pEnt->close();

	return mx;
}
