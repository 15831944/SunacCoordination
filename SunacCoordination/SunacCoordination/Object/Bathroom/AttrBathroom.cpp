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

bool AttrBathroom::IsInstanceEqual(const AttrBathroom& p_att) const
{
	//�Ƚ�ԭ�ͱ��
	if (m_prototypeCode != p_att.m_prototypeCode)
		return false;

	//�Ƚϳߴ���Ŵ�λ��
	if (m_width != p_att.m_width)
		return false;
	if (m_height != p_att.m_height)
		return false;
	if (m_windowDoorPos != p_att.m_windowDoorPos)
		return false;
	if (m_hasPaiQiDao != p_att.m_hasPaiQiDao)
		return false;

	//�Ƚϳ�������
	if (m_taipenWidth != p_att.m_taipenWidth)
		return false;
	if (m_matongWidth != p_att.m_matongWidth)
		return false;
	if (m_guanXiWidth != p_att.m_guanXiWidth)
		return false;

	//�Ƚ�������
	if (m_isGuoBiao != p_att.m_isGuoBiao)
		return false;
	if (m_isGuoBiao)
	{
		if (m_floorRange != p_att.m_floorRange)
			return false;
		if (m_airVentOffsetX != p_att.m_airVentOffsetX)
			return false;
		if (m_airVentOffsetY != p_att.m_airVentOffsetY)
			return false;
	}
	else
	{
		if (m_airVentW != p_att.m_airVentW)
			return false;
		if (m_airVentH != p_att.m_airVentH)
			return false;
	}

	return true;
}
