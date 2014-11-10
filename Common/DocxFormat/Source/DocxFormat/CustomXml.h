#pragma once
#ifndef OOX_CUSTOM_XML_INCLUDE_H_
#define OOX_CUSTOM_XML_INCLUDE_H_

#include "File.h"
#include "WritingElement.h"
#include "../Common/SimpleTypes_Shared.h"

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CCustomXML 22.5
	//--------------------------------------------------------------------------------	
	class CCustomXML : public OOX::File
	{
	public:

		class CShemaRef : public WritingElement
		{
		public:
			CShemaRef()
			{
			}
			CShemaRef(const XmlUtils::CXmlNode& oNode)
			{
				fromXML( (XmlUtils::CXmlNode&)oNode );
			}
			virtual ~CShemaRef()
			{
			}

		public:

			const CShemaRef& operator =(const XmlUtils::CXmlNode& oNode)
			{
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("ds:uri"), m_sUri );
			}
			virtual CString      toXML() const
			{
				CString sResult;
				sResult.Format( _T("<ds:schemaRef ds:uri=\"%s\" />"), m_sUri );
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_ds_schemaRef;
			}

		public:

			CString m_sUri;
		};

		class CShemaRefs : public WritingElementWithChilds<CShemaRef>
		{
		public:
			CShemaRefs()
			{
			}
			CShemaRefs(const XmlUtils::CXmlNode& oNode)
			{
				fromXML( (XmlUtils::CXmlNode&)oNode );
			}
			virtual ~CShemaRefs()
			{
			}

		public:

			const CShemaRefs& operator =(const XmlUtils::CXmlNode& oNode)
			{
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oNodes;
				if ( oNode.GetNodes( _T("ds:schemaRef"), oNodes ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oNodes.GetCount(); nIndex++ )
					{
						if ( oNodes.GetAt( nIndex, oItem ) )
						{
							CShemaRef *oShemeRef = new CShemaRef(oItem);
							if (oShemeRef) m_arrItems.push_back( oShemeRef );
						}
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<ds:schemaRefs>");

				for ( int nIndex = 0; nIndex < m_arrShemeRef.GetSize(); nIndex++ )
					sResult += m_arrShemeRef[nIndex].toXML();

				sResult += _T("</ds:schemaRefs>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_ds_schemaRefs;
			}

		public:

		};

	public:
	
		CCustomXML()
		{
		}
		CCustomXML(const CPath& oFilePath)
		{
			read( oFilePath );
		}
		virtual ~CCustomXML()
		{
		}

	public:
		virtual void read(const CPath& oFilePath)
		{
			XmlUtils::CXmlNode oCustomXml;
			oCustomXml.FromXmlFile( oFilePath.GetPath(), true );

			if ( _T("ds:datastoreItem") == oCustomXml.GetName() )
			{
				oCustomXml.ReadAttributeBase( _T("ds:itemID"), m_oItemID );

				XmlUtils::CXmlNode oItem;
				if ( oCustomXml.GetNode( _T("ds:schemaRefs"), oItem ) )
					m_oShemaRefs = oItem;
			}
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			CString sXml;
			sXml.Format( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><ds:datastoreItem ds:itemID=\"%s\" xmlns:ds=\"http://schemas.openxmlformats.org/officeDocument/2006/customXml\">"), m_oItemID.ToString() );

			if ( m_oShemaRefs.IsInit() )
				sXml += m_oShemaRefs->toXML();

			sXml += _T("</ds:datastoreItem>");
			CDirectory::SaveToFile( oFilePath.GetPath(), sXml );

			oContent.Registration( type().OverrideType(), oDirectory, oFilePath );
		}

	public:
		virtual const OOX::FileType type() const
		{
			return FileTypes::CustomXml;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}


	private:

		// Attributes
		SimpleTypes::CGuid   m_oItemID;

		// Childs
		nullable<CShemaRefs> m_oShemaRefs;
	};

} // namespace OOX

#endif // OOX_CUSTOM_XML_INCLUDE_H_