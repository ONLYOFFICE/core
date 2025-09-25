/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "Core.h"
#include "Docx.h"
#include "../XlsxFormat/Xlsx.h"

#include "../../DesktopEditor/common/SystemUtils.h"

namespace OOX
{
	CCore::CCore(OOX::Document* pMain) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCore = this;
		OOX::Spreadsheet::CXlsx* xlsx = dynamic_cast<OOX::Spreadsheet::CXlsx*>(File::m_pMainDocument);
		if (xlsx) xlsx->m_pCore = this;
	}
	CCore::CCore(OOX::Document* pMain, const CPath& oPath) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCore = this;
		OOX::Spreadsheet::CXlsx* xlsx = dynamic_cast<OOX::Spreadsheet::CXlsx*>(File::m_pMainDocument);
		if (xlsx) xlsx->m_pCore = this;

		read( oPath );
	}

	CCore::~CCore()
	{
	}

	void CCore::read(const CPath& oPath)
	{
		XmlUtils::CXmlNode oProperties;
		oProperties.FromXmlFile( oPath.GetPath(), true );

		if ( _T("cp:coreProperties") == oProperties.GetName() )
		{
			XmlUtils::CXmlNode oItem;

			if ( oProperties.GetNode( _T("cp:category"), oItem ) )
				m_sCategory = oItem.GetText();

			if ( oProperties.GetNode( _T("cp:contentStatus"), oItem ) )
				m_sContentStatus = oItem.GetText();

			if ( oProperties.GetNode( _T("dcterms:created"), oItem ) )
				m_sCreated = oItem.GetText();

			if ( oProperties.GetNode( _T("dc:creator"), oItem ) )
				m_sCreator = oItem.GetText();

			if ( oProperties.GetNode( _T("dc:description"), oItem ) )
				m_sDescription = oItem.GetText();

			if ( oProperties.GetNode( _T("dc:identifier"), oItem ) )
				m_sIdentifier = oItem.GetText();

			if ( oProperties.GetNode( _T("cp:keywords"), oItem ) )
				m_sKeywords = oItem.GetText();

			if ( oProperties.GetNode( _T("dc:language"), oItem ) )
				m_sLanguage = oItem.GetText();

			if ( oProperties.GetNode( _T("cp:lastModifiedBy"), oItem ) )
				m_sLastModifiedBy = oItem.GetText();

			if ( oProperties.GetNode( _T("cp:lastPrinted"), oItem ) )
				m_sLastPrinted = oItem.GetText();

			if ( oProperties.GetNode( _T("dcterms:modified"), oItem ) )
				m_sModified = oItem.GetText();

			if ( oProperties.GetNode( _T("cp:revision"), oItem ) )
				m_sRevision = oItem.GetText();

			if ( oProperties.GetNode( _T("dc:subject"), oItem ) )
				m_sSubject = oItem.GetText();

			if ( oProperties.GetNode( _T("dc:title"), oItem ) )
				m_sTitle = oItem.GetText();

			if ( oProperties.GetNode( _T("cp:version"), oItem ) )
				m_sVersion = oItem.GetText();
		}
	}
	void CCore::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml;
		sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<cp:coreProperties \
xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" \
xmlns:dc=\"http://purl.org/dc/elements/1.1/\" \
xmlns:dcterms=\"http://purl.org/dc/terms/\" \
xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" \
xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">");

		if ( m_sTitle.IsInit() )
		{
			sXml += _T("<dc:title>");
			sXml += XmlUtils::EncodeXmlString(m_sTitle.get());
			sXml += _T("</dc:title>");
		}
		if ( m_sSubject.IsInit() )
		{
			sXml += _T("<dc:subject>");
			sXml += XmlUtils::EncodeXmlString(m_sSubject.get());
			sXml += _T("</dc:subject>");
		}
		if ( m_sCreator.IsInit() )
		{
			sXml += _T("<dc:creator>");
			sXml += XmlUtils::EncodeXmlString(m_sCreator.get());
			sXml += _T("</dc:creator>");
		}
		if ( m_sKeywords.IsInit() )
		{
			sXml += _T("<cp:keywords>");
			sXml += XmlUtils::EncodeXmlString(m_sKeywords.get());
			sXml += _T("</cp:keywords>");
		}
		if ( m_sDescription.IsInit() )
		{
			sXml += _T("<dc:description>");
			sXml += XmlUtils::EncodeXmlString(m_sDescription.get());
			sXml += _T("</dc:description>");
		}
		if ( m_sIdentifier.IsInit() )
		{
			sXml += _T("<dc:identifier>");
			sXml += XmlUtils::EncodeXmlString(m_sIdentifier.get());
			sXml += _T("</dc:identifier>");
		}
		if ( m_sLanguage.IsInit() )
		{
			sXml += _T("<dc:language>");
			sXml += XmlUtils::EncodeXmlString(m_sLanguage.get());
			sXml += _T("</dc:language>");
		}
		if ( m_sLastModifiedBy.IsInit() )
		{
			sXml += _T("<cp:lastModifiedBy>");
			sXml += XmlUtils::EncodeXmlString(m_sLastModifiedBy.get());
			sXml += _T("</cp:lastModifiedBy>");
		}
		if ( m_sRevision.IsInit() )
		{
			sXml += _T("<cp:revision>");
			sXml += XmlUtils::EncodeXmlString(m_sRevision.get());
			sXml += _T("</cp:revision>");
		}
		if ( (m_sLastPrinted.IsInit()) && (!m_sLastPrinted->empty()))
		{
			sXml += _T("<cp:lastPrinted>");
			sXml += XmlUtils::EncodeXmlString(m_sLastPrinted.get());
			sXml += _T("</cp:lastPrinted>");
		}
		if ( (m_sCreated.IsInit()) && (!m_sCreated->empty()))
		{
			sXml += _T("<dcterms:created xsi:type=\"dcterms:W3CDTF\">");
			sXml += XmlUtils::EncodeXmlString(m_sCreated.get());
			sXml += _T("</dcterms:created>");
		}
		if ( (m_sModified.IsInit()) && (!m_sModified->empty()))
		{
			sXml += _T("<dcterms:modified xsi:type=\"dcterms:W3CDTF\">");
			sXml += XmlUtils::EncodeXmlString(m_sModified.get());
			sXml += _T("</dcterms:modified>");
		}
		if ( m_sCategory.IsInit() )
		{
			sXml += _T("<cp:category>");
			sXml += XmlUtils::EncodeXmlString(m_sCategory.get());
			sXml += _T("</cp:category>");
		}
		if ( m_sContentStatus.IsInit() )
		{
			sXml += _T("<cp:contentStatus>");
			sXml += XmlUtils::EncodeXmlString(m_sContentStatus.get());
			sXml += _T("</cp:contentStatus>");
		}
		if ( m_sVersion.IsInit() )
		{
			sXml += _T("<cp:version>");
			sXml += XmlUtils::EncodeXmlString(m_sVersion.get());
			sXml += _T("</cp:version>");
		}
		sXml += _T("</cp:coreProperties>");

		NSFile::CFileBinary::SaveToFile( oPath.GetPath(), sXml );
		oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
	}

	const FileType CCore::type() const
	{
		return FileTypes::Core;
	}

	const CPath CCore::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CCore::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CCore::SetDefaults()
	{
		//			m_sCreator = L"";
		m_sLastModifiedBy = L"";
	}
	void CCore::SetRequiredDefaults()
	{
		if (m_sCreator.IsInit())
		{
			std::wstring sCreator = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvCreator);
			if (!sCreator.empty())
				m_sCreator = sCreator;
		}
		if (m_sCreated.IsInit())
		{
			std::wstring sCreated = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvCreated);
			if (!sCreated.empty())
				m_sCreated = sCreated;
		}
		std::wstring sLastModifiedBy = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvLastModifiedBy);
		if (!sLastModifiedBy.empty())
			m_sLastModifiedBy = sLastModifiedBy;

		std::wstring sModified = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvModified);
		if (!sModified.empty())
			m_sModified = sModified;
	}
	void CCore::SetCreator(std::wstring sVal)
	{
		m_sCreator = sVal;
	}
	void CCore::SetLastModifiedBy(std::wstring sVal)
	{
		m_sLastModifiedBy = sVal;
	}
	void CCore::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->StartRecord(NSBinPptxRW::NSMainTables::Core);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

		pWriter->WriteString2(0, m_sTitle);
		pWriter->WriteString2(1, m_sCreator);
		pWriter->WriteString2(2, m_sLastModifiedBy);
		pWriter->WriteString2(3, m_sRevision);
		pWriter->WriteString2(4, m_sCreated);
		pWriter->WriteString2(5, m_sModified);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		//start new record because new attributes is incompatible with previous versions
		pWriter->StartRecord(0);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

		pWriter->WriteString2(6, m_sCategory);
		pWriter->WriteString2(7, m_sContentStatus);
		pWriter->WriteString2(8, m_sDescription);
		pWriter->WriteString2(9, m_sIdentifier);
		pWriter->WriteString2(10, m_sKeywords);
		pWriter->WriteString2(11, m_sLanguage);
		pWriter->WriteString2(12, m_sLastPrinted);
		pWriter->WriteString2(13, m_sSubject);
		pWriter->WriteString2(14, m_sVersion);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		pWriter->EndRecord();

		pWriter->EndRecord();
	}
	void CCore::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		pReader->Skip(1); // type
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // start attributes

		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			switch (_at)
			{
			case 0: m_sTitle = pReader->GetString2(); break;
			case 1: m_sCreator = pReader->GetString2(); break;
			case 2: m_sLastModifiedBy = pReader->GetString2(); break;
			case 3: m_sRevision = pReader->GetString2(); break;
			case 4: m_sCreated = pReader->GetString2(); break;
			case 5: m_sModified = pReader->GetString2(); break;
			default: break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _at = pReader->GetUChar();
			switch (_at)
			{
			case 0:
			{
				LONG _end_rec2 = pReader->GetPos() + pReader->GetRecordSize() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
					case 6: m_sCategory = pReader->GetString2(); break;
					case 7: m_sContentStatus = pReader->GetString2(); break;
					case 8: m_sDescription = pReader->GetString2(); break;
					case 9: m_sIdentifier = pReader->GetString2(); break;
					case 10: m_sKeywords = pReader->GetString2(); break;
					case 11: m_sLanguage = pReader->GetString2(); break;
					case 12: m_sLastPrinted = pReader->GetString2(); break;
					case 13: m_sSubject = pReader->GetString2(); break;
					case 14: m_sVersion = pReader->GetString2(); break;
					}
				}

				pReader->Seek(_end_rec2);
			}
			break;
			default:
			{
				pReader->SkipRecord();
				break;
			}
			}
		}

		pReader->Seek(_end_rec);
	}
	void CCore::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(_T("cp:coreProperties"));

		pWriter->StartAttributes();

		pWriter->WriteAttribute(_T("xmlns:cp"), PPTX::g_Namespaces.cp.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:dc"), PPTX::g_Namespaces.dc.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:dcterms"), PPTX::g_Namespaces.dcterms.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:dcmitype"), PPTX::g_Namespaces.dcmitype.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:xsi"), PPTX::g_Namespaces.xsi.m_strLink);

		pWriter->EndAttributes();

		pWriter->WriteNodeValue2(_T("dc:title"), m_sTitle);
		pWriter->WriteNodeValue2(_T("dc:subject"), m_sSubject);
		pWriter->WriteNodeValue2(_T("dc:creator"), m_sCreator);
		pWriter->WriteNodeValue2(_T("cp:keywords"), m_sKeywords);
		pWriter->WriteNodeValue2(_T("dc:description"), m_sDescription);
		pWriter->WriteNodeValue2(_T("dc:identifier"), m_sIdentifier);
		pWriter->WriteNodeValue2(_T("dc:language"), m_sLanguage);
		pWriter->WriteNodeValue2(_T("cp:lastModifiedBy"), m_sLastModifiedBy);
		pWriter->WriteNodeValue2(_T("cp:revision"), m_sRevision);

		if ((m_sLastPrinted.IsInit()) && (false == m_sLastPrinted->empty()))
		{
			pWriter->WriteNodeValue2(_T("cp:lastPrinted"), m_sLastPrinted);
		}
		if ((m_sCreated.IsInit()) && (false == m_sCreated->empty()))
		{
			pWriter->WriteNodeBegin(_T("dcterms:created xsi:type=\"dcterms:W3CDTF\""));
			pWriter->WriteStringXML(*m_sCreated);
			pWriter->WriteNodeEnd(_T("dcterms:created"));
		}
		if ((m_sModified.IsInit()) && (false == m_sModified->empty()))
		{
			pWriter->WriteNodeBegin(_T("dcterms:modified xsi:type=\"dcterms:W3CDTF\""));
			pWriter->WriteStringXML(*m_sModified);
			pWriter->WriteNodeEnd(_T("dcterms:modified"));
		}
		pWriter->WriteNodeValue2(_T("cp:category"), m_sCategory);
		pWriter->WriteNodeValue2(_T("cp:contentStatus"), m_sContentStatus);
		pWriter->WriteNodeValue2(_T("cp:version"), m_sVersion);

		pWriter->EndNode(_T("cp:coreProperties"));
	}
} // namespace OOX
