#pragma once

#include <vector>
#include <map>
#include "AttrWindow.h"
#include "..\..\Common\TYRect.h"
#include "WindowSelect.h"

using namespace std;

class CDocumentData;

//�Ŵ���ŵȹ�����,ͨ���Ŵ����ұ�����֣�����ͨ��������ֲ����Ŵ�
class CInstanceCodeTextMrg
{
	friend CDocumentData;
	CInstanceCodeTextMrg();
public:
	~CInstanceCodeTextMrg();

	void AddInstanceCode(const CSunacObjInCad &p_sunacObj, AcDbObjectId p_textId);

	void RemoveInstanceCode(const CSunacObjInCad & p_sunacObj);
	void RemoveInstanceCodeByWinId(AcDbObjectId p_winId);
	void RemoveInstanceCodeText(AcDbObjectId p_textId);
	void RemoveInvalidInstanceCode();  //�Ƴ���Ч���Ŵ���ţ���Ч���Ŵ����Ϊ�Ŵ����屻ɾ���ı��
	void RemoveAll();

	AcDbObjectId FindTextId(const CSunacObjInCad &p_sunacObj);
	bool SetNewInstanceCode(const CSunacObjInCad &p_sunacObj, CString p_newTextCode);//���±��ֵ

	bool IsTextIdIn(AcDbObjectId p_textId);

	//�õ���ǰͼֽ��Χ�ڵ����е��Ŵ�������ֵ�id
	static vector<AcDbObjectId> GetAllInstanceCodeIds();
	static vector<AcDbObjectId> GetInstanceCodeIdsInRect(const TYRect p_rect);

	
	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);

protected:
	map<CSunacObjInCad, AcDbObjectId> m_instanceMap;
};