#pragma once
#include <vector>
#include "AttrWindow.h"
#include "../../Common/TYRect.h"
using namespace std;

class CWinInCad
{
public:
	CWinInCad();

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
	static vector<CWinInCad> SelectWindows(eViewDir p_view, bool p_bAllWindow = false);
	static vector<CWinInCad> SelectWindowsByRect(eViewDir p_view, TYRect p_rect);
	
	static bool IsReferenctMirror(const AcDbObjectId refId);	//ֻ�жϿ����ñ����Ƿ��Ǿ���

	static AcGeMatrix3d GetReferenctWorldMatrix(const AcDbObjectId refId); //�õ�ʵ�����������

protected:
	static vector<CWinInCad> GetWinsInObjectIds(const vector<AcDbObjectId>& p_ids, eViewDir p_view);


	static int FindWindowsDeep(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);

	static int FindWindowInBlock(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);
	static int FindWindowInArray(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);

	static bool IsMxMirror(const AcGeMatrix3d p_parentMx); //ͨ�������ж��Ƿ�����������

};
