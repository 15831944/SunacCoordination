#include "stdafx.h"

#include "ComFun_Def.h"
#include "ComFun_Str.h"
#include "ComFun_Daqo.h"

int MD2010_SetCurrentLayout(const ACHAR * layoutname)
{
	AcApLayoutManager *pAcApLayoutManager = (AcApLayoutManager *)(acdbHostApplicationServices()->layoutManager());
	AcDbLayout * pLayout = NULL;
	const WCHAR *pcur = pAcApLayoutManager->findActiveLayout(true);
	if (wcscmp(pcur, layoutname) == 0)
	{
		return 0;
	}
	pLayout = pAcApLayoutManager->findLayoutNamed(layoutname);
	if (NULL == pLayout)
		return 1;
	pAcApLayoutManager->setCurrentLayout(layoutname);
	pAcApLayoutManager->updateCurrentPaper(Adesk::kTrue);
	pAcApLayoutManager->updateLayoutTabs();
	pLayout->close();
	return 0;
}




AcDbObjectId MD2010_PostModalToBlockTable(const ACHAR* entryName, AcDbEntity *pent)
{
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(entryName, pBlockTableRecord, AcDb::kForWrite);

	//������ת��
	if (pBlockTableRecord == NULL && wcscmp(entryName, L"*Model_Space") == 0)
	{
		pBlockTable->getAt(L"ģ��", pBlockTableRecord, AcDb::kForWrite);
	}
	if (pBlockTableRecord == NULL && wcscmp(entryName, L"ģ��") == 0)
	{
		pBlockTable->getAt(L"*Model_Space", pBlockTableRecord, AcDb::kForWrite);
	}

	if (pBlockTableRecord == NULL)
	{
		pBlockTable->close();
		return 0;
	}

	AcDbObjectId entID;
	Acad::ErrorStatus es = pBlockTableRecord->appendAcDbEntity(entID, pent);

	pBlockTable->close();
	pBlockTableRecord->close();

	return entID;
}



AcDbObjectId MD2010_InsertBlockDefineFromPathName(const WCHAR *pathname, CString blockName)
{
	acDocManager->lockDocument(curDoc());
	WCHAR blockname[256] = L"";
	CF_STR_get_file_name(pathname, blockname);
	CF_STR_get_file_name_2(blockname, blockname);
	if (blockName.IsEmpty())
		blockName = blockname;

	//Ѱ���Ƿ��Ѿ�����
	AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable *pBlkTable;
	AcDbObjectId blockId;
	pCurDb->getBlockTable(pBlkTable, AcDb::kForRead);
	if (!pBlkTable->has(blockName))
	{
		pBlkTable->close();

		AcDbDatabase *pDwg = new AcDbDatabase(Adesk::kFalse);
		Acad::ErrorStatus es = pDwg->readDwgFile(pathname);

		es = pCurDb->insert(blockId, blockName, pDwg);
		delete pDwg;

		if (es != Acad::eOk)
		{
			acutPrintf(L"\n��������.");
			acDocManager->unlockDocument(curDoc());
			return 0;
		}
	}
	else
		pBlkTable->getAt(blockName, blockId);

	pBlkTable->close();

	acDocManager->unlockDocument(curDoc());

	return blockId;
}
int MD2010_InsertBlockFromPathName(const WCHAR *layoutname, const WCHAR *pathname, CString &blockDefineName, AcDbObjectId &entId, AcGePoint3d origin, double angle, AcGeScale3d scale, int color)
{
	if (blockDefineName.IsEmpty())
	{
		WCHAR blockname[256] = L"";
		CF_STR_get_file_name(pathname, blockname);
		CF_STR_get_file_name_2(blockname, blockname);
		blockDefineName = blockname;
	}

	MD2010_InsertBlockDefineFromPathName(pathname, blockDefineName);
	MD2010_InsertBlockReference_Layout(layoutname, blockDefineName, entId, origin, angle, scale, color);

	return 0;
}

