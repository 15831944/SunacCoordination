#include "stdafx.h"
#include <io.h>
#include "ComFun_Str.h"
#include "ComFun_Math.h"
#include "ComFun_Sunac.h"
#include "ComFun_Def.h"
#include <acutmem.h>
#include "ComFun_ACAD_Common.h"
#include <dbmline.h>
#include <dbhatch.h>
#include <acedads.h>
#include <dbidmap.h>
#include "../Common/TYRect.h"
#include "../Object\WindowDoor\AttrWindow.h"
#include "../Object\AirCondition\AttrAirCon.h"
#include "../Object\Kitchen\AttrKitchen.h"
#include "../Object\Bathroom\AttrBathroom.h"
#include "../Object/Railing/AttrRailing.h"
#include "../Tool/DocLock.h"

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
	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, id, AcDb::kForRead);
	// Make sure its a block reference
	AcDbBlockReference * pBR = AcDbBlockReference::cast(pEnt);
	if (pBR == NULL)
	{
		if(pEnt)
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
		Adesk::Int32 length;
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

bool DQ_SetDynamicAttribute(AcDbObjectId p_blockRefId, CString p_attributename, double p_value)
{
	if (p_blockRefId==AcDbObjectId::kNull)
		return false;

	AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(p_blockRefId);
	if (pDynBlkRef == NULL)
		return false;

	AcDbDynBlockReferencePropertyArray blkPropAry;
	pDynBlkRef->getBlockProperties(blkPropAry);

	Acad::ErrorStatus err = Acad::eInvalidInput;
	for (long lIndex1 = 0L; lIndex1 < blkPropAry.length(); ++lIndex1)
	{
		AcDbDynBlockReferenceProperty blkProp = blkPropAry[lIndex1];
		if (blkProp.readOnly())
			continue;

		if (wcscmp(blkProp.propertyName().kACharPtr(), p_attributename) == 0)
		{
			AcDbEvalVariant eval(p_value);
			err = blkProp.setValue(eval);
			break;
		}
	}

	delete pDynBlkRef;

	return err == Acad::eOk;
}

bool DQ_SetDynamicAttribute(AcDbObjectId p_blockRefId, CString p_attributename, CString p_value)
{
	if (p_blockRefId == AcDbObjectId::kNull)
		return false;

	AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(p_blockRefId);
	if (pDynBlkRef == NULL)
		return false;

	CDocLock docLock;

	AcDbDynBlockReferencePropertyArray blkPropAry;
	pDynBlkRef->getBlockProperties(blkPropAry);

	Acad::ErrorStatus err = Acad::eInvalidInput;
	for (long lIndex1 = 0L; lIndex1 < blkPropAry.length(); ++lIndex1)
	{
		AcDbDynBlockReferenceProperty blkProp = blkPropAry[lIndex1];
		if (blkProp.readOnly())
			continue;

		if (wcscmp(blkProp.propertyName().kACharPtr(), p_attributename) == 0)
		{
			AcDbEvalVariant eval(p_value);
			err = blkProp.setValue(eval);
			break;
		}
	}

	delete pDynBlkRef;

	return err == Acad::eOk;
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


	for (UINT i = 0; i < allEntites.size(); i++)
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

bool TY_GetPoint(AcGePoint3d &ptOut, CString prompt)
{
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,prompt,pt)!=RTNORM) //��һ�ǵ�ѡ��
	{
		ptOut = AcGePoint3d(0, 0, 0);
		return false;
	}

	ptOut = AcGePoint3d(pt[0], pt[1], pt[2]);
	return true;
}

bool TY_GetOneRect(TYRect& p_rect)
{
	ads_point pt,result;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\n��ѡ���һ�ǵ�\n",pt)!=RTNORM) //��һ�ǵ�ѡ��
	{
		return false;
	}

	if(acedGetCorner(pt,L"\n��ѡ��Խǵ�\n",result)!=RTNORM)//��ѡ
	{
		return false;
	}

	AcGePoint3d pnt1(pt[0],pt[1],pt[2]);
	AcGePoint3d pnt2(result[0],result[1],result[2]);

	TYRect rect;
	rect.Add(pnt1);
	rect.Add(pnt2);

	p_rect = rect;
	return true;
}

