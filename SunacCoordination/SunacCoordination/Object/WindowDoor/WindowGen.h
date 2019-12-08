#pragma once

#include "AttrWindow.h"
#include "RCWindow.h"


struct CWindowInsertPara
{
	AcGePoint3d insertPos;
	AcGePoint3d mirrorBasePt;
	AcGeVector3d mirrorAxis;
	double rotateAngle;
};

class CWindowGen
{
public:
	//�����µ��Ŵ�
	static AcDbObjectId  GenerateWindow(const AttrWindow& curWinAtt, const AcGePoint3d pos, E_DIRECTION p_winDir, bool p_bDetailWnd, const AcDbObjectId p_fromWinId, CString p_sLayerName);

	//�����Ŵ�����, bUpdateRelatedWinΪtrue��ͬ�����¹������Ŵ���p_originalIdΪ��������޸ĸ��µ��Ŵ����Ա��ں��������޸�ʱ���ظ��޸�
	static AcDbObjectId UpdateWindow(const AcDbObjectId p_id, AttrWindow newWinAtt, const bool bUpdateRelatedWin, const AcDbObjectId p_originalId );//

protected:
	static CWindowInsertPara InitInsertPara(const AttrWindow& curWinAtt, const AcGePoint3d pos, eViewDir p_view, E_DIRECTION p_winDir);
	static void UpdateRcWindowPara(RCWindow &oneWindow, const AttrWindow& curWinAtt, eViewDir p_view, bool p_bDetailWnd);

};


