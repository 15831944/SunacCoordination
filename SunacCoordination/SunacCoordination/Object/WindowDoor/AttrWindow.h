
#pragma once

#include "dbmain.h"
#include "..\AttrObject.h"
#include "../../Sunac_def.h"
#include "../WindowStatistic/AluminumSeries.h"
#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW "ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW"
#endif

//#define INIT_HANLDLE_LATER_FOR_DWGIN //dwgin时使用handle,使用时才通过handle找到objectId

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
	CString prompt;  //说明
	double defaultValue;
	double value; //具体实例中的取值

public:
	CWindowsDimData();
	bool operator==(const CWindowsDimData &rhs) const;
	bool IsValueEqual(const CWindowsDimData &rhs)const;
	bool IsParaEqual(const CWindowsDimData &rhs)const;
	
	//排序用
	bool operator<(const CWindowsDimData &rhs) const;

	bool SetDefaultValue(double p_value);
	bool SetValue(double p_value);
};

//////////////////////////////////////////////////////////////////////////

struct CWindowMaterial	//门窗材料系列
{
	double heatCoeff;		//隔热系数
	CString sAluminumSerial; //型材系列
	CString sGlassSerial;
	bool bHasAuxiliaryFrame; //是否有附框
	CString sAuxiliaryFrame; //附框系列

	CWindowMaterial()
	{
		heatCoeff = 2.4;
		bHasAuxiliaryFrame = false;
	}
};



//////////////////////////////////////////////////////////////////////////


class DLLIMPEXP  AttrWindow : public AttrObject  //门窗共用AttrWindow
{
public:
	ACRX_DECLARE_MEMBERS(AttrWindow);

	AttrWindow();
	virtual ~AttrWindow();

	static AttrWindow* GetWinAtt(AcDbObjectId p_id, bool bReadMode);
	static CString GetWinInstanceCode(AcDbObjectId p_id);

	//AttrWindow(const AttrWindow &other);
	//virtual AttrWindow& operator=(const AttrWindow &rhs);
	void Clone(const AttrWindow& p_src); //克隆只克隆数据，不克隆关联关系，如m_instanceCodeId



	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual eRCType GetType()const;
	virtual bool isEqualTo(const AttrObject*other) const;//窗型是否一致
	bool IsPrototypeEqual_test(const AttrWindow& p_att);
	bool IsInstanceEqual(const AttrWindow& p_att) const;//主要用于自动编号区分

	E_WindowAluminumType GetWindowDoorAluminumType()const;

	CString GetPrototypeCode()const { return m_prototypeCode; }
	CString GetMainPrototypeCode()const; //返回原型主编码，如Window_NC2_1 返回的值为Window_NC2
	
	//////////////////////////////////////////////////////////////////////////
	const CWindowsDimData* GetDimData(CString p_sCode)const;
	void SetDimData(const CWindowsDimData& p_dataOut);
	void CheckAndComplementDimeData(); //检查并补全Dim数据，W/H/a确保都有

	double GetTongFengQty(bool bDefaultValue = false) const;
	double GetWindowArea()const;
	
	bool HasValue(CString p_sCode)const;
	bool IsValueCanbeSet(CString p_sCode)const; //当前代号对应的值能否被设置
	double GetValue(CString p_sCode, bool bDefaultValue = false) const;
	bool SetValue(CString p_sCode, double p_dValue);

	double GetH(bool bDefaultValue = false) const { return GetValue(L"H", bDefaultValue); }
	double GetH1(bool bDefaultValue = false) const { return GetValue(L"H1", bDefaultValue); }
	double GetH2(bool bDefaultValue = false) const { return GetValue(L"H2", bDefaultValue); }
	double GetH3(bool bDefaultValue = false) const { return GetValue(L"H2", bDefaultValue); }
	double GetW(bool bDefaultValue = false) const { return GetValue(L"W", bDefaultValue); }
	double GetW1(bool bDefaultValue = false) const { return GetValue(L"W1", bDefaultValue); }
	double GetW2(bool bDefaultValue = false) const { return GetValue(L"W2", bDefaultValue); }
	double GetW3(bool bDefaultValue = false) const { return GetValue(L"W3", bDefaultValue); }
	double GetA(bool bDefaultValue = false) const { return GetValue(L"a", bDefaultValue); } //塞缝尺寸
	double GetR(bool bDefaultValue = false) const { return GetValue(L"R", bDefaultValue); }
	double GetD(bool bDefaultValue = false) const { return GetValue(L"D", bDefaultValue); } //墙厚度
	bool SetH(double newValue) { return SetValue(L"H", newValue); }
	bool SetH1(double newValue) { return SetValue(L"H1", newValue); }
	bool SetH2(double newValue) { return SetValue(L"H2", newValue); }
	bool SetH3(double newValue) { return SetValue(L"H3", newValue); }
	bool SetW(double newValue) { return SetValue(L"W", newValue); }
	bool SetW1(double newValue) { return SetValue(L"W1", newValue); }
	bool SetW2(double newValue) { return SetValue(L"W2", newValue); }
	bool SetW3(double newValue) { return SetValue(L"W3", newValue); }
	bool SetA(double newValue) { return SetValue(L"a", newValue); } //塞缝尺寸
	bool SetR(double newValue) { return SetValue(L"R", newValue); }
	bool SetD(double newValue) { return SetValue(L"D", newValue); } //墙厚度

