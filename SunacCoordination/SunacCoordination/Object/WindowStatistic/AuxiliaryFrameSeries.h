#pragma once
#include <vector>

using namespace std;


enum EAuxiliaryFrameType
{
	EAuxiliaryFrame_�ָ���,
	EAuxiliaryFrame_���ܸ���,
};

class CAuxiliaryFrameData
{
public:
	CAuxiliaryFrameData();
	~CAuxiliaryFrameData();

	EAuxiliaryFrameType m_type;
	CString m_sName;
};

class CAuxiliaryFrameSeries
{
	CAuxiliaryFrameSeries();
public:
	static CAuxiliaryFrameSeries* Instance();
	virtual ~CAuxiliaryFrameSeries();

	static vector<CString> GetAllAuxiliaryFrameSerials();
	static vector<CString> GetAllAuxiliaryFrameSerials(EAuxiliaryFrameType p_type);

	static vector<double>  GetPlugSlotSize(bool p_bHasAuxiliaryFrame); //����ߴ�
};