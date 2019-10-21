#pragma once
#include "..\WindowDoor\AttrWindow.h"
#include "..\..\Tool\Excel\Excel.h"
#include "DeductedSize.h"
#include "AluminumSeries.h"
#include "WindowFormula.h"



//��������Ϸ�����  //ZJY
class CWindowMaterialUsage
{
public:
	CWindowMaterialUsage(const AttrWindow& p_winAtt, int p_nCount);  //p_nCount��ʾ��ǰ���͵�����
	virtual ~CWindowMaterialUsage();

	virtual void CalcMateriaUsage();
	virtual bool ExportReportToExcel(CString p_sReportFile); //������excel����
	virtual bool ExportReportToExcel(Excel::CExcelUtil& p_excel); //������excel����
	void GetAllData();
	
	//////////////////////////////////////////////////////////////////////////
	//���º���������д���ܱ�
	double GetHoleArea()const;
	double GetWindowFrameArea()const;
	CString GetAluminumeSeries()const ;		//��ȡ�ô������Ͳ�ϵ��

	double GetAluminumeUsageAmount()const ; //��ȡ�ô������Ͳ�����
	double GetGlassUsageAmount()const ;		//��ȡ�ô����Ĳ�������


	//////////////////////////////////////////////////////////////////////////


protected:
	//������ʽ
	void ParseFomula(CString p_sFomula);

	double GetVauleByFomula(CString p_sFomula);

	virtual void ExportWindowInfo(Excel::CExcelUtil& p_excel);//���������Ϣ
	virtual void ExprotAlInfo(Excel::CExcelUtil& p_excel);//����Ͳ�����
	virtual void ExportGlassInfo(Excel::CExcelUtil& p_excel); //�����������
	virtual void ExportHardwareInfo(Excel::CExcelUtil& p_excel);//����������
	virtual void ExportFuliaoInfo(Excel::CExcelUtil& p_excel);//�����������



protected:
	const AttrWindow m_winAtt;
	int m_nCount;
	
	//////////////////////////////////////////////////////////////////////////
	double m_alTotalQTY;  //�ܼ�����:
	double m_broAlQTY;    //���Ÿ������Ͳ�
	double m_nonBroAlQTY; //�Ƕ��Ÿ������Ͳ�
	double m_anoAlQTY;	  //�����������Ͳ�

	double m_glassTotalQTY;	//�����ϼ�

	int m_hardwareNumber;	//�������

	double m_foamingQTY;	//���ݼ�
	double m_glassSealantQTY; //���Թ�ͪ�ܷ⽺(�������ͺ�)
	double m_inSealantQTY;  //���Թ�ͪ�ܷ⽺(��ǽ��)
	double m_exSealantQTY;  //���Թ�ͪ�ܷ⽺(��ǽ��)
	double m_comGlueQTY;	//��ǽ�
	double m_sealStripQTY;	//�ܷ⽺��(m)
	double m_squSealStripQTY;//�ܷ⽺��(�O)
	double m_sealBurrsQTY;	//�ܷ�ë��
	double m_windowCost;	//��������װ���ķ�

	
	double m_yShuliaoTotallength;//ѹ�������ܳ�
	double m_yHengliaoTotallength;//ѹ�ߺ����ܳ�

	double m_cShuliaoTotallength;//���������ܳ�
	double m_cHengliaoTotallength;//���Ⱥ����ܳ�
};

//////////////////////////////////////////////////////////////////////////
//�ڿ���
class CWindowMaterialUsageNC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageNC(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsage(p_winAtt, p_nCount) {}
	~CWindowMaterialUsageNC() {}

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