bool TY_GetTwoPoints(AcGePoint3d &pnt1, AcGePoint3d &pnt2)
{
	ads_point pt,result;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"��ѡ���һ����\n",pt)!=RTNORM) //��һ�ǵ�ѡ��
	{
		return false;
	}

	if(acedGetPoint(pt,L"��ѡ��ڶ�����\n",result)!=RTNORM) //��һ�ǵ�ѡ��
	{
		return false;
	}

	pnt1 = AcGePoint3d(pt[0],pt[1],pt[2]);
	pnt2 = AcGePoint3d(result[0],result[1],result[2]);

	return true;
}

bool GetRealInput(const TCHAR* prompt, double defaultVal, int precision, double &ret)
{
	CString strPrompt = prompt;
	strPrompt.TrimRight();
	strPrompt.TrimRight(TEXT(":"));
	CString strDefaultValue;
	strDefaultValue.Format(TEXT("<%%.%df>:"), precision);	// �õ����ơ�%.2f���ĸ�ʽ�ַ���
	strPrompt.Format(strPrompt + strDefaultValue, defaultVal);

	ret = defaultVal;
	int rc = acedGetReal(strPrompt, &ret);
	if (rc == RTNORM || rc == RTNONE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GetStringInput(CString prompt, CString &strOut)
{
	ACHAR num[256] = L"\0";
	int rcc = acedGetString(0, prompt, num);
	if (rcc == RTCAN)
		return false;
	else if (rcc != RTNORM)
		return false;

	strOut = CString(num);
	return true;
}


AcDbObjectId TY_GetExtensionDictionaryID(AcDbObjectId id)
{
	AcDbObjectId dicId = AcDbObjectId::kNull;

	AcDbObject *pObj = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pObj, id, AcDb::kForRead);
	if(pObj!=NULL)
	{
		dicId = pObj->extensionDictionary();
		pObj->close();
	}
	return dicId;
}

AcDbObjectId TY_CreateExtensionDictionaryID(AcDbObjectId id)
{
	CDocLock doclock;

	AcDbObjectId dicId = AcDbObjectId::kNull;

	AcDbObject *pObj = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pObj, id, AcDb::kForWrite);
	if(pObj!=NULL)
	{
		Acad::ErrorStatus es = pObj->createExtensionDictionary();
		if (es == Acad::eOk || es == Acad::eAlreadyInDb)
		{
			dicId = pObj->extensionDictionary();
		}
		pObj->close();
	}
	
	return dicId;
}


//��UI����Ϣ����д��ͼ��
int TY_AddAttributeData(AcDbObjectId Id, AcDbObject *pDataEnt)
{
	if (pDataEnt == 0)
		return -2;

	AcDbObjectId dicID = TY_GetExtensionDictionaryID(Id);
	if (dicID == 0)
	{
		dicID = TY_CreateExtensionDictionaryID(Id);
	}
	if (dicID == 0) //������������δ�ɹ����򷵻�-1
	{
		return -1;
	}

	CDocLock lock;

	//ע��������ʱ����벻��ȥ�� ����Ϊû��ע��
	AcDbDictionary *pDict = NULL;
	if (acdbOpenObject(pDict, dicID, AcDb::kForWrite) == Acad::eOk)
	{
		AcDbObjectId patternID = NULL;
		Acad::ErrorStatus es = pDict->setAt(SUNAC_ATTRIBUTE_ENTITY, pDataEnt, patternID);
		pDict->close();
		pDataEnt->close();

		return 0;
	}
	else
	{
		return -3;
	}
}