	bool GetWRange(double& p_min, double& p_max);
	bool GetHRange(double& p_min, double& p_max);

	bool SetHeightUnderWindow(double newValue);
	double GetHeightUnderWindow()const { return m_heightUnderWindow; }

	void SetIsFireproofWindow(bool p_isFireproofWindow){ m_isFireproofWindow = p_isFireproofWindow; }
	bool GetIsFireproofWindow()const { return m_isFireproofWindow; }

	virtual void SetViewDir(eViewDir p_view){ m_viewDir = p_view; }

	CString GetPrototypeDwgFilePath(eViewDir p_view)const;

	bool IsRelatedGen()const { return m_fromWinId != AcDbObjectId::kNull; } //是否是通过其他的门窗关联生成


	bool IsMxMirror()const; //实际的矩阵是否镜像
	void SetMxMirror(bool p_bMirror);
	bool IsMirror()const; 
	void SetMirror(bool p_bMirror);

	AcDbObjectId GetFromWinId() const;
	AcDbObjectIdArray  GetRelatedWinIds() const;
	void SetFromWinId(AcDbObjectId p_id);
	void SetRelatedWinIds(const AcDbObjectIdArray& p_relatedWinIds);
	void ClearWinsRelation(); //移除关联关系

protected:
	CWindowsDimData* GetDimDataByCode(CString p_sCode);
	vector<CWindowsDimData> m_dimData; //存储W/W1/W2/W3   H/H1/H2/H3 R的尺寸数据

public:
	CDwgFileInfo m_frontViewFile;	//原型立面文件, 展开图用基类的m_fileName
	CDwgFileInfo m_topViewFile;		//原型俯视图文件
	CDwgFileInfo m_leftViewFile;	//原型侧视图文件
	
	CString m_tongFengFormula;//通风量计算公式,主要用于动态原型
	double m_tongFengQty;	//通风量

	CString m_openType;		//开启类型
	Adesk::Int32 m_openQty;	//开启扇数量
	CString m_gongNengquType;//功能区类型

	bool m_isZhuanJiao;		//是否转角窗
	bool m_isMirrorWindow;	//是否对称窗型 

	//////////////////////////////////////////////////////////////////////////
	//算量相关
	CWindowMaterial m_material;

	//////////////////////////////////////////////////////////////////////////
	//以下属性为具体外窗插入时才设置,单个窗户实例的属性，非原型属性
	//视图属性
	bool m_isMirror;//由于用户会通过CAD镜像，m_isMirror在使用时不一定准确，需要在使用前根据图形更新数据

	bool   m_isBayWindow;	 //是否凸窗
	bool   m_isFireproofWindow;//是否防火窗
	double m_wallDis;		 //外墙距离
	double m_heightUnderWindow; //窗下墙高度

protected:
	//////////////////////////////////////////////////////////////////////////
	AcDbObjectId m_fromWinId;  // 1912 表示此门窗是源自fromWinId生成(如平面到立面生成），用户操作生成的fromWinId为空
	AcDbObjectIdArray m_relatedWinIds; //由当前门窗生成的其他门窗 // 1912  (如平面到立面生成）
#ifdef INIT_HANLDLE_LATER_FOR_DWGIN 
	AcDbHandle m_fromWinHandle;
	vector<AcDbHandle> m_relatedWinHandles;
#endif 
};

typedef std::vector<AttrWindow> vAttrWindow;




