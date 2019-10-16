#pragma once
#include "..\WindowDoor\AttrWindow.h"
#include "..\..\Tool\Excel\Excel.h"
#include "DeductedSize.h"
#include "AluminumSeries.h"



//��������Ϸ�����
class CWindowMaterialUsage
{
public:
	CWindowMaterialUsage(const AttrWindow& p_winAtt, int p_nCount);  //p_nCount��ʾ��ǰ���͵�����
	virtual ~CWindowMaterialUsage();

	virtual void CalcMateriaUsage() = 0;
	virtual bool ExportReportToExcel(CString p_sReportFile) = 0; //������excel����
	virtual bool ExportReportToExcel(Excel::CExcelUtil& p_excel) = 0; //������excel����
	
	//////////////////////////////////////////////////////////////////////////
	//���º���������д���ܱ�
	double GetHoleArea()const;
	double GetWindowFrameArea()const;
	CString GetAluminumeSeries()const ;		//��ȡ�ô������Ͳ�ϵ��

	double GetAluminumeUsageAmount()const ; //��ȡ�ô������Ͳ�����
	double GetGlassUsageAmount()const ;		//��ȡ�ô����Ĳ�������


	//////////////////////////////////////////////////////////////////////////


protected:
	//����ԭ������p_propotypeName����������p_sMatName���Ҽ��㹫ʽ
	CString LookUpFomula(CString p_propotypeName, CString p_sMatName);

	//����ԭ������p_propotypeName����������p_sMatName��������
	CString LookUpCount(CString p_propotypeName, CString p_sMatName);


	//������ʽ
	void ParseFomula(CString p_sFomula);


protected:
	AttrWindow m_winAtt;
	int m_nCount;
	//////////////////////////////////////////////////////////////////////////
		//�ܼ�����:
		//���Ÿ������Ͳ�
		//�Ƕ��Ÿ������Ͳ�
		//�����������Ͳ�

		//�����ϼ�

		//�������

		//���ݼ�
		//���Թ�ͪ�ܷ⽺
		//���Թ�ͪ�ܷ⽺
		//���Թ�ͪ�ܷ⽺
		//��ǽ�
		//�ܷ⽺��
		//�ܷ⽺��
		//�ܷ�ë��
		//��������װ���ķ�
};

//////////////////////////////////////////////////////////////////////////
//�ڿ���
class CWindowMaterialUsageNC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageNC();
	~CWindowMaterialUsageNC();

protected:

};


//////////////////////////////////////////////////////////////////////////
//�⿪��
class CWindowMaterialUsageWC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageWC();
	~CWindowMaterialUsageWC();

protected:

};

//////////////////////////////////////////////////////////////////////////
//������
class CWindowMaterialUsageTC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageTC();
	~CWindowMaterialUsageTC();

protected:

};

//////////////////////////////////////////////////////////////////////////
//������,������������
class CWindowMaterialUsageTLM :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageTLM();
	~CWindowMaterialUsageTLM();

protected:

};

//////////////////////////////////////////////////////////////////////////
//�⿪��
class CWindowMaterialUsageWM :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageWM();
	~CWindowMaterialUsageWM();

protected:

};