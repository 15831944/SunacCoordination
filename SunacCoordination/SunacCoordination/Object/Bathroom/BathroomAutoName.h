#pragma once
#include <vector>
#include "AttrBathroom.h"

using namespace std;


class CBathroomAutoName
{
public:
	CBathroomAutoName();
	~CBathroomAutoName();

	static CBathroomAutoName* GetInstance();

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


protected:
	vector<AttrBathroom> m_allTypeBathrooms; //����������

};