int TY_GetAttributeData(AcDbObjectId tkId, AcDbObject *&pDataEnt, bool p_bRead)
{
	pDataEnt = NULL;
	AcDbObjectId dicID = TY_GetExtensionDictionaryID(tkId);
	if (dicID == AcDbObjectId::kNull)
		return -1;

	AcDbDictionary *pDict = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pDict, dicID, AcDb::kForRead);
	if(es ==Acad::eOk)
	{
		//es = pDict->getAt(SUNAC_ATTRIBUTE_ENTITY, (AcDbObject*&)pDataEnt, AcDb::kForRead);
		es = pDict->getAt(SUNAC_ATTRIBUTE_ENTITY, (AcDbObject*&)pDataEnt, p_bRead ? AcDb::kForRead : AcDb::kForWrite);

		pDict->close();
		if (pDataEnt!=NULL)
		{
			if (p_bRead==false)
			{
				pDataEnt->assertWriteEnabled();
			}
			pDataEnt->close();
		}
	}

	return pDataEnt == NULL ? 0 : -68;
}


bool TY_IsSunacObj(AcDbObjectId Id, eRCType p_rcType, eViewDir p_view)
{
	switch (p_rcType)
	{
	case S_WINDOW:
	case S_DOOR:
		return TY_IsWindow(Id, p_view);
		break;
	case S_KITCHEN:
		return TY_Iskitchen(Id);
		break;
	case S_BATHROOM:
		return TY_IsBathroom(Id);
		break;
	case S_AIRCON:
		return TY_IsAirCon(Id);
		break;
	case S_RAILING:
		return TY_IsRailing(Id, p_view);
		break;
	default:
		return false;
		break;
	}
}

bool TY_IsWindow(AcDbObjectId Id, eViewDir p_view)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt, true);
	if (pDataEnt == NULL)
		return false;

	AttrWindow * pWindow = dynamic_cast<AttrWindow *>(pDataEnt);
	if (pWindow == NULL)
		return false;

	bool bWindow = true;
	if (p_view != E_VIEW_ALL)
	{
		bWindow = pWindow->GetViewDir() == p_view;
	}
	pWindow->close();

	return true;
}

bool TY_IsRailing(AcDbObjectId Id, eViewDir p_view)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt, true);
	if (pDataEnt == NULL)
		return false;
	AttrRailing * pRailing = dynamic_cast<AttrRailing *>(pDataEnt);
	if (pRailing == 0)
		return false;

	bool isRailing = true;
	if (p_view != E_VIEW_ALL)
	{
		isRailing = pRailing->GetViewDir() == p_view;
	}
	return isRailing;
}

eRCType TY_GetType(AcDbBlockReference *pBlockReference)
{
	if (pBlockReference == 0)
		return S_TYPENUM;
	AcDbObject * pDataEnt = 0;
	AcDbObjectId dictid = pBlockReference->extensionDictionary();

	AcDbDictionary *pDict = NULL;
	if(acdbOpenObject(pDict, dictid, AcDb::kForRead)==Acad::eOk)
	{
		Acad::ErrorStatus es =pDict->getAt(SUNAC_ATTRIBUTE_ENTITY, (AcDbObject*&)pDataEnt, AcDb::kForRead);
		pDict->close();
		pDataEnt->close();
		if(pDataEnt == NULL)
			return S_TYPENUM;

		AttrWindow * pWindow = dynamic_cast<AttrWindow *>(pDataEnt);
		if (pWindow != 0)
			return S_WINDOW;

		AttrKitchen * pKit = dynamic_cast<AttrKitchen *>(pDataEnt);
		if (pKit != 0)
			return S_KITCHEN;

		AttrBathroom * pBath= dynamic_cast<AttrBathroom *>(pDataEnt);
		if (pBath != 0)
			return S_BATHROOM;

		AttrRailing * pRail = dynamic_cast<AttrRailing *>(pDataEnt);
		if (pRail != 0)
			return S_RAILING;

		AttrAirCon * pAir = dynamic_cast<AttrAirCon *>(pDataEnt);
		if (pAir != 0)
			return S_AIRCON;

		return S_TYPENUM;
	}
	else
		return S_TYPENUM;
}

