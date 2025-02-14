#include "StdAfx.h"
#include "DeductedSize.h"
#include "WindowFormula.h"
#include "AluminumSeries.h"
#include "..\..\Tool\SQLite\sqlite3.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_Convert.h"


CDeductedSize* CDeductedSize::Instance()
{
	static CDeductedSize instance;
	return &instance;
}
CDeductedSize::CDeductedSize()
{
}

CDeductedSize::~CDeductedSize()
{
}

sqlite3 * pDB2 = NULL;
static double localdata = 0;

static int OutputDeductedSize(void *NotUsed, int nCol, char **value, char **ColName)
{
	localdata = atof(value[0]);
	return 0;
}

bool CDeductedSize::GetDeductedSizeBySeriesAndName(E_WindowAluminumType p_winType, CString p_serials, CString sDeductedName, double &p_valueOut)
{
	CString sWinType = WindowDoorAluminumTypeToCSting(p_winType);
	return GetDeductedSizeBySeriesAndName(sWinType, p_serials, sDeductedName, p_valueOut);
}


bool CDeductedSize::GetDeductedSizeBySeriesAndName(CString p_winType, CString p_serials, CString sDeductedName, double &p_valueOut)
{
	const char * path = TY_GetAluminumDatabasePath();
	int nRes = sqlite3_open(path, &pDB2);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return false;
	}
	char* cErrMsg;
	CString sqlString;

	USES_CONVERSION;
	sqlString.Format(L"select %s from `DeductedSize` where `Type` = '%s' and `ProfileSeries` = '%s';", sDeductedName, p_winType, p_serials);
	char* sql = T2A(sqlString);
	ConvertStringToUTF8(sqlString, sql);
	//char* convert = T2A(GBKToUTF8(sql));
	int res = sqlite3_exec(pDB2, sql, OutputDeductedSize , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return false;
	}
	p_valueOut = localdata;

	return true;
}