#pragma once

void SendCommandToCAD(CString cmd); //�˺�����δ��ͨ

//����/�Ƴ�������
void CADPalette_AddP();
void CADPalette_RemoveP();

void CMD_Login();
void CMD_ShowCADPalette();
void CMD_SetUp();

//����
void CMD_SunacWindow();
void CMD_SunacDoor();
void CMD_SunacWindowAdvanceDesign();//�Ŵ�����
void CMD_SunacWindowDetail();//�Ŵ���ͼ
void CMD_SunacWindowsTable();//�Ŵ���
void CMD_SunacFloorWindowsTable();//�����Ŵ���
void CMD_SunacWindowsStatistics();//ͳ������
void CMD_SunacWindowFloorSetting();//�Ŵ�¥������
void CMD_SunacWindowTop2Front();//�Ŵ�ƽ�浽����
void CMD_SunacWindowFront2Top();//�Ŵ����浽ƽ��
void CMD_SunacWinAutoId();
void CMD_SunacNoHighlight();

//����
void CMD_SunacKitchen();
void CMD_SunacBathroom();
void CMD_SunacKitchenBathroomStatistic();

//����
void CMD_SunacRailing();
void CMD_SunacRailingDetail();
void CMD_SunacRailingStatistic();

//�յ�
void CMD_SunacAirconditioner();
void CMD_SunacAirconditionerStatistic();

void CMD_SunacMoldings(); //�߽�

//������
void CMD_SunacFilling();

//��׼����
void CMD_SunacFacade();

//��ˮ����
void CMD_SunacWaterproof();


//�˳�ǰ�ر����з�ģ̬�Ի���
void CloseModelessDialogs();
