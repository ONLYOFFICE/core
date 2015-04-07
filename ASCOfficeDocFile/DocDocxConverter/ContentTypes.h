#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include <atlbase.h>
    #include <atlstr.h>
#else
    #include "../../DesktopEditor/common/ASCVariant.h"
    #include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

namespace OpenXmlContentTypes
{
	// default content types
	static const TCHAR* Xml						=	_T("application/xml");

	// package content types
	static const TCHAR* Relationships			=	_T("application/vnd.openxmlformats-package.relationships+xml");

	static const TCHAR* CoreProperties			=	_T("application/vnd.openxmlformats-package.core-properties+xml");

	// general office document content types
	static const TCHAR* ExtendedProperties		=	_T("application/vnd.openxmlformats-officedocument.extended-properties+xml");
	static const TCHAR* Theme					=	_T("application/vnd.openxmlformats-officedocument.theme+xml");

	static const TCHAR* CustomXmlProperties		=	_T("application/vnd.openxmlformats-officedocument.customXmlProperties+xml");

	static const TCHAR* OleObject				=	_T("application/vnd.openxmlformats-officedocument.oleObject");
	static const TCHAR* Vml						=	_T("application/vnd.openxmlformats-officedocument.vmlDrawing");
	static const TCHAR* Drawing					=	_T("application/vnd.openxmlformats-officedocument.drawing+xml");

	static const TCHAR* MSExcel					=	_T("application/vnd.ms-excel");
	static const TCHAR* MSWord					=	_T("application/msword");
	static const TCHAR* MSPowerpoint			=	_T("application/vnd.ms-powerpoint");

	static const TCHAR* Emf						=	_T("image/x-emf");
	static const TCHAR* Jpeg					=	_T("image/jpeg");
	static const TCHAR* Png						=	_T("image/png");
	static const TCHAR* Tiff					=	_T("image/tiff");
	static const TCHAR* Wmf						=	_T("image/x-wmf");
}

namespace WordprocessingMLContentTypes
{
	// WordprocessingML content types
	static const TCHAR* MainDocument			=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
	static const TCHAR* MainDocumentMacro		=	_T("application/vnd.ms-word.document.macroEnabled.main+xml");
	static const TCHAR* MainDocumentTemplate	=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml");
	static const TCHAR* MainDocumentMacroTemplate =	_T("application/vnd.ms-word.template.macroEnabledTemplate.main+xml");

	static const TCHAR* Styles					=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml");
	static const TCHAR* Numbering				=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml");
	static const TCHAR* FontTable				=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml");
	static const TCHAR* WebSettings				=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml");
	static const TCHAR* Settings				=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml");

	static const TCHAR* Comments				=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.comments+xml");

	static const TCHAR* Footnotes				=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml"); 
	static const TCHAR* Endnotes				=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml");

	static const TCHAR* Header					=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.header+xml");
	static const TCHAR* Footer					=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.footer+xml");

	static const TCHAR* Glossary				=	_T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.glossary+xml");
}

namespace MicrosoftWordContentTypes
{
	static const TCHAR* KeyMapCustomization		=	_T("application/vnd.ms-word.keyMapCustomizations+xml");
	static const TCHAR* VbaProject				=	_T("application/vnd.ms-office.vbaProject");
	static const TCHAR* VbaData					=	_T("application/vnd.ms-word.vbaData+xml");
	static const TCHAR* Toolbars				=	_T("application/vnd.ms-word.attachedToolbars");
}

namespace OpenXmlNamespaces
{
	// package namespaces
	static const TCHAR* ContentTypes			=	_T("http://schemas.openxmlformats.org/package/2006/content-types");
	static const TCHAR* RelationshipsPackage	=	_T("http://schemas.openxmlformats.org/package/2006/relationships");
	static const TCHAR* Relationships			=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships");

	// Drawing ML namespaces
	static const TCHAR* DrawingML				=	_T("http://schemas.openxmlformats.org/drawingml/2006/main");
	static const TCHAR* DrawingMLPicture		=	_T("http://schemas.openxmlformats.org/drawingml/2006/picture");

	// WordprocessingML namespaces
	static const TCHAR* WordprocessingML		=	_T("http://schemas.openxmlformats.org/wordprocessingml/2006/main");
	static const TCHAR* WordprocessingDrawingML =	_T("http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing");
	static const TCHAR* VectorML				=	_T("urn:schemas-microsoft-com:vml");
	static const TCHAR* MicrosoftWordML			=	_T("http://schemas.microsoft.com/office/word/2006/wordml");

	// PresentationML namespaces
	static const TCHAR* PresentationML			=	_T("http://schemas.openxmlformats.org/presentationml/2006/main");

	// SpreadsheetML Namespaces
	static const TCHAR* SharedStringML			=	_T("http://schemas.openxmlformats.org/spreadsheetml/2006/main");
	static const TCHAR* WorkBookML				=	_T("http://schemas.openxmlformats.org/spreadsheetml/2006/main");
	static const TCHAR* StylesML				=	_T("http://schemas.openxmlformats.org/spreadsheetml/2006/main"); 

	//Office
	static const TCHAR* Office					=	_T("urn:schemas-microsoft-com:office:office");
	static const TCHAR* OfficeWord				=	_T("urn:schemas-microsoft-com:office:word");
}

namespace OpenXmlRelationshipTypes
{
	static const TCHAR* CoreProperties			=	_T("http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties");
	static const TCHAR* ExtendedProperties		=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties");

	static const TCHAR* Theme					=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme");
	static const TCHAR* OfficeDocument			=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");
	static const TCHAR* Styles					=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles");
	static const TCHAR* FontTable				=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable");
	static const TCHAR* Numbering				=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering"); 
	static const TCHAR* WebSettings				=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings");
	static const TCHAR* Settings				=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings");
	static const TCHAR* CustomXml				=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXml");
	static const TCHAR* CustomXmlProperties		=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXmlProps");
	static const TCHAR* Comments				=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments");
	static const TCHAR* Footnotes				=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes");
	static const TCHAR* Endnotes				=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes");
	static const TCHAR* Header					=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/header");
	static const TCHAR* Footer					=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer");
	static const TCHAR* Image					=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image");
	static const TCHAR* OleObject				=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject");
	static const TCHAR* GlossaryDocument		=	_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/glossaryDocument");
}

namespace MicrosoftWordRelationshipTypes
{
	static const TCHAR* KeyMapCustomizations	=	_T("http://schemas.microsoft.com/office/2006/relationships/keyMapCustomizations");
	static const TCHAR* VbaProject				=	_T("http://schemas.microsoft.com/office/2006/relationships/vbaProject");
	static const TCHAR* VbaData					=	_T("http://schemas.microsoft.com/office/2006/relationships/wordVbaData");
	static const TCHAR* Toolbars				=	_T("http://schemas.microsoft.com/office/2006/relationships/attachedToolbars");
}
