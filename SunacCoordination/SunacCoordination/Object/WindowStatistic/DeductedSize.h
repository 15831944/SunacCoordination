#pragma once
#include "AluminumSeries.h"


//�ۼ��ߴ�� //YMY
class CDeductedSize
{
	CDeductedSize();
public:
	static CDeductedSize* Instance();
	virtual ~CDeductedSize();

	//ͨ���Ŵ����͡��Ͳ�ϵ�У��Ͳ����ƻ�ȡ�Ͳ�����,û���ҵ�����false
	bool GetDeductedSizeBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sDeductedName, double &p_valueOut);
	bool GetDeductedSizeBySeriesAndName(CString p_sWinType, CString p_serials, CString sDeductedName, double &p_valueOut);


protected:

};
