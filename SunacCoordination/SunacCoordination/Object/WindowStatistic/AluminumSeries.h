#pragma once


//�Ŵ��Ͳķ���
enum E_WindowAluminumType  
{
	E_WindowAluminum_NC = 0,	//�ڿ����������ڿ������ڿ��ڵ���
	E_WindowAluminum_WC,		//�⿪���������⿪�����⿪������
	E_WindowAluminum_TC,		//������

	E_WindowAluminum_WM = 0x10,	//�⿪�ţ������⿪�š�������
	E_WindowAluminum_TLM,		//������
	E_WindowAluminum_TSTLM,		//����������
};

E_WindowAluminumType ToWindowDoorAluminumType(CString type);
CString WindowDoorAluminumTypeToCSting(E_WindowAluminumType type);


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
	E_WindowAluminumType windowDoorType;   //�Ŵ�����
	CString sName;  //�Ͳ�����
	CString sSerial; //�Ͳ�ϵ��
	double weightPerMeter; //����
	double wastageRate; //�����
	E_AluminumType aluminumType; //�Ͳ�����

	CAluminumData()
	{
		weightPerMeter = 0;
		windowDoorType = E_WindowAluminum_NC;
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
	bool GetAluminumDataBySeriesAndName(E_WindowAluminumType p_winType, CString p_serials, CString sName, CAluminumData& p_dataOut);

	//ͨ���Ͳı�Ż�ȡ��ǰ�Ͳı��������ϵ��
	bool GetAluminumSerialByCode(CString p_code, CString& p_serialOut);

	//����Ŵ������µ��Ͳ�ϵ��ѡ��
	vector<CString> GetAluminumSerialsByWindowType(E_WindowAluminumType p_winType);

	//���ԭ�͵��Ͳ�ϵ��ѡ��
	vector<CString> GetAluminumSerialsByPrototype(CString p_sProtoTypeCode);
	
protected:
};