bool TY_Iskitchen(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = NULL;
	TY_GetAttributeData(Id, pDataEnt, true);
	AttrKitchen * pKitchen = dynamic_cast<AttrKitchen *>(pDataEnt);
	if (pKitchen != NULL)
		return true;
	return false;
}


bool TY_IsAirCon(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = NULL;
	TY_GetAttributeData(Id, pDataEnt, true);
	AttrAirCon * pKitchen = dynamic_cast<AttrAirCon *>(pDataEnt);
	if (pKitchen != NULL)
		return true;
	return false;
}

bool TY_IsBathroom(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = NULL;
	TY_GetAttributeData(Id, pDataEnt, true);
	AttrBathroom * pKitchen = dynamic_cast<AttrBathroom *>(pDataEnt);
	if (pKitchen != NULL)
		return true;
	return false;
}

int vFind(RCPairKeyDValue &A, vRCPairKeyDValue &B)
{
	int ret = -1;
	for (UINT i = 0; i < B.size(); i++)
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
	for (UINT i = 0; i < B.size(); i++)
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
	for (UINT i = 0; i < B.size(); i++)
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
	for (UINT i = 0; i < allWindows.size(); i++)
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

	for (UINT i = 0; i < A.size(); i++)
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

	for (UINT i = 0; i < A.size(); i++)
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

	for (UINT i = 0; i < A.size(); i++)
	{
		int index = vFind(A[i],B);
		if (index == -1)
			return false;
	}
	return true;
}

CString TY_GetDataFilePath()
{
	CString appPath = MD2010_GetAppPath();
	return appPath + L"\\Sunac2019\\Data\\";
}

CString TY_GetPrototypeFilePath()
{
#ifdef WORK_LOCAL//����ģʽ
	return TY_GetPrototypeFilePath_Local();
#else
	return TY_GetPrototypeFilePath_Web();
#endif
}

CString TY_GetPrototypeImagePath()
{
#ifdef WORK_LOCAL//����ģʽ
	return TY_GetPrototypeImagePath_Local();
#else
	return TY_GetPrototypeImagePath_Web();
#endif
}
CString TY_GetPrototypeFilePath_Local()
{
	CString sPath = MD2010_GetAppPath()+ L"\\Sunac2019\\LocalMode\\";
	return sPath;
}
CString TY_GetPrototypeImagePath_Local()
{
	CString sPath = MD2010_GetAppPath() + L"\\Sunac2019\\Image\\";
	return sPath;
}
CString TY_GetPrototypeFilePath_Web()
{
	CString sPath = MD2010_GetAppPath() + L"\\Sunac2019\\Files";
	if (!JHCom_FileExist(sPath))//�ж�Files�ļ����Ƿ���ڣ�����������򴴽�
	{
		CreateDirectory(sPath, NULL);
	}
	return sPath + L"\\";
}
CString TY_GetPrototypeImagePath_Web()
{
	CString sPath = MD2010_GetAppPath() + L"\\Sunac2019\\Files";
	if (!JHCom_FileExist(sPath))//�ж�Files�ļ����Ƿ���ڣ�����������򴴽�
	{
		CreateDirectory(sPath, NULL);
	}
	return sPath + L"\\";
}

char* TY_GetAluminumDatabasePath()
{
	CString TempPath = MD2010_GetAppPath() + L"\\Sunac2019\\Data\\AluminumData.db";
	int n = TempPath.GetLength();
	int len = WideCharToMultiByte(CP_ACP, 0, TempPath, n, NULL, 0, NULL, NULL);
	char * DatabasePath = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, TempPath, n, DatabasePath, len, NULL, NULL);
	DatabasePath [len] = '\0';
	return DatabasePath;
}

