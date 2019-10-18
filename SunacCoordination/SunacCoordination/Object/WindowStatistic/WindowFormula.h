#pragma once

class CAluminumFormula
{
public:
	CAluminumFormula();
	~CAluminumFormula();

public:
	CString m_prototypeCode;   //ԭ�ͱ��
	CString m_alumunumClassify; //�Ͳķ���
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
	CString m_hightFormula; //�߹�ʽ
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

	vector<CAluminumFormula> GetAluminumFormulas(CString p_sPrototypeCode);
	vector<CGlassFormula> GetGlassFormulas(CString p_sPrototypeCode);
	vector<CHardwareData> GetHardwareData(CString p_sPrototypeCode);


protected:

};
