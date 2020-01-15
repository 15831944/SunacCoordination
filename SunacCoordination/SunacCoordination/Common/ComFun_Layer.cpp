#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <dbxutil.h >
#include <brletrav.h>
#include <brbvtrav.h>
#include <geell3d.h>
#include <genurb3d.h>
#include <geextsf.h>
#include <gexbndsf.h>
#include <gecylndr.h>
#include <gecsint.h>
#include <dbxline.h>
#include <float.h>
#include <gecurv3d.h>
#include <geblok3d.h >
#include <brbstrav.h>
#include <brbctrav.h>
#include <brsftrav.h>
#include <algorithm>
#include <gecomp2d.h>
#include "ComFun_Def.h"
using namespace std;


/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetEntityLayer
* Description     : This function set layer of entity if layer exist.
* Note            : If layer dose not exist, this function will return error.
* Input           : entId :  Entity to set layer.
*                   strLayerName :  layer name to set to entity.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int JHCOM_SetEntityLayer(AcDbObjectId objID, CString strLayerName)
{
	AcDbEntity *pent;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pent,objID,AcDb::kForWrite);
	if(es == Acad::eOk)
	{
	    es = pent->setLayer(strLayerName);//use cstring directly! cstring-->ACHAR
	    pent->close();
	}
	return es;
}


