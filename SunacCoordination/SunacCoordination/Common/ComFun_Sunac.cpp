#include "stdafx.h"
#include "ComFun_Str.h"
#include "ComFun_Math.h"
#include "ComFun_Sunac.h"
#include "ComFun_Def.h"
#include <acutmem.h>
#include "ComFun_ACAD_Common.h"
#include <dbmline.h>
#include <dbhatch.h>

bool DQ_IsALine(AcDbObjectId entId)
{
	if (entId == 0)
	{
		return false;
	}

	AcDbEntity *pEntity = 0;
	// ��ͼ�����ݿ��еĶ���
	acdbOpenObject(pEntity, entId, AcDb::kForRead);
	if (pEntity == 0)
		return false;

	AcDbLine *pLine = AcDbLine::cast(pEntity);
	pEntity->close();
	if (pLine == 0)
		return false;

	return true;
}

bool DQ_IsBlockReference(AcDbObjectId id)
{
	if (id == 0)
	{
		return false;
	}
	AcDbEntity * pEnt;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, id, AcDb::kForRead);
	// Make sure its a block reference
	AcDbBlockReference * pBR = AcDbBlockReference::cast(pEnt);
	if (pBR == NULL)
	{
		pEnt->close();
		return false;
	}
	pEnt->close();
	return true;
}

int DQ_GetBlockReferenceInsertPoint(AcDbObjectId id, AcGePoint3d &pnt)
{
	if (id == 0)
		return -1;

	AcDbEntity * pEnt;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, id, AcDb::kForRead);
	// Make sure its a block reference
	AcDbBlockReference * pBR = AcDbBlockReference::cast(pEnt);
	if (pBR == NULL)
	{
		pEnt->close();
		return -1;
	}
	pnt = pBR->position();
	pEnt->close();
	return 0;
}

//�õ����ߺ͵�������
int DQ_GetCurveClosedPointToOneGivenPoint(AcDbObjectId curvre, AcGePoint3d givenPnt, AcGePoint3d &closedPnt)
{
	AcDbEntity* pEnt = NULL;

	if (acdbOpenObject(pEnt, curvre, AcDb::kForRead) != Acad::eOk)
	{
		if (pEnt)
			pEnt->close();
		return -1;
	}

	AcDbCurve * pCrv = AcDbCurve::cast(pEnt);
	if (pCrv == 0)
	{
		return -2;
	}
	AcGePoint3d result;
	pCrv->getClosestPointTo(givenPnt, closedPnt);
	pEnt->close();
	return 0;
}

bool DQ_GetAttributeValue( AcDbBlockReference *pBlkRef, const TCHAR* tag, CString &value )
{
	// ��������¼�����Ա�����
	bool bFound = false;
	AcDbObjectIterator *pItr = pBlkRef->attributeIterator();
	if (pItr)
	{
		while (!pItr->done())
		{
			AcDbObjectId attId = pItr->objectId();
			AcDbAttribute *pAtt = NULL;
			if (pBlkRef->openAttribute(pAtt, attId, AcDb::kForRead) == Acad::eOk)
			{
				// ������е�����Tag��Զ�����Ǵ�д��ĸ����˴���Ĳ�����Ҫ���л�ת
				TCHAR *szTag = pAtt->tag();
				CString strTag = tag;
				strTag.MakeUpper();
				if (_tcscmp(strTag, szTag) == 0)
				{
					TCHAR *szValue = pAtt->textString();	
					value = szValue;
					acutDelString(szValue);
					bFound = true;
				}
				acutDelString(szTag);

				pAtt->close();
			}			

			pItr->step();
		}
		delete pItr;

		return bFound;
	}
	else
	{
		return false;
	}
}


bool DQ_HasAttribute_BlkDef(AcDbObjectId blockDefId, CString tag)
{

	AcDbBlockTableRecord *pBlkDefRcd = NULL;
	bool bRet = false;
	Acad::ErrorStatus es = acdbOpenObject(pBlkDefRcd, blockDefId, AcDb::kForRead);

	if (es == Acad::eOk)
	{
		// �鶨�����Ƿ�����˶�Ӧ������
		if (pBlkDefRcd->hasAttributeDefinitions())
		{
			AcDbBlockTableRecordIterator *pItr = NULL;
			pBlkDefRcd->newIterator(pItr);
			AcDbEntity *pEnt = NULL;

			for (pItr->start(); !pItr->done(); pItr->step())
			{
				pItr->getEntity(pEnt, AcDb::kForRead);

				// ����Ƿ������Զ���
				AcDbAttributeDefinition *pAttDef = AcDbAttributeDefinition::cast(pEnt);
				if (pAttDef != NULL)
				{
					TCHAR* pszTag = pAttDef->tag();
					CString strTag = pszTag;
					acutDelString(pszTag);
					if (strTag.CompareNoCase(tag) == 0)
					{
						pEnt->close();
						pBlkDefRcd->close();
						return true;
					}										
				}
				pEnt->close();
			}
			delete pItr;
		}
		else
		{
			bRet = false;
		}

		pBlkDefRcd->close();
	}	

	return bRet;
}


