#pragma once
#include <vector>
#include <map>
#include "AttrWindow.h"
#include "..\..\Common\TYRect.h"

using namespace std;


//CWinClassify��ʾĳ����ŵ��Ŵ�������Ӧ��id(���)
class CWinClassify
{
public:
	CWinClassify();
	~CWinClassify();

	void AddObject(AcDbObjectId p_objId);
	void AddObject(vector<AcDbObjectId> p_objIds);
	bool RemoveObject(AcDbObjectId p_objId);
	void CheckAndRemoveObjectNotBelong(); //���������AcDbObjectId�Ƿ���Ч�����Ƴ���ɾ�����ߺʹ��������Բ�һ�µ�ʵ��
	void ClearObjsects(); //�������еĹ����Ŵ�

	bool IsEmpty()const { return m_winsInCad.size() == 0; }
	bool IsObjectIn(AcDbObjectId p_objId)const;

	void Rename(const CString p_newName);

	vector<AcDbObjectId> GetObjects() { return m_winsInCad; }

protected:
	bool IsObjectBelongThisClassify(AcDbObjectId p_obj); //�ж��Ƿ�ʹ��Ŵ�������ͬ

public:
	AttrWindow m_winAtt;
	vector<AcDbObjectId> m_winsInCad; //��ͼ�ϵ�CAD 1912
};

class CDocumentData;
//////////////////////////////////////////////////////////////////////////
//�����漰�Ŵ���ż�����޸ĵĶ�ͨ��CWindowAutoName�ദ��
//----------����ֻ�����Ŵ���ţ������Ŵ�����ʵ�����κδ�����޸�-------------
//�Ƴ�ԭ�����Ŵ�ʱҪ����RemoveObject�� 
//�������Ŵ�ʱҪ����AddWindowType
//�޸��Ŵ�����ʱ�����������
//    ���1��ֻ�޸ĵ�ǰ�Ŵ��������Ŵ����ɴ��Ŵ����ɵ�����ͼ��,�ȵ���RemoveObject���ٵ���AddWindowType
//    ���2�����˱�ŵ��Ŵ�ȫ����������ܻ����Ŵ���ţ�����RenameWindows
class CWindowAutoName
{
	friend CDocumentData;
	CWindowAutoName();
public:
	~CWindowAutoName();


	//ͨ��AttrWindow������ֵ�Զ���ȡ����, ע���ȡ��û����ӵ��������
	CString GetWindowName(const AttrWindow& p_att);

	//��鵱ǰ�����Ƿ��������ͬ�������������򷵻�false
	bool IsNameValid(const AttrWindow& p_att, CString p_sName);

	//���봰���б���
	bool AddWindowType(const CString p_sInstanceCode, AcDbObjectId p_objId);
	bool AddWindowType(const AttrWindow& p_att, AcDbObjectId p_objId);
	bool AddWindowType(const AttrWindow& p_att, vector<AcDbObjectId> p_objIds);
	bool AddObject(AcDbObjectId p_id);
	void RemoveObject(AcDbObjectId p_id); //�Ŵ��Ƴ�ʱ���ô˺�������
	void RemoveObjectsByInstantCode(CString p_instanceCode);//�Ƴ�ĳ���Ŵ���ŵĶ���
	bool UpdateObject(AcDbObjectId p_id);//�Ŵ������仯ʱ���ô˺������£��������Ʊ仯�����Ա仯
	bool UpdateObject(const AttrWindow& p_oldAtt, const AttrWindow& p_newAtt); //ĳ�����͵��Ŵ�ȫ������Ϊ�µ�����
	bool RenameWindows(const CString p_preName, const CString p_newName);//��ָ������������
	
	bool IsInstanceCodeExist(CString p_code);
	vector<AcDbObjectId> GetAllIdsByInstantCode(CString p_code);
	vector<AcDbObjectId> GetAllIds();


	void CheckAndRemoveObjectNotBelong(); //���������AcDbObjectId�Ƿ���Ч�����Ƴ���ɾ�����ߺʹ��������Բ�һ�µ�ʵ��
	void RemoveAllObjects(); //�Ƴ����е�object�����Ǳ���ԭ�������ƿ�

	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);


	static CString GetMirrorInstanceCode(CString p_name);
	static bool IsSamePrototypeAndSize(CString p_instanceCode1, CString p_instanceCode2); //��������Ƿ�Ϊ��ͬ���Ŵ�ԭ�ͺͳߴ�
protected:
	CWinClassify* FindWinClassifyByAtt(const AttrWindow& p_att);
	CWinClassify* FindWinClassifyByInstantCode(const CString p_sCode);
	CWinClassify* FindWinClassifyByObjectId(const AcDbObjectId& p_id);

	void RemoveEmptyClassify();

protected:
	vector<CWinClassify> m_allTypeWindows; //���д���
};

