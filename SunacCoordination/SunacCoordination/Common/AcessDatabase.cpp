#include "stdafx.h"
#include<iostream>//atlstr
#include<atlstr.h>
#include "AcessDatabase.h"

#include "Shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

//ע��32λ�汾��access����ֻ�ܷ���32λ�汾�ĳ���64λ�汾��accessֻ�ܷ���64λ�汾�ĳ���
//ͬʱ�谲װ���׵�AccessDatabaseEngine

//https://blog.csdn.net/hffhjh111/article/details/51940935

CAcessDataBase::CAcessDataBase()
{
}

CAcessDataBase::~CAcessDataBase()
{
	ExitConnect();
}

E_AcessResult CAcessDataBase::OnInitADOConn(CString p_sAccessName)
{
	::CoInitialize(NULL);

	if(PathFileExists(p_sAccessName)==FALSE)
	{
		return E_Acess_database_not_exsit;
	}

	CStringA sAccessName;
	sAccessName = p_sAccessName;

	HRESULT hr;
	try
	{
		hr = m_pConnection.CreateInstance(__uuidof(Connection));//�ȼ��� //m_pConnection.CreateInstance("ADODB.Connection");

		
		//�˾������λ�����ݿ������ķ�����Ϣ VS2010��
		_bstr_t strConnect1 = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=Database1.accdb;Persist Security Info=False ";
		//����//_bstr_t strConnect = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Database1.accdb;Persist Security Info=False";//�˾������λ�����ݿ������ķ�����Ϣ
		//����//_bstr_t strConnect = "Provider=Microsoft.ACE.OLEDB.12.0;Driver = { Microsoft Access Driver(*.mdb, *.accdb) }; DBQ =Database1.accdb;Persist Security Info=False ";

		CStringA sConnect;
		sConnect.Format("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s;Persist Security Info=False ", sAccessName);
		//sConnect.Format("Provider=Microsoft.ACE.OLEDB.12.0;Driver = { Microsoft Access Driver(*.mdb, *.accdb) }; DBQ =%s;Persist Security Info=False ", sAccessName);
		//sConnect.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False ", sAccessName);

		//�˾������λ�����ݿ������ķ�����Ϣ, ����master���ݿ⣬�����롣
		hr = m_pConnection->Open(_bstr_t(sConnect),"","",adModeUnknown);	//�������ݿ�

		cout << "���ӳɹ�,���������ָ��" << endl;
	}
	catch (_com_error e)
	{
		cout <<e.Description() << endl;
		cout << e.ErrorMessage() << endl;
		return E_Acess_error;
	}

	return S_ACESS_SUCCES;
}

void CAcessDataBase::ExitConnect() 
{
	if (m_pRecordset != NULL)
	{
		m_pRecordset->Close();
		m_pRecordset = NULL;
	}
	m_pConnection->Close();

	::CoUninitialize();
}


//�õ���ǰ�ֶ����е�ֵ���ظ���ֵֻ��һ����
//L"select ���ͱ�׼ from ��ͷ�����淶 group by ���ͱ�׼" //"select *  from Test"
E_AcessResult CAcessDataBase::GetFieldValues(LPCTSTR p_sql, LPCTSTR p_fieldName, vector<CString>& p_out)
{
	return S_ACESS_SUCCES;

	//CString sSQL_W = p_sql;
	//CStringA sSQL;
	//sSQL = sSQL_W;
	//_bstr_t bsSQL = sSQL.AllocSysString();


	m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pRecordset->Open(p_sql, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	//��ȡ���е�name�ֶ��������ݣ�����ʾ
	while (!m_pRecordset->adoEOF)
	{		
		_variant_t var = m_pRecordset->GetCollect(p_fieldName);
		if (var.vt != VT_NULL)
		{
			CString strUnicode = var.bstrVal;
			p_out.push_back(strUnicode);
		}

		m_pRecordset->MoveNext();
	}

	if (m_pRecordset != NULL)
	{
		m_pRecordset->Close();
		m_pRecordset = NULL;
	}

	//::SysFreeString(bsSQL);

	return S_ACESS_SUCCES;
}

E_AcessResult CAcessDataBase::ReadData(LPCTSTR p_sql, vector<CString> vFieldNames, CMultiRowData& p_dataOut)
{
	p_dataOut.clear();
	//CString sSQL_W = p_sql;
	//CStringA sSQL;
	//sSQL = sSQL_W;
	//_bstr_t bsSQL = sSQL.AllocSysString();

	vector<CString> sFieldNames;
	sFieldNames.resize(vFieldNames.size());
	for (UINT i = 0; i < vFieldNames.size(); i++)
	{
		sFieldNames[i] = vFieldNames[i];
	}

	m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pRecordset->Open(p_sql, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	//��ȡ���е�name�ֶ��������ݣ�����ʾ
	while (!m_pRecordset->adoEOF)
	{
		COneRowData oneRow;
		for (UINT i = 0; i < sFieldNames.size(); i++)
		{
			BSTR bsTemp = sFieldNames[i].AllocSysString();
			//_variant_t  bsTemp = (_variant_t)(LPCTSTR)(sFieldNames[i]);
			_variant_t var = m_pRecordset->GetCollect(bsTemp);
			::SysFreeString(bsTemp);

			_variant_t  vTemp = (_variant_t)(LPCTSTR)(sFieldNames[i]);

			if (var.vt != VT_NULL)
			{
				CString strUnicode = var.bstrVal;
				oneRow.fieldDatas.push_back(strUnicode);
			}
		}

		p_dataOut.push_back(oneRow);

		m_pRecordset->MoveNext();
	}

	if (m_pRecordset != NULL)
	{
		m_pRecordset->Close();
		m_pRecordset = NULL;
	}
	
	//::SysFreeString(bsSQL);

	return S_ACESS_SUCCES;
}