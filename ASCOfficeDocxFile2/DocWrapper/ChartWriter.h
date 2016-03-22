#ifndef CHART_WRITER
#define CHART_WRITER

#if defined(_WIN32) || defined (_WIN64)
	#include <atlbase.h>
	#include <atlstr.h>
#else
	#include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include <string>
#include <map>
#include <vector>

namespace OOX
{
	namespace Spreadsheet
	{
		class CXlsx;
		class CT_Chart;
		class CWorksheet;
		class CCell;
		class CXfs;
		class CT_StrRef;
		class CT_NumRef;
		class CT_MultiLvlStrRef;
		class CT_AxDataSource;
		class CT_NumDataSource;
		class CT_Title;
	}
}

namespace BinXlsxRW {

	class ChartWriter
	{
	public:
		std::map<CString, std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>*> m_mapSheets;
		std::map<CString, int> m_mapFormats;
		std::vector<OOX::Spreadsheet::CXfs*> m_aXfs;
		std::vector<CString> m_aTableNames;
		int m_nRow1;
		int m_nCol1;
		int m_nRow2;
		int m_nCol2;
	public:
		ChartWriter();
		~ChartWriter();
		void toXlsx(OOX::Spreadsheet::CXlsx& oXlsx);
		void parseChart(const OOX::Spreadsheet::CT_Chart* pChart);
	private:
		OOX::Spreadsheet::CWorksheet* toXlsxGetSheet(std::map<CString, OOX::Spreadsheet::CWorksheet*>& mapWorksheets, const CString& sName);
		void toXlsxSheetdata(OOX::Spreadsheet::CWorksheet* pWorksheet, const std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>& rows, std::vector<CString>& aSharedStrings);
		void parseCell(const CString& sheet, const int& nRow, const int& nCol, const CString& val, CString* format);
		OOX::Spreadsheet::CCell* parseCreateCell(const int& nRow, const int& nCol, const CString& val, CString* format);
		void parseStrRef(const OOX::Spreadsheet::CT_StrRef* pStrRef, bool bUpdateRange, const wchar_t* cRangeName);
		void parseNumRef(const OOX::Spreadsheet::CT_NumRef* pNumRef, bool bUpdateRange, const wchar_t* cRangeName);
		void parseMultiLvlStrRef(const OOX::Spreadsheet::CT_MultiLvlStrRef* pMultiLvlStrRef, bool bUpdateRange, const wchar_t* cRangeName);
		void parseAxDataSource(const OOX::Spreadsheet::CT_AxDataSource* pAxDataSource, bool bUpdateRange, const wchar_t* cRangeName);
		void parseNumDataSource(const OOX::Spreadsheet::CT_NumDataSource* pNumDataSource, bool bUpdateRange, const wchar_t* cRangeName);
	};
}
#endif	// #ifndef CHART_WRITER
