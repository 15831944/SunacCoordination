#pragma once
#include "WindowMaterialUsage.h"


//�ۼ��ߴ�
class CDeductedSize
{
public:
	CDeductedSize();
	virtual ~CDeductedSize();

	//ͨ���Ŵ����͡��Ͳ�ϵ�У��Ͳ����ƻ�ȡ�Ͳ�����,û���ҵ�����false
	bool GetDeductedSizeBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sDeductedName, double &p_valueOut);


protected:

};
