#pragma once
#include <vector>
#include "AttrBathroom.h"

using namespace std;


class CDocumentData;
class CBathroomAutoName
{
	friend CDocumentData;
	CBathroomAutoName();
public:
	~CBathroomAutoName();

	//�Զ���ȡ����
	CString GetBathroomName(const AttrBathroom& p_att) const;

	//�����������б���
	void AddBathroomType(const AttrBathroom& p_att);

	//��鵱ǰ�����Ƿ��������ͬ���������������򷵻�false
	bool IsNameValid(const AttrBathroom& p_att, CString p_sName) const;

	//�����������������Զ�����
	void AutoNameAllBathroom();

	//��ָ��������������
	bool RenameBathroom(const AttrBathroom& p_att);

	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);

protected:
	vector<AttrBathroom> m_allTypeBathrooms; //����������

};
