#pragma once
#include "..\\Excel\CApplication.h"
#include "..\\Excel\CFont0.h"
#include "..\\Excel\CRange.h"
#include "..\\Excel\CRanges.h"
#include "..\\Excel\CWorkbook.h"
#include "..\\Excel\CWorkbooks.h"
#include "..\\Excel\CWorksheet.h"
#include "..\\Excel\CWorksheets.h"
#include "..\\Excel\CBorders.h"
#include "..\\Excel\CBorder.h"
#include "..\\Excel\Cnterior.h"
#include <io.h>

namespace Excel 
{
    class  CExcelUtil
    {
    protected:
        CString out_file_name;

    public:
        CApplication m_excelApp;
        CWorkbooks   m_excelBooks;
        CWorkbook    m_excelBook;
        CWorksheets  m_excelSheets;
        CWorksheet   m_excelSheet;
        CRange       m_excelRange;

    public:
		CExcelUtil(){};
		~CExcelUtil();


        CString GetOutFileName();
		void SetOutFileName(CString new_file_name);
		

        void CreateExcel();
		//��ʱ���޷���excel����Ϊexcel com�����������
		bool OpenExcel(CString fileName, bool readOnly = false);
        void CloseExcel();
		bool SaveExcel();
		BOOL SaveAs(CString sFileSavePath);
		void SetVisible(bool bvi);

		int GetSheetNum();
        CWorksheet GetSheet(CString SheetName);
        CWorksheet GetSheet(int index);
		int SetSheetName(int index, CString name);
		CString GetSheetName(int index);
		int DeleteSheet(int index);
		CWorksheet AddSheet(CString SheetName);

		void DeleteAllCell(); //ɾ����ǰ���е�����




        //ȡ��һ����Ԫ��
        void SetRange(int ColNum, int RowNum);

        //ȡ��һ����Ԫ������
        void SetRange(int StartColNum, int StartRowNum, int EndColNum, int EndRowNum);

        //�ϲ�һ����Ԫ������
        void Merge(int startRowIndex, int startColIndex, int endRowIndex, int endColIndex);

        //������Ԫ��/�ϲ���Ԫ��
        /// <summary>
        /// �����ض��ĵ�Ԫ��
        /// </summary>
        /// <param name="ColNum">�к�,��1��ʼ,������255</param>
        /// <param name="RowNum">�к�,��1��ʼ</param>
        /// <returns></returns>
        CString GetCell(int ColNum, int RowNum);

        //��Ԫ��ֵ/��Ԫ������ֵ
        void SetCellValue(CString value);
        void SetCellValue(int row, int col, CString value);
		void SetCellFormula(int row, int col, CString value);
        void SetCellStringFormat();
        void SetCellValue( int startRow,int startCol, int endRow, int endCol, CString value );
        CString GetCellValue( int row, int col );

		void SetCellActive(int row, int col);

        //���õ�Ԫ����ʽ
        void SetBoldFontAndSize( int size );

        //���ñ�����Ϊ����Ӧ���
        void SetAutoFitColumns( int startRow, int startCol, int endRow, int endCol );
        void SetCenterAlign();
        void SetBorder();


		//����������ӵ�201108 ר��ΪMD ��׼ȷ��
		int SetActiveSheet(long iSheetName);
		int SetSheetCellValue(long iSheetName, long irow, long icol,VARIANT &vt);
		int GetSheetCellValue(long iSheetName, long irow, long icol,VARIANT &vt);
		int GetSheetCellValue(long irow, long icol,VARIANT &vt);
		BOOL IsFileExist(CString p_sFileName, BOOL bDir);

    };
}
