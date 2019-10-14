#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrWindow.h"
#include "../../Common/ComFun_Sunac.h"
#include "../../Common/ComFun_Math.h"

eWindowDimType ToEWindowType(CString type)
{
	if (type == "�̶�ֵ")
	{
		return SINGLE;
	}
	else if (type == "ֵϵ��")
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
	else if (type == "��")
	{
		return NOVALUE;
	}
	else
	{
		ASSERT(FALSE);
		return NOVALUE;
	}
}
CWindowsDimData::CWindowsDimData()
{
	type = NOVALUE;
	minValue = 0;
	maxValue = 10000;
	defaultValue = 0;
	value = 0;
}

bool CWindowsDimData::operator==(const CWindowsDimData &rhs) const
{
	if (sCodeName != rhs.sCodeName || type != rhs.type)
	{
		return false;
	}

	switch (type)
	{
	case SINGLE:
	case MULTI:
		return valueOptions == rhs.valueOptions;
		break;
	case SCOPE:
		return JHCOM_equ(minValue, rhs.minValue, 0.1) &&
			JHCOM_equ(maxValue, rhs.maxValue, 0.1);
		break;
	case CALC:
		return sFomula == rhs.sFomula;
		break;
	default:
		break;
	}

	return true;
}

bool CWindowsDimData::IsValueEqual(const CWindowsDimData &rhs)const
{
	if (type==NOVALUE)
	{
		return true;
	}
	else
	{
		return JHCOM_equ(value, rhs.value, 0.1);
	}
}

//////////////////////////////////////////////////////////////////////////

//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrWindow, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEWINDOW, ZffCustomObjectDBWINDOW);
AttrWindow::AttrWindow()
{
	m_openType = L"";
	m_openQty = 1;
	m_isZhuanJiao = false;//�Ƿ�ת�Ǵ�
	m_isMirrorWindow = false;

	m_tongFengFormula = L"";//ͨ�������㹫ʽ
	m_tongFengQty = 0.0;

	//////////////////////////////////////////////////////////////////////////
	m_isMirror = false;
	m_viewDir = E_VIEW_FRONT;
	m_isBayWindow = false;
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
//	m_tongFengQty = other.m_tongFengQty;
//
//	m_isMirror = other.m_isMirror;
//	m_viewDir = other.m_viewDir;
//	m_wallDis = other.m_wallDis;
//
//	////////////////////////////
//	m_id = other.m_id;					//���
//	m_prototypeCode = other.m_prototypeCode;		//ԭ�ͱ��
//	m_name = other.m_name;		//ԭ���ļ�
//	m_scopeOfApplication = other.m_scopeOfApplication;	//���÷�Χ
//	m_Dynamic = other.m_Dynamic;			//�Ƿ�̬
//	m_functionType = other.m_functionType;		//����������
//	m_openType = other.m_openType;			//��������
//	m_openQty = other.m_openQty;			//��������
//	//widthMin = other.widthMin;			//�����Сֵ
//	//widthMax = other.widthMax;			//������ֵ
//	m_tongFengFormula = other.m_tongFengFormula;	//ͨ������ʽ	
//	m_width = other.m_width;		//��̬���	
//	m_height = other.m_height;		//��̬�߶�	
//	m_tongFengQty = other.m_tongFengQty;	//��̬ͨ����	
//	m_topViewFile = other.m_topViewFile;	//ԭ��ƽ���ļ�
//	m_leftViewFile = other.m_leftViewFile;//ԭ�͸���ͼ�ļ�
//
//	m_dimData = other.m_dimData;
//
//	return *this;
//}

const CWindowsDimData* AttrWindow::GetDimData(CString p_sCode)const
{
	for (UINT i = 0; i < m_dimData.size(); i++)
	{
		if (m_dimData[i].sCodeName.CompareNoCase(p_sCode) == 0)
		{
			return &(m_dimData[i]);
		}
	}

	assert(false);
	return NULL;
}
void AttrWindow::SetDimData(const CWindowsDimData& p_dim)
{
	for (UINT i = 0; i < m_dimData.size(); i++)
	{
		if (m_dimData[i].sCodeName.CompareNoCase(p_dim.sCodeName)==0)
		{
			m_dimData[i] = p_dim;
			return ;
		}
	}

	m_dimData.push_back(p_dim);
}

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

	filer->readItem(&tempStr);
	m_tongFengFormula = CString(tempStr);

	filer->readItem(&m_tongFengQty);


	//TODO �����е����Զ�ȡ��������

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
	filer->writeItem(m_tongFengFormula);
	filer->writeItem(m_tongFengQty);

	//TODO �����е����Զ�ȡ��������

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

	//TODO �����Ƚ�����

	return (m_openType == pRealObj->m_openType &&
		m_openQty == pRealObj->m_openQty &&
		m_isZhuanJiao == pRealObj->m_isZhuanJiao &&
		m_tongFengFormula == pRealObj->m_tongFengFormula &&
		m_tongFengQty == pRealObj->m_tongFengQty &&
		m_isMirror == pRealObj->m_isMirror &&
		m_viewDir == pRealObj->m_viewDir &&
		m_wallDis == pRealObj->m_wallDis
		);
}

bool AttrWindow::IsPrototypeEqual(const AttrWindow& p_att)
{
	if (p_att.m_dimData.empty()||m_dimData.empty())
	{
		vector<CWindowsDimData>::const_iterator it1 = p_att.m_dimData.begin();
		vector<CWindowsDimData>::iterator it2 = m_dimData.begin();
		while (it1 != p_att.m_dimData.end()&& it2 != m_dimData.end())
		{
			if ((*it1)==(*it2)==false)
			{
				return false;
			}

			it1++;
			it2++;
		}
	}

	if (p_att.m_calFormulas != m_calFormulas)
	{
		return false;
	}
	else if (p_att.m_instanceCode != m_instanceCode)
	{
		return false;
	}
	else if (p_att.m_isBayWindow != m_isBayWindow)
	{
		return false;
	}
	else if (p_att.m_isDynamic != m_isDynamic)
	{
		return false;
	}
	else if (p_att.m_isJiTuan != m_isJiTuan)
	{
		return false;
	}
	else if (p_att.m_isMirror != m_isMirror)
	{
		return false;
	}
	else if (p_att.m_isMirrorWindow != m_isMirrorWindow)
	{
		return false;
	}
	else if (p_att.m_isZhuanJiao != m_isZhuanJiao)
	{
		return false;
	}
	else if (p_att.m_openQty != m_openQty)
	{
		return false;
	}
	else if (p_att.m_openType != m_openType)
	{
		return false;
	}
	else if (p_att.m_plugslotSize != m_plugslotSize)
	{
		return false;
	}
	else if (p_att.m_prototypeCode != m_prototypeCode)
	{
		return false;
	}
	else if (p_att.m_quyuId != m_quyuId)
	{
		return false;
	}
	else if (p_att.m_quyuName != m_quyuName)
	{
		return false;
	}
	else if (p_att.m_tongFengQty != m_tongFengQty)
	{
		return false;
	}
	else if (p_att.m_tongFengFormula != m_tongFengFormula)
	{
		return false;
	}
	else if (p_att.m_version != m_version)
	{
		return false;
	}
	else if (p_att.m_viewDir != m_viewDir)
	{
		return false;
	}
	else if (p_att.m_wallDis != m_wallDis)
	{
		return false;
	}
	else return true;
}
