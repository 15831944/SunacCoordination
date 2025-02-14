#pragma once
#include <vector>
#include "AttrWindow.h"
#include "../../Common/TYRect.h"
using namespace std;

class CSunacObjInCad
{
public:
	CSunacObjInCad();

	bool operator < (const CSunacObjInCad& p_obj)const;

public:
	AcDbObjectId m_winId;	//门窗id，若是组合后的块参照，则为块定义内部的门窗id
	AcDbObjectId m_rootId; //针对块中块的根节点, 若常规门窗，则和m_winId 相同
	bool m_bMxMirror;	   //门窗块的矩阵是否镜像
	double m_rotateAngle;  //门窗块的旋转角度
	AcGeMatrix3d m_mx;
};
//////////////////////////////////////////////////////////////////////////

class CSunacSelect
{
public:
	static vector<CSunacObjInCad> SelectSunacObjs(const eRCType p_rcType, eViewDir p_view, bool p_bAllWindow = false);
	static vector<CSunacObjInCad> SelectSunacObjsByRect(const eRCType p_rcType, eViewDir p_view, TYRect p_rect);
	
	static bool IsReferenctMirror(const AcDbObjectId refId);	//只判断块引用本身是否是镜像

	static AcGeMatrix3d GetReferenctWorldMatrix(const AcDbObjectId refId); //得到实体的完整镜像

protected:
	static vector<CSunacObjInCad> GetSunacObjsInObjectIds(const vector<AcDbObjectId>& p_ids, const eRCType p_rcType, eViewDir p_view);


	static int FindSunacObjsDeep(const AcDbObjectId inputId, const eRCType p_rcType, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CSunacObjInCad> &outputIds);

	static int FindSunacObjsInBlock(const AcDbObjectId inputId, const eRCType p_rcType, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CSunacObjInCad> &outputIds);
	static int FindSunacObjsInArray (const AcDbObjectId inputId, const eRCType p_rcType, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CSunacObjInCad> &outputIds);

	static bool IsMxMirror(const AcGeMatrix3d p_parentMx); //通过矩阵判断是否做过镜像处理

};