char* TY_GetLocalDataDatabasePath()
{
	CString TempPath = MD2010_GetAppPath() + L"\\Sunac2019\\Data\\LocalData.db";
	int n = TempPath.GetLength();
	int len = WideCharToMultiByte(CP_ACP, 0, TempPath, n, NULL, 0, NULL, NULL);
	char * DatabasePath = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, TempPath, n, DatabasePath, len, NULL, NULL);
	DatabasePath [len] = '\0';
	return DatabasePath;
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

int TYCOM_Mirror(AcDbObjectId entId, AcGePoint3d first, AcGeVector3d dir)
{
	CDocLock docLock;
	AcDbEntity *pEntity = 0;
	Acad::ErrorStatus  es = acdbOpenObject(pEntity, entId, AcDb::kForWrite);
	if (pEntity == 0)
		return es;

	AcGeMatrix3d mx;
	mx.setToMirroring(AcGeLine3d(first,dir));

	AcDbEntity *newEnt = 0;
	pEntity->transformBy(mx);

	pEntity->close();
	return 0;
}

//��ת
int TYCOM_Rotate(AcDbObjectId entId, AcGePoint3d ptBase, double rotation)
{
	CDocLock docLock;
	AcDbEntity *pEntity = 0;
	// ��ͼ�����ݿ��еĶ���
	Acad::ErrorStatus  es = acdbOpenObject(pEntity, entId, AcDb::kForWrite);
	if (pEntity == 0)
		return es;

	AcGeMatrix3d xform;
	AcGeVector3d vec(0, 0, 1);
	xform.setToRotation(rotation, vec, ptBase);

	AcDbEntity *newEnt = 0;
	pEntity->transformBy(xform);

	pEntity->close();

	return 0;
}

int TYCOM_Move(AcDbObjectId entId, AcGeVector3d offset)
{
	CDocLock docLock;
	AcGeMatrix3d xform;
	xform.setToTranslation(offset);

	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite, false);
	if (es == Acad::eOk)
	{
		es = pEnt->transformBy(xform);
		pEnt->close();
	}
	return 0;
}

int TYCOM_Transform(AcDbObjectId entId, const AcGeMatrix3d& mat)
{
	if (entId == AcDbObjectId::kNull)
		return 0;

	CDocLock docLock;

	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite, false);
	if (es == Acad::eOk)
	{
		es = pEnt->transformBy(mat);
		pEnt->close();
	}
	return 0;
}

double GePlineLength(AcDbObjectId eId)
{

	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, eId, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"\nError opening entity.");
		if (pEnt)
			pEnt->close();
		return 0;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		acutPrintf(L"\nMust select a block insert.");
		pEnt->close();
		return 0;
	}

	double lem = 0;
	AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
	AcDbVoidPtrArray entitySet;
	pBlkRef->explode(entitySet);
	vAcDbObjectId arrayids111,arrayids222;
	for(int i = 0; i < entitySet.length(); i++)
	{
		AcDbEntity *pent = AcDbEntity::cast((AcRxObject*)entitySet[i]);
		AcDb::Visibility  vv = pent->visibility();
		if(vv == AcDb::kVisible)
		{
			AcDbPolyline *ppline = AcDbPolyline ::cast((AcRxObject*)entitySet[i]);
			//UINT color = ppline->entityColor();
			if(ppline!=NULL)
			{
				double endParm = 0;
				Acad::ErrorStatus es = ppline->getEndParam(endParm);
				if(es == Acad::eOk)
				{
					double temp = 0;
					ppline->getDistAtParam(endParm, temp);
					if(temp > lem)
						lem = temp;
				}
			}
		}
	}
	pEnt->close();
	return lem;
}

