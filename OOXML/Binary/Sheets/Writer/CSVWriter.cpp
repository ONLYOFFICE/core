/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "CSVWriter.h"
#include "../../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../../UnicodeConverter/UnicodeConverter_Encodings.h"
#include "../../../../DesktopEditor/common/StringBuilder.h"

#include "../../../../DesktopEditor/common/File.h"
#include "../../../XlsxFormat/Xlsx.h"

#include "../../../XlsxFormat/Workbook/Workbook.h"
#include "../../../XlsxFormat/SharedStrings/SharedStrings.h"
#include "../../../XlsxFormat/Styles/Styles.h"
#include "../../../XlsxFormat/Worksheets/Worksheet.h"
#include "../../../../Common/MS-LCID.h"

#include <boost/date_time.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>

class CSVWriter::Impl
{
public:
	Impl(OOX::Spreadsheet::CXlsx &oXlsx, unsigned int m_nCodePage, const std::wstring& sDelimiter, bool m_bJSON);
	~Impl();
	
	void Start(const std::wstring &sFileDst);
	void WriteSheetStart(OOX::Spreadsheet::CWorksheet* pWorksheet);
	void WriteRowStart(OOX::Spreadsheet::CRow *pRow);
	void WriteCell(OOX::Spreadsheet::CCell *pCell);
	void WriteRowEnd(OOX::Spreadsheet::CRow* pWorksheet);
	void WriteSheetEnd(OOX::Spreadsheet::CWorksheet* pWorksheet);
	void End();
	void Close();

private:
	NSFile::CFileBinary m_oFile;
	OOX::Spreadsheet::CXlsx& m_oXlsx;
	unsigned int m_nCodePage;
	const std::wstring& m_sDelimiter;
	bool m_bJSON;

	wchar_t* m_pWriteBuffer;
	int m_nCurrentIndex;
	std::wstring m_sEscape;
	int m_nRowCurrent;
	int m_nColCurrent;

	int m_nColDimension;

	bool m_bIsWriteCell; // Нужно только для записи JSON-а
	bool m_bStartRow;
	bool m_bStartCell;

	unsigned int language_code = 0;

	void GetDefaultFormatCode(int numFmt, std::wstring & format_code, boost::optional<int> & format_type);
	std::wstring ConvertValueCellToString(const std::wstring &Value, boost::optional<int> format_type, std::wstring format_code);

	int detect_format(std::wstring & format_code);
	std::wstring convert_date_time(const std::wstring & sValue, std::wstring format_code, bool bDate = true, bool bTime = true);
};

