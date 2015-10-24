
#include "LanguageIdMapping.h"
#include "../../Common/MS-LCID.h"

namespace DocFileFormat
{
	LanguageIdMapping::LanguageIdMapping (XmlUtils::CXmlWriter* pWriter, LanguageType type) : PropertiesMapping (pWriter)
	{
		_type = type;
	}

	LanguageIdMapping::LanguageIdMapping (XMLTools::XMLElement<wchar_t>* parentElement, LanguageType type) : PropertiesMapping(NULL)
	{
		_parent = parentElement;
		_type = type;
	}

	LanguageIdMapping::~LanguageIdMapping()
	{
	}
}

namespace DocFileFormat
{
	void LanguageIdMapping::Apply( IVisitable* lid )
	{
		if ( dynamic_cast<LanguageId*>( lid )->Code != Nothing )
		{
			wstring langcode = getLanguageCode( dynamic_cast<LanguageId*>( lid ) );

			XMLTools::XMLAttribute<wchar_t>* att = NULL;

			switch ( _type )
			{
			case Default:
				{
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:val" ), langcode.c_str() );
				}
				break;

			case EastAsian:
				{                    
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:eastAsia" ), langcode.c_str() );
				}
				break;

			case Complex:
				{
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:bidi" ), langcode.c_str() );
				}
				break;

			default:
				{    
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:val" ), langcode.c_str() );
				}
				break;
			}

			if (m_pXmlWriter)
			{
				// !!!TODO!!!
				m_pXmlWriter->WriteString( att->GetXMLString().c_str() );
			}
			else if ( _parent != NULL )
			{
				_parent->AppendAttribute( *att );
			}

			RELEASEOBJECT( att );
		}
	}

	wstring LanguageIdMapping::getLanguageCode( LanguageId* lid )
	{
		int intLCID				= lid->Code;
		std::wstring strLCID	= msLCID2wstring(intLCID);

		if (strLCID.empty()) strLCID = L"en-US";

		return strLCID;
	}
}