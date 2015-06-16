#ifndef _PDF_WRITER_SRC_ANNOTATION_H
#define _PDF_WRITER_SRC_ANNOTATION_H

#include "Objects.h"
#include "Types.h"

namespace PdfWriter
{
	class CDestination;

	enum EBorderSubtype
	{
		border_subtype_Solid,
		border_subtype_Dashed,
		border_subtype_Beveled,
		border_subtype_Inset,
		border_subtype_Underlined
	};
	enum EAnnotType
	{
		AnnotUnknown        = -1,
		AnnotTextNotes      = 0,
		AnnotLink           = 1,
		AnnotSound          = 2,
		AnnotFreeText       = 3,
		AnnotStamp          = 4,
		AnnotSquare         = 5,
		AnnotCircle         = 6,
		AnnotStrikeOut      = 7,
		AnnotHighLight      = 8,
		AnnotUnderline      = 9,
		AnnotInk            = 10,
		AnnotFileAttachment = 11,
		AnnotPopup          = 12
	};
	enum EAnnotHighlightMode
	{
		AnnotNoHighlight = 0,
		AnnotInvertBox,
		AnnotInvertBorder,
		AnnotDownAppearance,
		AnnotHighlightModeEOF
	};
	enum EAnnotIcon
	{
		AnnotIconComment      = 0,
		AnnotIconKey          = 1,
		AnnotIconNote         = 2,
		AnnotIconHelp         = 3,
		AnnotIconNewParagraph = 4,
		AnnotIconParagraph    = 5,
		AnnotIconInsert       = 6,

		AnnotIconMin          = 0, 
		AnnotIconMax          = 6
	};

	class CAnnotation : public CDictObject
	{
	public:
		CAnnotation(CXref* pXref, EAnnotType eType, const TRect& oRect);
		void SetBorderStyle(EBorderSubtype eSubtype, float fWidth, unsigned short nDashOn, unsigned short nDashOff, unsigned short nDashPhase);
		EDictType GetDictType() const
		{
			return dict_type_ANNOTATION;
		}
		virtual EAnnotType GetAnnotationType() const
		{
			return AnnotUnknown;
		}
	};
	class CLinkAnnotation : public CAnnotation
	{
	public:
		CLinkAnnotation(CXref* pXref, const TRect& oRect, CDestination* pDestination);
		EAnnotType GetAnnotationType() const
		{
			return AnnotLink;
		}
		void SetBorderStyle  (float fWidth, unsigned short nDashOn, unsigned short nDashOff);
		void SetHighlightMode(EAnnotHighlightMode eMode);
	};
	class CTextAnnotation : public CAnnotation
	{
	public:
		CTextAnnotation(CXref* pXref, const TRect& oRect, const char* sText);
		EAnnotType GetAnnotationType() const
		{
			return AnnotTextNotes;
		}
		void SetIcon(EAnnotIcon eIcon);
		void SetOpened(bool bOpened);
	};
	class CUriLinkAnnotation : public CAnnotation
	{
	public:
		CUriLinkAnnotation(CXref* pXref, const TRect& oRect, const char* sUri);
		EAnnotType GetAnnotationType() const
		{
			return AnnotLink;
		}
	};
}
#endif // _PDF_WRITER_SRC_ANNOTATION_H
