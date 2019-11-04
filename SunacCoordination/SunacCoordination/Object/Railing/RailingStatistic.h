#pragma once

#include "AttrRailing.h"

using namespace std;

class CRailingStatistic
{
public:
	int SelectRailings();
	void InsertRailing(AttrRailing* pAttr);

	AcDbObjectId InsertTableToCAD(AcGePoint3d insertPos);

protected:
	vector<pair<AttrRailing, int>> m_allRailings;
};

//���ڲ�����ʱ����
bool RailingCmp(const pair<AttrRailing, int>& x1, const pair<AttrRailing, int>& x2);