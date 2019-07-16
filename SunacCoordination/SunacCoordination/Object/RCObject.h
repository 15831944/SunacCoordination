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

	virtual RCTYPE GetType()const = 0;

public:
	CString m_name;
	void * m_userData;
};

typedef std::vector<RCObject> vRCObject;
typedef std::vector<RCObject*> vpRCObject;