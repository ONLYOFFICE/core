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

#include "DocxFlat.h"
#include "App.h"
#include "Core.h"

#include "HeaderFooter.h"
#include "Document.h"
#include "FontTable.h"
#include "Numbering.h"
#include "Styles.h"
#include "Comments.h"
#include "Footnote.h"
#include "Endnote.h"
#include "Settings/Settings.h"

namespace OOX
{
	CDocxFlat::CDocxFlat() : File(dynamic_cast<Document*>(this))
	{
	}
	CDocxFlat::CDocxFlat(const CPath& oFilePath) : File(dynamic_cast<Document*>(this))
	{
		read( oFilePath );
	}
	CDocxFlat::~CDocxFlat()
	{
	}
	void CDocxFlat::read(const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		fromXML(oReader);
	}
	void CDocxFlat::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml = toXML();

		NSFile::CFileBinary file;
		file.CreateFileW(oFilePath.GetPath());
		file.WriteStringUTF8(sXml);
		file.CloseFile();
	}
	const OOX::FileType CDocxFlat::type() const
	{
		return FileTypes::DocumentFlat;
	}
	const CPath CDocxFlat::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CDocxFlat::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CDocxFlat::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	void CDocxFlat::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_Read_if	( oReader, L"xml:space", m_oSpace )
		WritingElement_ReadAttributes_End( oReader )
	}
	void CDocxFlat::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		m_pComments = new OOX::CComments(this);
		m_pFootnotes = new OOX::CFootnotes(this);
		m_pEndnotes = new OOX::CEndnotes(this);

		int nDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode(nDepth) )
		{
			std::wstring sName = oReader.GetName();

			if ( L"w:body" == sName )
			{
				m_pDocument = new CDocument(dynamic_cast<Document*>(this));
				m_currentContainer = dynamic_cast<OOX::IFileContainer*>(m_pDocument.GetPointer());
				m_pDocument->fromXML(oReader);
			}
			else if ( L"w:fonts" == sName )
				m_pFontTable = oReader;
			else if (L"w:lists" == sName)
			{
				m_pNumbering = new CNumbering(dynamic_cast<Document*>(this));
				m_pNumbering->fromXML(oReader);
			}
			else if ( L"w:styles" == sName )
				m_pStyles = oReader;
			else if (L"w:docPr" == sName)
			{
				m_pSettings = new CSettings(dynamic_cast<Document*>(this));
				m_pSettings->fromXML(oReader);
			}
			else if (L"w:bgPict" == sName)
			{
				m_pBgPict = new Logic::CBgPict(dynamic_cast<Document*>(this));
				m_pBgPict->fromXML(oReader);
			}
			else if (L"o:DocumentProperties" == sName)
			{
				ReadDocumentProperties(oReader);
			}
		}

		if (false == m_pStyles->m_oDocDefaults.IsInit())
			m_pStyles->m_oDocDefaults.Init();

		if (false == m_pStyles->m_oDocDefaults->m_oParPr.IsInit())
			m_pStyles->m_oDocDefaults->m_oParPr.Init();

		if ((m_pFontTable.IsInit() && m_pStyles.IsInit()) && (m_pFontTable->m_oDefaultFonts.IsInit()))
		{
			if (false == m_pStyles->m_oDocDefaults->m_oRunPr.IsInit())
				m_pStyles->m_oDocDefaults->m_oRunPr.Init();

			if (!m_pStyles->m_oDocDefaults->m_oRunPr->m_oRFonts.IsInit())
				m_pStyles->m_oDocDefaults->m_oRunPr->m_oRFonts = m_pFontTable->m_oDefaultFonts;
		}
		if (m_pStyles.IsInit())
		{
			NSCommon::smart_ptr<OOX::File> file = NSCommon::smart_ptr<OOX::File>(m_pStyles.GetPointer()); file.AddRef();
			m_pDocument->Add(file);
		}
		if (m_pFontTable.IsInit())
		{
			NSCommon::smart_ptr<OOX::File> file = NSCommon::smart_ptr<OOX::File>(m_pFontTable.GetPointer()); file.AddRef();
			m_pDocument->Add(file);
		}
		if (m_pNumbering.IsInit())
		{
			NSCommon::smart_ptr<OOX::File> file = NSCommon::smart_ptr<OOX::File>(m_pNumbering.GetPointer()); file.AddRef();
			m_pDocument->Add(file);
		}
		if (m_pSettings.IsInit())
		{
			NSCommon::smart_ptr<OOX::File> file = NSCommon::smart_ptr<OOX::File>(m_pSettings.GetPointer()); file.AddRef();
			m_pDocument->Add(file);
		}
		if (m_pComments.IsInit())
		{
			NSCommon::smart_ptr<OOX::File> file = NSCommon::smart_ptr<OOX::File>(m_pComments.GetPointer()); file.AddRef();
			m_pDocument->Add(file);
		}
		if (m_pFootnotes.IsInit())
		{
			NSCommon::smart_ptr<OOX::File> file = NSCommon::smart_ptr<OOX::File>(m_pFootnotes.GetPointer()); file.AddRef();
			m_pDocument->Add(file);
		}
		if (m_pEndnotes.IsInit())
		{
			NSCommon::smart_ptr<OOX::File> file = NSCommon::smart_ptr<OOX::File>(m_pEndnotes.GetPointer()); file.AddRef();
			m_pDocument->Add(file);
		}
	}
	std::wstring CDocxFlat::toXML() const
	{
		std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";

		return sXml;
	}
	EElementType CDocxFlat::getType() const
	{
		return et_w_wordDocument;
	}
	void CDocxFlat::ReadDocumentProperties(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		
		m_pApp = new OOX::CApp(this);
		m_pCore = new OOX::CCore(this);

		int nDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nDepth))
		{
			std::wstring sName = oReader.GetNameNoNS();

			if (L"Category" == sName)
				m_pCore->m_sCategory = oReader.GetText2();
			else if (L"ContentStatus" == sName)
				m_pCore->m_sContentStatus = oReader.GetText2();
			else if (L"Created" == sName)
				m_pCore->m_sCreated = oReader.GetText2();
			else if (L"Author" == sName)
				m_pCore->m_sCreator = oReader.GetText2();
			else if (L"Description" == sName)
				m_pCore->m_sDescription = oReader.GetText2();
			else if (L"Identifier" == sName)
				m_pCore->m_sIdentifier = oReader.GetText2();
			else if (L"Keywords" == sName)
				m_pCore->m_sKeywords = oReader.GetText2();
			else if (L"Language" == sName)
				m_pCore->m_sLanguage = oReader.GetText2();
			else if (L"LastAuthor" == sName)
				m_pCore->m_sLastModifiedBy = oReader.GetText2();
			else if (L"lastPrinted" == sName)
				m_pCore->m_sLastPrinted = oReader.GetText2();
			else if (L"LastSaved" == sName)
				m_pCore->m_sModified = oReader.GetText2();
			else if (L"Revision" == sName)
				m_pCore->m_sRevision = oReader.GetText2();
			else if (L"Subject" == sName)
				m_pCore->m_sSubject = oReader.GetText2();
			else if (L"Title" == sName)
				m_pCore->m_sTitle = oReader.GetText2();
			else if (L"Version" == sName)
				m_pCore->m_sVersion = oReader.GetText2();
			else if (L"Application" == sName)
				m_pApp->m_sApplication = oReader.GetText2();
			else if (L"AppVersion" == sName)
				m_pApp->m_sAppVersion = oReader.GetText2();
			else if (L"Characters" == sName)
				m_pApp->m_nCharacters = oReader.GetText2();
			else if (L"CharactersWithSpaces" == sName)
				m_pApp->m_nCharactersWithSpaces = oReader.GetText2();
			else if (L"Company" == sName)
				m_pApp->m_sCompany = oReader.GetText2();
			else if (L"DocSecurity" == sName)
				m_pApp->m_nDocSecurity = oReader.GetText2();
			else if (L"HiddenSlides" == sName)
				m_pApp->m_nHiddenSlides = oReader.GetText2();
			else if (L"HyperlinkBase" == sName)
				m_pApp->m_sHyperlinkBase = oReader.GetText2();
			else if (L"HyperlinksChanged" == sName)
				m_pApp->m_bHyperlinksChanged = oReader.GetText2();
			else if (L"Lines" == sName)
				m_pApp->m_nLines = oReader.GetText2();
			else if (L"LinksUpToDate" == sName)
				m_pApp->m_bLinksUpToDate = oReader.GetText2();
			else if (L"Manager" == sName)
				m_pApp->m_sManager = oReader.GetText2();
			else if (L"MMClips" == sName)
				m_pApp->m_nMMClips = oReader.GetText2();
			else if (L"Notes" == sName)
				m_pApp->m_nNotes = oReader.GetText2();
			else if (L"Pages" == sName)
				m_pApp->m_nPages = oReader.GetText2();
			else if (L"Paragraphs" == sName)
				m_pApp->m_nParagraphs = oReader.GetText2();
			else if (L"ScaleCrop" == sName)
				m_pApp->m_bScaleCrop = oReader.GetText2();
			else if (L"SharedDoc" == sName)
				m_pApp->m_bSharedDoc = oReader.GetText2();
			else if (L"Slides" == sName)
				m_pApp->m_nSlides = oReader.GetText2();
			else if (L"Template" == sName)
				m_pApp->m_sTemplate = oReader.GetText2();
			else if (L"TotalTime" == sName)
				m_pApp->m_nTotalTime = oReader.GetText2();
			else if (L"Words" == sName)
				m_pApp->m_nWords = oReader.GetText2();
		}
	}
	OOX::CHdrFtr *CDocxFlat::GetHeaderOrFooter(const OOX::RId& rId) const
	{
		OOX::IFileContainer* pDocumentContainer = (OOX::IFileContainer*)m_pDocument.GetPointer();

		smart_ptr<OOX::File> pFile = pDocumentContainer->Find(rId);
		if (pFile.IsInit() && (OOX::FileTypes::Header == pFile->type() || OOX::FileTypes::Footer == pFile->type()))
			return (OOX::CHdrFtr*)pFile.GetPointer();
		else
			return NULL;
	}

}
