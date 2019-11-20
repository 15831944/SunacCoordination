#pragma once
#include "AttrWindow.h"

class CWindowTop2Front
{
public:
	static bool GenFrontFromTop();
	static AcDbObjectId  GenerateWindow(const AttrWindow& curWinAtt, AcGePoint3d pos, eViewDir p_view);

protected:
	static bool GetTopViewWindowDirection(E_DIRECTION &windowDir); //�õ�ƽ�洰���ķ�λ���ϡ�������
	static vector<double> GetAllXValueInFrontView(const vector<AcDbExtents> &allExtents, const E_DIRECTION windowDir); //��ȡ��������������ͼ�ϵ�xֵ

	static 	AcDbObjectIdArray CopyAllFloorByOneFloor(const AcDbObjectIdArray& oneFloorIds, const vector<AttrWindow> &winAtts);
};

