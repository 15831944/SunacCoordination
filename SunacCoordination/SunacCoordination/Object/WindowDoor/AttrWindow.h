
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

static eWindowDimType ToEWindowType(CString type)
{
	if (type == "�̶�ֵ"|| type == "ֵ")
	{
		return SINGLE;
	} 
	else if (type == "ֵϵ��"|| type == "ϵ��")
	{
		return MULTI;
	}
	else if (type == "��Χ")
	{
		return SCOPE;
	}
	else if (type == "����")
	{
		return UNLIMIT;
	}
	else if (type == "��ʽ")
	{
		return CALC;
	}
	else if(type == "��")
	{
		return NOVALUE;
	}
	else
	{
		ASSERT(FALSE);
		return NOVALUE;
	}

}


class CWindowsDimData
{
public:
	CString sCodeName;		//���� W W1 W2 W3 H H1 H2 H3
	eWindowDimType type;	//ֵ����
	vdouble valueOptions;	//ֵѡ��洢����ֵ��ϵ��
	double minValue;
	double maxValue;
	CString sFomula;	//��ʽ
	double defaultValue;
	CString prompt;  //˵��

	double value; //����ʵ���е�ȡֵ

public:
	CWindowsDimData();
	bool operator==(const CWindowsDimData &rhs) const;

	bool IsValueEqual(const CWindowsDimData &rhs)const;

};

typedef std::vector<CWindowsDimData> vWindowDimData;

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

	virtual eRCType GetType() {return WINDOW;}
	virtual bool isEqualTo(AttrObject*other);//�����Ƿ�һ��
	virtual bool IsPrototypeEqual(const AttrWindow& p_att);
	
	//////////////////////////////////////////////////////////////////////////
	const CWindowsDimData* GetDimData(CString p_sCode)const;
	void SetDimData(const CWindowsDimData& p_dataOut);

	double GetTongFengQty(bool bDefaultValue = false);
	
	double GetValue(CString p_sCode, bool bDefaultValue = false);
	double GetH(bool bDefaultValue = false) { return GetValue(L"H", bDefaultValue); }
	double GetH1(bool bDefaultValue = false) { return GetValue(L"H1", bDefaultValue); }
	double GetH2(bool bDefaultValue = false) { return GetValue(L"H2", bDefaultValue); }
	double GetW(bool bDefaultValue = false) { return GetValue(L"W", bDefaultValue); }
	double GetW1(bool bDefaultValue = false) { return GetValue(L"W1", bDefaultValue); }
	double GetW2(bool bDefaultValue = false) { return GetValue(L"W2", bDefaultValue); }
	double GetW3(bool bDefaultValue = false) { return GetValue(L"W3", bDefaultValue); }
	double GetA(bool bDefaultValue = false) { return bDefaultValue ? 0 : m_plugslotSize; } //����ߴ�

	bool SetValue(CString p_sCode, double p_dValue);
	bool SetH(double newValue);
	bool SetH1(double newValue);
	bool SetH2(double newValue);
	bool SetW(double newValue);
	bool SetW1(double newValue);
	bool SetW2(double newValue);
	bool SetW3(double newValue);
	bool SetA(double newValue);//����ߴ�

protected:
	vWindowDimData m_dimData; //�洢W/W1/W2/W3   H/H1/H2/H3 R�ĳߴ�����

public:
	CString m_openType;		//��������

	CDwgFileInfo m_frontViewFile;	//ԭ�������ļ�, չ��ͼ�û����m_fileName
	CDwgFileInfo m_topViewFile;		//ԭ�͸���ͼ�ļ�
	CDwgFileInfo m_leftViewFile;		//ԭ�Ͳ���ͼ�ļ�
	
	CString m_tongFengFormula;//ͨ�������㹫ʽ,��Ҫ���ڶ�̬ԭ��
	double m_tongFengQty;	//ͨ����

	Adesk::Int32 m_openQty;	//����������
	CString m_gongNengquType;//����������

	bool m_isZhuanJiao;		//�Ƿ�ת�Ǵ�
	bool m_isMirrorWindow;	//�Ƿ�Գƴ��� 

	//////////////////////////////////////////////////////////////////////////
	//�������
	vSCalcData m_calFormulas;

	//////////////////////////////////////////////////////////////////////////
	//��������Ϊ�����ⴰ����ʱ������,��������ʵ�������ԣ���ԭ������
	//��ͼ����
	bool m_isMirror;//�Ƿ���
	eViewDir m_viewDir;//��ͼ����

	bool   m_isBayWindow;	 //�Ƿ�͹��
	double m_wallDis;		 //��ǽ����

	//////////////////////////////////////////////////////////////////////////
	//���������
	double m_plugslotSize;		//����ߴ�

};

typedef std::vector<AttrWindow> vAttrWindow;