CSVWriter::CSVWriter()
{
}
CSVWriter::~CSVWriter()
{
}
void CSVWriter::Init(OOX::Spreadsheet::CXlsx &oXlsx, unsigned int nCodePage, const std::wstring& sDelimiter, bool bJSON)
{
	impl_ = boost::shared_ptr<Impl>(new CSVWriter::Impl(oXlsx, nCodePage, sDelimiter, bJSON));
}
void CSVWriter::Xlsx2Csv(const std::wstring &sFileDst, OOX::Spreadsheet::CXlsx &oXlsx, unsigned int nCodePage, const std::wstring& sDelimiter, bool bJSON)
{
	Init(oXlsx, nCodePage, sDelimiter, bJSON);
	
	impl_->Start(sFileDst);
	
	if (oXlsx.m_pWorkbook)
	{
		LONG lActiveSheet = oXlsx.m_pWorkbook->GetActiveSheetIndex();
		std::wstring sSheetRId = _T("Sheet1"); // Читаем не по rId, а по имени листа
											   // Get active sheet rId (для конвертации в CSV нужно использовать name, т.к. это наш бинарник из js-скриптов и еще нет rId
											   // А для json-а нужно пользовать rId, т.к. при открытии они используются
		if (oXlsx.m_pWorkbook->m_oSheets.IsInit() && !oXlsx.m_pWorkbook->m_oSheets->m_arrItems.empty())
		{
			OOX::Spreadsheet::CSheet* pSheet = NULL;
			if (lActiveSheet >= 0 && lActiveSheet < (LONG)oXlsx.m_pWorkbook->m_oSheets->m_arrItems.size())
			{
				pSheet = oXlsx.m_pWorkbook->m_oSheets->m_arrItems[lActiveSheet];
			}
			else
			{
				pSheet = oXlsx.m_pWorkbook->m_oSheets->m_arrItems.front();
			}
			sSheetRId = bJSON ? pSheet->m_oRid->GetValue() : *pSheet->m_oName;
		}

		std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>::const_iterator pFind = oXlsx.m_mapWorksheets.find(sSheetRId);
		if (pFind != oXlsx.m_mapWorksheets.end())
		{
			OOX::Spreadsheet::CWorksheet *pWorksheet = pFind->second;
			if (NULL != pWorksheet && pWorksheet->m_oSheetData.IsInit())
			{
				impl_->WriteSheetStart(pWorksheet);

				for (size_t i = 0; i < pWorksheet->m_oSheetData->m_arrItems.size(); ++i)
				{
					OOX::Spreadsheet::CRow *pRow = pWorksheet->m_oSheetData->m_arrItems[i];
					impl_->WriteRowStart(pRow);
					for (size_t j = 0; j < pRow->m_arrItems.size(); ++j)
					{
						impl_->WriteCell(pRow->m_arrItems[j]);
					}
					impl_->WriteRowEnd(pRow);
				}
				impl_->WriteSheetEnd(pWorksheet);
			}
		}
	}
	impl_->End();
}
void CSVWriter::Start(const std::wstring &sFileDst)
{
	if (impl_)
		impl_->Start(sFileDst);
}
void CSVWriter::WriteSheetStart(OOX::Spreadsheet::CWorksheet* pWorksheet)
{
	if (impl_)
		impl_->WriteSheetStart(pWorksheet);
}
void CSVWriter::WriteRowStart(OOX::Spreadsheet::CRow *pRow)
{
	if (impl_)
		impl_->WriteRowStart(pRow);
}
void CSVWriter::WriteCell(OOX::Spreadsheet::CCell *pCell)
{
	if (impl_)
		impl_->WriteCell(pCell);
}
void CSVWriter::WriteRowEnd(OOX::Spreadsheet::CRow* pWorksheet)
{
	if (impl_)
		impl_->WriteRowEnd(pWorksheet);
}
void CSVWriter::WriteSheetEnd(OOX::Spreadsheet::CWorksheet* pWorksheet)
{
	if (impl_)
		impl_->WriteSheetEnd(pWorksheet);
}
void CSVWriter::End()
{
	if (impl_)
		impl_->End();
}
void CSVWriter::Close()
{
	if (impl_)
		impl_->Close();
}
//---------------------------------------------------------------------------------------------------------------------------------
static std::wstring replace_unwanted(boost::wsmatch const & what)
{
	return L"";
}
int CSVWriter::Impl::detect_format(std::wstring & format_code)
{
	if (format_code.empty()) return SimpleTypes::Spreadsheet::celltypeStr;

	boost::wregex re_unwanted(L"([\"'])(.+?)\\1");

	std::wstring strFormatCode = boost::regex_replace(format_code, re_unwanted, &replace_unwanted, boost::match_default | boost::format_all);

	//find [$<Currency String>-<language info>].
	boost::wregex re(L"(?:\\[)(?:\\$)(\\S+)?\-(\\S+)(?:\\])");
	boost::wsmatch result;
	bool b = boost::regex_search(strFormatCode, result, re);

	std::wstring currency_str;

	if (b && result.size() >= 3)
	{
		currency_str = result[1];
		int code = -1;
		try
		{
			std::wstringstream ss;
			ss << std::hex << result[2];
			ss >> language_code;
		}
		catch (...) {}

		//format_code = boost::regex_replace( format_code,re,L"");
	}
	if (!currency_str.empty() && language_code != 0xF400 && language_code != 0xF800)
	{
		return SimpleTypes::Spreadsheet::celltypeCurrency;
	}

	if (false == format_code.empty()) //any
	{
		boost::wregex re1(L"([mMhHs{2,}S{2,}]+)");
		boost::wregex re2(L"([mMdDy{2,}Y{2,}]+)");

		std::wstring tmp = strFormatCode;

		std::list<std::wstring> result1;
		bool b1 = boost::regex_split(std::back_inserter(result1), tmp, re1);

		tmp = strFormatCode;
		std::list<std::wstring> result2;
		bool b2 = boost::regex_split(std::back_inserter(result2), tmp, re2);

		if (b1 && b2 && result1.size() > 2 && result2.size() > 2)
		{
			return SimpleTypes::Spreadsheet::celltypeDateTime;
		}
		if (b1 && result1.size() > 2)
		{
			return SimpleTypes::Spreadsheet::celltypeTime;
		}
		if (b2 && result2.size() > 2)
		{
			return SimpleTypes::Spreadsheet::celltypeDate;
		}
		if (std::wstring::npos != strFormatCode.find(L"%"))
		{
			return SimpleTypes::Spreadsheet::celltypePercentage;
		}
		if (std::wstring::npos != strFormatCode.find(L"#") ||
			std::wstring::npos != strFormatCode.find(L"?") ||
			std::wstring::npos != strFormatCode.find(L"0"))
		{
			return SimpleTypes::Spreadsheet::celltypeNumber;
		}
	}
	return SimpleTypes::Spreadsheet::celltypeStr;
}
inline size_t sizeof_symbol(const std::wstring & sValue, wchar_t symbol, size_t start)
{
	size_t i = start + 1;
	for (; i < sValue.length(); ++i)
		if (sValue[i] != symbol)
			break;
	return i - start;
}
std::wstring CSVWriter::Impl::convert_date_time(const std::wstring & sValue, std::wstring format_code, bool bDate, bool bTime)
{
	try
	{
		double dTime = XmlUtils::GetDouble(sValue);
		int iDate = (int)dTime;
		dTime -= iDate;

		boost::gregorian::date date_ = boost::gregorian::date(1900, 1, 1) + boost::gregorian::date_duration(iDate - 2);

		boost::posix_time::time_duration day(24, 0, 0);
		double millisec = day.total_milliseconds() * dTime;

		double sec = millisec / 1000.;
		int hours = (int)(sec / 60. / 60.);
		int minutes = (int)((sec - (hours * 60 * 60)) / 60.);
		sec = sec - (hours * 60 + minutes) * 60.;

		if (format_code.empty())
		{
			std::wstring date_str, time_str;

			if (bDate)
			{
				date_str = boost::lexical_cast<std::wstring>(date_.year()) + L"-" +
					(date_.month() < 10 ? L"0" : L"") + boost::lexical_cast<std::wstring>(date_.month().as_number()) + L"-" +
					(date_.day() < 10 ? L"0" : L"") + boost::lexical_cast<std::wstring>(date_.day());
			}

			if (bTime)
			{
				time_str = (hours < 10 ? L"0" : L"") + std::to_wstring(hours) + L":" +
					(minutes < 10 ? L"0" : L"") + std::to_wstring(minutes) + L":" +
					(sec < 10 ? L"0" : L"") + std::to_wstring((int)sec);
			}
			return (bDate ? date_str : L"") + (bDate & bTime ? L" " : L"") + (bTime ? time_str : L"");
		}
		else
		{
			if (std::wstring::npos != format_code.find(L"[$"))
			{
				size_t pos = format_code.find(L"]");
				format_code = format_code.substr(pos + 1);
			}
			int CodePage = 0;
			std::string LCID;

			if (language_code > 0 && language_code != 0xf800 && language_code != 0xf400)
			{
				unsigned short LanguageID = GETBITS(language_code, 0, 15);
				BYTE CalendarType = GETBITS(language_code, 16, 23);
				BYTE NumberType = GETBITS(language_code, 24, 31);

				CodePage = msLCID2DefCodePage(LanguageID);
				std::wstring wsLCID = msLCID2wstring(LanguageID);

				LCID = std::string(wsLCID.begin(), wsLCID.end());
			}
			std::wstring output;
#if 0
			NSUnicodeConverter::CUnicodeConverter oConverter;
			output = oConverter.FormatDateTime(iDate, format_code, LCID);
#else
			std::wstring sAferTime;
			if (std::wstring::npos != format_code.find(L"AM/PM"))
			{
				XmlUtils::replace_all(format_code, L"AM/PM", L"a");
				if (hours > 12)
				{
					hours -= 12;
					sAferTime += L"PM";
				}
				else sAferTime += L"AM";
			}

			bool bHourOutput = false;// for month or minutes
			for (size_t i = 0; i < format_code.length(); ++i)
			{
				size_t symbol_size = sizeof_symbol(format_code, format_code[i], i);

				switch (format_code[i])
				{
				case L'\\': continue;
				case L'/': output += L"."; break;
				case L'a': output += sAferTime; break;
				case L'd':
				case L'D':
				{
					//if (symbol_size == 3)
					//	output += date_.day_of_week().as_short_wstring();
					//else if (symbol_size == 4)
					//	output += date_.day_of_week().as_long_wstring();
					//else
					{
						unsigned short day = date_.day().as_number();
						if (symbol_size > 1 && day < 10) output += L"0";
						output += std::to_wstring(day);
					}
				}break;
				case L'h': //12
				case L'H': //24
				{
					bHourOutput = true;
					if (symbol_size > 1 && hours < 10) output += L"0";
					output += std::to_wstring(hours);
				}break;
				case L'm':
				case L'M':
				{
					if (bHourOutput)
					{
						if (symbol_size > 1 && minutes < 10) output += L"0";
						output += std::to_wstring(minutes);
					}
					else
					{
						unsigned short month = date_.month().as_number();
						if (symbol_size > 2 && month < 10) output += L"0";
						/*if (symbol_size < 3) */output += std::to_wstring(month);
						//else if (symbol_size == 3) output += date_.month().as_short_wstring();
						//else output += date_.month().as_long_wstring();
					}
				}break;
				case L's':
				case L'S':
				{
					if (symbol_size > 1 && sec < 10) output += L"0";
					output += std::to_wstring((int)sec);
				}break;
				case L'y':
				case L'Y':
				{
					std::wstring year = boost::lexical_cast<std::wstring>(date_.year());
					if (symbol_size > 2) output += year;
					else output += year.substr(2);
				}break;
				default:
					output += format_code[i];
				}
				i += symbol_size - 1; 
			}
#endif
			return output;
		}
	}
	catch (...)
	{
		return sValue;
	}
}
long gcd(long a, long b)
{
	if (a == 0)			return b;
	else if (b == 0)	return a;

	if (a < b)	return gcd(a, b % a);
	else		return gcd(b, a % b);
}
std::wstring convert_fraction(const std::wstring & sValue)
{
	double dValue = XmlUtils::GetDouble(sValue);

	double integral = std::floor(dValue);
	double frac = dValue - integral;

	const long precision = 1000000000;

	long gcd_ = gcd(round(frac * precision), precision);

	long denominator = precision / gcd_;
	long numerator = round(frac * precision) / gcd_;

	return std::to_wstring((int)integral) + L" " + std::to_wstring(numerator) + L"/" + std::to_wstring(denominator);
}
std::wstring convert_scientific(const std::wstring & sValue)
{
	double dValue = XmlUtils::GetDouble(sValue);

	std::wstringstream strm;
	strm << std::setprecision(2) << std::scientific << dValue;
	return strm.str();
}
//---------------------------------------------------------------------------------------------
static std::wstring g_sNewLineN = _T("\n");
static std::wstring g_sEndJson = _T("]");
static std::wstring g_sQuote = _T("\"");
static std::wstring g_sDoubleQuote = _T("\"\"");
static std::wstring g_sBkt = _T("[");
static std::wstring g_sBktComma = _T(",[");

