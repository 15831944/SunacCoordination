#pragma once

#include "..\Object\WindowDoor\WindowAutoName.h"
#include "..\Object\Kitchen\KitchenAutoName.h"
#include "..\Object\Bathroom\BathroomAutoName.h"

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

	bool m_bLoad;
};

CWindowAutoName* GetWindowAutoName();
CKitchenAutoName* GetKitchenAutoName();
CBathroomAutoName* GetBathroomAutoName();
CInstanceCodeTextMrg* GetInstanceCodeMrg();


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

