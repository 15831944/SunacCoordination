#pragma once
#include "AttrRailing.h"

class CRailingTop2Front
{
public:
	static bool GenFrontFromTop();

protected:
	static bool GetTopViewDirection(E_DIRECTION &windowDir); //�õ�ƽ�洰���ķ�λ���ϡ�������
	static vector<double> GetAllXValueInFrontView(const vector<AcDbExtents> &allExtents, const E_DIRECTION windowDir); //��ȡ��������������ͼ�ϵ�xֵ

	static 	AcDbObjectIdArray CopyAllFloorByOneFloor(const AcDbObjectId& oneFloorId, const AttrRailing* pRailingAtt);


	static bool GetBlockInsertPosAndExtent(const AcDbObjectId p_id, AcGePoint3d& p_blockInsertPos, AcDbExtents& p_blockExtent);
};

