/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCObject.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#pragma once
#include "RCObject.h"

enum RCTYPE {
	T_NONE=0,
	T_WINDOW = 1, //��
	T_DOOR,//��
	T_KITCHEN,//����
	T_TOILET,//������
	T_STDFACADE, //��׼����
	T_WATERPROOF, //��ˮ
	T_STDFILL,//���
	T_AIRCONDITION, //�յ�
	T_LANGAN,//����
	T_XIANJIAO,//�߽�
};

class RCObject
{
public:
	RCObject(void);
	RCObject(const RCObject&other);
	~RCObject(void);

	void SetName(CString  name) { m_name = name; }
	CString& GetName() { return m_name; }

	void SetUserData(void * userData){m_userData = userData;}
	void *GetUserData(){return m_userData;}

	virtual bool isEqualToAll(RCObject*other = 0);//����������ȫһ��
	virtual bool isEqualTo(RCObject*other = 0);//��������һ��

	virtual RCTYPE GetType()const = 0;

public:
	CString m_name;
	void * m_userData;
	AcDbObjectId m_id;

public:
	//һ�������һ��RCObject��һ��ʵ�����Ŀ����
	//������ӵ�һ���������ڱ�ʾ��RCObject����һ��typeʱ�����еĺ�����ͬ�Ķ����б�
	//��Ҫ�ǳ��Ŵ���ʱ��Ҫ�������
	vAcDbObjectId m_sameTypeIds;//�����Ҫ�������Լ���id
};

typedef std::vector<RCObject> vRCObject;
typedef std::vector<RCObject*> vpRCObject;
