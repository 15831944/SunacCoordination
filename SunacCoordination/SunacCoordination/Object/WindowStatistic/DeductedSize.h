#pragma once
#include "AluminumSeries.h"

//�ۼ��ߴ�� //YMY
class CDeductedSize
{
	CDeductedSize();
public:
	static CDeductedSize* Instance();
	virtual ~CDeductedSize();

	//ͨ���Ŵ�����p_sWinType���Ͳ�ϵ��p_serials���Ͳ�����sDeductedName��ȡ�Ͳ�����,û���ҵ�����false
	bool GetDeductedSizeBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sDeductedName, double &p_valueOut);

	//ʹ�ñ�DeductedSize
protected:
	bool GetDeductedSizeBySeriesAndName(CString p_sWinType, CString p_serials, CString sDeductedName, double &p_valueOut);

};

size_t ConvertStringToUTF8( LPCTSTR strIn, char *& strOutUTF8MB );