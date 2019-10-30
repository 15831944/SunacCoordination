#pragma once
#include <vector>
#include "AttrKitchen.h"

using namespace std;


class CKitchenAutoName
{
	CKitchenAutoName();
public:
	~CKitchenAutoName();

	static CKitchenAutoName* GetInstance();

	//�Զ���ȡ����
	CString GetKitchenName(const AttrKitchen& p_att) const;

	//��������б���
	void AddKitchenType(const AttrKitchen& p_att);

	//��鵱ǰ�����Ƿ��������ͬ�������������򷵻�false
	bool IsNameValid(const AttrKitchen& p_att, CString p_sName) const;

	//�����г��������Զ�����
	void AutoNameAllKitchen();

	//��ָ������������
	bool RenameKitchen(const AttrKitchen& p_att);


protected:
	vector<AttrKitchen> m_allTypeKitchens; //���г���

};
