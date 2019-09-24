#pragma  once

#include <vector>
#include "../Object/AttrWindow.h"
#include "../Object/AttrAirCon.h"

using namespace std;

class CLocalData
{
	
public:
	static CLocalData *GetInstance();

private:
	CLocalData();
	~CLocalData();

//�Ŵ�
public:
	void LoadDataFromExcel(CString p_file); //�ӱ���а����ݴ���m_windows��

	bool GetWindowById(CString p_sId,AttrWindow& value);  //ͨ��ԭ�ͱ�Ŵ�m_windows�л�ȡ����
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value);//ͨ���ļ�����m_windows�л�ȡ����

	
	//width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
	//openType, //��������
	//openNum,//����������
	//gongNengQu,//������
	//tongFengLiang//ͨ����
	//��ȡ�źʹ�
	std::vector<AttrWindow >  GetWindows(double width, CString openType, int openNum, CString gongNengQu);
	std::vector<AttrWindow >  GetDoors(double width, CString openType, int openNum, CString gongNengQu);

	//��excel�ڶ���һ���ߴ��6��CStringת����dimdata
	RCDimData ConvertStringToDimData
	(
		CString code,
		CString  valueType,
		CString value,
		CString defaultValue,
		CString state
	);

	vector<AttrWindow> GetAllWindows(); //��ȡ���д���

	vector<AttrWindow> GetAllDoors();  //��ȡ������

protected:
	vector<AttrWindow> m_windows; //�����Ŵ�
	vector<AttrWindow> m_wins;   //���д�
	vector<AttrWindow> m_doors;  //������

//�յ�
public:
	bool GetAirConById(CString p_sId,AttrAirCon& value);  //ͨ��ԭ�ͱ�Ŵ�m_aircon�л�ȡ�յ�
	bool GetAirConByFileName(CString p_sFileName, AttrAirCon&value);//ͨ���ļ�����m_aircon�л�ȡ�յ�

	vector<AttrAirCon> GetAllAirCon(); //��ȡ���пյ�
	std::vector<AttrAirCon >  GetAirCon(CString p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos);

protected:
	vector<AttrAirCon> m_aircon; //���пյ�
};

