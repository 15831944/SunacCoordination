#pragma once
#include <vector>
#include "dbjig.h"

using namespace  std;

class CPolylineJig : public AcEdJig
{
public:
	CPolylineJig();
	virtual ~CPolylineJig();


	//��ȡ���ڲ����Ķ����Ա����������������ò���
	bool doIt(vector<AcGePoint3d>  pts, AcGePoint3d& ptOut);

protected:
	virtual AcEdJig::DragStatus sampler();
	virtual Adesk::Boolean update();
	virtual AcDbEntity* entity() const;

	void Erase(AcDbObjectId entId);
	bool Init(vector<AcGePoint3d>&  pts);
	
protected:
	vector<AcGePoint3d>  m_pts;
	AcDbPolyline* m_pPolyline;
	AcDbObjectId m_polylineId;
	AcGePoint3d m_curPoint;		// �洢�û�����ƶ�ʱ�����ʱλ��
};

