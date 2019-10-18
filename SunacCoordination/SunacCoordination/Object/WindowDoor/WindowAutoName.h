#pragma once
#include <vector>
#include "AttrWindow.h"

using namespace std;


class CWindowAutoName
{
public:
	CWindowAutoName();
	~CWindowAutoName();

	static CWindowAutoName* GetInstance();

	//�Զ���ȡ���֣������뵽��ǰ���Ϳ���
	CString GetWindowName(const AttrWindow& p_att) const; 

	void AddWindowType(const AttrWindow& p_att);

	//�����û������������,��鵱ǰ�����Ƿ������������뵽���Ϳ��У����򷵻�false
	bool IsUserNameValid(const AttrWindow& p_att, CString p_sName);
	
	void AutoNameAllWindow(); //������ǰͼֽ�е����еĴ�������������Զ�����
	void ReNameWindow(const AttrWindow& p_att);	//��ͼֽ�е�ǰԭ����ͬ�Ĵ�����������


protected:
	vector<AttrWindow> m_allTypeWindows; //���д���

};
