#include "StdAfx.h"
#include "KitchenBathroomStatistic.h"
#include "../../Common/ComFun_Interactive.h"
#include "../../Common/ComFun_Sunac.h"
#include "../../Common/ComFun_ACad.h"
#include <dbtable.h>
#include <algorithm>

int CKitchenBathroomStatistic::SelectKitchenBathroom()
{
	m_allItems.clear();

	vAcDbObjectId ids;
	JHCOM_SelectEnts(ids);

	int count = 0;
	for (UINT i = 0; i < ids.size(); i++)
	{
		AcDbObject* pAttr = NULL;
		TY_GetAttributeData(ids[i], pAttr);
		AttrKitchen* pAttrKitchen = AttrKitchen::cast(pAttr);
		AttrBathroom* pAttrBathroom = AttrBathroom::cast(pAttr);
		if (pAttrKitchen != NULL)
		{
			count++;
			InsertKitchen(pAttrKitchen);
		}
		else if (pAttrBathroom != NULL)
		{
			count++;
			InsertBathroom(pAttrBathroom);
		}
	}

	return count;
}

void CKitchenBathroomStatistic::InsertKitchen(AttrKitchen* pAttr)
{
	KitchenBathroomItem item;
	item.amount = 1;
	if (!pAttr->m_shuiPenType.IsEmpty())
	{
		item.name = L"ˮ��";
		item.type = pAttr->m_shuiPenType;
		InsertKBItem(item);
	}
	if (!pAttr->m_bingXiangType.IsEmpty())
	{
		item.name = L"����";
		item.type = pAttr->m_bingXiangType;
		InsertKBItem(item);
	}
	if (!pAttr->m_shuiPenType.IsEmpty())
	{
		item.name = L"��̨";
		item.type = pAttr->m_zaoTaiType;
		InsertKBItem(item);
	}
}

void CKitchenBathroomStatistic::InsertBathroom(AttrBathroom* pAttr)
{
	KitchenBathroomItem item;
	item.amount = 1;
	if (!pAttr->m_taipenWidth.IsEmpty())
	{
		item.name = L"̨��";
		item.type = pAttr->m_taipenWidth;
		InsertKBItem(item);
	}
	if (!pAttr->m_matongWidth.IsEmpty())
	{
		item.name = L"��Ͱ";
		item.type = pAttr->m_matongWidth;
		InsertKBItem(item);
	}
}

void CKitchenBathroomStatistic::InsertKBItem(const KitchenBathroomItem& pItem)
{
	int pos = FindKBItem(pItem);
	if (pos == -1)
		m_allItems.push_back(pItem);
	else
		m_allItems[pos].amount++;
}

int CKitchenBathroomStatistic::FindKBItem(const KitchenBathroomItem& pItem)
{
	for (UINT i = 0; i < m_allItems.size(); i++)
	{
		if ((m_allItems[i].name == pItem.name) && (m_allItems[i].type == pItem.type))
			return i;
	}
	return -1;
}

AcDbObjectId CKitchenBathroomStatistic::InsertTableToCAD(AcGePoint3d insertPos)
{
	const double c_tableCellWidth[] = {700, 700, 1500, 700, 2000};
	const double c_tableCellHeight = 300;

	sort(m_allItems.begin(), m_allItems.end(), KBItemCmp);

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

	int rowSize = int(m_allItems.size()) + 2;
	int columSize = 5;
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
			pTable->setTextHeight(nRow, nColum, 150);
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
	pTable->setTextString(0, 0, L"����������");
	pTable->setTextString(1, 0, L"���");
	pTable->setTextString(1, 1, L"����");
	pTable->setTextString(1, 2, L"���");
	pTable->setTextString(1, 3, L"����");
	pTable->setTextString(1, 4, L"��ע");
	for (UINT i = 0; i < m_allItems.size(); i++)
	{
		CString sNum;
		sNum.Format(L"%d", i + 1);
		pTable->setTextString(i + 2, 0, sNum);
		pTable->setTextString(i + 2, 1, m_allItems[i].name);
		pTable->setTextString(i + 2, 2, m_allItems[i].type);
		sNum.Format(L"%d", m_allItems[i].amount);
		pTable->setTextString(i + 2, 3, sNum);
	}

	pTable->setPosition(insertPos);

	pTable->setRegen();

	return JHCOM_PostToModelSpace(pTable);
}

bool KBItemCmp(const KitchenBathroomItem& x1, const KitchenBathroomItem& x2)
{
	CString keys(L"ˮ�������̨̨����Ͱ");
	int pos1 = keys.Find(x1.name);
	int pos2 = keys.Find(x2.name);
	if (pos1 == pos2)
	{
		int posNum1 = x1.type.FindOneOf(L"0123456789");
		int posNum2 = x2.type.FindOneOf(L"0123456789");
		int num1 = _ttoi(x1.type.Mid(posNum1));
		int num2 = _ttoi(x2.type.Mid(posNum2));
		return (num1 < num2);
	}
	return (pos1 < pos2);
}
