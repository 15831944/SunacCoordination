#pragma once

#include "AttrWindow.h"
#include "RCWindow.h"


struct CWinTranslationPara
{
	AcGePoint3d insertPos;
	double rotateAngle;
};

struct CWinInsertPara
{
	AcGePoint3d insertPos;  //ʵ�ʵĲ���λ�ã�������ת�Ƕȿ��ܲ������½ǵ�
	AcGePoint3d leftBottomPos;
	eViewDir viewDir;
	E_DIRECTION insertDir;
	bool bDetailWnd;
	CString sLayerName;

	AcDbObjectId fromWinId;
	AcDbObjectIdArray relatedWinIds;
};



class CWindowGen
{
public:
	//�����µ��Ŵ�
	static AcDbObjectId  GenerateWindow(AttrWindow curWinAtt, const AcGePoint3d pos, E_DIRECTION p_winDir, bool p_bDetailWnd, const AcDbObjectId p_fromWinId, CString p_sLayerName);

	//�����Ŵ�����, bUpdateRelatedWinΪtrue��ͬ�����¹������Ŵ���p_originalIdΪ��������޸ĸ��µ��Ŵ����Ա��ں��������޸�ʱ���ظ��޸�
	static AcDbObjectId UpdateWindow(const AcDbObjectId p_id, AttrWindow newWinAtt, const bool bUpdateRelatedWin);//

protected:
	static double GetBlockRotateAngle(const AttrWindow& curWinAtt, eViewDir p_view, E_DIRECTION p_winDir);
	static AcGePoint3d GetBlockInsertPos(const AcDbObjectId p_id, const AttrWindow& curWinAtt, const AcGePoint3d pos, eViewDir p_view, E_DIRECTION p_winDir);
	static void UpdateRcWindowPara(const AcDbObjectId p_id, const AttrWindow& curWinAtt, eViewDir p_view, bool p_bDetailWnd);
	static void AddWinAtt(const AcDbObjectId p_id, AttrWindow p_winAtt);
	static void UpdateWinAtt(const AcDbObjectId p_id, AttrWindow p_winAtt);

	static void ModifyOneWindow(const AcDbObjectId p_id, AttrWindow newWinAtt);

	static AcGePoint3d GetWindowInsertPos(AcDbObjectId p_id);
	static AcGePoint3d GetWindowLeftBottomPos(AcDbObjectId p_id);
	static E_DIRECTION GetWindowInsertDir(AcDbObjectId p_id);
	static CWinInsertPara GetWindowInsertPara(AcDbObjectId p_id); //�����Ѳ�����Ŵ���ȡ��������Ϣ
	static bool IsWindowMirror(AcDbObjectId p_id);
	static AttrWindow* GetWinAtt(AcDbObjectId p_id);

	static bool IsPrototypeCodeSame(const AcDbObjectId p_id, const AttrWindow& newWinAtt); //�µ�ԭ�ͱ���Ƿ��֮ǰ�Ĳ�ͬ
	static bool SetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId p_fromWinId, AcDbObjectIdArray p_relatedIds);
	static bool GetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId& p_fromWinId, AcDbObjectIdArray& p_relatedIds);

	//�����Ŵ����
	static AcDbObjectId InsertWindowDoorCode(double p_width, double p_height, AcGePoint3d p_origin, CString p_number, eViewDir p_viewDir);

	static bool MirrorObjectByCenter(const AcDbObjectId p_id, E_DIRECTION p_winDir);
	static bool DeleteWindowObj(const AcDbObjectId p_id);



};


