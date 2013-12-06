
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Numbering.h"

namespace OOX
{
	Numbering::Level::Level()
	{
		Tentative	=	0;
		Align		=	Logic::Align();
		NumFmt		=	OOX::Logic::NumFormat();
	}

	Numbering::Level::~Level()
	{
	}

	Numbering::Level::Level(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
	}

	const Numbering::Level& Numbering::Level::operator =(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
		return *this;
	}

	void Numbering::Level::fromXML(XmlUtils::CXmlNode& oNode)
	{
		if ( _T("w:lvl") == oNode.GetName() )
		{
			Ilvl = _wtoi(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:ilvl"))));
			Tentative = _wtoi(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:tentative"))));
			Tplc = std::wstring(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:tentative"))));

			XmlUtils::CXmlNode oChild;
			
			if ( oNode.GetNode( _T("w:suff"), oChild ) )
				Suffix = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			
			if ( oNode.GetNode( _T("w:lvlText"), oChild ) )
				Text = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			
			if ( oNode.GetNode( _T("w:lvlJc"), oChild ) )
				Align = Logic::Align(std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val")))));
			
			if ( oNode.GetNode( _T("w:start"), oChild ) )
				Start = _wtoi(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));

			if ( oNode.GetNode( _T("w:numFmt"), oChild ) )
			{
				OOX::Logic::NumFormat fmt;
				fmt.fromXML(oChild);
				NumFmt = fmt;
			}
		}

		///NOTE:

		////ParagraphProperty	=	element.element("pPr");
		////RunProperty			=	element.element("rPr");
	}
} // namespace OOX