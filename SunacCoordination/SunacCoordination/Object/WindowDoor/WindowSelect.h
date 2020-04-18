#pragma once
#include <vector>
#include "AttrWindow.h"
#include "../../Common/TYRect.h"
using namespace std;

class CSunacObjInCad
{
public:
	CSunacObjInCad();

public:
	AcDbObjectId m_winId;
	AcDbObjectId m_rootId; //��Կ��п�ĸ��ڵ�
	bool m_bMxMirror;	   //�Ŵ���ľ����Ƿ���
	double m_rotateAngle;  //�Ŵ������ת�Ƕ�
	AcGeMatrix3d m_mx;
};
//////////////////////////////////////////////////////////////////////////

class CWindowSelect
{
public:
	static vector<CSunacObjInCad> SelectSunacObjs(eViewDir p_view, bool p_bAllWindow = false);
	static vector<CSunacObjInCad> SelectSunacObjsByRect(eViewDir p_view, TYRect p_rect);
	
	static bool IsReferenctMirror(const AcDbObjectId refId);	//ֻ�жϿ����ñ����Ƿ��Ǿ���

	static AcGeMatrix3d GetReferenctWorldMatrix(const AcDbObjectId refId); //�õ�ʵ�����������

protected:
	static vector<CSunacObjInCad> GetSunacObjsInObjectIds(const vector<AcDbObjectId>& p_ids, eViewDir p_view);


	static int FindSunacObjsDeep(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CSunacObjInCad> &outputIds);

	static int FindSunacObjsInBlock(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CSunacObjInCad> &outputIds);
	static int FindSunacObjsInArray (const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CSunacObjInCad> &outputIds);

	static bool IsMxMirror(const AcGeMatrix3d p_parentMx); //ͨ�������ж��Ƿ�����������

};
