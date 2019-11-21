#pragma once
#include "../Common/ComFun_ACad.h"
#include "../Common/ComFun_Str.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/WindowDoor/RCWindow.h"


//�Ľṹ�������������һ���Ŵ���ͼ�Ŀ�
//Sunac2019\Data\Template\WindowDetail.dwg
class CWindowDetailTemplate
{
public:
	CWindowDetailTemplate();

	bool InsertTemplates(int p_index, AcGePoint3d p_pnt);

public:
	AcGePoint3d pnt;//�ⴰ�����

	//�ܿ�ȸ߶�
	const static int width = 4000;
	const static int height = 4500;

	//ԭ�ͱ��
	const static int idX = 130;
	const static int idY = 650;

	//���
	const static int areaX = 1760;
	const static int areaY = 690;

	//�������
	const static int openAreaX = 1960;
	const static int openAreaY = 190;

	//������
	const static int gongNengQuX = 3220;
	const static int gongNengQuY = 690;

	//����ǽ�߶�
	const static int heightUnderWindowX = 3310;
	const static int heightUnderWindowY = 190;


	//��ע���
	const static int biaoZhuWid = 300;

	RCBlock thisBlock;

	//�õ�ģ��Ĳ����
	AcGePoint3d GetInsertPnt(AcGePoint3d basePnt, int rowIndex, int columnIndex)
	{
		return AcGePoint3d(basePnt.x + columnIndex*width, basePnt.y - (rowIndex + 1) * height, 0);
	}

	static CString GetTemplateFileName()
	{
		return MD2010_GetAppPath() + L"\\Sunac2019\\Data\\Template\\WindowDetail.dwg";;
	}

	//�����Ŵ��Ĳ����
	AcGePoint3d GetInsertPntWindow(double winwid, double winHei)
	{
		return AcGePoint3d(pnt.x + (width - winwid - biaoZhuWid * 2) / 2,//�������ƽ��
			pnt.y + 4500 - 900 - winHei,//�߶ȶ���һ��
			0);
	}
};

typedef std::vector<CWindowDetailTemplate> vSWindowDetailTemplate;


class CWindowDetail
{
public:
	static void DrawWindowDetail();

protected:
	static bool CreateDataText(const AttrWindow& winAtt, CWindowDetailTemplate winDetailTemplate);

	static int CreateDetailDims(const AttrWindow& winAtt, AcDbObjectId m_id);
};

