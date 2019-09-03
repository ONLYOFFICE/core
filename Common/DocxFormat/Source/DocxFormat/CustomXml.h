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
#ifndef OOX_CUSTOM_XML_INCLUDE_H_
#define OOX_CUSTOM_XML_INCLUDE_H_

#include "File.h"
#include "IFileContainer.h"
#include "FileTypes.h"
#include "WritingElement.h"
#include "../Common/SimpleTypes_Shared.h"

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CCustomXML 22.5
	//--------------------------------------------------------------------------------	
	class CCustomXMLProps : public OOX::File
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
				XmlMacroReadAttributeBase( oNode, _T("ds:uri"), m_sUri );
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<ds:schemaRef ds:uri=\"") + m_sUri  + _T("\" />");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_ds_schemaRef;
			}

		public:

			std::wstring m_sUri;
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
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<ds:schemaRefs>");

				for ( size_t nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
					sResult += m_arrItems[nIndex]->toXML();

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
	
		CCustomXMLProps(OOX::Document *pMain): OOX::File(pMain)
		{
		}
		CCustomXMLProps(OOX::Document *pMain, const OOX::CPath& oFilePath): OOX::File(pMain)
		{
			read( oFilePath );
		}
		virtual ~CCustomXMLProps()
		{
		}

	public:
		virtual void read(const CPath& oFilePath)
		{
			XmlUtils::CXmlNode oCustomXml;
			oCustomXml.FromXmlFile( oFilePath.GetPath(), true );

			if ( _T("ds:datastoreItem") == oCustomXml.GetName() )
			{
				m_oItemID = oCustomXml.ReadAttribute(_T("ds:itemID"));

				XmlUtils::CXmlNode oItem;
				if ( oCustomXml.GetNode( _T("ds:schemaRefs"), oItem ) )
					m_oShemaRefs = oItem;
			}
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			std::wstring sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><ds:datastoreItem ds:itemID=\"");
			sXml += m_oItemID.ToString();
			sXml += _T("\" xmlns:ds=\"http://schemas.openxmlformats.org/officeDocument/2006/customXml\">");

			if ( m_oShemaRefs.IsInit() )
				sXml += m_oShemaRefs->toXML();

			sXml += _T("</ds:datastoreItem>");
			NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), sXml);

			oContent.Registration( type().OverrideType(), type().DefaultDirectory(), oFilePath.GetFilename() );
		}

	public:
		virtual const OOX::FileType type() const
		{
			return FileTypes::CustomXmlProps;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}


	public:

		// Attributes
		SimpleTypes::CGuid   m_oItemID;

		// Childs
		nullable<CShemaRefs> m_oShemaRefs;
	};

	class CCustomXML : public OOX::File, public OOX::IFileContainer
	{
	public:
		CCustomXML(OOX::Document *pMain): OOX::File(pMain), OOX::IFileContainer(pMain)
		{
		}
		CCustomXML(OOX::Document *pMain, const CPath& oRootPath, const CPath& oPath): OOX::File(pMain), OOX::IFileContainer(pMain)
		{
			read( oRootPath, oPath );
		}
		virtual ~CCustomXML()
		{
		}

	public:
		virtual void read(const CPath& oPath)
		{
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		virtual void read(const CPath& oRootPath, const CPath& oFilePath)
		{
			IFileContainer::Read( oRootPath, oFilePath );

			NSFile::CFileBinary::ReadAllTextUtf8(oFilePath.GetPath(), m_sXml);
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			NSFile::CFileBinary::SaveToFile( oFilePath.GetPath(), m_sXml );

			oContent.Registration( type().OverrideType(), oDirectory, oFilePath.GetFilename() );
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

		std::wstring GetSchemaUrl()
		{
			std::vector<smart_ptr<OOX::File>>& container = GetContainer();
			for (size_t i = 0; i < container.size(); ++i)
			{
				if (OOX::FileTypes::CustomXmlProps == container[i]->type())
				{
					OOX::CCustomXMLProps* pCustomXmlProps = dynamic_cast<OOX::CCustomXMLProps*>(container[i].GetPointer());
					if(pCustomXmlProps->m_oShemaRefs.IsInit())
					{
						for (size_t j = 0; j < pCustomXmlProps->m_oShemaRefs->m_arrItems.size(); ++j)
						{
							return pCustomXmlProps->m_oShemaRefs->m_arrItems[j]->m_sUri;
						}
					}
				}
			}
			return L"";
		}


	public:
		// Childs
		std::wstring m_sXml;
	};

} // namespace OOX

#endif // OOX_CUSTOM_XML_INCLUDE_H_
