#pragma once
#include <vector>
#include "AttrKitchen.h"

using namespace std;


class CKitchenAutoName
{
public:
	CKitchenAutoName();
	~CKitchenAutoName();

	static CKitchenAutoName* GetInstance();

	//�Զ���ȡ���֣������뵽��ǰ���Ϳ���
	CString GetKitchenName(const AttrKitchen& p_att) const; 

	void AddKitchenType(const AttrKitchen& p_att);

	//�����û������������,��鵱ǰ�����Ƿ������������뵽���Ϳ��У����򷵻�false
	bool IsUserNameValid(const AttrKitchen& p_att, CString p_sName);
	
	void AutoNameAllKitchen(); //������ǰͼֽ�е����еĴ�������������Զ�����
	void ReNameKitchen(const AttrKitchen& p_att);	//��ͼֽ�е�ǰԭ����ͬ�Ĵ�����������


protected:
	vector<AttrKitchen> m_allTypeKitchens; //���д���

};
