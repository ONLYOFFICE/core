#pragma once

#include <string>
#include <map>
#include <list>

using namespace std;

#if defined(_WIN32) || defined(_WIN64)
    #include <atlbase.h>
    #include <atlstr.h>
#else
    #include "../../DesktopEditor/common/ASCVariant.h"
    #include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif


namespace XMLTools
{
	/*========================================================================================================
	class XMLAttribute
	========================================================================================================*/

	template <class T> class XMLAttribute
	{
	private:
		basic_string<T> m_Name;
		basic_string<T> m_Value;

	public:

		XMLAttribute()
		{

		}

		/*========================================================================================================*/

		XMLAttribute( const T* name ) : m_Name(name)
		{
		}

		/*========================================================================================================*/

		XMLAttribute( const T* name, const T* value ) :	m_Name(name), m_Value(value)
		{
		}

		/*========================================================================================================*/

		~XMLAttribute()
		{
		}

		/*========================================================================================================*/

		void SetValue( const T* value )
		{
			m_Value = basic_string<T>( value );
		}

		/*========================================================================================================*/

		basic_string<T> GetName() const
		{
			return m_Name;
		}

		/*========================================================================================================*/

		basic_string<T> GetValue() const
		{
			return m_Value;
		}

		/*========================================================================================================*/

		basic_string<T> GetXMLString()
		{
			basic_string<T> xmlString( _T( "" ) );

			xmlString += m_Name;
			xmlString += basic_string<T>( _T( "=\"" ) );
			xmlString += m_Value;
			xmlString += basic_string<T>( _T( "\"" ) );

			return xmlString;
		}
	};

	/*========================================================================================================
	class XMLElement
    ========================================================================================================*/