bool DQ_HasAttribute_BlkRef(AcDbObjectId blockRefId, CString tagText)
{
	if (blockRefId == 0)
	{
		return false;
	}
	AcDbEntity * pEnt;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, blockRefId, AcDb::kForRead);
	if (pEnt == 0)
		return false;
	// Make sure its a block reference
	AcDbBlockReference * pBR = AcDbBlockReference::cast(pEnt);
	if (pBR == NULL)
	{
		pEnt->close();
		return false;
	}

	pEnt->close();

	AcDbObjectId recID = pBR->blockTableRecord();

	return DQ_HasAttribute_BlkDef(recID,tagText);

}


void DQ_SelectMany(vAcDbObjectId &ids)
{
	AcDbObjectId id;
	ads_name ssname;
	ads_name ent;
	int rt = acedSSGet(NULL, NULL, NULL, NULL, ssname); // ��ʾ�û�ѡ�����
	if (rt == RTNORM)
	{
		long length;
		acedSSLength(ssname, &length);
		for (int i=0;i<length;i++)
		{
			acedSSName(ssname,i, ent);
			acdbGetObjectId(id, ent);
			ids.push_back(id);
		}
		acedSSFree(ssname);
	}
}

bool DQ_GetAttributeValue( AcDbObjectId blockRefId, CString tag, CString &value )
{
	if (blockRefId == 0)
	{
		return false;
	}
	AcDbEntity * pEnt;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, blockRefId, AcDb::kForRead);
	if (pEnt == 0)
		return false;
	// Make sure its a block reference
	AcDbBlockReference * pBR = AcDbBlockReference::cast(pEnt);
	if (pBR == NULL)
	{
		pEnt->close();
		return false;
	}
	pEnt->close();

	return DQ_GetAttributeValue( pBR, tag, value );
}

//��������
int MD2010_GetAllTypedObjectsInLayer(vAcDbObjectId &allEntites, CString layname, eACDBOBJECTTYPE type, vAcDbObjectId &vids)
{
	// �ж��Ƿ��������Ϊ�����ԡ���ͼ��
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pLayerTbl, AcDb::kForRead);
	if (!pLayerTbl->has(layname))
	{
		//acutPrintf("\n��ǰͼ����δ����\"����\"ͼ��!");
		pLayerTbl->close();
		return 0;
	}

	AcDbObjectId layerId; // �����ԡ�ͼ���ID
	pLayerTbl->getAt(layname, layerId);
	pLayerTbl->close();


	for (int i = 0; i < allEntites.size(); i++)
	{
		AcDbEntity *pEnt = 0;
		// ��ͼ�����ݿ��еĶ���
		acdbOpenObject(pEnt, allEntites[i], AcDb::kForRead);
		if (pEnt == 0)
			continue;

		// �Ƿ��ڡ����ԡ�ͼ����
		if (pEnt->layerId() == layerId)
		{
			switch(type)
			{
			case ACDB_POINT:
				{
					// �Ƿ��ǵ�
					AcDbPoint *prealobj = AcDbPoint::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_LINE:
				{
					// �Ƿ���ֱ��
					AcDbLine *prealobj = AcDbLine::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_CYCLE:
				{
					// �Ƿ���Բ
					AcDbCircle *prealobj = AcDbCircle::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_ARC:
				{
					// �Ƿ���Բ
					AcDbArc *prealobj = AcDbArc::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_2dPLINE:
				{
					// �Ƿ���2ά�����
					AcDb2dPolyline *prealobj = AcDb2dPolyline::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_MLINES:
				{
					// �Ƿ��Ƕ��߶�ƽ��
					AcDbMline *prealobj = AcDbMline::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_SPLINE:
				{
					// �Ƿ�����������
					AcDbSpline *prealobj = AcDbSpline::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_ELLIPSE: 
				{
					// �Ƿ�����Բ
					AcDbEllipse *prealobj = AcDbEllipse::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_TEXT: 
				{
					// �Ƿ�������
					AcDbText *prealobj = AcDbText::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_MTEXT:
				{
					// �Ƿ��Ƕ������
					AcDbMText  *prealobj = AcDbMText ::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_0PLINE:
				{
					// �Ƿ��Ƕ����
					AcDbPolyline *prealobj = AcDbPolyline::cast(pEnt);
					if (prealobj != NULL)
					{
					    vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_CURVE:
				{
					// �Ƿ�������
					AcDbCurve  *prealobj = AcDbCurve ::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_HATCH:
				{
					// �Ƿ�������
					AcDbHatch  *prealobj = AcDbHatch ::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			default:
				break;
			}
		}
		// ע����Ҫ�ر�ʵ��
		pEnt->close();
	}
	return 0;
}

void YT_UpdateBlockReference(AcDbObjectId &entId)
{
	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, entId, AcDb::kForWrite) != Acad::eOk)
	{
		acutPrintf(L"\nError opening entity.");
		if (pEnt)
			pEnt->close();
		return ;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		acutPrintf(L"\nMust select a block insert.");
		pEnt->close();
		return ;
	}
	AcDbEvalVariant var;
	AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
	pBlkRef->recordGraphicsModified(true) ;
	//pBlkRef->draw();
	pBlkRef->close();
}

