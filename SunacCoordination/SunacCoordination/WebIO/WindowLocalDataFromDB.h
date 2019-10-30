#pragma  once

#include <vector>
#include "..\Tool\Excel\Excel.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "..\Tool\SQLite\sqlite3.h"

using namespace std;

class CWindowLocalDataFromDB
{
public:
	CWindowLocalDataFromDB();
	~CWindowLocalDataFromDB();
	static CWindowLocalDataFromDB* Instance();

//�Ŵ�
public:
	bool GetWindowById(CString p_sId, AttrWindow& value)const;  //ͨ��ԭ�ͱ�Ŵ�m_windows�л�ȡ����
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value)const;//ͨ���ļ�����m_windows�л�ȡ����
	static int OutputWindowData(void *NotUsed, int nCol, char **value, char **ColName);//���ݿ��ѯ����

	//width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
	//openType, //��������
	//openNum,//����������
	//gongNengQu,//������
	//tongFengLiang//ͨ����
	//��ȡ�źʹ�
	std::vector<AttrWindow >  GetWindows(double width, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetDoors(double width, CString openType, int openNum, CString gongNengQu)const;

	vector<AttrWindow> GetAllWindows()const; //��ȡ���д���
	vector<AttrWindow> GetAllDoors()const;  //��ȡ������

protected:



protected:
	void LoadDataFromDataBase(); //�����ݿ��а����ݴ���m_windows��

	static CWindowsDimData GetWindowDimData(CString code, char **ColName, int nStart);

protected:
	vector<AttrWindow> m_windows; //�����Ŵ�
	vector<AttrWindow> m_wins;   //���д�
	vector<AttrWindow> m_doors;  //������
	sqlite3 *m_pDB;
};

