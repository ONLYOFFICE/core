/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
