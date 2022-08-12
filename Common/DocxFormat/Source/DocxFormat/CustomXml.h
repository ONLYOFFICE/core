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

#include "File.h"
#include "IFileContainer.h"
#include "FileTypes.h"
#include "WritingElement.h"
#include "../Common/SimpleTypes_Shared.h"
#include "Document.h"

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CCustomXML 22.5
	//--------------------------------------------------------------------------------	
	class CCustomXMLProps : public OOX::FileGlobalEnumerated, public WritingElement
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
			const CShemaRef& operator =(const XmlUtils::CXmlNode& oNode)
			{
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("ds:uri"), m_sUri );
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<ds:schemaRef ds:uri=\"") + m_sUri  + _T("\" />");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_ds_schemaRef;
			}
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(0);
					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
						pWriter->WriteString1(0, m_sUri);
					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
				pWriter->EndRecord();
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				BYTE type = pReader->GetUChar();

				LONG _rec_start = pReader->GetPos();
				LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();

					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
						m_sUri = pReader->GetString2();
					else
						break;
				}
				pReader->Seek(_end_rec);
			}
			std::wstring m_sUri;
		};
	//----------------------------------------------------------------------	
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
			const CShemaRefs& operator =(const XmlUtils::CXmlNode& oNode)
			{
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
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
			virtual std::wstring toXML() const
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
		};
	//----------------------------------------------------------------------	
		CCustomXMLProps(OOX::Document *pMain) : OOX::FileGlobalEnumerated(pMain)
		{
		}
		CCustomXMLProps(OOX::Document *pMain, const OOX::CPath& oFilePath): OOX::FileGlobalEnumerated(pMain)
		{
			read( oFilePath );
		}
		virtual ~CCustomXMLProps()
		{
		}
		const CCustomXMLProps& operator =(const XmlUtils::CXmlNode& oNode)
		{
			fromXML((XmlUtils::CXmlNode&)oNode);
			return *this;
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
			if (_T("ds:datastoreItem") == oNode.GetName())
			{
				m_oItemID = oNode.ReadAttribute(_T("ds:itemID"));

				XmlUtils::CXmlNode oItem;
				if (oNode.GetNode(_T("ds:schemaRefs"), oItem))
					m_oShemaRefs = oItem;
			}
		}
		virtual std::wstring toXML() const
		{
			std::wstring sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><ds:datastoreItem ds:itemID=\"");
			sXml += m_oItemID.ToString();
			sXml += _T("\" xmlns:ds=\"http://schemas.openxmlformats.org/officeDocument/2006/customXml\">");

			if (m_oShemaRefs.IsInit())
				sXml += m_oShemaRefs->toXML();

			sXml += _T("</ds:datastoreItem>");

			return sXml;
		}
		virtual void read(const CPath& oFilePath)
		{
			XmlUtils::CXmlNode oCustomXml;
			oCustomXml.FromXmlFile( oFilePath.GetPath(), true );

			fromXML(oCustomXml);
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), toXML());

			oContent.Registration( type().OverrideType(), OOX::CPath(L"customXml"), oFilePath.GetFilename() );
		}
		virtual EElementType getType() const
		{
			return OOX::et_ds_customXmlProps;
		}
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

	// Attributes
		SimpleTypes::CGuid   m_oItemID;

	// Childs
		nullable<CShemaRefs> m_oShemaRefs;
//------------- for write from binary
		std::wstring m_oCustomXmlContent;
		std::string m_oCustomXmlContentA;
	};

	class CCustomXML : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
	{
	public:
		CCustomXML(OOX::Document *pMain, bool bDocument = true) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument = bDocument;
		}
		CCustomXML(OOX::Document *pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument = (NULL != dynamic_cast<OOX::CDocument*>(pMain));
			read( oRootPath, oPath );
		}
		virtual ~CCustomXML()
		{
		}
		virtual void read(const CPath& oPath)
		{
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		virtual void read(const CPath& oRootPath, const CPath& oFilePath)
		{
			IFileContainer::Read( oRootPath, oFilePath );

			NSFile::CFileBinary::ReadAllTextUtf8A(oFilePath.GetPath(), m_sXmlA);
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			NSFile::CFileBinary oFile;
			if (true == oFile.CreateFileW(oFilePath.GetPath()))
			{
				if (false == m_sXmlA.empty())
					oFile.WriteFile((BYTE*)m_sXmlA.c_str(), m_sXmlA.length());
				oFile.CloseFile();
			}
			
			IFileContainer::Write(oFilePath, oDirectory, oContent);
		}
		virtual const OOX::FileType type() const
		{
			return FileTypes::CustomXml;
		}
		virtual const CPath DefaultDirectory() const
		{
			//if (m_bDocument) return type().DefaultDirectory();
			//else 
				return L"../" + type().DefaultDirectory();
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
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter)
		{
			pWriter->StartRecord(NSBinPptxRW::NSMainTables::Customs);

			std::vector<smart_ptr<OOX::File>>& containerCustom = GetContainer();
			for (size_t i = 0; i < containerCustom.size(); ++i)
			{
				if (OOX::FileTypes::CustomXmlProps == containerCustom[i]->type())
				{
					OOX::CCustomXMLProps* pCustomXmlProps = dynamic_cast<OOX::CCustomXMLProps*>(containerCustom[i].GetPointer());

					pWriter->StartRecord(0);
					pWriter->WriteStringW2(pCustomXmlProps->m_oItemID.ToString());
					pWriter->EndRecord();

					if (pCustomXmlProps->m_oShemaRefs.IsInit())
					{
						pWriter->WriteRecordArrayOfPointers(1, 0, pCustomXmlProps->m_oShemaRefs->m_arrItems);
					}
				}
			}

			pWriter->StartRecord(2);
			pWriter->WriteStringA(m_sXmlA);
			pWriter->EndRecord();

			pWriter->EndRecord();
		}
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			BYTE type = pReader->GetUChar();

			LONG _rec_start = pReader->GetPos();
			LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

			smart_ptr<OOX::CCustomXMLProps> pCustomXmlProps = new OOX::CCustomXMLProps(NULL);
			smart_ptr<OOX::File> pCustomXmlPropsFile = pCustomXmlProps.smart_dynamic_cast<OOX::File>();
			Add(pCustomXmlPropsFile);

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						pReader->Skip(4); // len
						pCustomXmlProps->m_oItemID = pReader->GetString2();
					}break;
					case 1:
					{
						pCustomXmlProps->m_oShemaRefs.Init();
						pReader->Skip(4); // len
						ULONG _c = pReader->GetULong();

						for (ULONG i = 0; i < _c; ++i)
						{
							pReader->Skip(1); // type
							pReader->Skip(4); // len
							CCustomXMLProps::CShemaRef *pItem = new CCustomXMLProps::CShemaRef();
							pItem->fromPPTY(pReader);
							
							pCustomXmlProps->m_oShemaRefs->m_arrItems.push_back(pItem);
						}
					}break;
					case 2:
					{
						pReader->Skip(4); // len
						m_sXmlA = pReader->GetString2A();
					}break;
				}
			}

			pReader->Seek(_end_rec);
		}


		std::string m_sXmlA;

		bool m_bUsed = false;
		bool m_bDocument = false;
	};

} // namespace OOX
