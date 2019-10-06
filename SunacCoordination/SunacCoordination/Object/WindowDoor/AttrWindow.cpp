/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrWindow.h"
#include "../../Common/ComFun_Sunac.h"


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrWindow, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEWINDOW, ZffCustomObjectDBWINDOW);
AttrWindow::AttrWindow()
{
	m_openType = L"";
	m_openQty = 1;
	m_isZhuanJiao = false;//是否转角窗
	m_isMirrorWindow = false;

	m_minWid = 0;//宽度尺寸最小值
	m_maxWid = 0;//宽度尺寸最大值
	m_tongFengFormula = L"";//通风量计算公式
	m_staticTongFengQty = 0.0;
	m_staticWidth = 0;
	m_staticHeight = 0;

	//////////////////////////////////////////////////////////////////////////
	m_isMirror = false;
	m_viewDir = E_VIEW_FRONT;
	m_isBayWindow = false;
	m_openWindowSize = 0;
	m_windowH2 = 0;
	m_plugslotSize = 0;
	m_wallDis = 0.0;
}

AttrWindow::~AttrWindow()
{

}

//AttrWindow::AttrWindow(const AttrWindow &other) : AttrObject(other)
//{
//	*this = other;
//}
//
//AttrWindow & AttrWindow::operator=(const AttrWindow &other)
//{
//	AttrObject::operator=(other);
//
//	m_openType = other.m_openType;
//	m_openQty = other.m_openQty;
//	m_isZhuanJiao = other.m_isZhuanJiao;
//	m_minWid = other.m_minWid;
//	m_maxWid = other.m_maxWid;
//	m_tongFengFormula = other.m_tongFengFormula;
//	m_staticTongFengQty = other.m_staticTongFengQty;
//
//	m_isMirror = other.m_isMirror;
//	m_viewDir = other.m_viewDir;
//	m_wallDis = other.m_wallDis;
//
//	////////////////////////////
//	m_id = other.m_id;					//序号
//	m_prototypeCode = other.m_prototypeCode;		//原型编号
//	m_name = other.m_name;		//原型文件
//	m_scopeOfApplication = other.m_scopeOfApplication;	//适用范围
//	m_Dynamic = other.m_Dynamic;			//是否动态
//	m_functionType = other.m_functionType;		//功能区类型
//	m_openType = other.m_openType;			//开启类型
//	m_openQty = other.m_openQty;			//开启数量
//	//widthMin = other.widthMin;			//宽度最小值
//	//widthMax = other.widthMax;			//宽度最大值
//	m_tongFengFormula = other.m_tongFengFormula;	//通风量公式	
//	m_width = other.m_width;		//静态宽度	
//	m_height = other.m_height;		//静态高度	
//	m_staticTongFengQty = other.m_staticTongFengQty;	//静态通风量	
//	m_topViewFile = other.m_topViewFile;	//原型平面文件
//	m_leftViewFile = other.m_leftViewFile;//原型俯视图文件
//
//	m_dimData = other.m_dimData;
//
//	return *this;
//}

Acad::ErrorStatus AttrWindow::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) 
		return es;

	ACHAR *tempStr = new ACHAR[SUNAC_COMMON_STR_LEN];
	filer->readItem(&tempStr);
	m_openType = CString(tempStr);

	filer->readItem(&m_openQty);
	filer->readItem(&m_isZhuanJiao);
	filer->readItem(&m_minWid);
	filer->readItem(&m_maxWid);

	filer->readItem(&tempStr);
	m_tongFengFormula = CString(tempStr);

	filer->readItem(&m_staticTongFengQty);


	//TODO 把所有的属性读取补充完整

	delete [] tempStr;


	return filer->filerStatus();
}

Acad::ErrorStatus AttrWindow::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AttrObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	filer->writeItem(m_openType);
	filer->writeItem(m_openQty);
	filer->writeItem(m_isZhuanJiao);
	filer->writeItem(m_minWid);
	filer->writeItem(m_maxWid);
	filer->writeItem(m_tongFengFormula);
	filer->writeItem(m_staticTongFengQty);

	//TODO 把所有的属性读取补充完整

	return filer->filerStatus();
}

bool AttrWindow::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	AttrWindow * pRealObj = dynamic_cast<AttrWindow *>(other);
	if (pRealObj == 0)
		return false;

	if (!AttrObject::isEqualTo(other))
		return false;

	//TODO 变量比较完整

	return (m_openType == pRealObj->m_openType &&
		m_openQty == pRealObj->m_openQty &&
		m_isZhuanJiao == pRealObj->m_isZhuanJiao &&
		m_minWid == pRealObj->m_minWid &&
		m_maxWid == pRealObj->m_maxWid &&
		m_tongFengFormula == pRealObj->m_tongFengFormula &&
		m_staticTongFengQty == pRealObj->m_staticTongFengQty &&
		m_isMirror == pRealObj->m_isMirror &&
		m_viewDir == pRealObj->m_viewDir &&
		m_wallDis == pRealObj->m_wallDis
		);
}

