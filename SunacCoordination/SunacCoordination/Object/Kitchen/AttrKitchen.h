
#pragma once

#include "dbmain.h"
#include "../AttrObject.h"
#include "..\PrototypeCommonDef.h"
#include "..\KitchenBathroomProp.h"

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

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual eRCType GetType() {return KITCHEN;}
	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��
	bool IsInstanceEqual(const AttrKitchen& p_att) const;

public:
	//ԭ��������ԣ���ʼ��ʱ��д
	CString m_kitchenType;		//��������
	eWindowDoorPos m_windowDoorPos;	//�Ŵ�λ�ù�ϵ
	bool m_hasPaiQiDao;			//�Ƿ���������

	//ԭ�����ݣ���¼ԭ�͵�֧�ֳߴ���Ŵ���ʼλ�ã���ʼ��ʱ��д
	CKitchenBathroomProp m_prop;

	//������ԭ�ͺ���д
	double m_width;				//����
	double m_height;			//����

	//����ʱ��д
	CString m_shuiPenType;		//ˮ������
	CString m_bingXiangType;	//��������
	CString m_zaoTaiType;		//��̨��� 

	//������������ã�����ʱ��д
	E_KITCHEN_FLOOR_RANGE m_floorRange;	//¥��ѡ��
	bool m_isGuoBiao;			//�Ƿ����
	double m_airVentW;			//����������
	double m_airVentH;			//���������
	double m_airVentOffsetX;	//������ƫ��x
	double m_airVentOffsetY;	//������ƫ��y

	//����ʱ��д
	bool m_isMirror;			//����

	//////////////////////////////////////////////////////////////////////////
	//ԭ���ļ�����
	double m_minWidth;//��С����
	double m_maxwidth;//��󿪼�
	double m_maxHeight;//������
	double m_minHeight;//��С����
};
