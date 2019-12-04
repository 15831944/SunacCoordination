#pragma once


//�ĵ����ݣ���ͨ��GetCurElecMrg��ȡ���Ա�ȷ���õ����ǵ�ǰ�ĵ�������
class CDocumentData
{
public:
	CDocumentData();
	~CDocumentData();

protected:

};

//////////////////////////////////////////////////////////////////////////
class CDocumentFactory
{
	CDocumentFactory();
public:
	static CDocumentFactory& Instance();
	static CDocumentData* GetCurElecMrg();

	static void RemoveCurElecMrg();

	virtual ~CDocumentFactory();

	//�Ƿ�Ϊ��ʱ����dwg����Ԥ������dwgͼ����dwg��Ϊ�����������SetTempLoadDwg(true) SetTempLoadDwg(false) ����ʹ��
	static bool IsTempLoadDwg() { return Instance().m_bTempLoadDwg; }
	static void SetTempLoadDwg(bool bTemp);

protected:
	map<AcApDocument*, CDocumentData*> m_pElecMrgs;

	bool m_bTempLoadDwg;
};

