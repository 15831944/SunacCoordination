#pragma once
#include <vector>
using namespace std;

#define  SUNAC_DEBUG
#define  SUNAC_ATTRIBUTE_ENTITY L"SUNAC_ATTRIBUTE_ENTITY"


#define  FILE_VERSION ((Adesk::Int32)5)
//�汾2���Ŵ��������Ʋ������Ա���
//20191113 �汾3���Ŵ����Ӵ���ǽ�߶�����
//20191213 �汾4���Ŵ����ӹ����Ŵ��ͱ������Ŵ��ľ��
//20200317 �汾5���Ŵ����ӷ�������

#define  WORK_LOCAL//�Ƿ񱾵ع���ģʽ
#define	 CHECK_USE//�Ƿ���Ȩ��

//ģ������
typedef enum eRCType
{
    WINDOW,//�ⴰ
	DOOR,//��
	KITCHEN,//����
	Bathroom,//������
	AIRCON,//�յ�
	RAILING,//����
	TYPENUM
}eRCType;


enum eViewDir
{
	E_VIEW_FRONT,//����ͼ
	E_VIEW_TOP,	//ƽ��ͼ
	E_VIEW_LEFT,//����ͼ
	E_VIEW_EXTEND,//չ��ͼ

	E_VIEW_ALL = 0xff,
};

CString ViewDir2String(const eViewDir p_viewDir);

enum E_DIRECTION
{
	E_DIR_BOTTOM = 0,	//�²ࡢ�Ϸ�
	E_DIR_RIGHT,	//�Ҳࡢ����
	E_DIR_TOP,		//�ϲࡢ����
	E_DIR_LEFT,		//��ࡢ����
	E_DIR_UNKNOWN
};

E_DIRECTION String2Direction(CString p_sDir);


typedef enum eRailingType
{
	E_RAILING_TIEYI,//����
	E_RAILING_BOLI,//����
	E_RAILING_ALL = 0x10,//ȫ��
}eRailingType;

eRailingType ToERailingType(CString type);

enum E_Prototype_Type
{
	E_DYNAMIC_PROTOTYPE,	//��̬ԭ��
	E_STATIC_PROTOTYPE,		//��̬ԭ��
};

typedef enum eWindowDoorPos//�Ŵ�λ�ù�ϵ
{
    DUIKAI,//�Կ�
	CHUIZHIKAI//��ֱ��
} eWindowDoorPos;

inline eWindowDoorPos ToEWindowDoorPos(CString type)
{
	if (type == "�Կ�")
	{
		return DUIKAI;
	} 
	else if (type == "��ֱ��")
	{
		return CHUIZHIKAI;
	}
	else
	{
		ASSERT(FALSE);
		return DUIKAI;
	}
}

struct CDwgFileInfo
{
	CString fileName;	// �ļ���,����·��
	CString previewFile; //Ԥ��ͼ,����·��
	int id; //�ڷ������е�id�����������ļ�
	int PreviewID;//����ͼ��ID��������������ͼ

	CDwgFileInfo()
	{
		id = 0;
		PreviewID = 0;
	}
};

class CFloorInfo
{
public:
	CFloorInfo();

	bool SetFloors(CString p_sFloors);
	bool AddFloors(CString p_sFloors); //��ԭ���Ļ��������
	CString GetFloors()const { return m_sFloors; }

	int GetFloorCount()const{ return (int)(m_nAllFloors.size()); }
	vector<int> GetAllFloor()const{ return m_nAllFloors; }
	int GetFloorCountByFloor(CString p_sFloor)const;

	bool SetFloorHeight(double p_height);
	double GetFloorHeight()const { return m_floorHeight; }

	static bool FloorLessCmp(CString p_sFloor1, CString p_sFloor2);
	static vector<int> StringToIntVector(const CString p_sFloors);

protected:
	CString m_sFloors; //¥�㷶Χ�����ŷָ�����¥����-�ָ���ʾ���� 2-5,7
	double m_floorHeight; //���

	vector<int> m_nAllFloors;
};