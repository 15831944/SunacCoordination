
#pragma once

#include "dbmain.h"
#include "..\AttrObject.h"
#include "../../Sunac_def.h"
#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW "ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW"
#endif



//门窗取值参数类型
typedef enum eWindowDimType
{
	SINGLE,//固定值
	MULTI,//值系列
	SCOPE,//范围
	UNLIMIT,//不限
	CALC,//公式
	NOVALUE //无
}eWindowDimType;

eWindowDimType ToEWindowType(CString type);


class CWindowsDimData
{
public:
	CString sCodeName;		//代号 W W1 W2 W3 H H1 H2 H3
	eWindowDimType type;	//值类型
	vdouble valueOptions;	//值选项存储单个值、系列
	double minValue;
	double maxValue;
	CString sFomula;	//公式
	double defaultValue;
	CString prompt;  //说明

	double value; //具体实例中的取值

public:
	CWindowsDimData();
	bool operator==(const CWindowsDimData &rhs) const;

	bool IsValueEqual(const CWindowsDimData &rhs)const;

};

typedef std::vector<CWindowsDimData> vWindowDimData;

//////////////////////////////////////////////////////////////////////////


class DLLIMPEXP  AttrWindow : public AttrObject  //门窗共用AttrWindow
{
public:
	ACRX_DECLARE_MEMBERS(AttrWindow);

	AttrWindow();
	virtual ~AttrWindow();

	//AttrWindow(const AttrWindow &other);
	//AttrWindow & operator=(const AttrWindow &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual eRCType GetType() {return WINDOW;}
	virtual bool isEqualTo(AttrObject*other);//窗型是否一致
	virtual bool IsPrototypeEqual(const AttrWindow& p_att);
	
	//////////////////////////////////////////////////////////////////////////
	const CWindowsDimData* GetDimData(CString p_sCode)const;
	void SetDimData(const CWindowsDimData& p_dataOut);
	
	//TODO
	double GetH();
	double GetH1();
	double GetH2();
	double GetW();
	double GetW1();
	double GetW2();
	double GetW3();
	double GetA(); //塞缝尺寸

	bool SetH(double newValue);
	bool SetH1(double newValue);
	bool SetH2(double newValue);
	bool SetW(double newValue);
	bool SetW1(double newValue);
	bool SetW2(double newValue);
	bool SetW3(double newValue);
	bool SetA(double newValue);//塞缝尺寸

protected:
	vWindowDimData m_dimData; //存储W/W1/W2/W3   H/H1/H2/H3 R的尺寸数据

public:
	CString m_openType;		//开启类型

	CDwgFileInfo m_frontViewFile;	//原型立面文件, 展开图用基类的m_fileName
	CDwgFileInfo m_topViewFile;		//原型俯视图文件
	CDwgFileInfo m_leftViewFile;		//原型侧视图文件
	
	CString m_tongFengFormula;//通风量计算公式,主要用于动态原型
	double m_tongFengQty;	//通风量

	Adesk::Int32 m_openQty;	//开启扇数量
	CString m_gongNengquType;//功能区类型

	bool m_isZhuanJiao;		//是否转角窗
	bool m_isMirrorWindow;	//是否对称窗型 

	//////////////////////////////////////////////////////////////////////////
	//算量相关
	vSCalcData m_calFormulas;

	//////////////////////////////////////////////////////////////////////////
	//以下属性为具体外窗插入时才设置,单个窗户实例的属性，非原型属性
	//视图属性
	bool m_isMirror;//是否镜像
	eViewDir m_viewDir;//视图方向

	bool   m_isBayWindow;	 //是否凸窗
	double m_wallDis;		 //外墙距离

	//////////////////////////////////////////////////////////////////////////
	//二次深化属性
	double m_plugslotSize;		//塞缝尺寸

};

typedef std::vector<AttrWindow> vAttrWindow;