int TY_HideBlockReferencesInBlockReference(AcDbObjectId blkRefId, vCString &hideBlockRecordNames)
{
	vAcDbObjectId vids;
	ACHAR *brecname = L"";
	MD2010_GetBlockReference_Record_name(blkRefId, brecname);
	MD2010_CycleBlockEntites(brecname, vids);

	vAcDbObjectId blkrefs;
	for (UINT i = 0; i < vids.size(); i++)
	{
		if(MD2010_GetBlockReference_Record_name(vids[i], brecname) == 0)
		{
			CString internalRecordName(brecname);
			int index =JHCOM_vFind(internalRecordName,hideBlockRecordNames);
			if (index >= 0)
			{
				JHCOM_ShowObject(vids[i],false,false);
			}
		}
	}

	TYCOM_FlushBlockDisplay(blkRefId);
	return 0;
}



AcDbObjectId CopyBlockDefFromDatabase(AcDbDatabase* pSourceDb, AcDbDatabase* pDestDb, const TCHAR* blkDefName)
{
	Acad::ErrorStatus es;
	AcDbObjectId blockRefId = AcDbObjectId::kNull;

	// ���ⲿͼ�����ݿ�Ŀ��Ѱ�Ҹ������ƵĿ���¼
	AcDbBlockTable* pBlkTable = NULL;
	es = pSourceDb->getBlockTable(pBlkTable, AcDb::kForRead);
	assert(es == Acad::eOk);
	if (es != Acad::eOk)
	{
		acutPrintf(TEXT("\n ����Ŀ��Դʱ���򿪿���¼ʧ��."));
		return blockRefId;
	}

	bool bRet = false;
	if (pBlkTable->has(blkDefName))	// ������ָ����ͼ��
	{
		AcDbObjectId destBlkDefId;		// ָ��ͼ��Ŀ���¼Id
		es = pBlkTable->getAt(blkDefName, destBlkDefId);

		// ��ָ����ͼ�������һ����ʱͼ�����ݿ�
		AcDbDatabase* pTempDb = NULL;	// ע�⣺����ǧ����new
		es = pSourceDb->wblock(pTempDb, destBlkDefId);
		assert(es == Acad::eOk);

		// �Ѹ���ʱͼ�����ݿ���Ϊ����뵽��ǰdwg
		es = pDestDb->insert(blockRefId, blkDefName, pTempDb, false);
		assert(es == Acad::eOk);
		delete pTempDb;
		pTempDb = NULL;
	}
	else
	{
		acutPrintf(TEXT("\n������Դ������, ������ָ�����ƵĿ���¼."));
	}
	pBlkTable->close();

	return blockRefId;
}

AcDbObjectId CopyBlockDefFromDwg(const TCHAR* fileName, const TCHAR* blkDefName)
{
	CDocLock docLock;

	AcDbObjectId blockRefId = AcDbObjectId::kNull;

	// ʹ��_SH_DENYNO������ͼ��(ֻ����)�����������û���д���ļ�
	AcDbDatabase* pSourceDwg = new AcDbDatabase(false);
#if (defined ARX_2018) || (defined ARX_2017)|| (defined ARX_2019)
	Acad::ErrorStatus es = pSourceDwg->readDwgFile(fileName, AcDbDatabase::kForReadAndAllShare);
#else
	Acad::ErrorStatus es = pSourceDwg->readDwgFile(fileName, _SH_DENYNO);
#endif //  ARX_2018

	
	if (es != Acad::eOk)
	{
		delete pSourceDwg;
		pSourceDwg = NULL;

		acutPrintf(TEXT("\n����dwgͼ�δ���, ͼ������: %s"), fileName);
		return blockRefId;
	}

	AcDbDatabase* pCurDb = acdbHostApplicationServices()->workingDatabase();
	blockRefId = CopyBlockDefFromDatabase(pSourceDwg, pCurDb, blkDefName);

	delete pSourceDwg;
	pSourceDwg = NULL;

	return blockRefId;
}

