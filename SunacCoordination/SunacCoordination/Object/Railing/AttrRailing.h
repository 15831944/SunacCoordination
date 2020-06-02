
#pragma once

#include "dbmain.h"
#include "../AttrObject.h"
#include "../../Sunac_def.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_RAILING
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_RAILING "ZFFCUSTOMOBJECTDB_DBXSERVICE_RAILING"
#endif

class DLLIMPEXP  AttrRailing : public AttrObject
{
public:
	ACRX_DECLARE_MEMBERS(AttrRailing);

	AttrRailing();
	virtual ~AttrRailing();

	//AttrRailing(const AttrRailing &other);
	//virtual AttrRailing & operator=(const AttrRailing &rhs);

	virtual bool isEqualTo(const AttrObject*other)const;//��������һ��
	virtual bool IsPrototypeEqual_test(const AttrRailing& p_att);
	virtual eRCType GetType() {return S_RAILING;}

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	CString AutoInstanceCode(); //�Զ����
	
	virtual void SetViewDir(eViewDir p_view);

	void SetRLength(double p_len);
	double GetRLength()const { return m_length; }
	double GetRLength(E_DIRECTION p_dir);
	double GetTotalLength()const;

	void SetRailingPath(vector<AcGePoint3d> p_pathPts);

protected:
	double m_length;	//���˳�
	vector<AcGePoint3d> m_railPath; //����·�������λ�õ㣬�Ե�һ��Ϊԭ�㣩

public:
    double m_height;		//���˸߶�
	double m_fanKanHeight;	//�����߶�
	eRailingType m_railingType; //��������
};

class CRailingAndCount
{
public:
	CRailingAndCount(){}

public:
	AttrRailing railAtt;
	int nCount;

	AcDbObjectIdArray objIds;
};

class CRailingCountArray
{
public:
	CRailingCountArray() {}

	void InitByRailingIds(const vAcDbObjectId& p_railingIds);
	void InitByRailingAtts(const vector<AttrRailing>& p_railingAtts, const vector<AcDbObjectId>& p_ids);

	int GetCount()const { return (int)(m_railCountArray.size()); }

	const CRailingAndCount& GetRailing(int p_index){ return m_railCountArray[p_index]; }

private:
	vector<CRailingAndCount> m_railCountArray;
};

