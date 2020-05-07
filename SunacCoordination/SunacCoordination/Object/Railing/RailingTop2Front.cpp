#include "StdAfx.h"
#include <dbidmap.h>
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Sunac.h"
#include "..\..\Src\DocumentData.h"
#include "..\..\GlobalSetting.h"
#include "..\WindowDoor\WindowSelect.h"
#include "RailingTop2Front.h"
#include "RCRailing.h"


bool CRailingTop2Front::GenFrontFromTop()
{
	//1.ѡ����Ҫ����¥�������
	const vector<CSunacObjInCad> railings = CSunacSelect::SelectSunacObjs(S_RAILING, E_VIEW_TOP);
	vAcDbObjectId ringIds;
	for (UINT i = 0; i < railings.size(); i++)
	{
		ringIds.push_back(railings[i].m_winId);
	}
	if (ringIds.size() == 0)
		return false;


	E_DIRECTION windowDir = E_DIR_BOTTOM;
	bool bSuc1 = GetTopViewDirection(windowDir);
	if (bSuc1 == false)
		return false;

	AcGePoint3d insertPos;
	bool bSuc = TY_GetPoint(insertPos, L"\n��ѡ������ͼ�����");
	if (bSuc == false)
		return false;

	//////////////////////////////////////////////////////////////////////////
	AcDbObjectIdArray idsNoFloorInfo; //û������¥��Ĵ���
	vector<AttrRailing*>  railingAtts;
	vector<AcGePoint3d> allPos; //�����
	vector<AcDbExtents> allExtents; //�����
	for (UINT i = 0; i < ringIds.size(); i++)
	{
		AcDbObject * pDataEnt = 0;
		TY_GetAttributeData(ringIds[i], pDataEnt, true);
		if (pDataEnt==NULL)
			continue;
		
		AttrRailing * pRailing = dynamic_cast<AttrRailing *>(pDataEnt);
		if (pRailing == NULL)
		{
			pDataEnt->close();
			continue;
		}

		if (pRailing->GetFloorInfo().GetAllFloor().size() == 0)
		{
			idsNoFloorInfo.append(ringIds[i]);
		}

		railingAtts.push_back(pRailing);
		pRailing->close();

		AcGePoint3d blockInsertPos;
		AcDbExtents blockExtent;
		GetBlockInsertPosAndExtent(ringIds[i], blockInsertPos, blockExtent);
		allPos.push_back(blockInsertPos);
		allExtents.push_back(blockExtent);
	}

	if (idsNoFloorInfo.length() > 0)
	{
		AfxMessageBox(_T("��������δ����¥��Ͳ��"));
		for (int i = 0; i < idsNoFloorInfo.length(); i++)
		{
			JHCOM_HilightObject(idsNoFloorInfo[i], true);
		}
		return false;
	}
	
	vector<double> allXvalue = GetAllXValueInFrontView(allExtents, windowDir);
	if (allXvalue.size()==0)
	{
		return false;
	}

	//��ԭ���Ĵ������������ҵ�������λ��
	double minX = 1e10;
	for (UINT i = 0; i < allXvalue.size(); i++)
	{
		if (allXvalue[i] < minX)
		{
			minX = allXvalue[i];
		}
	}


	//////////////////////////////////////////////////////////////////////////
	AcDbObjectIdArray idsOut;
	for (UINT i = 0; i < railingAtts.size(); i++)
	{
		AttrRailing curRailingAtt = *(railingAtts[i]);
		curRailingAtt.SetViewDir(E_VIEW_FRONT);  //�����ɵ�Ϊ����ͼ

		//curRailingAtt.ClearWinsRelation();
		//curRailingAtt.SetFromWinId(ringIds[i]);

		//��ǰ�еĲ����
		AcGePoint3d posColum = insertPos;
		posColum.x += allXvalue[i] - minX;

		const CFloorInfo floorInfo = curRailingAtt.GetFloorInfo();
		const vector<int>  allFloos = floorInfo.GetAllFloor();
		if (allFloos.size() == 0)
			continue;

		const int curFloor = allFloos[0];
		AcGePoint3d pos = posColum;
		//yuan ѡ�����½Ǵ���ʱ����ע�ͣ�Y����ѡ��¥���׼λ��ʱ�ſ�
		//pos.y += floorInfo.GetFloorHeight()* (curFloor - 1);
		//if (curWinAtt.GetType() == WINDOW)
		//{
		//	pos.y += curWinAtt.GetHeightUnderWindow();
		//}

		AcDbObjectId railingId;
		CRCRailing* pRailing = CreateRailing(curRailingAtt);
		int nRet = pRailing->GenerateRailing(pos, railingId);
		
		//����¥�㸴�Ʒ�ʽ
		AcDbObjectIdArray railingObjIds = CopyAllFloorByOneFloor(railingId, &curRailingAtt);
		//railingAtts[i]->SetRelatedWinIds(windowObjIds);

		idsOut.append(railingObjIds);
	}

	return true;
}

