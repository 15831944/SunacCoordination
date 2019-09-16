#pragma  once

#include <vector>
#include "../Object/AttrWindow.h"


using namespace std;

class CWindowLocalData
{
	CWindowLocalData();
public:
	~CWindowLocalData();

	static CWindowLocalData *GetInstance();
	
	void LoadFromExcel(CString p_file);

	const AttrWindow& GetWindowById(CString p_sId);
	const AttrWindow& GetWindowByFileName(CString p_sFileName);

	//width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
	//openType, //��������
	//openNum,//����������
	//gongNengQu,//������
	//tongFengLiang//ͨ����
	std::vector<AttrWindow *>  GetWindows(double width, CString openType, int openNum, CString gongNengQu, double tongFengLiang	);

protected:
	vector<AttrWindow> m_windows;

};

