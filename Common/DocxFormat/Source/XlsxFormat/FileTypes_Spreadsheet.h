#pragma once
#ifndef OOX_XSLXFILE_TYPES_SPREADSHEET_INCLUDE_H_
#define OOX_XSLXFILE_TYPES_SPREADSHEET_INCLUDE_H_

#include "../DocxFormat/FileType.h"


namespace OOX
{
	namespace Spreadsheet
	{
		namespace FileTypes
		{
			const FileType Workbook(L"xl", L"workbook.xml",
												_T("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument"));

			const FileType SharedStrings(L"", L"sharedStrings.xml",
												_T("application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings"));

			const FileType Styles(L"", L"styles.xml",
												_T("application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles"));

			const FileType Worksheet(L"worksheets", L"sheet.xml",
												_T("application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet"), true);

			const FileType Chartsheets(L"chartsheets", L"sheet.xml",
												_T("application/vnd.openxmlformats-officedocument.spreadsheetml.chartsheet+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartsheet"), true);

			const FileType CalcChain(L"", L"calcChain.xml",
												_T("application/vnd.openxmlformats-officedocument.spreadsheetml.calcChain+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/calcChain"));

			const FileType Drawings(L"../drawings", L"drawing.xml",
												_T("application/vnd.openxmlformats-officedocument.drawing+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing"), true, true);

			const FileType Comments(L"../", L"comments.xml",
												_T("application/vnd.openxmlformats-officedocument.spreadsheetml.comments+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments"), true, true);

			const FileType Image(L"../media", L"image.jpg",
												_T(""),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image"));

			const FileType Charts(L"../charts", L"chart.xml",
												_T("application/vnd.openxmlformats-officedocument.drawingml.chart+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart"), true, true);

			const FileType Table(L"../tables", L"table.xml",
												_T("application/vnd.openxmlformats-officedocument.spreadsheetml.table+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/table"), true, true);

			const FileType ExternalLinks(L"externalLinks", L"externalLink.xml",
												_T("application/vnd.openxmlformats-officedocument.spreadsheetml.externalLink+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLink"), true, true);

			const FileType ExternalLinkPath(L"", L"", 
												_T(""), 
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLinkPath"));
		} // namespace FileTypes
	}
} // namespace OOX

#endif // OOX_XSLXFILE_TYPES_SPREADSHEET_INCLUDE_H_