bool CRailingTop2Front::GetBlockInsertPosAndExtent(const AcDbObjectId p_id, AcGePoint3d& p_blockInsertPos, AcDbExtents& p_blockExtent)
{
	Acad::ErrorStatus es;
	AcDbEntity* pEnt = NULL;
	es = acdbOpenObject(pEnt, p_id, AcDb::kForRead);
	if (es != Acad::eOk)
	{
		if (pEnt)
			pEnt->close();
		return false;
	}

	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		pEnt->close();
		return 1;
	}

	AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
	if (pBlkRef!=NULL)
	{
		p_blockInsertPos = pBlkRef->position();
		pBlkRef->getGeomExtents(p_blockExtent);
	}

	pEnt->close();
	return true;
}

AcDbObjectIdArray CRailingTop2Front::CopyAllFloorByOneFloor(const AcDbObjectId& oneFloorId, const AttrRailing* pRailingAtt)
{
	AcDbObjectIdArray windowObjIds;
	windowObjIds.append(oneFloorId);
	
	//����¥����ø��Ʒ�ʽ
	const CFloorInfo floorInfo = pRailingAtt->GetFloorInfo();
	const vector<int>  allFloos = floorInfo.GetAllFloor();
	if (allFloos.size() == 0)
		return windowObjIds;

	AcDbObjectIdArray objList;
	objList.append(oneFloorId);
	const int firstFloor = allFloos[0];
	for (UINT n = 1; n < allFloos.size(); n++)
	{
		AcGeVector3d vOffset =AcGeVector3d(0, (allFloos[n] - firstFloor)*floorInfo.GetFloorHeight(), 0);

		AcDbObjectIdArray objListCloned;
		bool bSuc = CloneObjects(objList, objListCloned);
		if (bSuc && objListCloned.length()>0)
		{
			TYCOM_Move(objListCloned[0], vOffset);

			windowObjIds.append(objListCloned);
		}
	}

	return windowObjIds;
}


bool CRailingTop2Front::GetTopViewDirection(E_DIRECTION &windowDir) //�õ�ƽ�洰���ķ�λ���ϡ�������
{
	windowDir = E_DIR_BOTTOM;

	CString sDir = _T("S");

	bool bSuc = false;
	do 
	{
		bSuc = GetStringInput(_T("\n�����볯��[��(E) ��(W) ��(S) ��(N)]<S>:"), sDir);
		if (bSuc == false)
			return false;

		sDir.Trim();
		sDir.MakeUpper();
		if (sDir.IsEmpty())
		{
			windowDir = E_DIR_BOTTOM;
			break;
		}

		if (sDir.Find(_T('E')) >= 0 || sDir.Find(_T('��')) >= 0)
		{
			windowDir = E_DIR_RIGHT;
			break;
		}
		else if (sDir.Find(_T('W')) >= 0 || sDir.Find(_T('��')) >= 0)
		{
			windowDir = E_DIR_LEFT;
			break;
		}
		else if (sDir.Find(_T('S')) >= 0 || sDir.Find(_T('��')) >= 0)
		{
			windowDir = E_DIR_BOTTOM;
			break;
		}
		else if (sDir.Find(_T('N')) >= 0 || sDir.Find(_T('��')) >= 0)
		{
			windowDir = E_DIR_TOP;
			break;
		}

	} while (bSuc);

	return bSuc;
}

vector<double> CRailingTop2Front::GetAllXValueInFrontView(const vector<AcDbExtents> &allExtents, const E_DIRECTION windowDir) //��ȡ��������������ͼ�ϵ�xֵ
{
	vector<double> allXvalue;

	switch (windowDir)
	{
	case E_DIR_BOTTOM:
	{
						 for (UINT i = 0; i < allExtents.size(); i++)
						 {
							 allXvalue.push_back(allExtents[i].minPoint().x);
						 }
						 break;
	}

	case E_DIR_TOP:
	{
					  double maxX = -1e10;
					  for (UINT i = 0; i < allExtents.size(); i++)
					  {
						  double xValue = allExtents[i].maxPoint().x;
						  allXvalue.push_back(xValue);
						  if (maxX<xValue)
						  {
							  maxX = xValue;
						  }
					  }

					  for (UINT i = 0; i < allXvalue.size(); i++)
					  {
						  allXvalue[i] = maxX - allXvalue[i];
					  }
		break;
	}
	case E_DIR_LEFT:
	{
					   double maxY = -1e10;
					   for (UINT i = 0; i < allExtents.size(); i++)
					   {
						   double yValue = allExtents[i].minPoint().y;
						   allXvalue.push_back(yValue);
						   if (maxY < yValue)
						   {
							   maxY = yValue;
						   }
					   }
					   for (UINT i = 0; i < allXvalue.size(); i++)
					   {
						   allXvalue[i] = maxY - allXvalue[i];
					   }
					   break;
	}
	case E_DIR_RIGHT:
	{
						for (UINT i = 0; i < allExtents.size(); i++)
						{
							double valueY = allExtents[i].minPoint().y;
							allXvalue.push_back(valueY);
						}
						break;
	}
	default:
		assert(false);
		break;
	}

	return allXvalue;
}

