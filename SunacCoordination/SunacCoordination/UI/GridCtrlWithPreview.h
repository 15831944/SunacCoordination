#pragma once
#include "GridCtrlEx.h"
#include "GsPreviewCtrl.h"

class CGridCtrlWithPreview : public CGridCtrlEx
{
public:
	CGridCtrlWithPreview();
	~CGridCtrlWithPreview();

	void LoadDefaltSettings();
	int SetRowCount(int nRows);
	int SetColumnCount(int nCols);
	void SetDisplayRows(int nRows); //������ʾ�����ڵ�����
	void SetDisplayColumns(int nCols); //������ʾ�����ڵ�����
	void SetMargin(int nMargin); //���ñ߾�

	bool AddPreview(int nRow, int nCol, CString sPath); //��dwgԤ��ͼ��ָ����Ԫ��
	bool ClearAllPreviews();
	void ShowPreviews(); //��ʾԤ��ͼ
	void UpdatePreviews(); //�������仯ʱˢ��Ԥ��ͼ

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnPreviewSelected(WPARAM wParam,LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()

	vector<vector<CGsPreviewCtrl*>> m_pPreviews;
	int m_nDisplayRows;
	int m_nDisplayCols;
	int m_nHPos; //��¼ˮƽ������λ��
	int m_nVPos; //��¼��ֱ������λ��
	int m_nMargin;
};

