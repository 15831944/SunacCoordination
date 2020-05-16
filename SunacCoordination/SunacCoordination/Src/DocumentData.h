#pragma once

#include "..\Object\WindowDoor\WindowAutoName.h"
#include "..\Object\WindowDoor\InstanceCodeTextMrg.h"
#include "..\Object\WindowDoor\WinTangentOpenMap.h"
#include "..\Object\Kitchen\KitchenAutoName.h"
#include "..\Object\Bathroom\BathroomAutoName.h"
#include "ObjHighlight.h"

//�ĵ����ݣ���ͨ��GetCurDocData��ȡ���Ա�ȷ���õ����ǵ�ǰ�ĵ�������
class CDocumentData
{
public:
	CDocumentData();
	~CDocumentData();

	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);

public:
	CWindowAutoName m_winAutoName;
	CKitchenAutoName m_kitchenAutoName;
	CBathroomAutoName m_bathroomAutoName;

	CInstanceCodeTextMrg m_instanceCodeMrg;

	CWinTangentOpenMap m_winTangentMap;

	CObjsHighlight  m_hightLight; //���ڹ��̴�������Ҫ����

	bool m_bLoad;
};

CWindowAutoName* GetWindowAutoName();
CKitchenAutoName* GetKitchenAutoName();
CBathroomAutoName* GetBathroomAutoName();
CInstanceCodeTextMrg* GetInstanceCodeMrg();
CWinTangentOpenMap* GetWinTangentOpenMap();
CObjsHighlight* GetHightLightTool();


//////////////////////////////////////////////////////////////////////////
class CDocumentFactory
{
	CDocumentFactory();
public:
	virtual ~CDocumentFactory();
	static CDocumentFactory& Instance();

	CDocumentData* GetCurDocData();

	//��acrxEntryPoint����kLoadDwgMsg��Ϣʱ�Ƴ�
	CDocumentData* AddDocument(AcApDocument *pDoc);

	//��acrxEntryPoint����kUnloadDwgMsg��Ϣʱ�Ƴ�
	void RemoveDocument(AcApDocument *pDoc);
	void RemoveCurDoc();

	//��acrxEntryPoint����kUnloadAppMsg��Ϣʱ�������document
	void RemoveAllDocment();

protected:
	map<AcApDocument*, CDocumentData*> m_pElecMrgs;
};

