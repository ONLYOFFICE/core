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
#include "FileTypes.h"

namespace OOX
{
namespace Presentation
{
	namespace FileTypes
	{
		const FileType Presentation		(L"ppt", L"presentation.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");

		const FileType PresentationMacro(L"ppt", L"presentation.xml",
												L"application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");

		const FileType Slide			(L"slides", L"slide.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.slide+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide");

		const FileType SlideLayout		(L"slideLayouts", L"slideLayout.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout");

		const FileType SlideComments	(L"comments", L"comment.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.comment+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments");

		const FileType CommentAuthors	(L"", L"commentAuthors.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.commentAuthors.main+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/commentAuthors");
		
		const FileType ModernCommentAuthors(L"", L"authors.xml",
												L"application/vnd.ms-powerpoint.authors+xml",
												L"http://schemas.microsoft.com/office/2018/10/relationships/authors");

		const FileType ModernComments	(L"comments", L"modernComment.xml",
												L"application/vnd.ms-powerpoint.comments+xml",
												L"http://schemas.microsoft.com/office/2018/10/relationships/comments");
	
		const FileType SlideMaster		(L"slideMasters", L"slideMaster.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster");

		const FileType NotesSlide		(L"notesSlides", L"notesSlide.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.notesSlide+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide");

		const FileType NotesMaster		(L"notesMasters", L"notesMaster.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.notesMaster+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster");

		const FileType HandoutMaster	(L"handoutMasters", L"handoutMaster.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.handoutMaster+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/handoutMaster");

		const FileType PresProps		(L"", L"presProps.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.presProps+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/presProps");

		const FileType TableStyles		(L"", L"tableStyles.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/tableStyles");

		const FileType ViewProps		(L"", L"viewProps.xml",
												L"application/vnd.openxmlformats-officedocument.presentationml.viewProps+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/viewProps");
	} // namespace FileTypes
	} //Presentation
} // namespace OOX
