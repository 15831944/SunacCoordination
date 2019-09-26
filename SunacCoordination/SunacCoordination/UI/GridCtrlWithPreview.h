#pragma once
#include "GridCtrlEx.h"
#include "GridCellForPreview.h"

class CGridCtrlWithPreview : public CGridCtrlEx
{
public:
	CGridCtrlWithPreview();
	virtual ~CGridCtrlWithPreview();

	void LoadDefaltSettings();
	void SetDisplayRows(int nRows); //������ʾ�����ڵ�����
	void SetDisplayColumns(int nCols); //������ʾ�����ڵ�����

	bool AddPreview(int nRow, int nCol, CString sPath, CString sText, PREVIEW_LAYOUT_DIR dir = PREVIEW_LAYOUT_HORIZONTAL); //��dwgԤ��ͼ��ָ����Ԫ��
	void ClearAllPreviews();
	void SelectPreview(int nRow, int nCol);
	CGridCellForPreview* GetPreviewCell(int nRow, int nCol);

	afx_msg void OnPaint();

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	int m_nDisplayRows;
	int m_nDisplayCols;
};

