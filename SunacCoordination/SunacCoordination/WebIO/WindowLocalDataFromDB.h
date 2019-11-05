#pragma  once

#include <vector>
#include "..\Tool\Excel\Excel.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "..\Tool\SQLite\sqlite3.h"

using namespace std;

class CWindowLocalDataFromDB
{
	CWindowLocalDataFromDB();
public:
	~CWindowLocalDataFromDB();
	static CWindowLocalDataFromDB* Instance();

//�Ŵ�
public:
	bool GetWindowById(CString p_sId, AttrWindow& value)const;  //ͨ��ԭ�ͱ�Ŵ�m_windows�л�ȡ����
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value)const;//ͨ���ļ�����m_windows�л�ȡ����
	static int OutputWindowData(void *NotUsed, int nCol, char **value, char **ColName);//���ݿ��ѯ����

	//ע��߶�ֵ����Ϊ�������� 
	//width���ֵ��openType��������, openNum����������  gongNengQu������, tongFengLiangͨ����
	std::vector<AttrWindow >  GetWindows(double width, double height, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetDoors(double width, double height, CString openType, int openNum, CString gongNengQu)const;

	vector<AttrWindow> GetAllWindows()const; //��ȡ���д���
	vector<AttrWindow> GetAllDoors()const;  //��ȡ������

	vector<AttrWindow> GetAllWindowDoors()const{ return m_allWindowsData; }

protected:
	void LoadDataFromDataBase(); //�����ݿ��а����ݴ���m_windows��
	static CWindowsDimData GetWindowDimData(CString code, char **ColName, int nStart);

protected:
	vector<AttrWindow> m_allWindowsData; //�����Ŵ�
};

