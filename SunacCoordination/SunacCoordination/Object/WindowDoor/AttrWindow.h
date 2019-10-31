
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



//�Ŵ�ȡֵ��������
typedef enum eWindowDimType
{
	SINGLE,//�̶�ֵ
	MULTI,//ֵϵ��
	SCOPE,//��Χ
	UNLIMIT,//����
	CALC,//��ʽ
	NOVALUE //��
}eWindowDimType;

eWindowDimType ToEWindowType(CString type);

class CWindowsDimData
{
public:
	CString sCodeName;		//���� W W1 W2 W3 H H1 H2 H3
	eWindowDimType type;	//ֵ����
	vdouble valueOptions;	//ֵѡ��洢����ֵ��ϵ��
	double minValue;
	double maxValue;
	CString sFomula;	//��ʽ
	CString prompt;  //˵��
	double defaultValue;
	double value; //����ʵ���е�ȡֵ

public:
	CWindowsDimData();
	bool operator==(const CWindowsDimData &rhs) const;
	bool IsValueEqual(const CWindowsDimData &rhs)const;
	bool IsParaEqual(const CWindowsDimData &rhs)const;
	
	//������
	bool operator<(const CWindowsDimData &rhs) const;

	bool SetDefaultValue(double p_value);
	bool SetValue(double p_value);
};

//////////////////////////////////////////////////////////////////////////

struct CWindowMaterial	//�Ŵ�����ϵ��
{
	CString sAluminumSerial; //�Ͳ�ϵ��
	CString sGlassSerial;
	bool bHasAuxiliaryFrame; //�Ƿ��и���
	CString sAuxiliaryFrame; //����ϵ��

	CWindowMaterial()
	{
		bHasAuxiliaryFrame = false;
	}
};

//////////////////////////////////////////////////////////////////////////


class DLLIMPEXP  AttrWindow : public AttrObject  //�Ŵ�����AttrWindow
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

	virtual eRCType GetType()const;
	virtual bool isEqualTo(AttrObject*other);//�����Ƿ�һ��
	bool IsPrototypeEqual(const AttrWindow& p_att);
	bool IsInstanceEqual(const AttrWindow& p_att) const;

	E_WindowDoorType GetWindowDoorType()const;

	CString GetPrototypeCode()const { return m_prototypeCode; }
	CString GetMainPrototypeCode()const; //����ԭ�������룬��Window_NC2_1 ���ص�ֵΪWindow_NC2
	
	//////////////////////////////////////////////////////////////////////////
	const CWindowsDimData* GetDimData(CString p_sCode)const;
	void SetDimData(const CWindowsDimData& p_dataOut);
	void CheckAndComplementDimeData(); //��鲢��ȫDim���ݣ�W/H/aȷ������

	double GetTongFengQty(bool bDefaultValue = false) const;
	
	bool HasValue(CString p_sCode)const;
	double GetValue(CString p_sCode, bool bDefaultValue = false) const;
	double GetH(bool bDefaultValue = false) const { return GetValue(L"H", bDefaultValue); }
	double GetH1(bool bDefaultValue = false) const { return GetValue(L"H1", bDefaultValue); }
	double GetH2(bool bDefaultValue = false) const { return GetValue(L"H2", bDefaultValue); }
	double GetH3(bool bDefaultValue = false) const { return GetValue(L"H2", bDefaultValue); }
	double GetW(bool bDefaultValue = false) const { return GetValue(L"W", bDefaultValue); }
	double GetW1(bool bDefaultValue = false) const { return GetValue(L"W1", bDefaultValue); }
	double GetW2(bool bDefaultValue = false) const { return GetValue(L"W2", bDefaultValue); }
	double GetW3(bool bDefaultValue = false) const { return GetValue(L"W3", bDefaultValue); }
	double GetA(bool bDefaultValue = false) const { return GetValue(L"a", bDefaultValue); } //����ߴ�
	double GetR(bool bDefaultValue = false) const { return GetValue(L"R", bDefaultValue); }

	bool SetValue(CString p_sCode, double p_dValue);

	bool SetH(double newValue) { return SetValue(L"H", newValue); }
	bool SetH1(double newValue) { return SetValue(L"H1", newValue); }
	bool SetH2(double newValue) { return SetValue(L"H2", newValue); }
	bool SetH3(double newValue) { return SetValue(L"H2", newValue); }
	bool SetW(double newValue) { return SetValue(L"W", newValue); }
	bool SetW1(double newValue) { return SetValue(L"W1", newValue); }
	bool SetW2(double newValue) { return SetValue(L"W2", newValue); }
	bool SetW3(double newValue) { return SetValue(L"W3", newValue); }
	bool SetA(double newValue) { return SetValue(L"a", newValue); }//����ߴ�
	bool SetR(double newValue) { return SetValue(L"R", newValue); }

protected:
	CWindowsDimData* GetDimDataByCode(CString p_sCode);
	vector<CWindowsDimData> m_dimData; //�洢W/W1/W2/W3   H/H1/H2/H3 R�ĳߴ�����

public:
	CDwgFileInfo m_frontViewFile;	//ԭ�������ļ�, չ��ͼ�û����m_fileName
	CDwgFileInfo m_topViewFile;		//ԭ�͸���ͼ�ļ�
	CDwgFileInfo m_leftViewFile;	//ԭ�Ͳ���ͼ�ļ�
	
	CString m_tongFengFormula;//ͨ�������㹫ʽ,��Ҫ���ڶ�̬ԭ��
	double m_tongFengQty;	//ͨ����

	CString m_openType;		//��������
	Adesk::Int32 m_openQty;	//����������
	CString m_gongNengquType;//����������

	bool m_isZhuanJiao;		//�Ƿ�ת�Ǵ�
	bool m_isMirrorWindow;	//�Ƿ�Գƴ��� 

	//////////////////////////////////////////////////////////////////////////
	//�������
	CWindowMaterial m_material;

	//////////////////////////////////////////////////////////////////////////
	//��������Ϊ�����ⴰ����ʱ������,��������ʵ�������ԣ���ԭ������
	//��ͼ����
	bool m_isMirror;//�Ƿ���
	eViewDir m_viewDir;//��ͼ����

	bool   m_isBayWindow;	 //�Ƿ�͹��
	double m_wallDis;		 //��ǽ����

};

typedef std::vector<AttrWindow> vAttrWindow;
