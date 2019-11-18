#include "StdAfx.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Sunac.h"
#include "WindowTop2Front.h"
#include "AttrWindow.h"
#include "RCWindow.h"


bool CWindowTop2Front::GenFrontFromTop()
{
	//1.ѡ����Ҫ����¥����Ŵ�
	vAcDbObjectId m_vids = SelectWindows(E_VIEW_TOP);
	if (m_vids.size() == 0)
	{
		return false;
	}

	AcGePoint3d insertPos;
	bool bSuc = TY_GetPoint(insertPos, L"��ѡ������ͼ�����");
	if (bSuc == false)
		return false;

	//////////////////////////////////////////////////////////////////////////
	AcDbObjectIdArray idsNoFloorInfo; //û������¥��Ĵ���
	vector<AttrWindow>  winAtts;
	vector<AcGePoint3d> allPos; //�����
	vector<AcDbExtents> allExtents; //�����
	for (UINT i = 0; i < m_vids.size(); i++)
	{
		RCWindow oneWindow;
		oneWindow.m_id = m_vids[i];
		oneWindow.InitParameters();

		AttrWindow* pAtt = oneWindow.GetAttribute();
		if (pAtt != NULL)
		{
			if (pAtt->GetFloorInfo().GetAllFloor().size() == 0)
			{
				idsNoFloorInfo.append(m_vids[i]);
			}

			AttrWindow attTemp(*pAtt);
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
	
	vector<double> allXvalue = GetAllXValueInFrontView(allExtents);
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

		AcDbObjectId idOut = GenerateWindow(curWinAtt, pos, E_VIEW_FRONT);
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

			AcDbEntity*  pCopyObj = NULL;
			es = pEnt->getTransformedCopy(xform, pCopyObj);
			AcDbObjectId idOut = JHCOM_PostToModelSpace(pCopyObj);
			pCopyObj->close();

			windowObjIds.append(idOut);
		}

		pEnt->close();
	}

	return windowObjIds;
}

E_DIRECTION CWindowTop2Front::GetTopViewWindowDirection(const vector<AcDbExtents> &allExtents) //�õ�ƽ�洰���ķ�λ���ϡ�������
{
	E_DIRECTION viewDir = E_DIR_BOTTOM;

	//TODO
	return viewDir;
}

vector<double> CWindowTop2Front::GetAllXValueInFrontView(const vector<AcDbExtents> &allExtents) //��ȡ��������������ͼ�ϵ�xֵ
{
	//�жϴ����ķ���
	E_DIRECTION viewDir = GetTopViewWindowDirection(allExtents);

	vector<double> allXvalue;
	if (viewDir == E_DIR_BOTTOM)
	{
		for (UINT i = 0; i < allExtents.size(); i++)
		{
			allXvalue.push_back(allExtents[i].minPoint().x);
		}
	}
	else
	{
		//TODO
	}

	return allXvalue;
}

AcDbObjectId  CWindowTop2Front::GenerateWindow(const AttrWindow& curWinAtt, AcGePoint3d pos, eViewDir p_view)
{
	RCWindow oneWindow;
	AcDbObjectId id = oneWindow.Insert(curWinAtt.GetPrototypeDwgFilePath(p_view), pos, 0, L"0", 256);

	oneWindow.InitParameters();

	oneWindow.SetParameter(L"H", (double)curWinAtt.GetH());
	oneWindow.SetParameter(L"W", (double)curWinAtt.GetW());
	oneWindow.SetParameter(L"W1", (double)curWinAtt.GetW1());
	if (curWinAtt.HasValue(_T("H2")))
		oneWindow.SetParameter(L"H2", (double)curWinAtt.GetH2());
	if (curWinAtt.HasValue(_T("W3")))
		oneWindow.SetParameter(L"W3", (double)curWinAtt.GetW3());
	if (curWinAtt.HasValue(_T("H3")))
		oneWindow.SetParameter(L"H3", (double)curWinAtt.GetH3());

	oneWindow.RunParameters();

	if (curWinAtt.m_isMirror && (curWinAtt.m_isMirrorWindow == false))
	{
		AcGePoint3d basePt(pos.x + curWinAtt.GetW() / 2, 0, 0);
		TYCOM_Mirror(oneWindow.m_id, basePt, AcGeVector3d(0, 1, 0));
	}


	//��UI�����ݼ�¼��ͼ�����չ�ֵ���
	AttrWindow * pWindow = new AttrWindow(curWinAtt);
	pWindow->m_viewDir = p_view;
	oneWindow.AddAttribute(pWindow);
	pWindow->close();

	return id;
}
