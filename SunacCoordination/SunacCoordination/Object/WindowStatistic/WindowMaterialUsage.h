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

	//virtual void CalcMateriaUsage();
	bool ExportReportToExcel(CString p_sReportFile); //������excel����
	bool ExportReportToExcel(Excel::CExcelUtil& p_excel); //������excel����

	
	//////////////////////////////////////////////////////////////////////////
	//���º���������д���ܱ�
	double GetHoleArea()const;
	double GetWindowFrameArea()const;
	CString GetAluminumeSeries()const ;		//��ȡ�ô������Ͳ�ϵ��

	double GetAluminumeUsageAmount()const ; //��ȡ�ô������Ͳ�����
	double GetGlassUsageAmount()const ;		//��ȡ�ô����Ĳ�������

protected:
	double GetVauleByFomula(CString p_sFomula);

	virtual void ExportWindowInfo(Excel::CExcelUtil& p_excel);//���������Ϣ
	virtual void ExprotAlInfo(Excel::CExcelUtil& p_excel);//����Ͳ�����
	virtual void ExportGlassInfo(Excel::CExcelUtil& p_excel); //�����������
	virtual void ExportHardwareInfo(Excel::CExcelUtil& p_excel);//����������
	virtual void ExportFuliaoInfo(Excel::CExcelUtil& p_excel);//�����������

	//�����������Ҫ�õ��������͵��Ͳģ����º�����ȡ��Ӧ���Ͳ�
	E_WindowDoorType  GetSpecialWinType(E_WindowDoorType winType, CString p_sAlName)const ; //p_sAlName���Ͳ�����
	CString GetSpecialAlSerial(E_WindowDoorType winType, CString p_sAlName)const;

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
	
	double m_yaxianTotalLength;//ѹ���ܳ�
	double m_chuangshanTotalLength;//�����ܳ�
	double m_shanliaoTotalLength; //�����ܳ�
	double m_menshanTotalLength; //�����ܳ�
};

//////////////////////////////////////////////////////////////////////////
//�ڿ���
class CWindowMaterialUsageNC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageNC(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsage(p_winAtt, p_nCount) {}

protected:
	
};


//////////////////////////////////////////////////////////////////////////
//�⿪��
class CWindowMaterialUsageWC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageWC(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsage(p_winAtt, p_nCount) {}

protected:
	virtual void ExportFuliaoInfo(Excel::CExcelUtil& p_excel);//�����������
};

//////////////////////////////////////////////////////////////////////////
//������
class CWindowMaterialUsageTC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageTC(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsage(p_winAtt, p_nCount) {}

protected:
	virtual void ExportFuliaoInfo(Excel::CExcelUtil& p_excel);//�����������
};

//////////////////////////////////////////////////////////////////////////
//������
class CWindowMaterialUsageTLM :public CWindowMaterialUsageTC
{
public:
	CWindowMaterialUsageTLM(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsageTC(p_winAtt, p_nCount) {}

protected:
	
};

//����������
class CWindowMaterialUsageTSTLM :public CWindowMaterialUsageTC
{
public:
	CWindowMaterialUsageTSTLM(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsageTC(p_winAtt, p_nCount) {}

protected:

};

//////////////////////////////////////////////////////////////////////////
//�⿪��
class CWindowMaterialUsageWM :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageWM(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsage(p_winAtt, p_nCount) {}

protected:
	virtual void ExportFuliaoInfo(Excel::CExcelUtil& p_excel);//�����������
};