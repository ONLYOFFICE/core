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
#ifndef CSV_WRITER
#define CSV_WRITER

#include "../../DesktopEditor/common/File.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CRow;
		class CCell;
	}
}
namespace CSVWriter
{
	class CCSVWriter
	{
	protected:
		NSFile::CFileBinary m_oFile;
		OOX::Spreadsheet::CXlsx& m_oXlsx;
		UINT m_nCodePage;
		const std::wstring& m_sDelimiter;
		bool m_bJSON;

		WCHAR* m_pWriteBuffer;
		INT m_nCurrentIndex;
		std::wstring m_sEscape;
		INT m_nRowCurrent;
		INT m_nColCurrent;
		bool m_bIsWriteCell; // Нужно только для записи JSON-а
		bool m_bStartRow;
		bool m_bStartCell;
	public:
		CCSVWriter(OOX::Spreadsheet::CXlsx &oXlsx, UINT m_nCodePage, const std::wstring& sDelimiter, bool m_bJSON);
		~CCSVWriter();
		void Start(const std::wstring &sFileDst);
		void WriteSheetStart(OOX::Spreadsheet::CWorksheet* pWorksheet);
		void WriteRowStart(OOX::Spreadsheet::CRow *pRow);
		void WriteCell(OOX::Spreadsheet::CCell *pCell);
		void WriteRowEnd(OOX::Spreadsheet::CRow* pWorksheet);
		void WriteSheetEnd(OOX::Spreadsheet::CWorksheet* pWorksheet);
		void End();
		void Close();
	};
	void WriteFromXlsxToCsv(const std::wstring &sFileDst, OOX::Spreadsheet::CXlsx &oXlsx, UINT nCodePage, const std::wstring& wcDelimiter, bool bJSON);
}

#endif //CSV_WRITER
