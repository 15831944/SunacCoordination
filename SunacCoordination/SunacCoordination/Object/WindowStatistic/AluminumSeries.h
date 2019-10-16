#pragma once
#include "WindowMaterialUsage.h"


struct CAluminumData
{
	CString sCode; //�ͲĴ���
	CString sName; 
	CString sSerial;
	double weightPerMeter; //����

	CAluminumData()
	{
		weightPerMeter = 0;
	}
}; 

class CAluminumSeries
{
public:
	CAluminumSeries();
	virtual ~CAluminumSeries();

	//ͨ���Ŵ����͡��Ͳ�ϵ�У��Ͳ����ƻ�ȡ�Ͳ�����
	bool GetAluminumCodeBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sName, CAluminumData& p_dataOut);


protected:

};
