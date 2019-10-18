#include "StdAfx.h"
#include "AluminumSeries.h"
#include "..\..\Tool\SQLite\sqlite3.h"
#include "DeductedSize.h"
#include "WindowFormula.h"

CAluminumSeries* CAluminumSeries::Instance()
{
	static CAluminumSeries instance;
	return &instance;
}

CAluminumSeries::CAluminumSeries()
{
}

CAluminumSeries::~CAluminumSeries()
{
}

CAluminumData AluminumData;
sqlite3 * pDB3 = NULL;
CString AlString;
vector<CString> AlSeries;

E_WindowDoorType ToE_WindowDoorType(CString type)
{
	if (type == "内开窗")
	{
		return E_WindowDoor_NC;
	}
	else if (type == "外开窗")
	{
		return E_WindowDoor_WC;
	}
	else if (type == "推拉窗")
	{
		return E_WindowDoor_TC;
	}
	else if (type == "外开门")
	{
		return E_WindowDoor_WM;
	}
	else if (type == "推拉门")
	{
		return E_WindowDoor_TLM1;
	}
	else if (type == "提升推拉门")
	{
		return E_WindowDoor_TLM2;
	}
	else
	{
		return E_WindowDoor_NC;
	}
}

CString WindowTypeToCString(E_WindowDoorType type)
{
	switch(type)
	{
	case E_WindowDoor_NC:
		{
			return L"内开窗";
			break;
		}
	case E_WindowDoor_WC:
		{
			return L"外开窗";
			break;
		}
	case E_WindowDoor_TC:
		{
			return L"推拉窗";
			break;
		}
	case E_WindowDoor_WM:
		{
			return L"外开门";
			break;
		}
	case E_WindowDoor_TLM1:
		{
			return L"推拉门";
			break;
		}
	case E_WindowDoor_TLM2:
		{
			return L"提升推拉门";
			break;
		}
	default:
		{
			return L"内开窗";
			break;
		}

	}
}

E_AluminumType ToE_AluminumType(CString type)
{
	if (type == "断桥隔热铝型材")
	{
		return E_断桥隔热铝型材;
	}
	else if (type == "非断桥隔热铝型材")
	{
		return E_非断桥隔热铝型材;
	}
	else if (type == "阳极氧化铝型材")
	{
		return E_阳极氧化铝型材;
	}
	else return E_断桥隔热铝型材;
}

CString AluminumTypeToCSting(E_AluminumType type)
{
	switch(type)
	{
	case E_断桥隔热铝型材:
		{
			return L"断桥隔热铝型材";
			break;
		}
	case E_非断桥隔热铝型材:
		{
			return L"非断桥隔热铝型材";
			break;
		}
	case E_阳极氧化铝型材:
		{
			return L"阳极氧化铝型材";
			break;
		}
	default:
		{
			return L"断桥隔热铝型材";
			break;
		}
	}
}

static int OutputAlData(void *NotUsed, int nCol, char **value, char **ColName)
{
	for (int i = 0; i < nCol; i++)
	{
		if (strcmp(ColName[i], "WindowDoorType") == 0)
		{
			AluminumData.windowDoorType = ToE_WindowDoorType(UTF8ToGBK(value[i]));
		}
		else if (strcmp(ColName[i],"Code") == 0)
		{
			AluminumData.sCode = value[i];
		}
		else if (strcmp(ColName[i] ,"Name") == 0)
		{
			AluminumData.sName = UTF8ToGBK(value[i]);		
		}
		else if (strcmp(ColName[i] ,"Serial") == 0)
		{
			AluminumData.sSerial = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i] , "WeightPerMeter") == 0)
		{
			AluminumData.weightPerMeter = atof(value[i]);
		}
		else if (strcmp(ColName[i] , "WastageRate") == 0)
		{
			AluminumData.wastageRate = atof(value[i]);
		}
		else if (strcmp(ColName[i], "AluminumType") == 0)
		{
			AluminumData.aluminumType = ToE_AluminumType(UTF8ToGBK(value[i]));
		}
	}
	return 0;
}

static int OutputAlSerial(void *NotUsed, int nCol, char **value, char **ColName)
{
	AlString = UTF8ToGBK(value[0]);
	return 0;
}

static int OutputAlSeriesFromWindowType(void *NotUsed, int nCol, char **value, char **ColName)
{
	AlSeries.push_back(UTF8ToGBK(value[0]));
	return 0;
}

bool CAluminumSeries::GetAluminumDataBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sName, CAluminumData& p_dataOut)
{
	int nRes = sqlite3_open("C:/Program Files/Autodesk/AutoCAD 2014/Support/Sunac2019/RCData.db", &pDB3);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return false;
	}
	char* cErrMsg;
	CString sWinType = WindowTypeToCString(p_winType);
	CString sqlString;
	sqlString.Format(L"select * from `AluminumSeries` where `WindowDoorType` = '%s' and `Serial` = '%s' and `Name` = '%s';", sWinType, p_serials, sName);
	USES_CONVERSION;
	char* sql;
	ConvertStringToUTF8(sqlString, sql);
	int res = sqlite3_exec(pDB3, sql, OutputAlData , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return false;
	}

	p_dataOut = AluminumData;
	sqlite3_close(pDB3);
	return true;
}

bool CAluminumSeries::GetAluminumSerialByCode(CString p_code, CString& p_serialOut)
{
	int nRes = sqlite3_open("C:/Program Files/Autodesk/AutoCAD 2014/Support/Sunac2019/RCData.db", &pDB3);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return false;
	}
	char* cErrMsg;
	CString sqlString;
	sqlString.Format(L"select Serial from `AluminumSeries` where `Code` = '%s';", p_code);
	USES_CONVERSION;
	char* sql;
	ConvertStringToUTF8(sqlString, sql);
	int res = sqlite3_exec(pDB3, sql, OutputAlSerial , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return false;
	}

	p_serialOut = AlString;
	sqlite3_close(pDB3);
	return true;
}

vector<CString> CAluminumSeries::GetAluminumSerialsByWindowType(E_WindowDoorType p_winType)
{
	AlSeries.clear();
	int nRes = sqlite3_open("C:/Program Files/Autodesk/AutoCAD 2014/Support/Sunac2019/RCData.db", &pDB3);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return AlSeries;
	}
	char* cErrMsg;
	CString sWinType = WindowTypeToCString(p_winType);
	CString sqlString;
	sqlString.Format(L"select distinct Serial  from `AluminumSeries` where `WindowDoorType` = '%s' and `Serial`  !='';", sWinType);
	USES_CONVERSION;
	char* sql;
	ConvertStringToUTF8(sqlString, sql);
	int res = sqlite3_exec(pDB3, sql, OutputAlSeriesFromWindowType , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return AlSeries;
	}

	sqlite3_close(pDB3);
	return AlSeries;
}