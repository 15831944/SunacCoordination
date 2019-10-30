#pragma once

#include "AttrRailing.h"

class CRailingStatistic
{
public:
	int SelectRailings();
	void InsertRailing(AttrRailing* pAttr);

	AcDbObjectId InsertTableToCAD(AcGePoint3d insertPos);

protected:
	vector<AttrRailing> m_allRailings;
	vector<int> m_amount;
};

//���ڲ�����ʱ����
bool RailingCmp(const AttrRailing& x1, const AttrRailing& x2);