	template <class T> class XMLElement
	{
		typedef pair< basic_string<T>, basic_string<T> > AttributeValuePair;

	private:
		basic_string<T> m_Name;
		basic_string<T> m_ElementText;
		map<basic_string<T>, basic_string<T>> m_AttributeMap;
        list<XMLElement<T>> m_Elements;

        typedef typename list<XMLElement<T>>::iterator          ElementsIterator;
        typedef typename list<XMLElement<T>>::const_iterator    ElementsIteratorConst;

        typedef typename map<basic_string<T>, basic_string<T>>::iterator            AttMapIterator;
        typedef typename map<basic_string<T>, basic_string<T>>::const_iterator      AttMapIteratorConst;

    public:

		XMLElement()
		{

		}

		/*========================================================================================================*/

		XMLElement( const T* name ) : m_Name(name)
		{

		}

		/*========================================================================================================*/

		XMLElement( const T* prefix, const T* localName ) : m_Name( basic_string<T>( prefix ) + basic_string<T>( _T( ":" ) ) + basic_string<T>( localName ) ), m_ElementText( _T( "" ) )
		{

		}

		/*========================================================================================================*/

		~XMLElement()
		{

		}

		/*========================================================================================================*/

		void AppendText( const T* text )
		{
			m_ElementText = basic_string<T>( text );
		}

		/*========================================================================================================*/

		void AppendTextSymbol( const T symbol )
		{
			m_ElementText += basic_string<T>( &symbol );
		}

		/*========================================================================================================*/

		void AppendAttribute( const XMLAttribute<T>& attribute )
		{
			AttributeValuePair p( attribute.GetName(), attribute.GetValue() );

			m_AttributeMap.insert( p );
		}

		/*========================================================================================================*/

		void AppendAttribute( const T* name, const T* value )
		{
			AttributeValuePair p( basic_string<T>( const_cast<T*>( name ) ), basic_string<T>( const_cast<T*>( value ) ) );

			m_AttributeMap.insert( p );
		}

		/*========================================================================================================*/

		void AppendChild( const XMLElement<T>& element )
		{
			m_Elements.push_back( element );
		}

		/*========================================================================================================*/

		void RemoveChild( const XMLElement<T>& element )
		{
			m_Elements.remove( element );
		}

		/*========================================================================================================*/

		bool FindChild( const XMLElement<T>& element )
		{
			bool result = false;

            for ( ElementsIterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++ )
			{
				if ( *iter == element )
				{
					result = true;

					break;
				}
			}

			return result;
		}

		/*========================================================================================================*/

		bool FindChildByName( const T* elementName ) const
		{
			bool result = false;

            for ( ElementsIterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++ )
			{
				if ( iter->m_Name == basic_string<T>( elementName ) )
				{
					result = true;

					break;
				}
			}

			return result;
		}

		/*========================================================================================================*/

		bool RemoveChildByName( const basic_string<T>& elementName )
		{
			bool result = false;

            for ( ElementsIterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++ )
			{
				if ( iter->m_Name == elementName )
				{
					m_Elements.erase( iter );

					result = true;

					break;
				}
			}

			return result;  
		}

		/*========================================================================================================*/

		bool operator == ( const XMLElement<T>& element )
		{
			bool result = false;

			result = ( m_Name == element.m_Name );

			if ( m_AttributeMap.size() != element.m_AttributeMap.size() )
			{
				result = false;
			}
			else
			{

                AttMapIterator  thisIter = m_AttributeMap.begin();
                AttMapIteratorConst elementIter = element.m_AttributeMap.begin();

				for ( ; thisIter != m_AttributeMap.end(); thisIter++, elementIter++ )
				{
					if ( ( thisIter->first != elementIter->first ) || ( thisIter->second != elementIter->second ) )
					{
						result = false;
					}
				}
			}

			if ( m_Elements.size() != element.m_Elements.size() )
			{
				result = false;
			}
			else
			{
                ElementsIterator        thisIter    = m_Elements.begin();
                ElementsIteratorConst  elementIter = element.m_Elements.begin();

				for ( ; thisIter != m_Elements.end(); thisIter++, elementIter++ )
				{
					if ( !( (*thisIter) == (*elementIter) ) )
					{
						result = false;
					}
				}
			}

			return result;
		}

		/*========================================================================================================*/

        basic_string<T> GetName() const
		{
			return m_Name;
		}

		/*========================================================================================================*/

        basic_string<T> GetXMLString()
		{
			basic_string<T> xmlString( _T( "" ) );

            bool bIsNameExists = ( m_Name != basic_string<T>( _T( "" ) ) );
            bool bIsTextExists = ( m_ElementText != basic_string<T>( _T( "" ) ) );

			if ( bIsNameExists )
			{
                xmlString += basic_string<T>( _T( "<" ) ) + m_Name;
			}

            if ( ( bIsNameExists ) && ( m_AttributeMap.size() > 0 ) )
			{
                for ( AttMapIterator iter = m_AttributeMap.begin(); iter != m_AttributeMap.end(); iter++ )
				{
					xmlString += basic_string<T>( _T( " " ) );
					xmlString += iter->first;
					xmlString += basic_string<T>( _T( "=\"" ) );
					xmlString += iter->second;
					xmlString += basic_string<T>( _T( "\"" ) );
				}
			}

			if ( ( m_Elements.size() > 0 ) || ( bIsTextExists ) )
			{
				if ( bIsNameExists )
				{
					xmlString += basic_string<T>( _T( ">" ) );
				}

                for ( ElementsIterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++ )
				{
					xmlString += iter->GetXMLString();
				}

				if ( bIsTextExists )
				{
					xmlString += m_ElementText;
				}

				if ( bIsNameExists )
				{
                    xmlString += basic_string<T>( _T( "</" ) );
					xmlString += m_Name;
                    xmlString += basic_string<T>( _T( ">" ) );
				}
			}
			else
			{
				if ( bIsNameExists )
				{
                    xmlString += basic_string<T>( _T( "/>" ) );
				}
			}

			return xmlString;
		}

		/*========================================================================================================*/

		unsigned int GetAttributeCount() const
		{
			return (unsigned int)m_AttributeMap.size();
		}

		/*========================================================================================================*/

		unsigned int GetChildCount() const
		{
			return (unsigned int)m_Elements.size();
		}
	};

}
