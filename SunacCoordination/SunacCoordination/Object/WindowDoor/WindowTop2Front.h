#pragma once
#include "AttrWindow.h"

class CWindowTop2Front
{
public:
	static bool GenFrontFromTop();

protected:
	static bool GetTopViewWindowDirection(E_DIRECTION &windowDir); //得到平面窗户的方位，上、下左右
	static vector<double> GetAllXValueInFrontView(const vector<AcDbExtents> &allExtents, const E_DIRECTION windowDir); //获取各窗户在立面视图上的x值

	static 	AcDbObjectIdArray CopyAllFloorByOneFloor(const AcDbObjectId& oneFloorId, const AttrWindow* pWinAtt);
};

