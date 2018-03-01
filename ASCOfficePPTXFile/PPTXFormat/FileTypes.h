/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once
#ifndef PPTXOOX_FILE_TYPES_INCLUDE_H_
#define PPTXOOX_FILE_TYPES_INCLUDE_H_

#include "../../Common/DocxFormat/Source/DocxFormat/FileType.h"

namespace OOX
{
namespace Presentation
{
	namespace FileTypes
	{
		const FileType Presentation		(L"ppt", L"presentation.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument"));
		const FileType PresentationMacro(L"ppt", L"presentation.xml",
												_T("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument"));
		const FileType Slide			(L"slides", L"slide.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.slide+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide"));

		const FileType SlideLayout		(L"slideLayouts", L"slideLayout.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout"));

		const FileType SlideComments	(L"comments", L"comment.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.comment+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments"));

		const FileType CommentAuthors	(L"", L"commentAuthors.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.commentAuthors.main+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/commentAuthors"));

		const FileType SlideMaster		(L"slideMasters", L"slideMaster.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster"));

		const FileType NotesSlide		(L"notesSlides", L"notesSlide.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.notesSlide+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide"));

		const FileType NotesMaster		(L"notesMasters", L"notesMaster.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.notesMaster+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster"));

		const FileType HandoutMaster	(L"handoutMasters", L"handoutMaster.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.handoutMaster+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/handoutMaster"));

		const FileType PresProps		(L"", L"presProps.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.presProps+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/presProps"));

		const FileType TableStyles		(L"", L"tableStyles.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/tableStyles"));

		const FileType ViewProps		(L"", L"viewProps.xml",
												_T("application/vnd.openxmlformats-officedocument.presentationml.viewProps+xml"),
												_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/viewProps"));
	} // namespace FileTypes
	} //Presentation
} // namespace OOX

#endif // PPTXOOX_FILE_TYPES_INCLUDE_H_
