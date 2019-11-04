#include "StdAfx.h"
#include "AirConStatistic.h"
#include "../../Common/ComFun_Interactive.h"
#include "../../Common/ComFun_Sunac.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Math.h"
#include <dbtable.h>
#include <algorithm>

int CAirConStatistic::SelectAirCons()
{
	m_allAirCons.clear();

	vAcDbObjectId ids;
	JHCOM_SelectEnts(ids);

	int count = 0;
	for (UINT i = 0; i < ids.size(); i++)
	{
		AcDbObject* pAttr = NULL;
		TY_GetAttributeData(ids[i], pAttr);
		AttrAirCon* pAttrAirCon = AttrAirCon::cast(pAttr);
		if (pAttrAirCon == NULL)
			continue;
		count++;
		InsertAirCon(pAttrAirCon);
	}

	return count;
}

void CAirConStatistic::InsertAirCon(AttrAirCon* pAttr)
{
	for (UINT i = 0; i < m_allAirCons.size(); i++)
	{
		if (JHCOM_equ(m_allAirCons[i].first, pAttr->m_power))
		{
			m_allAirCons[i].second++;
			return;
		}
	}
	m_allAirCons.push_back(make_pair(pAttr->m_power, 1));
}

AcDbObjectId CAirConStatistic::InsertTableToCAD(AcGePoint3d insertPos)
{
	const double c_tableCellWidth[] = {20, 20, 20, 50};
	const double c_tableCellHeight = 6;

	sort(m_allAirCons.begin(), m_allAirCons.end());

	Acad::ErrorStatus es;
	AcDbDictionary *pDict = NULL;
	acdbHostApplicationServices()->workingDatabase()->getTableStyleDictionary(pDict, AcDb::kForRead);
	AcDbObjectId idTblStyle;
	es = pDict->getAt(_T("Standard"), idTblStyle);
	pDict->close();

	AcDbTable* pTable = new AcDbTable;
	pTable->setTableStyle(idTblStyle);

	pTable->suppressTitleRow(false);//������Ҫ����
	pTable->suppressHeaderRow(true); //��ͷ����Ҫ

	int rowSize = int(m_allAirCons.size()) + 2;
	int columSize = 4;
	pTable->setNumColumns(columSize);
	pTable->setNumRows(rowSize);

	//�����и�
	pTable->setVertCellMargin(1);
	for (int i = 0; i < rowSize; i++)
	{
		pTable->setRowHeight(i, c_tableCellHeight);
	}

	//�����п�
	for (int i = 0; i < columSize; i++)
	{
		pTable->setColumnWidth(i, c_tableCellWidth[i]);
	}

	for (int nRow = 0; nRow < rowSize; nRow++)
	{
		for (int nColum = 0; nColum < columSize; nColum++)
		{
			pTable->setAlignment(nRow, nColum, AcDb::kMiddleCenter);
		}
	}

	// ����������ʽ
	AcDbObjectId stylerid = MD2010_GetTextStyler(_T("CCFS"));
	if (stylerid != AcDbObjectId::kNull)
	{
		pTable->setTextStyle(stylerid);
	}

	pTable->generateLayout();

	//���ñ���
	pTable->setTextString(0, 0, L"�յ�������");
	pTable->setTextString(1, 0, L"���");
	pTable->setTextString(1, 1, L"ƥ��");
	pTable->setTextString(1, 2, L"����");
	pTable->setTextString(1, 3, L"��ע");
	for (UINT i = 0; i < m_allAirCons.size(); i++)
	{
		CString sNum;
		sNum.Format(L"%d", i + 1);
		pTable->setTextString(i + 2, 0, sNum);
		sNum.Format(L"%.1lf", m_allAirCons[i].first);
		pTable->setTextString(i + 2, 1, sNum);
		sNum.Format(L"%d", m_allAirCons[i].second);
		pTable->setTextString(i + 2, 2, sNum);
	}

	pTable->setPosition(insertPos);

	pTable->setRegen();

	return JHCOM_PostToModelSpace(pTable);
}
