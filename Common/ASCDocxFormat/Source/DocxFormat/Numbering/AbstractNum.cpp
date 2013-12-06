
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Numbering.h"
#include "Exception/log_runtime_error.h"

namespace OOX
{
	Numbering::AbstractNum::AbstractNum()
	{
		Id	=	0;
	}

	Numbering::AbstractNum::~AbstractNum()
	{

	}

	Numbering::AbstractNum::AbstractNum(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
	}

	const Numbering::AbstractNum& Numbering::AbstractNum::operator =(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
		return *this;
	}

	void Numbering::AbstractNum::fromXML(XmlUtils::CXmlNode& oNode)
	{
		if ( _T("w:abstractNum") == oNode.GetName() )
		{
			Id = _wtoi(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:abstractNumId"))));

			XmlUtils::CXmlNode oChild;
			if ( oNode.GetNode( _T("w:nsid"), oChild ) )
				Nsid = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			if ( oNode.GetNode( _T("w:multiLevelType"), oChild ) )
				MultiLevelType = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			if ( oNode.GetNode( _T("w:tmpl"), oChild ) )
				Tmpl = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			if ( oNode.GetNode( _T("w:numStyleLink"), oChild ) )
				numStyleLink = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));

			XmlUtils::CXmlNodes oLvlList;
			if ( oNode.GetNodes( _T("w:lvl"), oLvlList ) )
			{
				XmlUtils::CXmlNode oLvlNode;
				for ( int nIndex = 0; nIndex < oLvlList.GetCount(); ++nIndex )
				{
					if ( oLvlList.GetAt( nIndex, oLvlNode ) )
					{
						OOX::Numbering::Level oLvl;
						oLvl.fromXML(oLvlNode);
						Levels.push_back(oLvl);
					}
				}
			}
		}
	}

	const Numbering::Level Numbering::AbstractNum::getLevel(const int numLevel) const
	{
		for (std::vector<Level>::const_iterator iter = Levels.begin(); iter != Levels.end(); ++iter)
		{
			if ((*iter).Ilvl == numLevel)
				return (*iter);
		}

		throw log_runtime_error("bad abstractNum");
	}

} // namespace OOX