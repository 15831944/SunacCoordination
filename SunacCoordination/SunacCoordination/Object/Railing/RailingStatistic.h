#pragma once

#include "AttrRailing.h"

using namespace std;

class CRailingStatistic
{
public:
	int SelectRailings();
	void InsertRailing(AttrRailing* pAttr);

	AcDbObjectId InsertTableToCAD(AcGePoint3d insertPos);

	vector<pair<AttrRailing, int>> AllRailings(){ return m_allRailings; }

protected:
	vector<pair<AttrRailing, int>> m_allRailings;
};

//���ڲ�����ʱ����
bool RailingCmp(const pair<AttrRailing, int>& x1, const pair<AttrRailing, int>& x2);