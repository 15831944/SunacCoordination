#include "StdAfx.h"
#include "WindowAutoName.h"
#include "..\..\Common\ComFun_Sunac.h"

CWinClassify::CWinClassify()
{
}

CWinClassify::~CWinClassify()
{
}

void CWinClassify::AddObject(AcDbObjectId p_objId)
{
	if (p_objId == AcDbObjectId::kNull)
	{
		return;
	}

	if (IsObjectIn(p_objId))
	{
		return;
	}

	m_winsInCad.push_back(p_objId);
}
void CWinClassify::AddObject(vector<AcDbObjectId> p_objIds)
{
	for (UINT i = 0; i < p_objIds.size(); i++)
	{
		AddObject(p_objIds[i]);
	}
}

bool CWinClassify::RemoveObject(AcDbObjectId p_objId)
{
	for (vector<AcDbObjectId>::iterator it = m_winsInCad.begin(); it < m_winsInCad.end(); )
	{
		if ((*it) == p_objId)
		{
			m_winsInCad.erase(it);
			return true;
		}
	}
	return false;
}

void CWinClassify::CheckObjectValid() //���AcDbObjectId�Ƿ���Ч������û���CAD��ֱ��ɾ�����Ŵ������
{
	for (vector<AcDbObjectId>::iterator it = m_winsInCad.begin(); it < m_winsInCad.end(); )
	{
		if (IsObjectBelongThisClassify(*it) == false)
		{
			it = m_winsInCad.erase(it);
		}
		else
		{
			it++;
		}
	}
}

bool CWinClassify::IsObjectBelongThisClassify(AcDbObjectId p_id) //�ж��Ƿ�ʹ��Ŵ�������ͬ
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(p_id, pDataEnt);
	if (pDataEnt==NULL)
		return false;
	
	AttrWindow * pWindow = AttrWindow::cast(pDataEnt);
	if (pWindow == NULL)
		return false;

	bool bSuc = pWindow->IsInstanceEqual(m_winAtt);
	pWindow->close();

	return bSuc;
}

void CWinClassify::ClearObjsects() //�������еĹ����Ŵ�
{
	m_winsInCad.clear();
}

bool CWinClassify::IsObjectIn(AcDbObjectId p_objId)const
{
	for (UINT i = 0; i < m_winsInCad.size(); i++)
	{
		if (m_winsInCad[i] == p_objId)
		{
			return true;
		}
	}
	return false;
}

void CWinClassify::Rename(const CString p_newName)
{
	m_winAtt.SetInstanceCode(p_newName);

	for (UINT i = 0; i < m_winsInCad.size(); i++)
	{
		AcDbObject * pDataEnt = 0;
		TY_GetAttributeData(m_winsInCad[i], pDataEnt);
		AttrWindow * pWindow = AttrWindow::cast(pDataEnt);
		if (pWindow == NULL)
			continue;

		pWindow->SetInstanceCode(p_newName);

		pDataEnt->close();
	}
}


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

CString CWindowAutoName::GetWindowName(const AttrWindow& p_att)
{
	//��ʾ����������뵱ǰ������ͬ�ұ����ͬ�ģ����ҵ���ֱ�ӷ���
	CWinClassify* pWinClassify = FindWinClassifyByAtt(p_att);
	if (pWinClassify!=NULL)
	{
		return pWinClassify->m_winAtt.GetInstanceCode();
	}


	//ȥ��ԭ�ͱ���е�"Window_"ǰ׺
	CString prototype = p_att.GetMainPrototypeCode();
	prototype.MakeUpper();
	prototype.Trim();
	prototype.Replace(L"WINDOW_", L"");
	prototype.Replace(L"DOOR_", L"");

	//����"ԭ�ͱ��_�ߴ���"�����Ŵ����
	CString sWindowName;
	sWindowName.Format(L"%s_%02d%02d", prototype, (int)p_att.GetW() / 100, (int)p_att.GetH() / 100);

	//����������"M"��׺
	CString sMirror;
	if (!p_att.m_isMirrorWindow && p_att.m_isMirror)
		sMirror = L"M";

	//����һ��δ��ռ�õ��Ŵ����
	CString sWindowFullName = sWindowName + sMirror;
	for (int i = 1; !IsNameValid(p_att, sWindowFullName); i++)
	{
		sWindowFullName.Format(L"%s_%d%s", sWindowName, i, sMirror);
	}

	return sWindowFullName;
}

CWinClassify* CWindowAutoName::FindWinClassifyByAtt(const AttrWindow& p_att)
{
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		if (m_allTypeWindows[i].m_winAtt.IsInstanceEqual(p_att))
		{
			return &(m_allTypeWindows[i]);
		}
	}

	return NULL;
}

