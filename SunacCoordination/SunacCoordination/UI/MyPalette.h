#pragma once

#include "DlgLeftBar.h"
//�Զ�������
//л��
class CMyPalette:public CAdUiPalette
{
	DECLARE_DYNAMIC(CMyPalette)

public:
	CMyPalette(void);
	~CMyPalette(void);


	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClick1(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DlgLeftBar m_leftBar;
};
