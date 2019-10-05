
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


class DLLIMPEXP  AttrWindow : public AttrObject
{
public:
	ACRX_DECLARE_MEMBERS(AttrWindow);

	AttrWindow();
	virtual ~AttrWindow();

	AttrWindow(const AttrWindow &other);
	AttrWindow & operator=(const AttrWindow &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual eRCType GetType() {return WINDOW;}
	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��

public:
	CString m_openType;//��������
	Adesk::Int32 m_openQty;//����������

	bool m_isZhuanJiao;//�Ƿ�ת�Ǵ�
	bool m_isMirrorWindow; //�Ƿ�Գƴ���		//TODO ����

	//��̬��������
	double m_minWid;//��ȳߴ���Сֵ
	double m_maxWid;//��ȳߴ����ֵ
	CString m_tongFengFormula;//ͨ�������㹫ʽ

	//��̬��������
	double m_tongFengQty; //ͨ����
	double m_width;					//TODO ����
	double m_height;				//TODO ����


	//�������
	vSCalcData m_calFormulas;
	
	//////////////////////////////////////////////////////////////////////////
	//��������Ϊ�����ⴰ����ʱ������
	//��ͼ����
	bool m_isMirror;//�Ƿ���
	eViewDir m_viewDir;//��ͼ����

	bool m_isBayWindow; //�Ƿ�͹��
	double m_openWindowSize; //�����ȿ��
	double m_windowH2;  //�¹̶�ֵ���������¹̶�ʱ��
	double m_wallDis;//��ǽ����

	////CString	prototypeId;		//ԭ�ͱ��
	////CString	prototypeFile;		//ԭ���ļ�
	////CString	openType;			//��������
	////CString	openNum;			//��������
	////CString	ventilationFormula;	//ͨ������ʽ	
	////CString	staticWidth;		//��̬���	
	////CString	staticHeigth;		//��̬�߶�	
	////CString	staticVentilation;	//��̬ͨ����

	double m_plugslotSize;		//����ߴ�
	CString m_sizeId;			//�ߴ����
	CString m_id;					//���
	CString	m_scopeOfApplication;	//���÷�Χ
	CString	m_Dynamic;				//��̬
	CString	m_functionType;		//����������
	CString m_jiTuan;			//��������
	CString m_DynamicType;		//��̬����

	CString	m_valueType;		//ֵ����	
	CString	m_value;			//ֵ	
	CString	m_min;				//��Сֵ
	CString	m_max;				//���ֵ
	CString	m_defaultValue;		//Ĭ��ֵ
	CString	m_state;			//˵��

	CString	m_prototypeFlatFile;	//ԭ��ƽ���ļ�
	CString	m_prototypeTopViewFile;//ԭ�͸���ͼ�ļ�
	CString m_prototypeFacadeFile; //ԭ�������ļ�
	CString m_prototypeSpreadFile; //ԭ��չ��ͼ�ļ�

	vSRCDimData m_dimData;
};

typedef std::vector<AttrWindow> vAttrWindow;
