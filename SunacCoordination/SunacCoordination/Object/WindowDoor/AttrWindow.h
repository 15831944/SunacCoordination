
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

	//AttrWindow(const AttrWindow &other);
	//AttrWindow & operator=(const AttrWindow &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual eRCType GetType() {return WINDOW;}
	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��

public:
	CString m_openType;		//��������
	Adesk::Int32 m_openQty;	//����������
	CString m_gongNengquType;//����������

	bool m_isZhuanJiao;		//�Ƿ�ת�Ǵ�
	bool m_isMirrorWindow;	//�Ƿ�Գƴ��� 

	CString m_frontViewFile;	//ԭ�������ļ�, չ��ͼ�û����m_fileName
	CString	m_topViewFile;		//ԭ�͸���ͼ�ļ�
	CString m_leftViewFile;		//ԭ�Ͳ���ͼ�ļ�
	

	//��̬��������
	double m_minWid;//��ȳߴ���Сֵ
	double m_maxWid;//��ȳߴ����ֵ
	CString m_tongFengFormula;//ͨ�������㹫ʽ

	//��̬��������
	double m_staticTongFengQty;	//ͨ����
	double m_staticWidth;		
	double m_staticHeight;		

	vSRCDimData m_dimData;

	//////////////////////////////////////////////////////////////////////////
	//�������
	vSCalcData m_calFormulas;
	




	//////////////////////////////////////////////////////////////////////////
	//��������Ϊ�����ⴰ����ʱ������,��������ʵ�������ԣ���ԭ������
	//��ͼ����
	bool m_isMirror;//�Ƿ���
	eViewDir m_viewDir;//��ͼ����

	bool   m_isBayWindow;	 //�Ƿ�͹��
	double m_openWindowSize; //�����ȿ��
	double m_windowH2;		 //�¹̶�ֵ���������¹̶�ʱ��
	double m_wallDis;		 //��ǽ����

	//////////////////////////////////////////////////////////////////////////
	//���������
	double m_plugslotSize;		//����ߴ�


};

typedef std::vector<AttrWindow> vAttrWindow;
