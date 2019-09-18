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
	std::vector<AttrWindow *>  GetWindows(double width, CString openType, int openNum, CString gongNengQu, double tongFengLiang	);

	//��excel�ڶ���һ���ߴ��6��CStringת����dimdata
	RCDimData ConvertStringToDimData
	(
		CString code,
		CString  valueType,
		CString value,
		CString defaultValue,
		CString state
	);

	vector<AttrWindow> GetAllWindows();
protected:
	vector<AttrWindow> m_windows;
};

