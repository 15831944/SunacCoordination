#pragma once

WCHAR* CharToWchar(const char * pchar);
CHAR* WCHARTOCHAR(const WCHAR * pchar);

void CF_STR_get_file_name(const WCHAR *fullname, WCHAR *filename);
void CF_STR_get_file_name_2(const WCHAR *filename, WCHAR *filename2);

//0623Ŀǰ���������С�仯�����⻹û�������0623��ʱ��ע��, ����scale���С����
void CF_STR_Process_MText_basic_on_chs(const WCHAR *instr,CString &outstr, 
                                       const WCHAR* font = L"ARIAL", double scale1 = 1, double scale2 = 1);
