#include "StdAfx.h"
#include "DocumentDataSerialize.h"
#include "Object/WindowDoor/WindowAutoName.h"
#include "Object/Kitchen/KitchenAutoName.h"
#include "Object/Bathroom/BathroomAutoName.h"
#include "Src/DocumentData.h"

void CDocumentDataSerialize::InitSerialize()
{
	AcDbDictionary *pNameObjDict = NULL;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameObjDict, AcDb::kForWrite);
	if (es!=Acad::eOk)
	{
		return;
	}

	AcDbObjectId dictObjId = AcDbObjectId::kNull;
	es = pNameObjDict->getAt(cSunacDocmentDataKey, dictObjId);
	if (es!=Acad::eOk)
	{
		CDocumentDataSerialize* pAutoNameSerialize = new CDocumentDataSerialize();
		pNameObjDict->setAt(cSunacDocmentDataKey, pAutoNameSerialize, dictObjId);
		pAutoNameSerialize->close();


		CDocumentData* pCurDocData = CDocumentFactory::Instance().GetCurDocData();
		if (pCurDocData!=NULL)
		{
			pCurDocData->m_bLoad = true; //����ӵ�Ҳ�������Ϊ
		}
	}

	pNameObjDict->close();
}

 //ACRX_DXF_DEFINE_MEMBERS(CLASS_NAME,PARENT_CLASS,DWG_VERSION,\
 //             MAINTENANCE_VERSION,PROXY_FLAGS,DXF_NAME,APP) \

ACRX_DXF_DEFINE_MEMBERS(CDocumentDataSerialize, AcDbObject,
						AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						0, SunacSerialize, "SunacCAD");

//��Ҫ�ڳ�ʼ����initAppʱ�����CDocumentDataSerialize::rxInit();  

CDocumentDataSerialize::CDocumentDataSerialize()
{
	m_version = 1;
}
CDocumentDataSerialize::~CDocumentDataSerialize()
{
}


Acad::ErrorStatus CDocumentDataSerialize::dwgInFields(AcDbDwgFiler* pFiler)
{
	CDocumentData* pCurDocData = CDocumentFactory::Instance().GetCurDocData();
	if (pCurDocData == NULL || pCurDocData->m_bLoad) //��ǰ�ĵ�������ֻ�ڴ��ĵ�ʱ����һ�Σ����ڿ����Ͳ����ļ���Ϊ��ʱ����˺������������ȡdwg���ĵ�����
	{
		return pFiler->filerStatus();
	}
	
	//1. ���ø���ĺ���
	assertWriteEnabled();
	Acad::ErrorStatus es = AcDbObject::dwgInFields(pFiler);
	if (es != Acad::eOk) 
		return es;

	//2. ��ȡ���ݰ汾��
	pFiler->readItem(&m_version);
	
	//3. ��ȡ����
	pCurDocData->ReadFromDwg(pFiler, m_version);
	
	//4. ����
	return pFiler->filerStatus();
}

Acad::ErrorStatus CDocumentDataSerialize::dwgOutFields(AcDbDwgFiler* pFiler)const
{
	//1. ���ø���ĺ���
	assertReadEnabled();
	Acad::ErrorStatus es= AcDbObject::dwgOutFields(pFiler);
	if (es  != Acad::eOk)
		return es;

	//2. д�����ݰ汾��
	Adesk::UInt32 nVersion = FILE_VERSION;
	pFiler->writeItem(nVersion);

	//3. д������
	CDocumentFactory::Instance().GetCurDocData()->WriteToDwg(pFiler);
	
	//4. ����
	return pFiler->filerStatus();
}
