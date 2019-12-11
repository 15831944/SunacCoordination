#include "StdAfx.h"
#include "AutoNameSerialize.h"
#include "Object/WindowDoor/WindowAutoName.h"
#include "Object/Kitchen/KitchenAutoName.h"
#include "Object/Bathroom/BathroomAutoName.h"
#include "Src/DocumentData.h"

static const Adesk::UInt32  c_nAutoNameVersion = 1; //�����ã�ÿ���޸����л�����+1�����汾�Ų�һ�£��򲻶�ȡ

void InitSerialize()
{
	AcDbDictionary *pNameObjDict = NULL;
	AcDbObjectId dictObjId;

	acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameObjDict, AcDb::kForWrite);

	CAutoNameSerialize* pAutoNameSerialize = new CAutoNameSerialize();
	pNameObjDict->setAt(DWG_AutoName_KEY, pAutoNameSerialize, dictObjId);
	pAutoNameSerialize->close();

	pNameObjDict->close();
}

ACRX_DXF_DEFINE_MEMBERS(CAutoNameSerialize, AcDbObject,
						AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						0, AutoNameSerialize, "Sunac");

//��Ҫ�ڳ�ʼ����initAppʱ�����CAutoNameSerialize::rxInit();  

CAutoNameSerialize::CAutoNameSerialize()
{
	m_version = 1;
}
CAutoNameSerialize::~CAutoNameSerialize()
{
}


Acad::ErrorStatus CAutoNameSerialize::dwgInFields(AcDbDwgFiler* pFiler)
{
	//1. ���ø���ĺ���
	assertWriteEnabled();
	Acad::ErrorStatus es = AcDbObject::dwgInFields(pFiler);
	if (es != Acad::eOk) 
		return es;

	//2. ��ȡ���ݰ汾��
	pFiler->readItem(&m_version);
	
	//3. ��ȡ����
	GetWindowAutoName()->ReadFromDwg(pFiler);
	GetKitchenAutoName()->ReadFromDwg(pFiler);
	GetBathroomAutoName()->ReadFromDwg(pFiler);
	
	//4. ����
	return pFiler->filerStatus();
}

Acad::ErrorStatus CAutoNameSerialize::dwgOutFields(AcDbDwgFiler* pFiler)const
{
	//1. ���ø���ĺ���
	assertReadEnabled();
	Acad::ErrorStatus es= AcDbObject::dwgOutFields(pFiler);
	if (es  != Acad::eOk)
		return es;

	//2. д�����ݰ汾��
	Adesk::UInt32 nVersion = c_nAutoNameVersion;
	pFiler->writeItem(nVersion);

	//3. д������
	GetWindowAutoName()->WriteToDwg(pFiler);
	GetKitchenAutoName()->WriteToDwg(pFiler);
	GetBathroomAutoName()->WriteToDwg(pFiler);
	
	//4. ����
	return pFiler->filerStatus();
}