void escapeJson(const std::wstring& sInput, NSStringUtils::CStringBuilder& oBuilder)
{
	//http://stackoverflow.com/questions/7724448/simple-json-string-escape-for-c
	for (size_t i = 0; i < sInput.length(); ++i)
	{
		wchar_t c = sInput[i];
		switch (c)
		{
		case '"': oBuilder.WriteString(_T("\\\"")); break;
		case '\\': oBuilder.WriteString(_T("\\\\")); break;
		case '\b': oBuilder.WriteString(_T("\\b")); break;
		case '\f': oBuilder.WriteString(_T("\\f")); break;
		case '\n': oBuilder.WriteString(_T("\\n")); break;
		case '\r': oBuilder.WriteString(_T("\\r")); break;
		case '\t': oBuilder.WriteString(_T("\\t")); break;
		default:
			if ('\x00' <= c && c <= '\x1f')
			{
				oBuilder.WriteString(_T("\\u00"));
				oBuilder.WriteHexByte((unsigned char)c);
			}
			else
			{
				oBuilder.AddCharSafe(c);
			}
		}
	}
}
void WriteFile(NSFile::CFileBinary *pFile, wchar_t **pWriteBuffer, int &nCurrentIndex, const std::wstring &sWriteString, unsigned int &nCodePage, bool bIsEnd = false)
{
	if (NULL == pFile || NULL == pWriteBuffer)
		return;
	size_t nCountChars = sWriteString.length();
	if (0 == nCountChars && !bIsEnd)
		return;

	const size_t c_nSize = 1048576; // 1024 * 1024
	const size_t nSizeWchar = sizeof(wchar_t);

	if (NULL == *pWriteBuffer)
	{
		*pWriteBuffer = new wchar_t[c_nSize];
		memset(*pWriteBuffer, 0, nSizeWchar * c_nSize);
		nCurrentIndex = 0;
	}

	if (nCountChars + nCurrentIndex > c_nSize || bIsEnd)
	{
		// Буффер заполнился, пишем
		if (nCodePage == 48 && 2 == sizeof(wchar_t))//todo 48 временно CP_UTF16
		{
			pFile->WriteFile((BYTE*)*pWriteBuffer, sizeof(wchar_t) * nCurrentIndex);
		}
		else
		{
			const NSUnicodeConverter::EncodindId& oEncodindId = NSUnicodeConverter::Encodings[nCodePage];
			NSUnicodeConverter::CUnicodeConverter oUnicodeConverter;
			std::string sFileDataA = oUnicodeConverter.fromUnicode(*pWriteBuffer, nCurrentIndex, oEncodindId.Name);
			pFile->WriteFile((BYTE*)sFileDataA.c_str(), (DWORD)sFileDataA.length());
		}

		memset(*pWriteBuffer, 0, nSizeWchar * c_nSize);
		nCurrentIndex = 0;
	}

	if (!bIsEnd)
	{
		memcpy(*pWriteBuffer + nCurrentIndex, sWriteString.c_str(), nCountChars * nSizeWchar);
		nCurrentIndex += (int)nCountChars;
	}
}
CSVWriter::Impl::Impl(OOX::Spreadsheet::CXlsx &m_oXlsx, unsigned int m_nCodePage, const std::wstring& m_sDelimiter, bool m_bJSON) : m_oXlsx(m_oXlsx), m_nCodePage(m_nCodePage), m_sDelimiter(m_sDelimiter), m_bJSON(m_bJSON)
{
	m_pWriteBuffer = NULL;
	m_nCurrentIndex = 0;
	m_sEscape = _T("\"\n");
	m_sEscape += m_sDelimiter;
	m_nRowCurrent = 1;
	m_nColCurrent = 1;
	m_bIsWriteCell = false;
	m_bStartRow = true;
	m_bStartCell = true;

	m_nColDimension = 1;
}
CSVWriter::Impl::~Impl()
{
	Close();
}
void CSVWriter::Impl::Start(const std::wstring &sFileDst)
{
	m_oFile.CreateFileW(sFileDst);

	// Нужно записать шапку
	if (46 == m_nCodePage)//todo 46 временно CP_UTF8
	{
		BYTE arUTF8[3] = { 0xEF, 0xBB, 0xBF };
		m_oFile.WriteFile(arUTF8, 3);
	}
	else if (48 == m_nCodePage)//todo 48 временно CP_UTF16
	{
		BYTE arUTF16[2] = { 0xFF, 0xFE };
		m_oFile.WriteFile(arUTF16, 2);
	}
	else if (49 == m_nCodePage)//todo 49 временно CP_unicodeFFFE
	{
		BYTE arBigEndian[2] = { 0xFE, 0xFF };
		m_oFile.WriteFile(arBigEndian, 2);
	}
}
void CSVWriter::Impl::WriteSheetStart(OOX::Spreadsheet::CWorksheet* pWorksheet)
{
	m_nColDimension = 1;
	m_nRowCurrent = 1;
	if (m_bJSON)
	{
		WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, g_sBkt, m_nCodePage);
	}
}
void CSVWriter::Impl::WriteRowStart(OOX::Spreadsheet::CRow *pRow)
{
	int nRow = pRow->m_oR.IsInit() ? pRow->m_oR->GetValue() : m_bStartRow ? m_nRowCurrent : m_nRowCurrent + 1;

	if (m_bJSON)
	{
		WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, (m_bStartRow ? g_sBkt : g_sBktComma), m_nCodePage);
	}
	else
	{
		while (nRow > m_nRowCurrent)
		{
			// Write new line
			++m_nRowCurrent;
			WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, g_sNewLineN, m_nCodePage);
		}
	}
	m_bStartRow = false;
	m_bStartCell = true;
	m_nColCurrent = 1;
	m_bIsWriteCell = false;
}
void CSVWriter::Impl::WriteCell(OOX::Spreadsheet::CCell *pCell)
{
	int nRowTmp = 0;
	int nCol = 0;

	if (pCell->isInitRef() && pCell->getRowCol(nRowTmp, nCol))
	{
		nRowTmp++;
		nCol++;
	}
	else
	{
		nCol = m_bStartCell ? m_nColCurrent : m_nColCurrent + 1;
	}

	while (nCol > m_nColCurrent)
	{
		if (m_bJSON && false == m_bIsWriteCell)
		{
			// Запишем пустые строки (для JSON-а)
			WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, g_sDoubleQuote, m_nCodePage);
		}
		// Write delimiter
		++m_nColCurrent;
		WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, m_sDelimiter, m_nCodePage);
		m_bIsWriteCell = false;
	}

	if (m_nColDimension < m_nColCurrent)
		m_nColDimension = m_nColCurrent;

	std::wstring sCellValue;
	boost::optional<int> format_type;

	if (pCell->m_oType.IsInit())
		format_type = (int)pCell->m_oType->GetValue();

	//if (pCell->m_oCacheValue.IsInit())
	//{
	//	sCellValue = *pCell->m_oCacheValue;
	//}
	//else 
	bool bString = false;
	if (pCell->m_oValue.IsInit())
	{
		sCellValue = pCell->m_oValue->ToString();

		if (SimpleTypes::Spreadsheet::celltypeSharedString == format_type.get_value_or(SimpleTypes::Spreadsheet::celltypeNumber))
		{
			int nValue = XmlUtils::GetInteger(sCellValue);

			if (nValue >= 0 && nValue < (int)m_oXlsx.m_pSharedStrings->m_arrItems.size())
			{
				OOX::Spreadsheet::CSi *pSi = m_oXlsx.m_pSharedStrings->m_arrItems[nValue];
				if (NULL != pSi)
				{
					sCellValue = pSi->ToString();
					bString = true;
				}
			}
		}
		if (!bString)
		{
			std::wstring format_code;

			if (pCell->m_oStyle.IsInit() && m_oXlsx.m_pStyles)
			{
				OOX::Spreadsheet::CXfs* xfs = m_oXlsx.m_pStyles->m_oCellXfs->m_arrItems[*pCell->m_oStyle];
				if (xfs)
				{
					if ((xfs->m_oApplyNumberFormat.IsInit()) && (xfs->m_oApplyNumberFormat->ToBool()) || !xfs->m_oApplyNumberFormat.IsInit())
					{
						if ((xfs->m_oNumFmtId.IsInit()) /*&& (xfs->m_oNumFmtId->GetValue() != 0*/)
						{
							int numFmt = xfs->m_oNumFmtId->GetValue();

							GetDefaultFormatCode(numFmt, format_code, format_type);

							if (m_oXlsx.m_pStyles->m_oNumFmts.IsInit())
							{
								std::map<unsigned int, size_t>::iterator pFind = m_oXlsx.m_pStyles->m_oNumFmts->m_mapNumFmtIndex.find(numFmt);
								if (pFind != m_oXlsx.m_pStyles->m_oNumFmts->m_mapNumFmtIndex.end())
								{
									OOX::Spreadsheet::CNumFmt *fmt = m_oXlsx.m_pStyles->m_oNumFmts->m_arrItems[pFind->second];
									if (fmt)
									{
										if (fmt->m_oFormatCode.IsInit())
											format_code = *fmt->m_oFormatCode;
									}
								}
							}
						}
					}
				}
			}
			sCellValue = ConvertValueCellToString(sCellValue, format_type, format_code);

		}
	}

	// Escape cell value
	if (m_bJSON)
	{
		NSStringUtils::CStringBuilder oBuilder;
		oBuilder.WriteString(_T("\""));
		escapeJson(sCellValue, oBuilder);
		oBuilder.WriteString(_T("\""));
		sCellValue = std::wstring(oBuilder.GetBuffer(), oBuilder.GetCurSize());
	}
	else if (std::wstring::npos != sCellValue.find_first_of(m_sEscape))
	{
		NSStringExt::Replace(sCellValue, g_sQuote, g_sDoubleQuote);
		sCellValue = g_sQuote + sCellValue + g_sQuote;
	}
	// Write cell value
	WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, sCellValue, m_nCodePage);
	m_bIsWriteCell = true;
	m_bStartCell = false;
}
void CSVWriter::Impl::WriteRowEnd(OOX::Spreadsheet::CRow* pWorksheet)
{
	if (m_bJSON)
		WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, g_sEndJson, m_nCodePage);
	else
	{
		while (m_nColDimension > m_nColCurrent) // todooo - прописывать в бинарнике dimension - и данные брать оттуда
		{
			// Write delimiter
			++m_nColCurrent;
			WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, m_sDelimiter, m_nCodePage);
			m_bIsWriteCell = false;
		}
	}
}
void CSVWriter::Impl::WriteSheetEnd(OOX::Spreadsheet::CWorksheet* pWorksheet)
{
	if (m_bJSON)
	{
		WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, g_sEndJson, m_nCodePage);
		WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, g_sEndJson, m_nCodePage, true);
	}
}
void CSVWriter::Impl::End()
{
	// Теперь мы пишем как MS Excel (новую строку записываем в файл)
	if (!m_bJSON)
	{
		WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, g_sNewLineN, m_nCodePage);
		WriteFile(&m_oFile, &m_pWriteBuffer, m_nCurrentIndex, g_sNewLineN, m_nCodePage, true);
	}
	Close();
}
void CSVWriter::Impl::Close()
{
	RELEASEARRAYOBJECTS(m_pWriteBuffer);
	m_oFile.CloseFile();
}
void CSVWriter::Impl::GetDefaultFormatCode(int numFmt, std::wstring & format_code, boost::optional<int> & format_type)
{
	switch (numFmt)
	{
	case 1:		format_code = L"0";					format_type = SimpleTypes::Spreadsheet::celltypeNumber; break;
	case 2:		format_code = L"0.00";				format_type = SimpleTypes::Spreadsheet::celltypeNumber; break;
	case 3:		format_code = L"#,##0";				format_type = SimpleTypes::Spreadsheet::celltypeNumber; break;
	case 4:		format_code = L"#,##0.00";			format_type = SimpleTypes::Spreadsheet::celltypeNumber; break;

	case 9:		format_code = L"0%";				format_type = SimpleTypes::Spreadsheet::celltypePercentage; break;
	case 10:	format_code = L"0.00%";				format_type = SimpleTypes::Spreadsheet::celltypePercentage; break;

	case 11:	format_code = L"0.00E+00";			format_type = SimpleTypes::Spreadsheet::celltypeScientific; break;
	case 12:	format_code = L"# ?/?";				format_type = SimpleTypes::Spreadsheet::celltypeFraction; break;
	case 13:	format_code = L"# ??/??";			format_type = SimpleTypes::Spreadsheet::celltypeFraction; break;

	case 14:	format_code = L"mm-dd-yy";			format_type = SimpleTypes::Spreadsheet::celltypeDate; break;
	case 15:	format_code = L"d-mmm-yy";			format_type = SimpleTypes::Spreadsheet::celltypeDate; break;
	case 16:	format_code = L"d-mmm";				format_type = SimpleTypes::Spreadsheet::celltypeDate; break;
	case 17:	format_code = L"mmm-yy";			format_type = SimpleTypes::Spreadsheet::celltypeDate; break;

	case 18:	format_code = L"h:mm AM/PM";		format_type = SimpleTypes::Spreadsheet::celltypeTime; break;
	case 19:	format_code = L"h:mm:ss AM/PM";		format_type = SimpleTypes::Spreadsheet::celltypeTime; break;
	case 20:	format_code = L"h:mm";				format_type = SimpleTypes::Spreadsheet::celltypeTime; break;
	case 21:	format_code = L"h:mm:ss";			format_type = SimpleTypes::Spreadsheet::celltypeTime; break;
	case 22:	format_code = L"m/d/yy h:mm";		format_type = SimpleTypes::Spreadsheet::celltypeDateTime; break;

	case 37:	format_code = L"#,##0 ;(#,##0)";		format_type = SimpleTypes::Spreadsheet::celltypeNumber; break;
	case 38:	format_code = L"#,##0 ;[Red](#,##0)";	format_type = SimpleTypes::Spreadsheet::celltypeNumber; break;
	case 39:	format_code = L"#,##0.00;(#,##0.00)";	format_type = SimpleTypes::Spreadsheet::celltypeNumber; break;
	case 40:	format_code = L"#,##0.00;[Red](#,##0.00)"; format_type = SimpleTypes::Spreadsheet::celltypeNumber; break;

	case 45:	format_code = L"mm:ss";			format_type = SimpleTypes::Spreadsheet::celltypeTime; break;
	case 46:	format_code = L"[h]:mm:ss";		format_type = SimpleTypes::Spreadsheet::celltypeTime; break;
	case 47:	format_code = L"mmss.0";		format_type = SimpleTypes::Spreadsheet::celltypeTime; break;

	default:
		/////////////////////////////////// с неопределенным format_code .. он задается в файле
		if (numFmt >= 5 && numFmt <= 8)		format_type = SimpleTypes::Spreadsheet::celltypeCurrency;
		if (numFmt >= 41 && numFmt <= 44)	format_type = SimpleTypes::Spreadsheet::celltypeCurrency;

		if (numFmt >= 27 && numFmt <= 31)	format_type = SimpleTypes::Spreadsheet::celltypeDate;
		if (numFmt >= 50 && numFmt <= 54)	format_type = SimpleTypes::Spreadsheet::celltypeDate;
		if (numFmt >= 57 && numFmt <= 58)	format_type = SimpleTypes::Spreadsheet::celltypeDate;
		if (numFmt == 36)					format_type = SimpleTypes::Spreadsheet::celltypeDate;

		if (numFmt >= 32 && numFmt <= 35)	format_type = SimpleTypes::Spreadsheet::celltypeTime;
		if (numFmt >= 55 && numFmt <= 56)	format_type = SimpleTypes::Spreadsheet::celltypeTime;

		if (numFmt >= 60 && numFmt <= 62)	format_type = SimpleTypes::Spreadsheet::celltypeNumber;
		if (numFmt >= 69 && numFmt <= 70)	format_type = SimpleTypes::Spreadsheet::celltypeNumber;

		if (numFmt >= 67 && numFmt <= 68)	format_type = SimpleTypes::Spreadsheet::celltypePercentage;

		if (numFmt >= 71 && numFmt <= 74)	format_type = SimpleTypes::Spreadsheet::celltypeDate;
		if (numFmt >= 75 && numFmt <= 80)	format_type = SimpleTypes::Spreadsheet::celltypeTime;
		if (numFmt == 81)					format_type = SimpleTypes::Spreadsheet::celltypeDate;
	}
}
std::wstring CSVWriter::Impl::ConvertValueCellToString(const std::wstring &value, boost::optional<int> format_type, std::wstring format_code)
{
	if (false == format_code.empty())
	{
		std::vector<std::wstring> format_codes;
		boost::algorithm::split(format_codes, format_code, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);

		format_code = format_codes[0];
		int format_type_detect = detect_format(format_code);
		if (!format_type)
			format_type = format_type_detect;
	}

	switch (format_type.get_value_or(SimpleTypes::Spreadsheet::celltypeStr))
	{
	case SimpleTypes::Spreadsheet::celltypeDate:		return convert_date_time(value, format_code, true, false);
	case SimpleTypes::Spreadsheet::celltypeTime:		return convert_date_time(value, format_code, false, true);
	case SimpleTypes::Spreadsheet::celltypeDateTime:	return convert_date_time(value, format_code);
	case SimpleTypes::Spreadsheet::celltypeFraction:	return convert_fraction(value);
	case SimpleTypes::Spreadsheet::celltypeScientific:	return convert_scientific(value);

	case SimpleTypes::Spreadsheet::celltypeInlineStr:
	case SimpleTypes::Spreadsheet::celltypeSharedString:
	case SimpleTypes::Spreadsheet::celltypeStr:
	case SimpleTypes::Spreadsheet::celltypeError:
		return value;

	default:
		if (format_code.empty())
			return value;
		else
		{
			try
			{
				std::wstring format_code_tmp;

				int count_d = 0;
				bool bFloat = false, bStart = true, bEnd = false, bPercent = false;

				size_t pos_skip = format_code.rfind(L"#");
				if (pos_skip == std::wstring::npos) pos_skip = 0;
				else pos_skip++;

				for (size_t i = pos_skip; i < format_code.size(); ++i)
				{
					if (format_code[i] == L'\\' || format_code[i] == L'\"')
						continue;
					else if (format_code[i] != L'0')
					{
						if (count_d > 0)
						{
							if (bStart) format_code_tmp += L"% 0"; //padding
							format_code_tmp += std::to_wstring(count_d);

							if (!bStart && bFloat)
							{
								format_code_tmp += L"f";
								bEnd = true;
							}
							bStart = false;
							count_d = 0;
						}
						if (format_code[i] == L'.')
						{
							bFloat = true;
							format_code_tmp += format_code[i];
						}
						else if (format_code[i] == L',')
						{

						}
						else
						{
							if (!bStart && !bEnd)
							{
								format_code_tmp += L"d";
								bEnd = true;
							}

							if ((bStart && count_d < 1) || bEnd)
							{
								if (format_code[i] == L'%')
								{
									bPercent = true;
									format_code_tmp += (std::wstring(L"%") + format_code[i]);
								}
								else
									format_code_tmp += format_code[i];
							}
						}
					}
					else if (!bEnd)
					{
						count_d++;
					}
				}
				if (count_d > 0)
				{
					if (bStart) format_code_tmp += L"% 0"; //padding
					format_code_tmp += std::to_wstring(count_d);
					bStart = false;

				}
				else
					format_code_tmp += L"%";
				if (!bStart && !bEnd) format_code_tmp += bFloat ? L"f" : L"ld";


				double dValue = XmlUtils::GetDouble(value);
				if (bPercent)
					dValue *= 100.;

				std::wstringstream stream;

				if (bFloat)
				{
					stream << boost::wformat(format_code_tmp) % dValue;
				}
				else
				{
					_INT64 iValue = dValue;
					stream << boost::wformat(format_code_tmp) % iValue;
				}

				return stream.str();
			}
			catch (...)
			{
				return value;
			}
		}
	}
}
