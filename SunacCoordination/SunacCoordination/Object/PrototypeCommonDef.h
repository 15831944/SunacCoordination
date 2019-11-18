#pragma once


enum EKitchType
{
	E_KITCH_U,	//U��
	E_KITCH_L,	//L��
	E_KITCH_I,	//I��
	E_KITCH_OTHER = 0xf, //����
	E_KITCH_ALL = 0xff,	 //��������
};

inline CString KitchenTypeToCString(EKitchType type)
{
	switch(type)
	{
	case E_KITCH_U:
		return L"U��";
		break;
	case E_KITCH_L:
		return L"L��";
		break;
	case E_KITCH_I:
		return L"I��";
		break;
	case E_KITCH_OTHER:
		return L"����";
		break;
	case E_KITCH_ALL:
		return L"��������";
		break;
	default:
		return L"";
		break;
	}
}

enum EBathroomType
{
	E_BATHROOM_U,	//U��
	E_BATHROOM_L,	//L��
	E_BATHROOM_I,	//I��
	E_BATHROOM_OTHER = 0xf, //����
	E_BATHROOM_ALL = 0xff,	 //��������
};

inline CString BathroomTypeToCString(EBathroomType type)
{
	switch(type)
	{
	case E_KITCH_U:
		return L"U��";
		break;
	case E_KITCH_L:
		return L"L��";
		break;
	case E_KITCH_I:
		return L"I��";
		break;
	case E_KITCH_OTHER:
		return L"����";
		break;
	case E_KITCH_ALL:
		return L"��������";
		break;
	default:
		return L"";
		break;
	}
}


//¥������
enum E_KITCHEN_FLOOR_RANGE
{
	E_KITCHEN_FLOOR_1_7 = 0,
	E_KITCHEN_FLOOR_8_14,
	E_KITCHEN_FLOOR_15_21,
	E_KITCHEN_FLOOR_22_28,
	E_KITCHEN_FLOOR_29_35,
};

enum E_BATHROOM_FLOOR_RANGE
{
	E_BATHROOM_FLOOR_1_14 = 0,
	E_BATHROOM_FLOOR_15_28,
	E_BATHROOM_FLOOR_29_35,
};

const double c_kitchenAirVentSize[] = { 250, 300, 400, 450, 500 };

const double c_bathroomAirVentSize[] = { 250, 300, 400 };

struct PrototypeSize
{
	int xLen;
	int yLen;
};
