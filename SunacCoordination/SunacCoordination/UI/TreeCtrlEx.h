#pragma once

// CMyTreeCtrl

#define WM_MENU_CLICKED (WM_USER + 1002)

#define ID_COMMAND_START 40000
#define ID_COMMAND_END 49999

class CTreeCtrlEx : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlEx)
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point); //����������ؼ�ʱ����
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); //�������չ��
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct); //���ò˵���ĸ߶ȺͿ��
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct); //���Ʋ˵���
	afx_msg void OnClickedPopupMenu(UINT nID);

	void DrawBackGround(CDC* pDC);                   //���Ʊ���
	void DrawItem(CDC* pDC);                         //�������ؼ��е�ÿһ��
	void DrawItemText(CDC * pDC,HTREEITEM pItem,CRect Rect);//�������ؼ�ÿһ����ı�

	void SetMenuID(UINT menuID);
	void SetMenuWidth(UINT menuWidth) { m_MenuWidth = menuWidth; }
	void UpdateMenuIDRange(CMenu* pMenu);

public:
	CTreeCtrlEx();
	virtual ~CTreeCtrlEx();

protected:
	DECLARE_MESSAGE_MAP()

	HTREEITEM m_CurMouseItem;                       //��ǰ����ƶ����Ľڵ���
	CRect m_ClientRect;                             //���ؼ��ͻ��˴�С
	UINT m_MenuID;									//�󶨵ĵ����˵���ԴID
	UINT m_MenuWidth;								//�����˵��Ŀ�ȣ�Ĭ��Ϊ150
	UINT m_StartID;									//�󶨲˵�����ʼID
	UINT m_EndID;									//�󶨲˵�����ֹID
};
