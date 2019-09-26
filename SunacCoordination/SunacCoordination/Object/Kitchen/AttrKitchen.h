
#pragma once

#include "dbmain.h"
#include "../AttrObject.h"
#include "..\PrototypeCommonDef.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN "ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN"
#endif



class DLLIMPEXP  AttrKitchen : public AttrObject
{
public:
	ACRX_DECLARE_MEMBERS(AttrKitchen);

	// Constructor / Destructor
	AttrKitchen();
	virtual ~AttrKitchen();

	AttrKitchen(const AttrKitchen &other);
	AttrKitchen & operator=(const AttrKitchen &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual eRCType GetType() {return KITCHEN;}
	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��

public:
	bool m_hasPaiQiDao;//�Ƿ���������
	bool m_isGuoBiao;//������
	E_FLOOR_RANGE m_floorRange;//¥��ѡ��
	double m_airVentW;//����������
	double m_airVentH;//���������
	double m_airVentOffsetX;//������ƫ��x
	double m_airVentOffsetY;//������ƫ��y

	bool m_isMirror;//����

	CString m_kitchenType;//��������
	CString m_windowDoorPos;//�Ŵ�λ�ù�ϵ

	CString m_shuiPenType;//ˮ������
	CString m_bingXiangType;//��������
	CString m_zaoTaiType;//��̨��� 
	double m_width;//���� ���
	double m_height;//��� ����
};
