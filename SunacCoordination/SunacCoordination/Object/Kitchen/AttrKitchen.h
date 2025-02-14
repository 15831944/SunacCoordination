
#pragma once

#include "dbmain.h"
#include "../AttrObject.h"
#include "..\PrototypeCommonDef.h"
#include "..\KitchenBathroom\KitchenBathroomProp.h"

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

	virtual eRCType GetType() {return S_KITCHEN;}
	virtual bool isEqualTo(const AttrObject*other)const;//基础数据一致
	bool IsInstanceEqual(const AttrKitchen& p_att) const;

	CString GetPrototypeCode() const;

public:
	//原型相关属性，初始化时填写
	CString m_kitchenType;		//厨房类型
	eWindowDoorPos m_windowDoorPos;	//门窗位置关系
	bool m_hasPaiQiDao;			//是否含有排气道

	//原型数据，记录原型的支持尺寸和门窗初始位置，初始化时填写
	CKitchenBathroomProp m_prop;

	//搜索到原型后填写
	double m_width;				//开间
	double m_height;			//进深

	//插入时填写
	CString m_shuiPenType;		//水盆类型
	CString m_bingXiangType;	//冰箱类型
	CString m_zaoTaiType;		//灶台宽度 

	//排气道相关设置，插入时填写
	bool m_isGuoBiao;			//是否国标
	E_KITCHEN_FLOOR_RANGE m_floorRange;	//楼层选项
	double m_airVentW;			//排气道长度
	double m_airVentH;			//排气道宽度
	double m_airVentOffsetX;	//排气道偏移x
	double m_airVentOffsetY;	//排气道偏移y

	//插入时填写
	int m_angle;				//逆时针旋转角度
	bool m_isMirror;			//镜像

	//////////////////////////////////////////////////////////////////////////
	//原型文件属性
	//double m_minWidth;//最小开间
	//double m_maxWidth;//最大开间
	//double m_maxHeight;//最大进深
	//double m_minHeight;//最小进深
};
