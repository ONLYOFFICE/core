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
#pragma once

#if defined(_WIN32) || defined(_WIN64)
	#include <tchar.h>
#endif

#include "../../DesktopEditor/common/ASCVariant.h"

namespace OpenXmlContentTypes
{
	// default content types
    static const wchar_t* Xml					=	L"application/xml";

	// package content types
    static const wchar_t* Relationships			=	L"application/vnd.openxmlformats-package.relationships+xml";

    static const wchar_t* CoreProperties		=	L"application/vnd.openxmlformats-package.core-properties+xml";

	// general office document content types
    static const wchar_t* ExtendedProperties	=	L"application/vnd.openxmlformats-officedocument.extended-properties+xml";
    static const wchar_t* Theme					=	L"application/vnd.openxmlformats-officedocument.theme+xml";

    static const wchar_t* CustomXmlProperties	=	L"application/vnd.openxmlformats-officedocument.customXmlProperties+xml";

    static const wchar_t* OleObject				=	L"application/vnd.openxmlformats-officedocument.oleObject";
    static const wchar_t* Vml					=	L"application/vnd.openxmlformats-officedocument.vmlDrawing";
    static const wchar_t* Drawing				=	L"application/vnd.openxmlformats-officedocument.drawing+xml";
	
    static const wchar_t* MSWordDocx			=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.document";

    static const wchar_t* MSExcel				=	L"application/vnd.ms-excel";
    static const wchar_t* MSWord				=	L"application/msword";
    static const wchar_t* MSPowerpoint			=	L"application/vnd.ms-powerpoint";

    static const wchar_t* Emf					=	L"image/x-emf";
    static const wchar_t* Jpeg					=	L"image/jpeg";
    static const wchar_t* Png					=	L"image/png";
    static const wchar_t* Tiff					=	L"image/tiff";
    static const wchar_t* Wmf					=	L"image/x-wmf";
    static const wchar_t* Bmp					=	L"image/bmp";
	static const wchar_t* Pcz					=	L"image/x-pcz";
}
namespace DocPrContentTypes
{
    static const wchar_t* App			=	L"application/vnd.openxmlformats-officedocument.extended-properties+xml";
    static const wchar_t* Core			=	L"application/vnd.openxmlformats-package.core-properties+xml";
}
namespace WordprocessingMLContentTypes
{
	// WordprocessingML content types
    static const wchar_t* MainDocument			=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml";
    static const wchar_t* MainDocumentMacro		=	L"application/vnd.ms-word.document.macroEnabled.main+xml";
    static const wchar_t* MainDocumentTemplate	=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml";
    static const wchar_t* MainDocumentMacroTemplate =	L"application/vnd.ms-word.template.macroEnabledTemplate.main+xml";

    static const wchar_t* Styles				=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml";
    static const wchar_t* Numbering				=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml";
    static const wchar_t* FontTable				=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml";
    static const wchar_t* WebSettings			=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml";
    static const wchar_t* Settings				=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml";

    static const wchar_t* Comments				=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.comments+xml";
	static const wchar_t* CommentsExtended		=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.commentsExtended+xml";

    static const wchar_t* Footnotes				=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml";
    static const wchar_t* Endnotes				=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml";

    static const wchar_t* Header				=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.header+xml";
    static const wchar_t* Footer				=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.footer+xml";

    static const wchar_t* Glossary				=	L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.glossary+xml";
}

namespace MicrosoftWordContentTypes
{
    static const wchar_t* KeyMapCustomization	=	L"application/vnd.ms-word.keyMapCustomizations+xml";
    static const wchar_t* VbaProject			=	L"application/vnd.ms-office.vbaProject";
    static const wchar_t* VbaData				=	L"application/vnd.ms-word.vbaData+xml";
    static const wchar_t* Toolbars				=	L"application/vnd.ms-word.attachedToolbars";
}

namespace OpenXmlNamespaces
{
	// package namespaces
    static const wchar_t* ContentTypes			=	L"http://schemas.openxmlformats.org/package/2006/content-types";
    static const wchar_t* RelationshipsPackage	=	L"http://schemas.openxmlformats.org/package/2006/relationships";
    static const wchar_t* Relationships			=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships";

	// Drawing ML namespaces
    static const wchar_t* DrawingML				=	L"http://schemas.openxmlformats.org/drawingml/2006/main";
    static const wchar_t* DrawingMLPicture		=	L"http://schemas.openxmlformats.org/drawingml/2006/picture";

	// WordprocessingML namespaces
    static const wchar_t* WordprocessingML		=	L"http://schemas.openxmlformats.org/wordprocessingml/2006/main";
    static const wchar_t* WordprocessingDrawingML =	L"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing";
    static const wchar_t* VectorML				=	L"urn:schemas-microsoft-com:vml";
    static const wchar_t* MicrosoftWordML		=	L"http://schemas.microsoft.com/office/word/2006/wordml";

	// PresentationML namespaces
    static const wchar_t* PresentationML		=	L"http://schemas.openxmlformats.org/presentationml/2006/main";

	// SpreadsheetML Namespaces
    static const wchar_t* SharedStringML		=	L"http://schemas.openxmlformats.org/spreadsheetml/2006/main";
    static const wchar_t* WorkBookML			=	L"http://schemas.openxmlformats.org/spreadsheetml/2006/main";
    static const wchar_t* StylesML				=	L"http://schemas.openxmlformats.org/spreadsheetml/2006/main";

	//Office
    static const wchar_t* Office				=	L"urn:schemas-microsoft-com:office:office";
    static const wchar_t* OfficeWord			=	L"urn:schemas-microsoft-com:office:word";
}

namespace OpenXmlRelationshipTypes
{
	static const wchar_t* CoreProperties		=	L"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties";
    static const wchar_t* ExtendedProperties	=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties";

    static const wchar_t* Theme					=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme";
    static const wchar_t* OfficeDocument		=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument";
    static const wchar_t* Styles				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles";
    static const wchar_t* FontTable				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable";
    static const wchar_t* Numbering				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering";
    static const wchar_t* WebSettings			=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings";
    static const wchar_t* Settings				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings";
    static const wchar_t* CustomXml				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXml";
    static const wchar_t* CustomXmlProperties	=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXmlProps";
    static const wchar_t* Comments				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments";
	static const wchar_t* CommentsExtended		=	L"http://schemas.microsoft.com/office/2011/relationships/commentsExtended";
    static const wchar_t* Footnotes				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes";
    static const wchar_t* Endnotes				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes";
    static const wchar_t* Header				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/header";
    static const wchar_t* Footer				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer";
    static const wchar_t* Image					=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image";
    static const wchar_t* OleObject				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject";
    static const wchar_t* GlossaryDocument		=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/glossaryDocument";
    static const wchar_t* Package				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package";
    static const wchar_t* VbaProject			=	L"http://schemas.microsoft.com/office/2006/relationships/vbaProject";
	static const wchar_t* Hyperlink				=	L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink";
}

namespace MicrosoftWordRelationshipTypes
{
    static const wchar_t* KeyMapCustomizations	=	L"http://schemas.microsoft.com/office/2006/relationships/keyMapCustomizations";
    static const wchar_t* VbaProject			=	L"http://schemas.microsoft.com/office/2006/relationships/vbaProject";
    static const wchar_t* VbaData				=	L"http://schemas.microsoft.com/office/2006/relationships/wordVbaData";
    static const wchar_t* Toolbars				=	L"http://schemas.microsoft.com/office/2006/relationships/attachedToolbars";
}
