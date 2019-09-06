#include "stdafx.h"
#include "ComFun_Str.h"
#include "ComFun_Math.h"
#include "ComFun_Sunac.h"
#include "ComFun_Def.h"
#include <acutmem.h>
#include "ComFun_ACAD_Common.h"
#include <dbmline.h>
#include <dbhatch.h>
#include <acedads.h>
#include "../Common/TYRect.h"
#include "../Object\AttrWindow.h"
#include "../Object\AttrAirCon.h"
#include "../Object\AttrDoor.h"
#include "../Object\AttrKitchen.h"
#include "../Object\AttrToilet.h"
#include "../Object\AttrRailing.h"

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

void TY_GetAllWindowFiles(vCString &files)
{
	CString appPath = MD2010_GetAppPath();
	files.push_back(appPath + L"\\support\\Sunac2019\\Window\\Window_1.0.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Window\\Window_2.0.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Window\\Window_1.1.dwg");
}

void TY_GetAllKitchenFiles(vCString &files)
{
	CString appPath = MD2010_GetAppPath();
	files.push_back(appPath + L"\\support\\Sunac2019\\Kitchen\\Kitchen_1.0.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Kitchen\\Kitchen_2.0.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Kitchen\\Kitchen_1.0.dwg");
}

void TY_GetAllTieYiLanGanFiles(vCString &files)
{
	files.clear();
	CString appPath = MD2010_GetAppPath();
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_1.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_2.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_3.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_4.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_5.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_6.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_7.dwg");
}

void TY_GetAllBoLiLanGanFiles(vCString &files)
{
	files.clear();
	CString appPath = MD2010_GetAppPath();
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_1.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_2.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_3.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_4.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_5.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_6.dwg");
	files.push_back(appPath + L"\\support\\Sunac2019\\Railing\\��������_7.dwg");
}


AcGePoint3d TY_GetPoint(CString prompt)
{
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,prompt,pt)!=RTNORM) //��һ�ǵ�ѡ��
	{
		return AcGePoint3d(0,0,0);
	}

	AcGePoint3d pnt(pt[0], pt[1], pt[2]);

	return pnt;
}

TYRect TY_GetOneRect()
{
	ads_point pt,result;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\npoint\n",pt)!=RTNORM) //��һ�ǵ�ѡ��
	{
		return TYRect();
	}
	if(acedGetCorner(pt,L"\ncorner\n",result)!=RTNORM)//��ѡ
	{
		return TYRect();
	}

	AcGePoint3d pnt1(pt[0],pt[1],pt[2]);
	AcGePoint3d pnt2(result[0],result[1],result[2]);

	TYRect rect;
	rect.Add(pnt1);
	rect.Add(pnt2);

	return rect;
}

int TY_GetTwoPoints(AcGePoint3d &pnt1, AcGePoint3d &pnt2)
{
	ads_point pt,result;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"��ѡ���һ����\n",pt)!=RTNORM) //��һ�ǵ�ѡ��
	{
		return 0;
	}

	if(acedGetPoint(pt,L"��ѡ��ڶ�����\n",result)!=RTNORM) //��һ�ǵ�ѡ��
	{
		return 0;
	}

	pnt1 = AcGePoint3d(pt[0],pt[1],pt[2]);
	pnt2 = AcGePoint3d(result[0],result[1],result[2]);

	return 0;
}

AcDbObjectId TY_GetExtensionDictionaryID(AcDbObjectId id)
{
	AcDbObject *pObj;
	acDocManager->lockDocument(curDoc());
	if(acdbOpenObject(pObj, id, AcDb::kForWrite)==Acad::eOk)
	{
		if (pObj->createExtensionDictionary()==Acad::eOk||pObj->createExtensionDictionary()==Acad::eAlreadyInDb)
		{
			pObj->close();
			acDocManager->unlockDocument(curDoc());
			return pObj->extensionDictionary();
		}
		else
			pObj->close();
	}
	acDocManager->unlockDocument(curDoc());
	return 0;
}