bool CloneObjects(AcDbObjectIdArray objList, AcDbObjectIdArray &objListCloned)
{
	Acad::ErrorStatus es;
	objListCloned.removeAll();

	AcDbBlockTable *pBlockTable;
	es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	AcDbObjectId  modelSpaceId = AcDbObjectId::kNull;
	pBlockTable->getAt(ACDB_MODEL_SPACE, modelSpaceId);
	pBlockTable->close();
	
	AcDbIdMapping idMap;
	es = acdbHostApplicationServices()->workingDatabase()->deepCloneObjects(objList, modelSpaceId, idMap);
	if (es!=Acad::eOk)
	{
		return false;
	}
	
	AcDbIdMappingIter iter(idMap);
	for (iter.start(); !iter.done(); iter.next())
	{
		AcDbIdPair idPair;
		iter.getMap(idPair);
		if (idPair.isCloned() && idPair.isPrimary())
		{
			objListCloned.append(idPair.value());
		}
	}

	return true;
}

AcDbObjectId InsertBlockRefFromDwg(const TCHAR* fileName, const TCHAR* blkDefName, const WCHAR *layoutname, AcGePoint3d origin)
{
	AcDbObjectId blockRefId = CopyBlockDefFromDwg(fileName, blkDefName);
	if (blockRefId == AcDbObjectId::kNull)
	{
		return AcDbObjectId::kNull;
	}
	
	AcDbObjectId entId = AcDbObjectId::kNull;
	int nSuc = MD2010_InsertBlockReference_Layout(layoutname, blkDefName, entId, origin);
	return entId;
}


bool SelectViewDir(eViewDir& p_viewDir)
{
	CString sDir;

	bool bSuc = false;
	do
	{
		bSuc = GetStringInput(_T("\n��Ҫѡ����Ŵ���ͼ����[ƽ��ͼ(T) ����ͼ(F)]<T>:"), sDir);
		if (bSuc == false)
			return false;

		sDir.Trim();
		sDir.MakeUpper();
		if (sDir.IsEmpty())
		{
			p_viewDir = E_VIEW_TOP;
			break;
		}

		if (sDir.Find(_T('T')) >= 0 || sDir.Find(_T("ƽ��")) >= 0)
		{
			p_viewDir = E_VIEW_TOP;
			break;
		}
		else if (sDir.Find(_T('F')) >= 0 || sDir.Find(_T("����")) >= 0)
		{
			p_viewDir = E_VIEW_FRONT;
			break;
		}

	} while (bSuc);

	return bSuc;
}

vAcDbObjectId SelectWindows_old(eViewDir p_view, bool p_bAllWindow)
{
	Acad::ErrorStatus es;
	vAcDbObjectId vIds;//��ǰѡ���ids

	acutPrintf(L"\n��ѡ���Ŵ�");

	ads_name sset;
	if (p_bAllWindow)
	{
		acedSSGet(_T("A"), NULL, NULL, NULL, sset);
	}
	else
	{
		acedSSGet(NULL, NULL, NULL, NULL, sset);
	}


	Adesk::Int32 length = 0;
	acedSSLength(sset, &length);
	for (int i = 0; i < length; i++)
	{
		ads_name ent;
		acedSSName(sset, i, ent);

		AcDbObjectId objId = 0;
		es = acdbGetObjectId(objId, ent);
		if (es!=Acad::eOk || objId==AcDbObjectId::kNull)
		{
			continue;
		}

#if 1
		vAcDbObjectId allWindowIds;
		TYCOM_DeepCycleBlockReferences(objId, p_view, TY_IsWindow, allWindowIds);
		if (allWindowIds.size() > 0)
		{
			vIds.insert(vIds.end(), allWindowIds.begin(), allWindowIds.end());
		}
#else
		if (objId != 0 && TY_IsWindow(objId, p_view))
		{
			vIds.push_back(objId);
		}
#endif
	}
	acedSSFree(sset);

	if (vIds.size() == 0)
	{
		acutPrintf(L"\nδѡ���Ŵ�\n");
	}
	else
	{
		CString info;
		info.Format(L"\n��ѡ����%d���Ŵ�\n", vIds.size());
		acutPrintf(info);
	}
	return vIds;
}

