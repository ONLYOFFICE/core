/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
#include "./FileTypes_Draw.h"

namespace OOX
{
	namespace Draw
	{
		namespace FileTypes
		{
			const FileType Document			(L"visio", L"document.xml",
												L"application/vnd.ms-visio.drawing.main+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/document");

			const FileType DocumentMacro	(L"visio", L"document.xml",
												L"application/vnd.ms-visio.drawing.macroEnabled.main+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/document");

			const FileType Windows			(L"", L"windows.xml",
												L"application/vnd.ms-visio.windows+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/windows");

			const FileType Validation		(L"", L"validation.xml",
												L"application/vnd.ms-visio.validation+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/validation");
			
			const FileType Comments			(L"", L"comments.xml",
												L"application/vnd.ms-visio.comments+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/comments");

			const FileType Connections		(L"data", L"connections.xml",
												L"application/vnd.ms-visio.connections+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/connections");

			const FileType Pages			(L"pages", L"pages.xml",
												L"application/vnd.ms-visio.pages+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/pages");


			const FileType Masters			(L"masters", L"masters.xml",
												L"application/vnd.ms-visio.masters+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/masters");


			const FileType Recordsets		(L"data", L"recordsets.xml",
												L"application/vnd.ms-visio.recordsets+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/recordsets");

			const FileType Solutions		(L"solutions", L"solutions.xml",
												L"application/vnd.ms-visio.solutions+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/solutions");

			const FileType Page				(L"", L"page.xml",
												L"application/vnd.ms-visio.page+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/page",
												L"pages/page", true);

			const FileType Master			(L"", L"master.xml",
												L"application/vnd.ms-visio.master+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/master", 
												L"masters/master", true);

			const FileType Recordset		(L"", L"recordset.xml",
												L"application/vnd.ms-visio.recordset+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/recordset",
												L"data/recordset", true);

			const FileType Solution			(L"", L"solution.xml",
												L"application/vnd.ms-visio.solution+xml",
												L"http://schemas.microsoft.com/visio/2010/relationships/solution",
												L"solutions/solution", true);
		} // namespace FileTypes
	}
} // namespace OOX
