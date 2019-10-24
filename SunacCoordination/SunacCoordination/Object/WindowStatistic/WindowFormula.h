#pragma once

class CAluminumFormula
{
public:
	CAluminumFormula();
	~CAluminumFormula();

public:
	CString m_prototypeCode;   //ԭ�ͱ��
	CString m_aluminumClassify; //�Ͳķ���
	CString m_name; //�Ͳ�����
	CString m_pos;	//λ��
	CString m_formula; //��ʽ
	int m_nCount;
};

//�������㹫ʽ
class CGlassFormula
{
public:
	CGlassFormula();
	~CGlassFormula();

public:
	CString m_prototypeCode;   //ԭ�ͱ��
	CString m_glassClassify; //��������
	CString m_name; //��������
	CString m_widthFormula; //��ʽ
	CString m_heightFormula; //��ʽ
	int m_nCount;
};

class CHardwareData //��������
{
public:
	CHardwareData();
	~CHardwareData();

public:
	CString m_prototypeCode;   //ԭ�ͱ��
	CString m_name; 
	int m_nCount;
};


//////////////////////////////////////////////////////////////////////////

//���͹�ʽ��ѯ��  //YMY  
class CWindowFormula
{
	CWindowFormula();
public:
	static CWindowFormula* Instance();
	virtual ~CWindowFormula();

	vector<CAluminumFormula> GetAluminumFormulas(CString p_sPrototypeCode, CString p_sAluminumSeries = _T(""));//ʹ�ñ�AluminumFormulas
	vector<CGlassFormula> GetGlassFormulas(CString p_sPrototypeCode);//ʹ�ñ�GlassFormulas
	vector<CHardwareData> GetHardwareData(CString p_sPrototypeCode);//ʹ�ñ�HardwareData

protected:

};

CString GBKToUTF8(const std::string& strGBK);
CString UTF8ToGBK(const std::string& strUTF8);