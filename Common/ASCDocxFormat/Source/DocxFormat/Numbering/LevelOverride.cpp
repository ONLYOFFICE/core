
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Numbering.h"

namespace OOX
{
	Numbering::LevelOverride::LevelOverride()
	{
		Ilvl			=	0;
		StartOverride	=	0;
	}

	Numbering::LevelOverride::~LevelOverride()
	{
	}

	Numbering::LevelOverride::LevelOverride(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
	}

	const Numbering::LevelOverride& Numbering::LevelOverride::operator =(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
		return *this;
	}

	void Numbering::LevelOverride::fromXML(XmlUtils::CXmlNode& oNode)
	{
		Ilvl = _wtoi(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:ilvl"))));

		XmlUtils::CXmlNode oChild;
		if ( oNode.GetNode( _T("w:startOverride"), oChild ) )
			StartOverride = _wtoi(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
		
		if ( oNode.GetNode( _T("w:lvl"), oChild ) )
		{
			OOX::Numbering::Level lvl;
			lvl.fromXML(oChild);
			Level = lvl;
		}
	}
} // namespace OOX