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

	//�Զ���ȡ���֣������뵽��ǰ���Ϳ���
	CString GetBathroomName(const AttrBathroom& p_att) const; 

	void AddBathroomType(const AttrBathroom& p_att);

	//�����û������������,��鵱ǰ�����Ƿ������������뵽���Ϳ��У����򷵻�false
	bool IsUserNameValid(const AttrBathroom& p_att, CString p_sName);
	
	void AutoNameAllBathroom(); //������ǰͼֽ�е����еĴ�������������Զ�����
	void ReNameBathroom(const AttrBathroom& p_att);	//��ͼֽ�е�ǰԭ����ͬ�Ĵ�����������


protected:
	vector<AttrBathroom> m_allTypeBathrooms; //���д���

};