CWinClassify* CWindowAutoName::FindWinClassifyByInstantCode(const CString p_sCode)
{
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		if (m_allTypeWindows[i].m_winAtt.GetInstanceCode().CompareNoCase(p_sCode)==0)
		{
			return &(m_allTypeWindows[i]);
		}
	}

	return NULL;
}

void CWindowAutoName::AddWindowType(const AttrWindow& p_att, AcDbObjectId p_objId)
{
	CWinClassify* pWinClassify = FindWinClassifyByAtt(p_att);
	if (pWinClassify != NULL)
	{
		pWinClassify->AddObject(p_objId);
		return;
	}

	CWinClassify newClassify;
	newClassify.m_winAtt = p_att;
	newClassify.AddObject(p_objId);
	m_allTypeWindows.push_back(newClassify);
}
void CWindowAutoName::AddWindowType(const AttrWindow& p_att, vector<AcDbObjectId> p_objIds)
{
	CWinClassify* pWinClassify = FindWinClassifyByAtt(p_att);
	if (pWinClassify != NULL)
	{
		pWinClassify->AddObject(p_objIds);
		return;
	}

	CWinClassify newClassify;
	newClassify.m_winAtt = p_att;
	newClassify.AddObject(p_objIds);
	m_allTypeWindows.push_back(newClassify);
}

bool CWindowAutoName::IsNameValid(const AttrWindow& p_att, CString p_sName) 
{
	if (p_sName.IsEmpty())
		return false;

	CWinClassify* pWinClassify = FindWinClassifyByInstantCode(p_sName);
	if (pWinClassify != NULL)
	{
		return pWinClassify->m_winAtt.IsInstanceEqual(p_att);
	}
	else
	{
		return  true;
	}
}

void CWindowAutoName::AutoNameAllWindow()
{
	//TODO ��������ԭ���ı�ţ�ֻ���ڱ�Ų����������²����±�� 
	//vector<CWinClassify> temp = m_allTypeWindows;
	//m_allTypeWindows.clear();
	//for (UINT i = 0; i < temp.size(); i++)
	//{
	//	temp[i].m_winAtt.m_instanceCode = GetWindowName(temp[i].m_winAtt);
	//	AddWindowType(temp[i].m_winAtt);
	//}


	//TODO ������ǰͼ�ϵ������Ŵ������Ŵ��������±��
}

void CWindowAutoName::AutoNameWindows(const vector<AcDbObjectId>& p_ids)
{
	//TODO
}

bool CWindowAutoName::RenameWindow(const CString p_preName, const CString p_newName)
{
	CWinClassify* pWinClassify = FindWinClassifyByInstantCode(p_preName);
	if (pWinClassify == NULL)
	{
		return false;
	}

	CWinClassify* pWinClassify2 = FindWinClassifyByInstantCode(p_newName);
	if (pWinClassify2 == NULL) //�µı���Ѿ���ռ�ã�����ʹ��
	{
		return false;
	}

	pWinClassify->Rename(p_newName);
	return true;
}

Acad::ErrorStatus CWindowAutoName::ReadFromDwg(AcDbDwgFiler* pFiler)
{
	Adesk::UInt32 size;
	pFiler->readItem(&size);
	m_allTypeWindows.resize(size);
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		m_allTypeWindows[i].m_winAtt.dwgInFields(pFiler);
	}
	return Acad::eOk;
}

Acad::ErrorStatus CWindowAutoName::WriteToDwg(AcDbDwgFiler* pFiler)
{
	pFiler->writeItem((Adesk::UInt32)m_allTypeWindows.size());
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		m_allTypeWindows[i].m_winAtt.dwgOutFields(pFiler);
	}
	return Acad::eOk;
}


vector<AcDbObjectId> CWindowAutoName::GetAllIdsByInstantCode(CString p_code)
{
	vector<AcDbObjectId> idsOut;
	CWinClassify* pWinClassify = FindWinClassifyByInstantCode(p_code);
	if (pWinClassify==NULL)
	{
		return idsOut;
	}

	return pWinClassify->m_winsInCad;
}

void CWindowAutoName::CheckObjectValid() //�����еļ����Ч�ԣ������Ƴ��仯���ʵ��
{
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		m_allTypeWindows[i].CheckObjectValid();
	}
}

void CWindowAutoName::RemoveObject(AcDbObjectId p_id) //�Ŵ������仯ʱ���ô˺�������
{
	for (vector<CWinClassify>::iterator it = m_allTypeWindows.begin(); it < m_allTypeWindows.end(); it++)
	{
		if (it->RemoveObject(p_id))
		{
			if (it->IsEmpty())
			{
				m_allTypeWindows.erase(it);
			}
			return;
		}
	}
}