#include "StdAfx.h"
#include "KitchenAutoName.h"


//////////////////////////////////////////////////////////////////////////
CKitchenAutoName::CKitchenAutoName()
{
}

CKitchenAutoName::~CKitchenAutoName()
{
}

CKitchenAutoName* CKitchenAutoName::GetInstance()
{
	static CKitchenAutoName instance;
	return &instance;
}

CString CKitchenAutoName::GetKitchenName(const AttrKitchen& p_att) const
{
	CString sKitchenName;
	//ȥ��ԭ�ͱ���еĺ�׺
	CString prototype = p_att.m_prototypeCode;
	int pos = prototype.Find(L'_');
	if (pos != -1)
		prototype = prototype.Left(pos);
	//����"ԭ�ͱ��_�ߴ���"�����Ŵ����
	sKitchenName.Format(L"%s-%.0lf��%.0lf", prototype, p_att.m_width, p_att.m_height);

	if (prototype.Find(L"_c"))
		sKitchenName += L"/c";

	//�����������"_m"��׺
	CString sMirror;
	if (p_att.m_isMirror)
		sMirror = L"_m";

	CString sKitchenFullName = sKitchenName + sMirror;

	//�����Ѵ��ڵĳ���
	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
	{
		if (m_allTypeKitchens[i].IsInstanceEqual(p_att))
			return m_allTypeKitchens[i].m_instanceCode;
	}

	//����һ��δ��ռ�õ��Ŵ����
	for (int i = 1; !IsNameValid(p_att, sKitchenFullName); i++)
	{
		sKitchenFullName.Format(L"%s_%d%s", sKitchenName, i, sMirror);
	}

	return sKitchenFullName;
}

void CKitchenAutoName::AddKitchenType(const AttrKitchen& p_att)
{
	//����ǰȷ��������
	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
	{
		if (m_allTypeKitchens[i].m_instanceCode == p_att.m_instanceCode)
			return;
	}
	m_allTypeKitchens.push_back(p_att);
}

bool CKitchenAutoName::IsNameValid(const AttrKitchen& p_att, CString p_sName) const
{
	if (p_sName.IsEmpty())
		return false;
	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
	{
		if ((p_sName == m_allTypeKitchens[i].m_instanceCode) && !m_allTypeKitchens[i].IsInstanceEqual(p_att))
			return false;
	}
	return true;
}

void CKitchenAutoName::AutoNameAllKitchen()
{
	vector<AttrKitchen> temp = m_allTypeKitchens;
	m_allTypeKitchens.clear();
	for (UINT i = 0; i < temp.size(); i++)
	{
		temp[i].m_instanceCode = GetKitchenName(temp[i]);
		AddKitchenType(temp[i]);
	}
}

bool CKitchenAutoName::RenameKitchen(const AttrKitchen& p_att)
{
	if (!IsNameValid(p_att, p_att.m_instanceCode))
		return false;

	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
	{
		if (m_allTypeKitchens[i].IsInstanceEqual(p_att))
		{
			m_allTypeKitchens[i].m_instanceCode = p_att.m_instanceCode;
			return true;
		}
	}

	return false;
}

Acad::ErrorStatus CKitchenAutoName::ReadFromDwg(AcDbDwgFiler* pFiler)
{
	Adesk::UInt32 size;
	pFiler->readItem(&size);
	m_allTypeKitchens.resize(size);
	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
		m_allTypeKitchens[i].dwgInFields(pFiler);
	return Acad::eOk;
}

Acad::ErrorStatus CKitchenAutoName::WriteToDwg(AcDbDwgFiler* pFiler)
{
	pFiler->writeItem((Adesk::UInt32)m_allTypeKitchens.size());
	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
		m_allTypeKitchens[i].dwgOutFields(pFiler);
	return Acad::eOk;
}
