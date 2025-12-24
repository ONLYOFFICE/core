#/*
 * (c) Copyright Ascensio System SIA 2010-2025
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "xls_writer.h"
#include "../Format/Binary/CompoundFile.h"
#include "../Format/Binary/CFStreamCacheWriter.h"
#include "../Format/Logic/WorkbookStreamObject.h"
#include <iomanip>

bool XlsWriter::Open(const std::wstring &fileName)
{
	xls_file = boost::shared_ptr<XLS::CompoundFile>(new XLS::CompoundFile(fileName, XLS::CompoundFile::cf_WriteMode));
	if(xls_file->isError())
		return false;
	return true;
}
bool XlsWriter::WriteWorkbook(XLS::BaseObjectPtr streamObject)
{
	auto WokrkbokStreamName = L"Workbook";
	if(globalInfoPtr == nullptr)
		globalInfoPtr = boost::shared_ptr<XLS::GlobalWorkbookInfo>(new XLS::GlobalWorkbookInfo(XLS::WorkbookStreamObject::DefaultCodePage, nullptr));
	auto BookStream = xls_file->createNamedStream(WokrkbokStreamName);
	if(BookStream == nullptr)
		return false;
	XLS::StreamCacheWriterPtr cacheWriter(new XLS::CFStreamCacheWriter(BookStream, globalInfoPtr));
	XLS::BinWriterProcessor stream_proc(cacheWriter, nullptr);
	stream_proc.mandatory(*streamObject);

	//writing fileptrs
	{
		auto filePos = BookStream->getStreamPointer();
		for(auto sheet : globalInfoPtr->sheets_info)
		{
			//bof of worksheet
			BookStream->seekFromBegin(sheet.BoundSheetPos);
			BookStream->write(&sheet.StreamPos, 4);
		}
		BookStream->seekFromBegin(filePos);
	}

	return true;
}
bool XlsWriter::WriteStreamObject(std::wstring &streamName, XLS::BaseObjectPtr streamObject)
{
	if(globalInfoPtr == nullptr)
		globalInfoPtr = boost::shared_ptr<XLS::GlobalWorkbookInfo>(new XLS::GlobalWorkbookInfo(XLS::WorkbookStreamObject::DefaultCodePage, nullptr));
	XLS::StreamCacheWriterPtr cacheWriter(new XLS::CFStreamCacheWriter(xls_file->createNamedStream(streamName), globalInfoPtr));

	XLS::BinWriterProcessor stream_proc(cacheWriter, nullptr);
	stream_proc.mandatory(*streamObject);
	return true;
}

bool XlsWriter::WritePivotCache(XLS::BaseObjectPtr streamObject, _UINT32 cacheId)
{
	auto DirectoryName = L"_SX_DB_CUR/";
	std::wstringstream ss;
	ss << std::hex << std::setw(4) << std::setfill(L'0') << std::uppercase << cacheId;
	std::wstring streamName = DirectoryName + ss.str();
	return WriteStreamObject(streamName, streamObject);
}
