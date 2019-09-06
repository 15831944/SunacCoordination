/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_WINDOW)
#define ARX__ZFFDWGSCALE_H__20140205_113541_WINDOW

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "AttrObject.h"
#include "../Sunac_def.h"
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

	// Constructor / Destructor
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
private:

public:
	CString m_openType;//��������
	Adesk::Int32 m_openQty;//����������
	bool m_isZhuanJiao;//�Ƿ�ת�Ǵ�
	double m_minWid;//��ȳߴ���Сֵ
	double m_maxWid;//��ȳߴ����ֵ
	CString m_tongFengFormula;//ͨ�������㹫ʽ
	double m_tongFengQty;
	vSCalcData m_calFormulas;

	bool m_isMirror;//�Ƿ���
	eViewDir m_viewDir;//��ͼ����
	double m_wallDis;//��ǽ����
};

typedef std::vector<AttrWindow> vAttrWindow;


#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_WINDOW)