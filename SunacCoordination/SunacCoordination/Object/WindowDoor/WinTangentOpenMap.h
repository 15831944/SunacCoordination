#pragma once
#include <map>
using namespace std;


class CWinTangentOpenMap
{
public:
	//�������Ŵ�����appand���Ŵ������ơ�����������Ȳ���)ʱ���ô˺��������Ŵ����Ŷ�����
	void AddWindow(AcDbObjectId p_winId, AcDbObjectId p_tangentOpenId);
	void RemoveWindow(AcDbObjectId p_winId);
	AcDbObjectId GetTangentOpenId(AcDbObjectId p_winId);



	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);

protected:
	map<AcDbObjectId, AcDbObjectId> m_idsMap;

};