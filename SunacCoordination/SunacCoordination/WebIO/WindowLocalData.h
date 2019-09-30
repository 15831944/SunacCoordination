#pragma  once

#include <vector>
#include "../Object/AttrWindow.h"

using namespace std;

class CWindowLocalData
{
public:
	CWindowLocalData();
	~CWindowLocalData();

//�Ŵ�
public:
	bool GetWindowById(CString p_sId, AttrWindow& value)const;  //ͨ��ԭ�ͱ�Ŵ�m_windows�л�ȡ����
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value)const;//ͨ���ļ�����m_windows�л�ȡ����

	//width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
	//openType, //��������
	//openNum,//����������
	//gongNengQu,//������
	//tongFengLiang//ͨ����
	//��ȡ�źʹ�
	std::vector<AttrWindow >  GetWindows(double width, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetDoors(double width, CString openType, int openNum, CString gongNengQu)const;


protected:
	vector<AttrWindow> GetAllWindows(); //��ȡ���д���
	vector<AttrWindow> GetAllDoors();  //��ȡ������


protected:
	void LoadDataFromExcel(CString p_file); //�ӱ���а����ݴ���m_windows��

	//��excel�ڶ���һ���ߴ��6��CStringת����dimdata
	RCDimData ConvertStringToDimData
	(
		CString code,
		CString  valueType,
		CString value,
		CString defaultValue,
		CString state
		)const;

protected:
	vector<AttrWindow> m_windows; //�����Ŵ�
	vector<AttrWindow> m_wins;   //���д�
	vector<AttrWindow> m_doors;  //������


};

