#include "StdAfx.h"
#include <dbidmap.h>
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Sunac.h"
#include "WindowTop2Front.h"
#include "AttrWindow.h"
#include "RCWindow.h"


bool CWindowTop2Front::GenFrontFromTop()
{
	//1.ѡ����Ҫ����¥����Ŵ�
	vAcDbObjectId winIds = SelectWindows(E_VIEW_TOP);
	if (winIds.size() == 0)
	{
		return false;
	}


	E_DIRECTION windowDir = E_DIR_BOTTOM;
	bool bSuc1 = GetTopViewWindowDirection(windowDir);
	if (bSuc1 == false)
		return false;

	AcGePoint3d insertPos;
	bool bSuc = TY_GetPoint(insertPos, L"\n��ѡ������ͼ�����");
	if (bSuc == false)
		return false;

	//////////////////////////////////////////////////////////////////////////
	AcDbObjectIdArray idsNoFloorInfo; //û������¥��Ĵ���
	vector<AttrWindow>  winAtts;
	vector<AcGePoint3d> allPos; //�����
	vector<AcDbExtents> allExtents; //�����
	for (UINT i = 0; i < winIds.size(); i++)
	{
		RCWindow oneWindow;
		oneWindow.m_id = winIds[i];
		oneWindow.InitParameters();

		AttrWindow* pAtt = oneWindow.GetAttribute();
		if (pAtt != NULL)
		{
			if (pAtt->GetFloorInfo().GetAllFloor().size() == 0)
			{
				idsNoFloorInfo.append(winIds[i]);
			}

			AttrWindow attTemp(*pAtt);
			attTemp.m_viewDir = E_VIEW_FRONT;  //�����ɵ�Ϊ����ͼ
			winAtts.push_back(attTemp);
			pAtt->close();

			allPos.push_back(oneWindow.m_blockInsertPos);
			allExtents.push_back(oneWindow.m_blockExtent);
		}
	}

	if (idsNoFloorInfo.length() > 0)
	{
		AfxMessageBox(_T("���ִ���δ����¥��Ͳ��"));
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
	AcDbObjectIdArray oneFloorIds;
	for (UINT i = 0; i < winAtts.size(); i++)
	{
		const AttrWindow& curWinAtt = winAtts[i];

		//��ǰ�еĲ����
		AcGePoint3d posColum = insertPos;
		posColum.x += allXvalue[i] - minX;

		const CFloorInfo floorInfo = curWinAtt.GetFloorInfo();
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

		AcDbObjectId idOut = GenerateWindow(curWinAtt, pos, E_VIEW_FRONT, E_DIR_BOTTOM, false, L"Sunac_Window");
		oneFloorIds.append(idOut);
	}

	AcDbObjectIdArray windowObjIds = CopyAllFloorByOneFloor(oneFloorIds, winAtts);

	return true;
}

AcDbObjectIdArray CWindowTop2Front::CopyAllFloorByOneFloor(const AcDbObjectIdArray& oneFloorIds, const vector<AttrWindow> &winAtts)
{
	AcDbObjectIdArray windowObjIds;
	windowObjIds.append(oneFloorIds);


	Acad::ErrorStatus es;
	//����¥����ø��Ʒ�ʽ
	for (UINT i = 0; i < winAtts.size(); i++)
	{
		const AttrWindow& curWinAtt = winAtts[i];

		AcDbEntity* pEnt = NULL;
		es = acdbOpenObject(pEnt, oneFloorIds[i], AcDb::kForRead);
		if (es != Acad::eOk)
			continue;

		const CFloorInfo floorInfo = curWinAtt.GetFloorInfo();
		const vector<int>  allFloos = floorInfo.GetAllFloor();
		if (allFloos.size() == 0)
			continue;

		const int firstFloor = allFloos[0];
		for (UINT n = 1; n < allFloos.size(); n++)
		{
			AcGeMatrix3d xform;
			xform.setTranslation(AcGeVector3d(0, (allFloos[n] - firstFloor)*floorInfo.GetFloorHeight(), 0));

			AcDbEntity*  pCopyEntity = NULL;
			es = pEnt->getTransformedCopy(xform, pCopyEntity);
			if (pCopyEntity!=NULL)
			{
				AcDbObjectId idOut = JHCOM_PostToModelSpace(pCopyEntity);
				pCopyEntity->close();

				AttrWindow * pWindowAtt = new AttrWindow(curWinAtt);
				TY_AddAttributeData(idOut, pWindowAtt);
				pWindowAtt->close();

				windowObjIds.append(idOut);
			}
		}

		pEnt->close();
	}

	return windowObjIds;
}

bool CWindowTop2Front::GetTopViewWindowDirection(E_DIRECTION &windowDir) //�õ�ƽ�洰���ķ�λ���ϡ�������
{
	windowDir = E_DIR_BOTTOM;

	CString sDir = _T("S");

	bool bSuc = false;
	do 
	{
		bSuc = GetStringInput(_T("\n�����봰������[��(E) ��(W) ��(S) ��(N)]<S>:"), sDir);
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

vector<double> CWindowTop2Front::GetAllXValueInFrontView(const vector<AcDbExtents> &allExtents, const E_DIRECTION windowDir) //��ȡ��������������ͼ�ϵ�xֵ
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

