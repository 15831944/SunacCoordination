#pragma once

void SendCommandToCAD(CString cmd); //�˺�����δ��ͨ

//����/�Ƴ�������
void CADPalette_AddP();
void CADPalette_RemoveP();

void CMD_Login();

//����
void CMD_SunacWindow();
void CMD_SunacDoor();
void CMD_SunacWindowAdvanceDesign();//�Ŵ�����
void CMD_SunacWindowsStatistics();//ͳ������
void CMD_SunacWindowDetail();//�Ŵ���ͼ

//����
void CMD_SunacKitchen();
void CMD_SunacBathroom();

//����
void CMD_SunacRailing();
void CMD_SunacRailingStatistic();

//�յ�
void CMD_SunacAirconditioner();

void CMD_SunacMoldings(); //�߽�

//������
void CMD_SunacFilling();

//��׼����
void CMD_SunacFacade();

//��ˮ����
void CMD_SunacWaterproof();


//�˳�ǰ�ر����з�ģ̬�Ի���
void CloseModelessDialogs();
