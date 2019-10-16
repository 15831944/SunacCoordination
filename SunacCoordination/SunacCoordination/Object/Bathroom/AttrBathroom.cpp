/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrBathroom.h"
#include "..\..\WebIO\WebIO.h"


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrBathroom, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEBathroom, ZffCustomObjectDBBathroom);

AttrBathroom::AttrBathroom()
{
	//������ԭ�ͺ���д
	m_width = 0;				//����
	m_height = 0;			//����

	//����ʱ��д
	m_taipenWidth = "";		//̨����
	m_matongWidth = "";		//��Ͱ���
	m_guanXiWidth = 0;		//��ϴ�����

	//������������ã�����ʱ��д
	m_isGuoBiao = 0;			//�Ƿ����
	m_airVentW = 0;			//����������
    m_airVentH = 0;			//���������
	m_airVentOffsetX = 0;	//������ƫ��x
	m_airVentOffsetY = 0;	//������ƫ��y

}

AttrBathroom::~AttrBathroom()
{

}

Acad::ErrorStatus AttrBathroom::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) {
		return es;
	}

	return filer->filerStatus();
}

Acad::ErrorStatus AttrBathroom::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AttrObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	return filer->filerStatus();
}

bool AttrBathroom::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	AttrBathroom * pRealObj = dynamic_cast<AttrBathroom *>(other);
	if (pRealObj == 0)
		return false;

	if (!AttrObject::isEqualTo(other))
		return false;

	return true;
	/*return (m_openType == pRealObj->m_openType
		);*/
}