/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetEntityLayer
* Description     : This function set layer of entity if layer exist.
* Note            : If layer dose not exist, this function will return error.
* Input           : entId :  Entity to set layer.
*                   layerID :  layer id.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int JHCOM_SetEntityLayer(AcDbObjectId objID, AcDbObjectId layerID)
{
	CString layerName = JHCOM_GetLayerName(layerID);
	if (layerName.GetLength()>0)
	{
		return JHCOM_SetEntityLayer(objID, layerName);
	}
	return 0;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetEntityLayer
* Description     : This function get layer name of entity.
* Note            : NULL.
* Input           : objID : Entity to get layer.
* Output          : NULL.
* Return          : String of layer name.
--------------------------------------------------------------------------*/
CString JHCOM_GetEntityLayer(AcDbObjectId objID)
{
	AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForRead);
	ACHAR* layname = pent->layer();
	CString strLayerName(layname);//use ACHAR directly! ACHAR-->cstring
	pent->close();
	return strLayerName;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetEntityLayerID
* Description     : This function get layer id of entity.
* Note            : NULL.
* Input           : objID : Entity to get layer id.
* Output          : NULL.
* Return          : layer id.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_GetEntityLayerID(AcDbObjectId objID)
{
	AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForRead);
	AcDbObjectId id = pent->layerId();
	pent->close();
	return id;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateNewLayer
* Description     : This function create a new layer with given new layer name.
* Note            : If layer name has been exist, will return 0.
* Input           : newlayername : new layer name for new created layer.
* Output          : NULL.
* Return          : new created layer id.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_CreateNewLayer(CString newname,Adesk::UInt16 colorIndex)
{
	// ��õ�ǰͼ�εĲ��
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl,AcDb::kForWrite);
	if (pLayerTbl->has(newname))
	{
		pLayerTbl->close();
		return 0;
	}
	//�����µĲ���¼
	AcDbLayerTableRecord *pLayerTableRecord = new AcDbLayerTableRecord;
	pLayerTableRecord->setName(newname);


	AcCmColor color;    
	color.setColorIndex(colorIndex);  
	pLayerTableRecord->setColor(color);  

	// ���½��Ĳ���¼��ӵ������
	AcDbObjectId id;
	pLayerTbl->add(id,pLayerTableRecord);

	//
	pLayerTableRecord->close();
	pLayerTbl->close();

	return id;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_DeleteLayer
* Description     : This function delete a layer with given new layer name.
* Note            : NULL.
* Input           : layerName : layer to delete.
* Output          : NULL.
* Return          : 0 : successful, 1 : fail.
--------------------------------------------------------------------------*/
int JHCOM_DeleteLayer(CString layerName)
{
	// ��õ�ǰͼ�εĲ��
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getLayerTable(pLayerTbl, AcDb::kForRead);
	// �ж��Ƿ����ָ�����ƵĲ���¼
	if (!pLayerTbl->has(layerName))
	{
		pLayerTbl->close();
		return 1;
	}
	// ���ָ������¼��ָ��
	AcDbLayerTableRecord *pLayerTblRcd;
	pLayerTbl->getAt(layerName, pLayerTblRcd, AcDb::kForWrite);
	pLayerTblRcd->erase(); // Ϊ�����á�ɾ�������
	pLayerTblRcd->close();
	pLayerTbl->close();

	return 0;
}

AcDbObjectId JHCOM_GetLayerID(CString layerName)
{
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);

	// �ж��Ƿ����ָ�����ƵĲ���¼
	if (!pLayerTbl->has(layerName))
	{
		pLayerTbl->close();
		return 0;
	}

	// ���ָ������¼��ָ��
	AcDbLayerTableRecord *pLayerTblRcd;
	pLayerTbl->getAt(layerName, pLayerTblRcd, AcDb::kForWrite);

	AcDbObjectId ids = pLayerTblRcd->objectId();
	pLayerTblRcd->close();
	pLayerTbl->close();

	return ids;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetLayerName
* Description     : This function get layer name of a given layer id.
* Note            : if this is not a layer id, return "".
* Input           : id : layer id to get layer name.
* Output          : NULL.
* Return          : layer name of layer id.
--------------------------------------------------------------------------*/
CString JHCOM_GetLayerName(AcDbObjectId id)
{
	AcDbObject* pObj;
	acdbOpenAcDbObject(pObj, id, AcDb::kForRead);
	AcDbLayerTableRecord *pLayerTableRecord = NULL;

	// �Ƿ��ǲ�����
	pLayerTableRecord = AcDbLayerTableRecord::cast(pObj);
	if (pLayerTableRecord == NULL)
		return L"";
	WCHAR * name;
	pLayerTableRecord->getName(name);
	pLayerTableRecord->close();
	return name;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetAllLayers
* Description     : This function get all layer names and ids of current database.
* Note            : NULL.
* Input           : NULL.
* Output          : vstr : all layer names.
*                   vids : all layer ids with same index of vstr
* Return          : layer size.
--------------------------------------------------------------------------*/
int JHCOM_GetAllLayers(vCString &vstr, vAcDbObjectId &vids)
{
	// ��ò��ָ��
	AcDbLayerTable *pLayerTbl;
	AcDbLayerTableRecord *pLayerTblRcd;
	AcDbObjectId id;
	int num = 0;
	acdbHostApplicationServices()->workingDatabase()
		->getLayerTable(pLayerTbl, AcDb::kForRead);

	// ʹ�ñ���������ÿһ������¼
	AcDbLayerTableIterator *pItr;
	pLayerTbl->newIterator(pItr);
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getRecord(pLayerTblRcd, AcDb::kForRead);
		// ���ͼ�����Ϣ
		WCHAR *layerName;
		pLayerTblRcd->getName(layerName);
		vstr.push_back(layerName);
		id = pLayerTblRcd->objectId();
		vids.push_back(id);
		pLayerTblRcd->close();
		num++;
	}
	delete pItr;
	pLayerTbl->close();
	return num;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetLayerCount
* Description     : This function get layer count of current database.
* Note            : NULL.
* Input           : NULL.
* Output          : NULL.
* Return          : layer size.
--------------------------------------------------------------------------*/
int JHCOM_GetLayerCount()
{
	// ��ò��ָ��
	AcDbLayerTable *pLayerTbl;
	int num = 0;
	acdbHostApplicationServices()->workingDatabase()
		->getLayerTable(pLayerTbl, AcDb::kForRead);

	// ʹ�ñ���������ÿһ������¼
	AcDbLayerTableIterator *pItr;
	pLayerTbl->newIterator(pItr);
	for (pItr->start(); !pItr->done(); pItr->step())
		num++;
	delete pItr;
	pLayerTbl->close();
	return num;
}


/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetCurrentLayer
* Description     : This function set current layer by layer name.
* Note            : NULL.
* Input           : name : layer name to set to current layer.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int JHCOM_SetCurrentLayer(CString name)
{
	AcDbObjectId id = JHCOM_GetLayerID(name);
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->setClayer(id);
	return 0;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetCurrentLayer
* Description     : This function get current layer name and id.
* Note            : NULL.
* Input           : name : layer name of current layer.
* Output          : NULL.
* Return          : current layer id.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_GetCurrentLayer(CString &name)
{
	AcDbObjectId id;
	id = acdbHostApplicationServices()->workingDatabase()->clayer();
	name = JHCOM_GetLayerName(id);
	return id;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetLayerColor
* Description     : This function set layer color for given layer name.
* Note            : NULL.
* Input           : layerName : layer name to set to color.
*                   newcolor : new color to set to layer.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int JHCOM_SetLayerColor(CString layerName,AcCmColor newcolor)
{
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);

	// �ж��Ƿ����ָ�����ƵĲ���¼
	if (!pLayerTbl->has(layerName))
	{
		pLayerTbl->close();
		return 0;
	}

	// ���ָ������¼��ָ��
	AcDbLayerTableRecord *pLayerTblRcd;
	pLayerTbl->getAt(layerName, pLayerTblRcd, AcDb::kForWrite);

	AcCmColor oldcolor = pLayerTblRcd->color();

	if (oldcolor != newcolor)
		pLayerTblRcd->setColor(newcolor);

	pLayerTblRcd->close();
	pLayerTbl->close();

	return 0;
}

//����ͼ���ӡ״̬
int JHCOM_SetLayerPrint(CString layerName, bool isPrint)
{
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);

	// �ж��Ƿ����ָ�����ƵĲ���¼
	if (!pLayerTbl->has(layerName))
	{
		pLayerTbl->close();
		return 0;
	}

	// ���ָ������¼��ָ��
	AcDbLayerTableRecord *pLayerTblRcd;
	pLayerTbl->getAt(layerName, pLayerTblRcd, AcDb::kForWrite);
	
	Acad::ErrorStatus es = pLayerTblRcd->setIsPlottable(isPrint);

	pLayerTblRcd->close();
	pLayerTbl->close();

	return 0;
}


vector<AcDbObjectId> GetAllEntityId(CString layername)
{
	vector<AcDbObjectId> entIds;
	bool bFilterlayer = false;
	AcDbObjectId layerId;

	//��ȡָ��ͼ�����ID
	if (layername != L"")
	{
		AcDbLayerTable *pLayerTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTbl, AcDb::kForRead);
		if (!pLayerTbl->has(layername))
		{
			pLayerTbl->close();
			return entIds;
		}
		pLayerTbl->getAt(layername, layerId);
		pLayerTbl->close();
		bFilterlayer = true;
	}

	//��ÿ��
	AcDbBlockTable *pBlkTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlkTbl, AcDb::kForRead);

	//����¼
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();

	//���������������η���ģ�Ϳռ��е�ÿһ��ʵ��
	AcDbBlockTableRecordIterator *it = NULL;
	pBlkTblRcd->newIterator(it);
	for (it->start(); !it->done(); it->step())
	{
		AcDbEntity *pEnt = NULL;
		Acad::ErrorStatus es = it->getEntity(pEnt, AcDb::kForRead);
		if (es == Acad::eOk)
		{
			if (bFilterlayer)//����ͼ��
			{
				if (pEnt->layerId() == layerId)
				{
					entIds.push_back(pEnt->objectId());
				}
			}
		}
		pEnt->close();
	}
	delete it;
	pBlkTblRcd->close();
	return entIds;
}