#pragma once


enum EKitchType
{
	E_KITCH_U,	//U��
	E_KITCH_L,	//L��
	E_KITCH_I,	//I��
	E_KITCH_OTHER = 0xf, //����
	E_KITCH_ALL = 0xff,	 //��������
};


enum E_DIRECTION
{
	E_DIR_BOTTOM= 0,
	E_DIR_RIGHT,
	E_DIR_TOP,
	E_DIR_LEFT,
	E_DIR_UNKNOWN
};

//¥������
enum E_FLOOR_RANGE
{
	E_FLOOR_1_7 = 0,
	E_FLOOR_8_14,
	E_FLOOR_15_21,
	E_FLOOR_22_28,
	E_FLOOR_29_35,
};

const double c_airVentSize[] = { 350, 400, 450, 500, 550 };
