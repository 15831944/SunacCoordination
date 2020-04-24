#include "StdAfx.h"
#include <algorithm>
#include "RailingStatistic.h"
#include "../../Tool/DocLock.h"
#include "../../Common/ComFun_Interactive.h"
#include "../../Common/ComFun_Sunac.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_String.h"
#include "../../Command/CommandHighlight.h"
#include "..\WindowDoor\WindowSelect.h"

//���ڲ�����ʱ����
static bool RailingCmp(const pair<AttrRailing, int>& x1, const pair<AttrRailing, int>& x2)
{
	return (x1.first.m_instanceCode < x2.first.m_instanceCode);
}

int CRailingStatistic::SelectRailings()
{
	m_allRailings.clear();

	vAcDbObjectId ids;
	JHCOM_SelectEnts(ids);

	int count = 0;
	for (UINT i = 0; i < ids.size(); i++)
	{
		AcDbObject* pAttr = NULL;
		TY_GetAttributeData(ids[i], pAttr);
		if (pAttr==NULL)
			continue;
		
		AttrRailing* pAttrRailing = AttrRailing::cast(pAttr);
		if (pAttrRailing == NULL)
		{
			pAttr->close();
			continue;
		}

		RailingClassify(pAttrRailing);

		pAttr->close();
		count++;
	}

	return count;
}

void CRailingStatistic::RailingClassify(AttrRailing* pAttr) //���˰���ŷ���
{
	int nCurRailingCount = 1;
	if (pAttr->GetViewDir() == E_VIEW_TOP)
	{
		nCurRailingCount = pAttr->m_floorInfo.GetFloorCount();
	}

	for (UINT i = 0; i < m_allRailings.size(); i++)
	{
		if (m_allRailings[i].first.m_instanceCode == pAttr->m_instanceCode)
		{
			m_allRailings[i].second += nCurRailingCount;
			return;
		}
	}

	//δ�ҵ���������µ�
	m_allRailings.push_back(make_pair(*pAttr, nCurRailingCount));
}

AcDbObjectId CRailingStatistic::InsertTableToCAD(AcGePoint3d insertPos)
{
	const double c_tableCellWidth[] = {500, 2500, 1000, 1000, 1500};
	const double c_tableCellHeight = 400;

	sort(m_allRailings.begin(), m_allRailings.end(), RailingCmp);

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

	int rowSize = int(m_allRailings.size()) + 2;
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
			pTable->setTextHeight(nRow, nColum, 120);
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
	pTable->setTextString(0, 0, L"���˱�");
	pTable->setTextString(1, 0, L"���");
	pTable->setTextString(1, 1, L"���˱��");
	pTable->setTextString(1, 2, L"����");
	pTable->setTextString(1, 3, L"����");
	pTable->setTextString(1, 4, L"��ע");
	for (UINT i = 0; i < m_allRailings.size(); i++)
	{
		CString sNum;
		sNum.Format(L"%d", i + 1);
		pTable->setTextString(i + 2, 0, sNum);
		pTable->setTextString(i + 2, 1, m_allRailings[i].first.m_instanceCode);
		sNum.Format(L"%.0lf", m_allRailings[i].first.m_length);
		pTable->setTextString(i + 2, 2, sNum);
		sNum.Format(L"%d", m_allRailings[i].second);
		pTable->setTextString(i + 2, 3, sNum);
	}

	pTable->setPosition(insertPos);

	pTable->setRegen();

	return JHCOM_PostToModelSpace(pTable);
}

