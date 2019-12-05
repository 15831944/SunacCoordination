#pragma once
#include <vector>
#include "AttrWindow.h"

using namespace std;


class CWinClassify  //1912
{
public:
	CWinClassify();
	~CWinClassify();

	void AddObject(AcDbObjectId p_objId);
	void AddObject(vector<AcDbObjectId> p_objIds);
	bool RemoveObject(AcDbObjectId p_objId);
	void CheckObjectValid(); //���AcDbObjectId�Ƿ���Ч������û���CAD��ֱ��ɾ�����Ŵ������
	void ClearObjsects(); //�������еĹ����Ŵ�

	bool IsEmpty()const { return m_winsInCad.size() == 0; }
	bool IsObjectIn(AcDbObjectId p_objId)const;

	void Rename(const CString p_newName);

protected:
	bool IsObjectBelongThisClassify(AcDbObjectId p_obj); //�ж��Ƿ�ʹ��Ŵ�������ͬ

public:
	AttrWindow m_winAtt;
	vector<AcDbObjectId> m_winsInCad; //��ͼ�ϵ�CAD 1912
};


//////////////////////////////////////////////////////////////////////////
//�����漰�Ŵ���ż�����޸ĵĶ�ͨ��CWindowAutoName�ദ��
class CWindowAutoName
{
	CWindowAutoName();
public:
	~CWindowAutoName();

	static CWindowAutoName* GetInstance();

	//�Զ���ȡ����, ע���ȡ��û����ӵ��������
	CString GetWindowName(const AttrWindow& p_att);

	//��鵱ǰ�����Ƿ��������ͬ�������������򷵻�false
	bool IsNameValid(const AttrWindow& p_att, CString p_sName);

	//���봰���б���
	void AddWindowType(const AttrWindow& p_att, AcDbObjectId p_objId);
	void AddWindowType(const AttrWindow& p_att, vector<AcDbObjectId> p_objIds);


	//�����д��������Զ�����
	void AutoNameAllWindow();
	void AutoNameWindows(const vector<AcDbObjectId>& p_ids);

	//��ָ������������
	bool RenameWindow(const CString p_preName, const CString p_newName);

	void CheckObjectValid(); //�����еļ����Ч�ԣ������Ƴ��仯���ʵ��
	void RemoveObject(AcDbObjectId p_id); //�Ŵ������仯ʱ���ô˺�������
	vector<AcDbObjectId> GetAllIdsByInstantCode(CString p_code);

	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);

protected:
	CWinClassify* FindWinClassifyByAtt(const AttrWindow& p_att);
	CWinClassify* FindWinClassifyByInstantCode(const CString p_sCode);

protected:
	vector<CWinClassify> m_allTypeWindows; //���д���

};
