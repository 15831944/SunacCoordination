/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrObject.h"
#include "../Common/ComFun_Sunac.h"
#include "../Tool/DocLock.h"


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrObject, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEOBJECT, ZffCustomObjectDBOBJECT);

AttrObject::AttrObject()
{
	m_version = 0;
	m_isJiTuan = true;
	m_isDynamic = true;

	m_instanceCodeId = AcDbObjectId::kNull;

	m_viewDir = E_VIEW_TOP;
}

AttrObject::~AttrObject()
{
}

AttrObject::AttrObject(const AttrObject &other)
{
	*this = other;
}

AttrObject& AttrObject::operator=(const AttrObject &rhs)
{
	m_version = rhs.m_version;

	m_prototypeCode = rhs.m_prototypeCode;
	m_file = rhs.m_file;
	m_isDynamic = rhs.m_isDynamic;
	m_isJiTuan = rhs.m_isJiTuan;
	m_quyuId = rhs.m_quyuId;
	m_quyuName = rhs.m_quyuName;

	m_instanceCode = rhs.m_instanceCode;
	m_instanceCodeId = rhs.m_instanceCodeId;
	//SetInstanceCode(rhs.m_instanceCode);//ʵ�����

	m_floorInfo = rhs.m_floorInfo;
	return *this;
}

void AttrObject::Clone(const AttrObject& p_src) //��¡ֻ��¡���ݣ�����¡������ϵ����m_instanceCodeId
{
	m_version = p_src.m_version;		//�ļ��汾 ���л�ʱʹ��
										
	//ԭ������
	m_prototypeCode = p_src.m_prototypeCode;//ԭ�ͱ�� //����ԭ��������ԭ�ͱ��
	m_file = p_src.m_file;	//ԭ���ļ���,����.dwg����������·��
	m_isDynamic = p_src.m_isDynamic;		//�Ƿ�̬��
	m_isJiTuan = p_src.m_isJiTuan;		//�Ƿ���
	m_quyuId = p_src.m_quyuId;			//����ID
	m_quyuName = p_src.m_quyuName;			//��������

	//////////////////////////////////////////////////////////////////////////
	//ʵ������
	SetInstanceCode(p_src.m_instanceCode);//ʵ�����

	m_floorInfo = p_src.m_floorInfo; //¥����Ϣ
}

Acad::ErrorStatus AttrObject::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AcDbObject::dwgInFields(filer)) != Acad::eOk) 
	{
		return es;
	}

	filer->readItem(&m_version);

	AcString tempStr;
    filer->readString(tempStr);
	m_prototypeCode = tempStr.kACharPtr();

	filer->readString(tempStr);
	m_instanceCode = tempStr.kACharPtr();

	dwgInFileInfo(filer, m_file);

	filer->readItem(&m_isDynamic);

	filer->readItem(&m_isJiTuan);

	filer->readString(tempStr);
	m_quyuName = tempStr.kACharPtr();

	filer->readString(tempStr);
	m_quyuId = tempStr.kACharPtr();

	if (m_version >= 6) //20200324 �汾6��¥����Ϣ��AttrWindow�Ƶ����࣬�Ա�֧�����е�����
	{
		filer->readString(tempStr);
		CString sFloors = tempStr.kACharPtr();
		m_floorInfo.SetFloors(sFloors);

		double floorHeight;
		filer->readItem(&floorHeight);
		m_floorInfo.SetFloorHeight(floorHeight);

		filer->readItem((Adesk::UInt32*)&m_viewDir);
	}

	return filer->filerStatus();
}

Acad::ErrorStatus AttrObject::dwgInFileInfo(AcDbDwgFiler* filer, CDwgFileInfo& pFileInfo)
{
	AcString tempStr;
	filer->readString(tempStr);
	pFileInfo.fileName = tempStr.kACharPtr();
	filer->readString(tempStr);
	pFileInfo.previewFile = tempStr.kACharPtr();
	filer->readItem(&pFileInfo.id);
	filer->readItem(&pFileInfo.PreviewID);
	return Acad::eOk;
}

Acad::ErrorStatus AttrObject::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AcDbObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	Adesk::Int32 version = FILE_VERSION;
	filer->writeItem(version);

	filer->writeItem(m_prototypeCode);

	filer->writeItem(m_instanceCode);

	dwgOutFileInfo(filer, m_file);

	filer->writeItem(m_isDynamic);

	filer->writeItem(m_isJiTuan);

	filer->writeItem(m_quyuName);

	filer->writeItem(m_quyuId);

	//FILE_VERSION 6 ����
	{
		filer->writeItem(m_floorInfo.GetFloors());
		filer->writeItem(m_floorInfo.GetFloorHeight());

		filer->writeItem((Adesk::UInt32)m_viewDir);
	}

	return filer->filerStatus();
}

Acad::ErrorStatus AttrObject::dwgOutFileInfo(AcDbDwgFiler* filer, const CDwgFileInfo& pFileInfo) const
{
	filer->writeItem(pFileInfo.fileName);
	filer->writeItem(pFileInfo.previewFile);
	filer->writeItem(pFileInfo.id);
	filer->writeItem(pFileInfo.PreviewID);
	return Acad::eOk;
}

bool AttrObject::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	//���ñȽ�version
	if (m_prototypeCode != other->m_prototypeCode)
		return false;

	if (m_instanceCode != other->m_instanceCode)
		return false;

	//if (m_name != other->m_name)
	//	return false;

	if (m_isJiTuan != other->m_isJiTuan)
		return false;

	if (m_quyuName != other->m_quyuName)
		return false;

	//if (m_type != other->m_type)
	//	return false;

	if (m_isDynamic != other->m_isDynamic)
		return false;

	return true;

	/*return (m_id == other->m_id && 
		m_name == other->m_name && 
		m_isJiTuan == other->m_isJiTuan && 
		m_quyuName == other->m_quyuName && 
		m_type == other->m_type && 
		m_isDynamic == other->m_isDynamic
		);*/
}

void AttrObject::SetInstanceCode(CString  bianHao)
{
	if (bianHao.CompareNoCase(m_instanceCode)==0)
	{
		return;
	}
	m_instanceCode = bianHao;

	//���������֣���ͬʱ��������
	if (m_instanceCodeId!=AcDbObjectId::kNull)
	{
		CDocLock dokLock;

		AcDbEntity * pEnt = NULL;
		if (Acad::eOk==acdbOpenObject(pEnt, m_instanceCodeId,  AcDb::kForWrite))
		{
			AcDbText* pTextEnt = AcDbText::cast(pEnt);
			if (pTextEnt!=NULL)
			{
				pTextEnt->setTextString(m_instanceCode);
			}

			pEnt->close();
		}
	}
}

void AttrObject::SetInstanceCodeObjectId(AcDbObjectId p_id)
{
	//m_instanceCodeId = p_id; //20200106 ���п���������ſ��ܻ���־����������˱�ŵ�����id���ٺ����԰�
}