
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Numbering.h"

namespace OOX
{
	Numbering::Num::Num()
	{
		NumId			=	0;
		AbstractNumId	=	0;
	}

	Numbering::Num::~Num()
	{
	}

	Numbering::Num::Num(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
	}

	const Numbering::Num& Numbering::Num::operator =(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
		return *this;
	}

	void Numbering::Num::fromXML(XmlUtils::CXmlNode& oNode)
	{
		NumId = _wtoi(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:numId"))));

		XmlUtils::CXmlNode oChild;
		if ( oNode.GetNode( _T("w:startOverride"), oChild ) )
			AbstractNumId = _wtoi(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));

		XmlUtils::CXmlNodes oLvlList;
		if ( oNode.GetNodes( _T("w:lvlOverride"), oLvlList ) )
		{
			XmlUtils::CXmlNode oLvlNode;
			for ( int nIndex = 0; nIndex < oLvlList.GetCount(); ++nIndex )
			{
				if ( oLvlList.GetAt( nIndex, oLvlNode ) )
				{
					OOX::Numbering::LevelOverride oLvl;
					oLvl.fromXML(oLvlNode);
					LevelOverrides.push_back(oLvl);
				}
			}
		}
	}

} // namespace OOX