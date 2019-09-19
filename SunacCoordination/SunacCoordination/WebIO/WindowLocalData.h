#pragma  once

#include <vector>
#include "../Object/AttrWindow.h"


using namespace std;

class CWindowLocalData
{
	
public:
	static CWindowLocalData *GetInstance();

private:
	CWindowLocalData();
	~CWindowLocalData();

	
public:
	void LoadFromExcel(CString p_file); //�ӱ���а����ݴ���m_windows��

	bool GetWindowById(CString p_sId,AttrWindow& value);  //ͨ��ԭ�ͱ�Ŵ�m_windows�л�ȡ����
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value);//ͨ���ļ�����m_windows�л�ȡ����

	
	//width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
	//openType, //��������
	//openNum,//����������
	//gongNengQu,//������
	//tongFengLiang//ͨ����
	//��ȡ�źʹ�
	std::vector<AttrWindow >  GetWindows(double width, CString openType, int openNum, CString gongNengQu);

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

};