bool IsObjectExsit(AcDbObjectId p_id)
{
	AcDbEntity *pEntity;
	Acad::ErrorStatus es = acdbOpenObject(pEntity, p_id, AcDb::kForRead);
	if (es==Acad::eOk)
	{
		pEntity->close();
		return true;
	}

	return false;
}

bool IsFileExist(const CString & strFileName)
{
	_tfinddata_t fileinfo;
	intptr_t hFile = _tfindfirst(strFileName, &fileinfo);
	if (hFile != -1)
	{
		_findclose(hFile);
		return true;
	}
	else
	{
		_findclose(hFile);
		return false;
	}
}

bool TYCOM_DeleteBlkXData(AcDbBlockReference *pBlkRef, CString Key)
{
	CString strAppName(Key);
	//ע��Ӧ�ó�������
	acdbRegApp(Key);

	//�����������������
	struct resbuf *rb = acutBuildList(AcDb::kDxfRegAppName, strAppName, RTNONE);
	//Ӧ�ó�������

	//��������Ϊ�յĽ���������������ɾ����չ����
	pBlkRef->setXData(rb);

	acutRelRb(rb);

	pBlkRef->close();

	return true;
}

bool TYCOM_DeleteBlkXData(AcDbObjectId id, CString Key)
{
	AcDbBlockReference *pBlkRef = 0;
	AcDbEntity *pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, id, AcDb::kForWrite);
	if (es == Acad::eOk)
	{
		pBlkRef = AcDbBlockReference::cast(pEnt);
		if (pBlkRef)
		{
			TYCOM_DeleteBlkXData(pBlkRef, Key);
		}
		pEnt->close();
	}
	return 0;
}

int TYCOM_SaveBlkString(AcDbBlockReference *pBlkRef, CString Key, CString value)
{
	if (pBlkRef == 0)
		return -1;
	////��չ���ݵ�����
	struct resbuf* pRb;
	////ע��Ӧ�ó�������
	acdbRegApp(Key);
	CString strAppName(Key);
	//�����������������
	pRb = acutBuildList(AcDb::kDxfRegAppName, strAppName,
		AcDb::kDxfXdAsciiString, value,
		RTNONE);

	Acad::ErrorStatus es = pBlkRef->setXData(pRb);

	acutRelRb(pRb);
	pBlkRef->close();

	return 0;
}

int TYCOM_SaveBlkString(AcDbObjectId id, CString Key, CString value)
{
	AcDbBlockReference *pBlkRef = 0;
	AcDbEntity *pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, id, AcDb::kForWrite);
	if (es == Acad::eOk)
	{
		pBlkRef = AcDbBlockReference::cast(pEnt);
		if (pBlkRef)
		{
			TYCOM_SaveBlkString(pBlkRef, Key, value);
		}
		pEnt->close();
	}
	return 0;
}

int TYCOM_GetBlkString(AcDbObjectId id, CString Key, CString &value)
{
	AcDbBlockReference *pBlkRef = 0;
	AcDbEntity *pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, id, AcDb::kForWrite);
	if (es == Acad::eOk)
	{
		pBlkRef = AcDbBlockReference::cast(pEnt);
		if (pBlkRef)
		{
			TYCOM_GetBlkString(pBlkRef, Key, value);
		}
		pEnt->close();
	}
	return 0;
}

int TYCOM_GetBlkString(AcDbBlockReference *pBlkRef, CString Key, CString &value)
{
	if (pBlkRef == 0)
		return -1;

	struct resbuf* pRb;
	pRb = pBlkRef->xData(Key);

	if (pRb != NULL)
	{
		////����������ʾ���е���չ����
		struct resbuf* pTemp;
		pTemp = pRb;
		////����Ҫ����Ӧ�ó�������
		pTemp = pTemp->rbnext;
		value = pTemp->resval.rstring;
		acutRelRb(pRb);
	}
	else
	{
		acutPrintf(_T("\n��ʵ�岻�����κε���չ����"));
	}

	return 0;
}