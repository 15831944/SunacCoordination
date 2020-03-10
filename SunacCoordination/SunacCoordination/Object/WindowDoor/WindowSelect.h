#pragma once
#include <vector>
#include "AttrWindow.h"
#include "../../Common/TYRect.h"
using namespace std;

class CWinInCad
{
public:
	CWinInCad();

public:
	AcDbObjectId m_winId;
	AcDbObjectId m_rootId; //��Կ��п�ĸ��ڵ�
	bool m_bMxMirror;	   //�Ŵ���ľ����Ƿ���
	double m_rotateAngle;  //�Ŵ������ת�Ƕ�
	AcGeMatrix3d m_mx;
};
//////////////////////////////////////////////////////////////////////////

class CWindowSelect
{
public:
	static vector<CWinInCad> SelectWindows(eViewDir p_view, bool p_bAllWindow = false);
	static vector<CWinInCad> SelectWindowsByRect(eViewDir p_view, TYRect p_rect);
	
	static bool IsReferenctMirror(const AcDbObjectId refId);	//ֻ�жϿ����ñ����Ƿ��Ǿ���

	static AcGeMatrix3d GetReferenctWorldMatrix(const AcDbObjectId refId); //�õ�ʵ�����������

protected:
	static vector<CWinInCad> GetWinsInObjectIds(const vector<AcDbObjectId>& p_ids, eViewDir p_view);


	static int FindWindowsDeep(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);

	static int FindWindowInBlock(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);
	static int FindWindowInArray(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);

	static bool IsMxMirror(const AcGeMatrix3d p_parentMx); //ͨ�������ж��Ƿ�����������

};

//////////////////////////////////////////////////////////////////////////
class CWindowAndCount
{
public:
	CWindowAndCount();

public:
	AttrWindow winAtt;
	int nCount;

	AcDbObjectIdArray objIds;
};

class CWindowCountArray
{
	bool InitByWindowIds(const vAcDbObjectId& p_winIds);
public:
	bool InitByWindowIds(const vector<CWinInCad>& p_winIds);
	bool InitByWindowAtts(const vector<AttrWindow>& p_winAtts, const vector<AcDbObjectId>& p_ids);

	int GetCount()const { return (int)(m_winCountArray.size()); }

	//�ŵ�����
	int GetDoorsCount();

	//��������
	int GetWindowsCount();

	const CWindowAndCount& GetWindow(int p_index) { return m_winCountArray[p_index]; }

private:
	vector<CWindowAndCount> m_winCountArray;
};

class CSplitWindowDoorArray
{
public:
	void SplitWindowDoor(int p_numWindowDoor, const CWindowAndCount& winAndCount); //��m_winCountArray��ֳɸ������͵��Ŵ�(NC��NDC...)
	void ClearSplitWindowDoor(); //���m_winNC��vector

	int GetWindowNCCount()const { return (int)(m_winNC.size()); }
	int GetWindowNDCCount()const { return (int)(m_winNDC.size()); }
	int GetWindowWCCount()const { return (int)(m_winWC.size()); }
	int GetWindowTCCount()const { return (int)(m_winTC.size()); }
	int GetDoorTLMCount()const { return (int)(m_doorTLM.size()); }
	int GetDoorTSTLMCount()const { return (int)(m_doorTSTLM.size()); }
	int GetDoorWMCount()const { return (int)(m_doorWM.size()); }
	int GetDoorWLCCount()const { return (int)(m_doorWLC.size()); }

	const CWindowAndCount& GetWindowNC(int p_index) { return m_winNC[p_index]; }
	const CWindowAndCount& GetWindowNDC(int p_index) { return m_winNDC[p_index]; }
	const CWindowAndCount& GetWindowWC(int p_index) { return m_winWC[p_index]; }
	const CWindowAndCount& GetWindowTC(int p_index) { return m_winTC[p_index]; }
	const CWindowAndCount& GetDoorTLM(int p_index) { return m_doorTLM[p_index]; }
	const CWindowAndCount& GetDoorTSTLM(int p_index) { return m_doorTSTLM[p_index]; }
	const CWindowAndCount& GetDoorWM(int p_index) { return m_doorWM[p_index]; }
	const CWindowAndCount& GetDoorWLC(int p_index) { return m_doorWLC[p_index]; }

private:
	vector<CWindowAndCount> m_winNC;
	vector<CWindowAndCount> m_winNDC;
	vector<CWindowAndCount> m_winWC;
	vector<CWindowAndCount> m_winTC;
	vector<CWindowAndCount> m_doorTLM;
	vector<CWindowAndCount> m_doorTSTLM;
	vector<CWindowAndCount> m_doorWM;
	vector<CWindowAndCount> m_doorWLC;
};