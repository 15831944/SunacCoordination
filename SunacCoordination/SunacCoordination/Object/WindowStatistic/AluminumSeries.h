#pragma once

enum E_WindowDoorType
{
	E_WindowDoor_NC = 0,	//�ڿ���
	E_WindowDoor_WC,		//�⿪��
	E_WindowDoor_TC,		//������

	E_WindowDoor_WM = 0x10,	//�⿪��
	E_WindowDoor_TLM,		//������
	E_WindowDoor_TSTLM,		//����������
};

E_WindowDoorType ToWindowDoorType(CString type);
CString WindowTypeToCString(E_WindowDoorType type);



enum E_AluminumType
{
	E_���Ÿ������Ͳ�,
	E_�Ƕ��Ÿ������Ͳ�,
	E_�����������Ͳ�,
};

E_AluminumType ToE_AluminumType(CString type);
CString AluminumTypeToCSting(E_AluminumType type);

//�Ͳ�����
struct CAluminumData
{
	CString sCode; //�ͲĴ���
	E_WindowDoorType windowDoorType;   //�Ŵ�����
	CString sName;  //�Ͳ�����
	CString sSerial; //�Ͳ�ϵ��
	double weightPerMeter; //����
	double wastageRate; //�����
	E_AluminumType aluminumType; //�Ͳ�����

	CAluminumData()
	{
		weightPerMeter = 0;
		windowDoorType = E_WindowDoor_NC;
		wastageRate = 0.125;
		aluminumType = E_���Ÿ������Ͳ�;
	}
}; 

//�Ͳ�ϵ�� //YMY
class CAluminumSeries
{
	CAluminumSeries();
public:
	static CAluminumSeries* Instance();
	virtual ~CAluminumSeries();

	void InitAluminum();

	//ͨ���Ŵ����͡��Ͳ�ϵ�У��Ͳ����ƻ�ȡ�Ͳ�����,��ȡʧ�ܷ���false
	bool GetAluminumDataBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sName, CAluminumData& p_dataOut);

	//ͨ���Ͳı�Ż�ȡ��ǰ�Ͳı��������ϵ��
	bool GetAluminumSerialByCode(CString p_code, CString& p_serialOut);

	//����Ŵ������µ��Ͳ�ϵ��ѡ��
	vector<CString> GetAluminumSerialsByWindowType(E_WindowDoorType p_winType);


	//ʹ�ñ�AluminumSeries


protected:
};
