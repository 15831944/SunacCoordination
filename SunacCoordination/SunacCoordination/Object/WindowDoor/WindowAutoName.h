#pragma once
#include <vector>
#include "AttrWindow.h"

using namespace std;


class CWindowAutoName
{
	CWindowAutoName();
public:
	~CWindowAutoName();

	static CWindowAutoName* GetInstance();

	//�Զ���ȡ����
	CString GetWindowName(const AttrWindow& p_att) const;

	//���봰���б���
	void AddWindowType(const AttrWindow& p_att);

	//��鵱ǰ�����Ƿ��������ͬ�������������򷵻�false
	bool IsNameValid(const AttrWindow& p_att, CString p_sName) const;

	//�����д��������Զ�����
	void AutoNameAllWindow();
	void AutoNameWindows(const vector<AcDbObjectId>& p_ids);

	//��ָ������������
	bool RenameWindow(const AttrWindow& p_att);

	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);

protected:
	vector<AttrWindow> m_allTypeWindows; //���д���

};