int MD2010_InsertBlockReference_Layout(const WCHAR * layoutname, const WCHAR * blockname, AcDbObjectId &entId, AcGePoint3d origin, double angle, AcGeScale3d scale, int color)
{
	AcDbBlockTable *pBlkTbl;

	if (MD2010_SetCurrentLayout(layoutname) == 1)
	{
		if (wcscmp(layoutname, ACDB_MODEL_SPACE) == 0)
		{
			if (MD2010_SetCurrentLayout(L"ģ��") == 1)
				return 1;
		}
	}


	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	if (!pBlkTbl->has(blockname))
	{
		acutPrintf(L"\n no %s", blockname);
		pBlkTbl->close();
		return 1;
	}

	Acad::ErrorStatus es;
	// ����û�ָ���Ŀ���¼
	AcDbObjectId blkDefId;
	es = pBlkTbl->getAt(blockname, blkDefId);

	// ��������ն���
	AcDbBlockReference *pBlkRef = new AcDbBlockReference(origin, blkDefId);
	// ���������ӵ�ģ�Ϳռ�
	AcDbBlockTableRecord *pBlkTblRcd;
	if (wcscmp(layoutname, L"ģ��") != 0 && wcscmp(layoutname, ACDB_MODEL_SPACE) != 0)
	{
		es = pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd, AcDb::kForWrite);
	}
	else
	{
		es = pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForWrite);
	}

	AcDbBlockTableRecord *pBlkDefRcd = NULL;
	if (acdbOpenObject(pBlkDefRcd, pBlkRef->blockTableRecord(), AcDb::kForRead) == Acad::eOk)
	{
		if (pBlkDefRcd->hasAttributeDefinitions())
		{
			// ��������¼�����������ڷ��ʿ鶨���е�����ʵ��
			AcDbBlockTableRecordIterator *pItr = NULL;
			pBlkDefRcd->newIterator(pItr);

			for (pItr->start(); !pItr->done(); pItr->step())
			{
				AcDbEntity *pEnt = NULL;
				if (pItr->getEntity(pEnt, AcDb::kForRead) == Acad::eOk)
				{
					// ��������Զ��壬���������������
					AcDbAttributeDefinition *pAttDef = AcDbAttributeDefinition::cast(pEnt);
					if (pAttDef != NULL)
					{
						AppendAttributeToBlkRef(pBlkRef, pAttDef);
					}
					pEnt->close();
				}
			}
			delete pItr;
		}

		pBlkDefRcd->close();
	}

	//AcDbObjectId entId;
	es = pBlkTblRcd->appendAcDbEntity(entId, pBlkRef);

	//�޸Ĳ���
	es = pBlkRef->setScaleFactors(scale);
	es = pBlkRef->setRotation(angle);

	AcCmColor cmc;
	if (color == -2)
	{
		cmc.setColorMethod(AcCmEntityColor::kByBlock);
		pBlkRef->setColor(cmc);
	}
	else if (color == -1)
	{
		cmc.setColorMethod(AcCmEntityColor::kByLayer);
		pBlkRef->setColor(cmc);
	}
	else
	{
		pBlkRef->setColorIndex(color);
	}

	// �ر����ݿ�Ķ���
	es = pBlkRef->close();
	es = pBlkTblRcd->close();
	es = pBlkTbl->close();

	return 0;
}




void AppendAttributeToBlkRef(AcDbBlockReference * pBlkRef, AcDbAttributeDefinition * pAttDef)
{
	// ����һ���µ����Զ���
	AcDbAttribute *pAtt = new AcDbAttribute();
	// �����Զ��������Զ���Ķ�������
	pAtt->setPropertiesFrom(pBlkRef);
	pAtt->setLayer(pBlkRef->layerId());
	// �������Զ������������
	pAtt->setInvisible(pAttDef->isInvisible());
	pAtt->setPosition(pAttDef->position());
	pAtt->setHeight(pAttDef->height());
	pAtt->setWidthFactor(pAttDef->widthFactor());
	pAtt->setRotation(pAttDef->rotation());
	pAtt->setHorizontalMode(pAttDef->horizontalMode());
	pAtt->setVerticalMode(pAttDef->verticalMode());
	pAtt->setAlignmentPoint(pAttDef->alignmentPoint());
	pAtt->setTextStyle(pAttDef->textStyle());
	pAtt->setAttributeFromBlock(pBlkRef->blockTransform());

	// ������Զ����Tag��Prompt��TextString
	TCHAR *pStr = pAttDef->tag();
	pAtt->setTag(pStr);
	acutDelString(pStr);
	pAtt->setFieldLength(pAttDef->fieldLength());
	pAtt->setTextString(pAttDef->textString());

	// ������׷�����Զ���
	pBlkRef->appendAttribute(pAtt);
	pAtt->close();
}
