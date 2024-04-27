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

#include "HeaderFooter.h"

#include "Logic/Annotations.h"
#include "Logic/Paragraph.h"
#include "Logic/Hyperlink.h"
#include "Logic/Sdt.h"
#include "Logic/Table.h"
#include "Math/oMathPara.h"
#include "Math/OMath.h"

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CHdrFtr 17.10.3 (Part 1)
	//--------------------------------------------------------------------------------

	CHdrFtr::CHdrFtr(OOX::Document *pMain) : OOX::File(pMain), IFileContainer(pMain), WritingElement(pMain)
	{
		m_eType = et_Unknown;
	}
	CHdrFtr::CHdrFtr(OOX::Document *pMain, const CPath& oRootPath, const CPath& oFilePath) : OOX::File(pMain), IFileContainer(pMain), WritingElement(pMain)
	{
		m_eType = et_Unknown;
		read( oRootPath, oFilePath );
	}
	CHdrFtr::~CHdrFtr()
	{
		for ( size_t i = 0; i < m_arrItems.size(); ++i )
		{
			if ( m_arrItems[i] ) delete m_arrItems[i];
		}

		m_arrItems.clear();
	}

	void CHdrFtr::read(const CPath& oFilePath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CHdrFtr::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		m_oReadPath = oFilePath;
		IFileContainer::Read( oRootPath, oFilePath );

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		fromXML(oReader);
	}
	void CHdrFtr::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	void CHdrFtr::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

		if (L"ftr" == sName)
			m_eType = et_w_ftr;
		else if (L"hdr" == sName)
			m_eType = et_w_hdr;
		else
			return;

		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nHdrFtrDepth = oReader.GetDepth();
		CreateElements(oReader, nHdrFtrDepth);
	}
	void CHdrFtr::CreateElements(XmlUtils::CXmlLiteReader& oReader, int Depth)
	{
		OOX::Document* document = WritingElement::m_pMainDocument;

		while (oReader.ReadNextSiblingNode(Depth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			WritingElement* pItem = NULL;

			if (L"altChunk" == sName)
				pItem = new Logic::CAltChunk(document);
			else if (L"bookmarkEnd" == sName)
				pItem = new Logic::CBookmarkEnd(document);
			else if (L"bookmarkStart" == sName)
				pItem = new Logic::CBookmarkStart(document);
			else if (L"commentRangeEnd" == sName)
				pItem = new Logic::CCommentRangeEnd(document);
			else if (L"commentRangeStart" == sName)
				pItem = new Logic::CCommentRangeStart(document);
			//else if ( L"customXml") == sName )
			//	pItem = new Logic::CCustomXml( document );
			else if (L"customXmlDelRangeEnd" == sName)
				pItem = new Logic::CCustomXmlDelRangeEnd(document);
			else if (L"customXmlDelRangeStart" == sName)
				pItem = new Logic::CCustomXmlDelRangeStart(document);
			else if (L"customXmlInsRangeEnd" == sName)
				pItem = new Logic::CCustomXmlInsRangeEnd(document);
			else if (L"customXmlInsRangeStart" == sName)
				pItem = new Logic::CCustomXmlInsRangeStart(document);
			else if (L"customXmlMoveFromRangeEnd" == sName)
				pItem = new Logic::CCustomXmlMoveFromRangeEnd(document);
			else if (L"customXmlMoveFromRangeStart" == sName)
				pItem = new Logic::CCustomXmlMoveFromRangeStart(document);
			else if (L"customXmlMoveToRangeEnd" == sName)
				pItem = new Logic::CCustomXmlMoveToRangeEnd(document);
			else if (L"customXmlMoveToRangeStart" == sName)
				pItem = new Logic::CCustomXmlMoveToRangeStart(document);
			else if (L"del" == sName)
				pItem = new Logic::CDel(document);
			else if (L"ins" == sName)
				pItem = new Logic::CIns(document);
			else if (L"moveFrom" == sName)
				pItem = new Logic::CMoveFrom(document);
			else if (L"moveFromRangeEnd" == sName)
				pItem = new Logic::CMoveFromRangeEnd(document);
			else if (L"moveFromRangeStart" == sName)
				pItem = new Logic::CMoveFromRangeStart(document);
			else if (L"moveTo" == sName)
				pItem = new Logic::CMoveTo(document);
			else if (L"moveToRangeEnd" == sName)
				pItem = new Logic::CMoveToRangeEnd(document);
			else if (L"moveToRangeStart" == sName)
				pItem = new Logic::CMoveToRangeStart(document);
			else if (L"oMath" == sName)
				pItem = new Logic::COMath(document);
			else if (L"oMathPara" == sName)
				pItem = new Logic::COMathPara(document);
			else if (L"p" == sName)
				pItem = new Logic::CParagraph(document, this);
			else if (L"permEnd" == sName)
				pItem = new Logic::CPermEnd(document);
			else if (L"permStart" == sName)
				pItem = new Logic::CPermStart(document);
			else if (L"proofErr" == sName)
				pItem = new Logic::CProofErr(document);
			else if (L"sdt" == sName)
				pItem = new Logic::CSdt(document);
			else if (L"tbl" == sName)
				pItem = new Logic::CTbl(document);
			else if (L"body" == sName && !oReader.IsEmptyNode())
			{
				int nWBodyDepth = oReader.GetDepth();
				CreateElements(oReader, nWBodyDepth);
			}
			else if (L"sect" == sName && !oReader.IsEmptyNode())
			{
				int nWxSectDepth = oReader.GetDepth();
				CreateElements(oReader, nWxSectDepth);
			}
			else if (L"sub-section" == sName && !oReader.IsEmptyNode())
			{
				int nWxSubSectDepth = oReader.GetDepth();
				CreateElements(oReader, nWxSubSectDepth);
			}
			else if (L"pBdrGroup" == sName && !oReader.IsEmptyNode())
			{
				int nWxBdrGroupDepth = oReader.GetDepth();
				CreateElements(oReader, nWxBdrGroupDepth);
			}
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	std::wstring CHdrFtr::toXML() const
	{
		std::wstring sXml;
		if ( et_w_ftr == m_eType )
			sXml = _T("<w:ftr xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
mc:Ignorable=\"w14 w15 wp14\">");

		else if ( et_w_hdr == m_eType )
			sXml = _T("<w:hdr xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
mc:Ignorable=\"w14 w15 wp14\">");

		else
			return sXml;

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if ( m_arrItems[i] )
			{
				sXml += m_arrItems[i]->toXML();
			}
		}

		if ( et_w_ftr == m_eType )
			sXml += _T("</w:ftr>");
		else if ( et_w_hdr == m_eType )
			sXml += _T("</w:hdr>");

		return sXml;
	}
	void CHdrFtr::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
		sXml  += toXML();

		CDirectory::SaveToFile( oFilePath.GetPath(), sXml );

		oContent.Registration( type().OverrideType(), oDirectory, oFilePath.GetFilename() );
		IFileContainer::Write( oFilePath, oDirectory, oContent );
	}
	const OOX::FileType CHdrFtr::type() const
	{
		if ( et_w_hdr == m_eType )
			return FileTypes::Header;
		else if ( et_w_ftr == m_eType )
			return FileTypes::Footer;

		return FileTypes::Unknown;
	}
	const CPath CHdrFtr::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CHdrFtr::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	const CPath& CHdrFtr::GetReadPath()
	{
		return m_oReadPath;
	}
	EElementType CHdrFtr::getType() const
	{
		return m_eType;
	}
	void CHdrFtr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"type", m_oType )
		WritingElement_ReadAttributes_End_No_NS(oReader)
	}

} // namespace OOX