void CRailingStatistic::InsertRailingTableToCAD()
{
	CDocLock lock;

	CCommandHighlight::GetInstance()->SunacNoHighlight();

	//1��ѡ����Ҫͳ�Ƶ�����
	const vector<CSunacObjInCad> railings = CSunacSelect::SelectSunacObjs(S_RAILING, E_VIEW_TOP);
	vAcDbObjectId ringIds;
	for (UINT i = 0; i < railings.size(); i++)
	{
		ringIds.push_back(railings[i].m_winId);
	}
	if (ringIds.size() == 0)
		return;

	//2��ȷ�������
	AcGePoint3d pnt;
	bool bSuc = TY_GetPoint(pnt, L"��ѡ�����˱�����");
	if (bSuc == false)
		return;

	//3����ȡ�������ݲ�ȷ���Ƿ�����¥��
	vAcDbObjectId idsNoFloorInfo; //û������¥�������

	for (UINT i = 0; i < ringIds.size(); i++)
	{
		AcDbObject * pDataEnt = 0;
		TY_GetAttributeData(ringIds[i], pDataEnt);
		if (pDataEnt == NULL)
			continue;

		AttrRailing * pRailing = dynamic_cast<AttrRailing *>(pDataEnt);
		if (pRailing == NULL)
		{
			pDataEnt->close();
			continue;
		}

		if (pRailing->GetFloorInfo().GetAllFloor().size() == 0)
		{
			idsNoFloorInfo.push_back(ringIds[i]);
		}

		pRailing->close();
	}

	if (idsNoFloorInfo.size() > 0)
	{
		AfxMessageBox(_T("��������δ����¥��Ͳ��"));
		CCommandHighlight::GetInstance()->SunacHighlight(idsNoFloorInfo);
		return;
	}

	CRailingCountArray railingCountArray;
	railingCountArray.InitByRailingIds(ringIds);

	//4������¥����Ϣȷ�����˱�����
	std::vector<CString> floorColumns;
	for (int i = 0; i < railingCountArray.GetCount(); i++)
	{
		std::vector<CString> floorsTemp = YT_SplitCString(railingCountArray.GetRailing(i).railAtt.GetFloorInfo().GetFloors(), L',');
		for (UINT j = 0; j < floorsTemp.size(); j++)
		{
			bool bFind = false;
			for (UINT n = 0; n < floorColumns.size(); n++)
			{
				if (floorColumns[n].CompareNoCase(floorsTemp[j]) == 0)
				{
					bFind = true;
					break;
				}
			}

			if (bFind == false)
			{
				floorColumns.push_back(floorsTemp[j]);
			}
		}
	}

	sort(floorColumns.begin(), floorColumns.end(), CFloorInfo::FloorLessCmp);

	const int floorColumnCount = (int)floorColumns.size();

	//5����ʼ������ݣ���д�����Ϣ
	AcDbTable *table = new AcDbTable();
	table->setPosition(pnt);
	table->setAlignment(AcDb::kMiddleCenter);

	//5.1 ������������, ˵����2 �Ǳ���������, 7�ǳ�ȥ����¥����Ϣ��֮�������
	int numRailing = (int)railingCountArray.GetCount();
	int allRowNum = 2 + numRailing;
	table->setNumRows(allRowNum);
	table->setNumColumns(7 + floorColumnCount);
	table->setColumnWidth(1000);
	table->setRowHeight(300);
	//�������д���
	table->setColumnWidth(0, 700);
	table->setColumnWidth(1, 1500);
	table->setRowHeight(1, 500);
	//��������
	table->setTextHeight(0, 0, 100);
	for (int i = 1; i < allRowNum; i++)
	{
		for (int j = 0; j < 7 + floorColumnCount; j++)
		{
			table->setTextHeight(i, j, 100);
			table->setAlignment(i, j, AcDb::kMiddleCenter);
		}
	}
	//�ϲ���Ԫ��
	table->mergeCells(0, 0, 0, 6 + floorColumnCount);

	//5.2 ������д����
	//д�����
	Acad::ErrorStatus es;
	es = table->setTextString(0, 0, L"���˱�");//0,0��ͷ
	table->setTextString(1, 0, L"����");
	table->setTextString(1, 1, L"��Ʊ��");
	table->setTextString(1, 2, L"���˸߶�(mm)");
	table->setTextString(1, 3, L"���˸߶�(mm)");
	for (int i = 0; i < floorColumnCount; i++)
	{
		table->setTextString(1, 4 + i, floorColumns[i] + L"F");
	}
	table->setTextString(1, 4 + floorColumnCount, L"�ϼ�");
	table->setTextString(1, 5 + floorColumnCount, L"ѡ��ͼ��");
	table->setTextString(1, 6 + floorColumnCount, L"��ע");
	table->mergeCells(2, allRowNum - 1, 6 + floorColumnCount, 6 + floorColumnCount);

	//�ϲ��У�д��ѡ��ͼ��
	table->mergeCells(2, allRowNum - 1, 5 + floorColumnCount, 5 + floorColumnCount);
	table->setTextString(2, 5 + floorColumnCount, L"���Ŵ���ͼ");

	//�������Ͳ������
	m_blRailings.clear();
	m_tyRailings.clear();
	for (int i = 0; i < numRailing; i++)
	{
		const CRailingAndCount& railingAndCount = railingCountArray.GetRailing(i);
		SpliteRailingByType(railingAndCount);
	}

	//����ʼ�к���Ϊ2�������к���Ϊ1
	int dataStartRow = 2;
	int dataEndRow = 1;

	//�������
	int nRailingNumTY = (int)m_tyRailings.size();
	if (nRailingNumTY > 0)
	{
		dataEndRow = dataStartRow + nRailingNumTY - 1;
		table->mergeCells(dataStartRow, dataEndRow, 0, 0);
		table->setTextString(dataStartRow, 0, L"��������");
		for (int i = 0; i < nRailingNumTY; i++)
		{
			const CRailingAndCount& railingAndCount = m_tyRailings[i];
			WriteDataToRailingTable(table, dataStartRow + i, floorColumnCount, floorColumns, railingAndCount);
		}
	}
	
	//�������
	int nRailingNumBL = (int)m_blRailings.size();
	if (nRailingNumBL > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + nRailingNumBL - 1;
		table->mergeCells(dataStartRow, dataEndRow, 0, 0);
		table->setTextString(dataStartRow, 0, L"��������");
		for (int i = 0; i < nRailingNumBL; i++)
		{
			const CRailingAndCount& railingAndCount = m_blRailings[i];
			WriteDataToRailingTable(table, dataStartRow + i, floorColumnCount, floorColumns, railingAndCount);
		}
	}

	//��ѡ������˸���
	CCommandHighlight::GetInstance()->SunacHighlight(ringIds);

	AcDbObjectId tableId = JHCOM_PostToModelSpace(table);

	return;
}

