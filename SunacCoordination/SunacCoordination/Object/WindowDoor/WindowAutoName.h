#pragma once
#include <vector>
#include "AttrWindow.h"

using namespace std;


class CWindowAutoName
{
public:
	CWindowAutoName();
	~CWindowAutoName();

	//�Զ���ȡ���֣������뵽��ǰ���Ϳ���
	CString GetWinodwName(const AttrWindow& p_att); 

	//�����û������������,��鵱ǰ�����Ƿ������������뵽���Ϳ��У����򷵻�false
	bool AddWindowName(const AttrWindow& p_att, CString p_sName);
	
	void AutoNameAllWindow(); //������ǰͼֽ�е����еĴ�������������Զ�����
	void ReNameWindow(const AttrWindow& p_att);	//��ͼֽ�е�ǰԭ����ͬ�Ĵ�����������


protected:
	vector<AttrWindow> m_allTypeWindows; //���д���

};
