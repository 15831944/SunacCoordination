#include "StdAfx.h"
#include "WindowAutoName.h"


//////////////////////////////////////////////////////////////////////////
CWindowAutoName::CWindowAutoName()
{
}

CWindowAutoName::~CWindowAutoName()
{
}

CWindowAutoName* CWindowAutoName::GetInstance()
{
	static CWindowAutoName instance;
	return &instance;
}

CString CWindowAutoName::GetWindowName(const AttrWindow& p_att) const
{
	CString sWindowName;
	//ȥ��ԭ�ͱ���е�"Window_"ǰ׺
	CString prototype = p_att.GetMainPrototypeCode();
	prototype.Replace(L"Window_", L"");
	//����"ԭ�ͱ��_�ߴ���"�����Ŵ����
	sWindowName.Format(L"%s_%02d%02d", prototype, (int)p_att.GetW() / 100, (int)p_att.GetH() / 100);

	//����������"_m"��׺
	CString sMirror;
	if (!p_att.m_isMirrorWindow && p_att.m_isMirror)
		sMirror = L"_m";

	CString sWindowFullName = sWindowName + sMirror;

	//�����Ѵ��ڵĴ���
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		if (m_allTypeWindows[i].IsInstanceEqual(p_att))
			return m_allTypeWindows[i].m_instanceCode;
	}

	//����һ��δ��ռ�õ��Ŵ����
	for (int i = 1; !IsNameValid(p_att, sWindowFullName); i++)
	{
		sWindowFullName.Format(L"%s_%d%s", sWindowName, i, sMirror);
	}

	return sWindowFullName;
}

void CWindowAutoName::AddWindowType(const AttrWindow& p_att)
{
	//����ǰȷ��������
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		if (m_allTypeWindows[i].m_instanceCode == p_att.m_instanceCode)
			return;
	}
	m_allTypeWindows.push_back(p_att);
}

bool CWindowAutoName::IsNameValid(const AttrWindow& p_att, CString p_sName) const
{
	if (p_sName.IsEmpty())
		return false;
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		if ((p_sName == m_allTypeWindows[i].m_instanceCode) && !m_allTypeWindows[i].IsInstanceEqual(p_att))
			return false;
	}
	return true;
}

void CWindowAutoName::AutoNameAllWindow()
{
	vector<AttrWindow> temp = m_allTypeWindows;
	m_allTypeWindows.clear();
	for (UINT i = 0; i < temp.size(); i++)
	{
		temp[i].m_instanceCode = GetWindowName(temp[i]);
		AddWindowType(temp[i]);
	}
}

bool CWindowAutoName::RenameWindow(const AttrWindow& p_att)
{
	if (!IsNameValid(p_att, p_att.m_instanceCode))
		return false;

	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		if (m_allTypeWindows[i].IsInstanceEqual(p_att))
		{
			m_allTypeWindows[i].m_instanceCode = p_att.m_instanceCode;
			return true;
		}
	}

	return false;
}