//��UI����Ϣ����д��ͼ��
int TY_AddAttributeData(AcDbObjectId Id, AcDbObject *pDataEnt)
{
	if (pDataEnt == 0)
		return -2;

	AcDbObjectId m_dicID = TY_GetExtensionDictionaryID(Id);
	if (m_dicID == 0)
		return -1;

	AcDbObject *pObj = NULL;
	AcDbObjectId patternID = NULL;
	AcDbDictionary *pDict = NULL;

	//ע��������ʱ����벻��ȥ�� ����Ϊû��ע��
	Acad::ErrorStatus es = acDocManager->lockDocument(curDoc());
	if(acdbOpenObject(pDict, m_dicID, AcDb::kForWrite)==Acad::eOk)
	{
		es =pDict->setAt(SUNAC_ATTRIBUTE_ENTITY, pDataEnt, patternID);
		pDict->close();
		acDocManager->unlockDocument(curDoc());
		return 0;
	}
	else
	{
		//pObj->close();
		acDocManager->unlockDocument(curDoc());
		return -3;
	}
	acDocManager->unlockDocument(curDoc());
	return 0;
}

int TY_GetAttributeData(AcDbObjectId tkId, AcDbObject *&pDataEnt)
{
	AcDbObjectId m_dicID = TY_GetExtensionDictionaryID(tkId);
	if (m_dicID == 0)
		return -1;

	AcDbDictionary *pDict = NULL;
	if(acdbOpenObject(pDict, m_dicID, AcDb::kForRead)==Acad::eOk)
	{
		Acad::ErrorStatus es =pDict->getAt(SUNAC_ATTRIBUTE_ENTITY, (AcDbObject*&)pDataEnt, AcDb::kForRead);
		pDict->close();
		if(pDataEnt == NULL)
			return 68;
		pDataEnt->close();
	}
	return 0;
}

bool TY_IsWindow(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt);
	AttrWindow * pWindow = dynamic_cast<AttrWindow *>(pDataEnt);
	if (pWindow != 0)
		return true;
	return false;
}

bool TY_Iskitchen(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt);
	AttrKitchen * pKitchen = dynamic_cast<AttrKitchen *>(pDataEnt);
	if (pKitchen != 0)
		return true;
	return false;
}

bool TY_IsRailing(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt);
	AttrRailing * pKitchen = dynamic_cast<AttrRailing *>(pDataEnt);
	if (pKitchen != 0)
		return true;
	return false;
}

bool TY_IsAirCon(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt);
	AttrAirCon * pKitchen = dynamic_cast<AttrAirCon *>(pDataEnt);
	if (pKitchen != 0)
		return true;
	return false;
}

bool TY_IsToilet(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt);
	AttrToilet * pKitchen = dynamic_cast<AttrToilet *>(pDataEnt);
	if (pKitchen != 0)
		return true;
	return false;
}

bool TY_IsDoor(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt);
	AttrDoor * pKitchen = dynamic_cast<AttrDoor *>(pDataEnt);
	if (pKitchen != 0)
		return true;
	return false;
}

int vFind(RCPairKeyDValue &A, vRCPairKeyDValue &B)
{
	int ret = -1;
	for (int i = 0; i < B.size(); i++)
	{
		if (A.first == B[i].first  &&
			fabs(A.second - B[i].second) < TOL)
		{
			return i;
		}
	}
	return ret;
}

int vFind(RCPairKeyIValue &A, vRCPairKeyIValue &B)
{
	int ret = -1;
	for (int i = 0; i < B.size(); i++)
	{
		if (A.first == B[i].first  &&
			A.second == B[i].second)
		{
			return i;
		}
	}
	return ret;
}

int vFind(RCPairKeyStrValue &A, vRCPairKeyStrValue &B)
{
	int ret = -1;
	for (int i = 0; i < B.size(); i++)
	{
		if (A.first == B[i].first  &&
			A.second == B[i].second)
		{
			return i;
		}
	}
	return ret;
}

int vFind(RCWindow &oneWindow, vRCWindow &allWindows)
{
	for (int i = 0; i < allWindows.size(); i++)
	{
		if (oneWindow.isEqualTo(&(allWindows[i])))
		{
			return i;
		}
	}

	return -1;
}

bool TY_IsPairsEqual(vRCPairKeyDValue &A, vRCPairKeyDValue &B)
{
	if (A.size() != B.size())
		return false;

	for (int i = 0; i < A.size(); i++)
	{
		int index = vFind(A[i],B);
		if (index == -1)
			return false;
	}
	return true;
}

