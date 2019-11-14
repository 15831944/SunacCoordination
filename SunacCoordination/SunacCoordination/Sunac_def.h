#pragma once
#include <vector>
using namespace std;

#define  SUNAC_DEBUG
#define  SUNAC_ATTRIBUTE_ENTITY L"SUNAC_ATTRIBUTE_ENTITY"


#define  FILE_VERSION ((Adesk::Int32)3)
//�汾2���Ŵ��������Ʋ������Ա���
//20191113 �汾3���Ŵ����Ӵ���ǽ�߶�����


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


typedef enum eViewDir
{
	E_VIEW_FRONT,//����ͼ
	E_VIEW_TOP,	//ƽ��ͼ
	E_VIEW_LEFT,//����ͼ
	E_VIEW_EXTEND,//չ��ͼ

	E_VIEW_ALL = 0xff,
}eViewDir;

enum E_DIRECTION
{
	E_DIR_BOTTOM = 0,
	E_DIR_RIGHT,
	E_DIR_TOP,
	E_DIR_LEFT,
	E_DIR_UNKNOWN
};

typedef enum eRailingType
{
	E_RAILING_TIEYI,//����
	E_RAILING_BOLI,//����
	E_RAILING_ALL = 0x10,//ȫ��
}eRailingType;

inline eRailingType ToERailingType(CString type)
{
	if (type == "����" || type == "��������")
	{
		return E_RAILING_TIEYI;
	} 
	else if (type == "����" || type == "��������")
	{
		return E_RAILING_BOLI;
	}
	else
	{
		ASSERT(FALSE);
		return E_RAILING_TIEYI;
	}
}

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
	CString GetFloors()const { return m_sFloors; }

	int GetFloorCount()const{ return (int)(m_nAllFloors.size()); }
	vector<int> GetAllFloor()const{ return m_nAllFloors; }

	bool SetFloorHeight(double p_height);
	double GetFloorHeight()const { return m_floorHeight; }

protected:
	CString m_sFloors; //¥�㷶Χ�����ŷָ�����¥����-�ָ���ʾ���� 2-5,7
	double m_floorHeight; //���

	vector<int> m_nAllFloors;
};