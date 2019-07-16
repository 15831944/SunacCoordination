#pragma once
#include <vector>


#include <comdef.h>
#import"C:\Program Files\Common Files\System\ado\msado15.dll"  rename("EOF","adoEOF"), rename("EOS","adoEOS"), rename("BOF","adoBOF")

using namespace ADODB;
using namespace std;

enum E_AcessResult
{
	E_Acess_database_not_open = -100,
	E_Acess_database_not_exsit,
	E_Acess_Table_not_found,
	E_Acess_Wrong_para,
	E_Acess_Search_error,
	E_Acess_error,

	S_ACESS_SUCCES = 0,
	S_Acess_Filed_not_find,

};

struct COneRowData
{
	vector<CString> fieldDatas;
};
typedef vector<COneRowData> CMultiRowData;


class CAcessDataBase
{
public:
	CAcessDataBase();
	~CAcessDataBase();

	E_AcessResult OnInitADOConn(CString p_sAccessName);
	void ExitConnect();


	//���ݲ�ѯ����sql���õ���ǰ���µ������µĶ���ֶμ��ϣ���ȫ��ֻ֧���ַ������͵�����)
	E_AcessResult ReadData(LPCTSTR p_sql, vector<CString> vFieldNames, CMultiRowData& p_dataOut);

	//�õ���ǰ�ֶ����е�ֵ
	E_AcessResult GetFieldValues(LPCTSTR p_sql, LPCTSTR p_fieldName, vector<CString>& p_out);


protected:
	_ConnectionPtr   m_pConnection; // ���ݿ�ָ��
	_RecordsetPtr    m_pRecordset; // ����ָ��
	_CommandPtr      HX_pCommand; // ��¼ָ��
};