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
