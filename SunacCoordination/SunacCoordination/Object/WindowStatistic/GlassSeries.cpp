#include "StdAfx.h"
#include "AluminumSeries.h"
#include "GlassSeries.h"
#include "..\..\Tool\SQLite\sqlite3.h"
#include "..\..\Common\ComFun_Sunac.h"


CGlassData::CGlassData()
{
	m_heatInsulationCoefficient = 0;
	m_shadingCoefficient = 0; 
	m_glassCount = 2;
}

CGlassData::~CGlassData()
{
	
}


CGlassSeries* CGlassSeries::Instance()
{
	static CGlassSeries instance;
	return &instance;
}

CGlassSeries::CGlassSeries()
{
}

CGlassSeries::~CGlassSeries()
{
}

vector<CString> CGlassSeries::GetAllGlassSerials()
{
	vector<CString> glassSerials;
	glassSerials.push_back(_T("5+9A+5"));
	glassSerials.push_back(_T("5+12A+5"));
	glassSerials.push_back(_T("5+12A+5+12A+5"));
	glassSerials.push_back(_T("5+12A+5+12A+5Low-E"));
	glassSerials.push_back(_T("5+12A+5Low-E"));
	glassSerials.push_back(_T("5+12A+5˫��Low-E"));
	glassSerials.push_back(_T("5+12Ar+5(2#��Low-E)"));
	glassSerials.push_back(_T("5+12Ar+5+12Ar+5(����Low-E)"));
	glassSerials.push_back(_T("5+12Ar+5+12Ar+5Low-E˫��"));
	glassSerials.push_back(_T("5+12Ar+5+12Ar+5����Low-E��˫ů�ߣ�"));
	glassSerials.push_back(_T("5+12Ar+5Low-E˫��"));
	glassSerials.push_back(_T("5+6A+5"));
	glassSerials.push_back(_T("5+9A+5"));
	glassSerials.push_back(_T("5Low-E+12A+5"));
	glassSerials.push_back(_T("5Low-E+12Ar+5"));
	glassSerials.push_back(_T("5Low-E+15Ar+5��������ů�ߣ�"));
	glassSerials.push_back(_T("5Low-E+6A+5"));
	glassSerials.push_back(_T("5Low-E+9A+5"));
	glassSerials.push_back(_T("5˫��Low-E+12A+5"));
	glassSerials.push_back(_T("6+12A+6"));
	glassSerials.push_back(_T("6Low-E+12A+6"));
	glassSerials.push_back(_T("6Low-E+9A+6"));
	glassSerials.push_back(_T("6����Low-E+12A+6"));
	glassSerials.push_back(_T("6����Low-E+12Ar+6"));
	glassSerials.push_back(_T("6��͸+12Ar+6��������ů�ߣ�"));
	glassSerials.push_back(_T("6��͸Low-E+12A+6"));
	glassSerials.push_back(_T("6��͸��Low-E+12A+6"));
	glassSerials.push_back(_T("6˫��Low-E+12A+6"));
	glassSerials.push_back(_T("6��͸��Low-E+12A+6"));
	glassSerials.push_back(_T("6��͸��Low-E+9A+6"));	

	//������Ϊ�����ݿ�

	return glassSerials;
}