void CRailingStatistic::WriteDataToRailingTable(AcDbTable *p_table, int p_dataStartRow, int p_floorColumnCount, vector<CString> p_floorColumns, const CRailingAndCount& p_railingAndCount)
{
	CString str, str1;
	const AttrRailing * pRailingAttr = &(p_railingAndCount.railAtt);

	//��Ʊ��
	p_table->setTextString(p_dataStartRow, 1, pRailingAttr->GetInstanceCode());

	//���˸߶�
	str.Format(L"%d", (int)pRailingAttr->m_height);
	p_table->setTextString(p_dataStartRow, 2, str);

	//���˳���
	str.Format(L"%d", (int)pRailingAttr->m_length);
	p_table->setTextString(p_dataStartRow, 3, str);

	//¥����
	vector<int> nAllFloorCount; //���ںϼ�
	nAllFloorCount.clear();

	for (int i = 0; i < p_floorColumnCount; i++)
	{
		// ��p_floorColumns��"-"��֣�ȷ���Ƿ���¥������
		int nPos = p_floorColumns[i].Find(_T('-'));
		if (nPos >= 0)
		{
			CString str1 = p_floorColumns[i].Left(nPos);
			CString str2 = p_floorColumns[i].Mid(nPos + 1);
			int nStart = _ttoi(str1);
			int nEnd = _ttoi(str2);
			if (nStart > nEnd || nStart == 0)
			{
				AfxMessageBox(_T("����¥����Ϣ��������"));
				return;
			}

			int nFloorCount = nEnd - nStart + 1;

			int numRailing = p_railingAndCount.nCount;
			str.Format(L"%d*%d=%d", numRailing, nFloorCount, numRailing * nFloorCount);
			p_table->setTextString(p_dataStartRow, 4 + i, str);
			nAllFloorCount.push_back(numRailing * nFloorCount);
		}
		else
		{
			int nFloorCount = _ttoi(p_floorColumns[i]);
	
			nAllFloorCount.push_back(p_railingAndCount.nCount);
			str.Format(L"%d", nAllFloorCount[i]);
			p_table->setTextString(p_dataStartRow, 4 + i, str);
		}
	}

	//�ϼ�
	int allFloorCount = 0;
	for (int i = 0; i < nAllFloorCount.size(); i++)
	{
		allFloorCount += nAllFloorCount[i];
	}

	str.Format(L"%d", allFloorCount);
	p_table->setTextString(p_dataStartRow, 4 + p_floorColumnCount, str);

	//��ע
}

void CRailingStatistic::SpliteRailingByType(const CRailingAndCount& railingAndCount)
{
	const AttrRailing * pRailingAtt = &(railingAndCount.railAtt);
	if (pRailingAtt->m_railingType == E_RAILING_TIEYI)
		m_tyRailings.push_back(railingAndCount);

	if (pRailingAtt->m_railingType == E_RAILING_BOLI)
		m_blRailings.push_back(railingAndCount);
}