bool TY_IsPairsEqual(vRCPairKeyIValue &A, vRCPairKeyIValue &B)
{
	if (A.size() != B.size())
		return false;

	for (int i = 0; i < A.size(); i++)
	{
		int index = vFind(A[i],B);
		if (index == -1)
			return false;
	}
	return true;
}

bool TY_IsPairsEqual(vRCPairKeyStrValue &A, vRCPairKeyStrValue &B)
{
	if (A.size() != B.size())
		return false;

	for (int i = 0; i < A.size(); i++)
	{
		int index = vFind(A[i],B);
		if (index == -1)
			return false;
	}
	return true;
}

CString TY_GetLocalFilePath()
{
	CString appPath = MD2010_GetAppPath();
#ifdef WORK_LOCAL//����ģʽ
	return appPath + L"\\support\\Sunac2019\\LocalMode\\";
#else
	return appPath + L"\\support\\Sunac2019\\Files\\";
#endif
}

/************************************************************************
˵����
    ���ļ����в����ļ�������ָ���ļ��в��ң����ݹ飩
������
    fileName:Ҫ���ҵ��ļ�����֧��ͨ���*
    directory:Ҫ���ҵ��ļ����ڵ�Ŀ¼·��
����ֵ��
    pair<string,string>���͵����飬map<�ļ������ļ�·��>
************************************************************************/
vector<pair<CString,CString>> TY_FindFilesInDirecotry(CString fileName,CString directory )
{
    vector<pair<CString,CString>> files;
 
    if(directory.Right(1) != "\\") //��֤Ŀ¼����\��β��
    {  
        directory += "\\";
    }
    directory += fileName;
    CFileFind finder;
    BOOL success = finder.FindFile(directory);
    while(success)
    {
        success = finder.FindNextFile();
        CString name = finder.GetFileName();
        CString path = finder.GetFilePath();
        pair<CString,CString> fileMap(name,path);
        files.push_back(fileMap);
    }
    finder.Close();
    return files;
}

/************************************************************************
˵����
    ���ļ����еݹ�����ļ�
������
    fileName:Ҫ���ҵ��ļ�����֧��ͨ���*
    directory:Ҫ���ҵ��ļ����ڵ�Ŀ¼·��
    recursionCount:�ݹ���ҵ���ȣ�Ĭ��Ϊ-1����������ȣ�,0��ʾֻ����������directoryĿ¼
����ֵ��
    pair<string,string>���͵����飬map<�ļ������ļ�·��>
************************************************************************/
vector<pair<CString,CString>> TY_FindFilesInDirecotryRecursion( CString fileName,CString directory,int recursionCount)
{
    vector<pair<CString,CString>> files;
 
    int curRecursionCount = recursionCount;
 
    //�ȶԲ������������ļ��в���
    vector<pair<CString,CString>> localFiles =  TY_FindFilesInDirecotry(fileName,directory);
    files.insert(files.end(),localFiles.begin(),localFiles.end());
 
    //Ȼ��Դ��ļ�����������ļ��еݹ����
    CFileFind finder;
    if(directory.Right(1) != "\\") //��֤Ŀ¼����\��β��
    {  
        directory += "\\";
    }
    directory += "*.*";
 
    BOOL res = finder.FindFile(directory);
    while (res)
    {
        res = finder.FindNextFile();
        CString path = finder.GetFilePath();
        CString name = finder.GetFileName();
        curRecursionCount = recursionCount;  //����ͬ��Ŀ¼��ʱ�򽫵�ǰ�������Ϊ����ֵ
        if(finder.IsDirectory() && !finder.IsDots())
        {
            if(curRecursionCount >= 1 || curRecursionCount == -1 )
            {
                if(curRecursionCount >= 1)
                {
                    curRecursionCount--;
                }
                localFiles = TY_FindFilesInDirecotryRecursion(fileName,path,curRecursionCount); //���Ŀ���Ǹ��ļ��У�������Ƕ��������
                files.insert(files.end(),localFiles.begin(),localFiles.end());
            }
        }
    }
    finder.Close();
    return files;
}