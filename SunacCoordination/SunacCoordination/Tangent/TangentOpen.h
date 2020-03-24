#pragma once

struct CTOpenData
{
	double width; 
	double height;
	double bottomHeight; //��̨��
	CString sWinCode;	//�Ŵ����
};


class CTangentOpen
{
public:
	static HRESULT SetTangentOpenProp( AcDbObjectId p_winId, CTOpenData p_winData);

	static HRESULT InsertWinOpenning(AcGePoint3d p_centerPt, CTOpenData p_winData, AcDbObjectId &p_tWinOpenIdOut);

protected:
	static HRESULT SetTangentOpenProp_TCH10_COM9_T20V5X64( AcDbObjectId p_winId, CTOpenData p_winData);

};


