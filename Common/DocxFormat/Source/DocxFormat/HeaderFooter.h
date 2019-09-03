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
#ifndef OOX_HEADER_FOOTER_INCLUDE_H_
#define OOX_HEADER_FOOTER_INCLUDE_H_

#include "File.h"
#include "../Base/Nullable.h"

#include "WritingElement.h"

#include "Logic/Annotations.h"
#include "Logic/Paragraph.h"
#include "Logic/Sdt.h"
#include "Logic/Table.h"
#include "Math/oMathPara.h"
#include "Math/OMath.h"

// TO DO: Нехватающие классы:
//        <w:customXml>
//        <w:moveFrom>
//        <w:moveTo>

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CHdrFtr 17.10.3 (Part 1)
	//--------------------------------------------------------------------------------	
	class CHdrFtr : public OOX::File, public IFileContainer
	{
	public:
		CHdrFtr(OOX::Document *pMain) : OOX::File(pMain), IFileContainer(pMain)
		{
			m_eType = et_Unknown;
		}
		CHdrFtr(OOX::Document *pMain, const CPath& oRootPath, const CPath& oFilePath) : OOX::File(pMain), IFileContainer(pMain)
		{
			m_eType = et_Unknown;
			read( oRootPath, oFilePath );
		}
		virtual ~CHdrFtr()
		{
            for ( size_t i = 0; i < m_arrItems.size(); ++i )
			{
                if ( m_arrItems[i] ) delete m_arrItems[i];
			}

			m_arrItems.clear();
		}

		virtual void read(const CPath& oFilePath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oFilePath);
		}
		virtual void read(const CPath& oRootPath, const CPath& oFilePath)
		{
			m_oReadPath = oFilePath;
			IFileContainer::Read( oRootPath, oFilePath );

			XmlUtils::CXmlLiteReader oReader;
			
			if ( !oReader.FromFile( oFilePath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			std::wstring sName = oReader.GetName();

			if ( _T("w:ftr") == sName )
				m_eType = et_w_ftr;
			else if ( _T("w:hdr") == sName )
				m_eType = et_w_hdr;
			else
				return;

			if ( !oReader.IsEmptyNode() )
			{
				int nDocumentDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nDocumentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					WritingElement *pItem = NULL;

					if ( _T("w:altChunk") == sName )
						pItem = new Logic::CAltChunk( oReader );
					else if ( _T("w:bookmarkEnd") == sName )
						pItem = new Logic::CBookmarkEnd( oReader );
					else if ( _T("w:bookmarkStart") == sName )
						pItem = new Logic::CBookmarkStart( oReader );
					else if ( _T("w:commentRangeEnd") == sName )
						pItem = new Logic::CCommentRangeEnd( oReader );
					else if ( _T("w:commentRangeStart") == sName )
						pItem = new Logic::CCommentRangeStart( oReader );
					//else if ( _T("w:customXml") == sName )
					//	pItem = new Logic::CCustomXml( oReader );
					else if ( _T("w:customXmlDelRangeEnd") == sName )
						pItem = new Logic::CCustomXmlDelRangeEnd( oReader );
					else if ( _T("w:customXmlDelRangeStart") == sName )
						pItem = new Logic::CCustomXmlDelRangeStart( oReader );
					else if ( _T("w:customXmlInsRangeEnd") == sName )
						pItem = new Logic::CCustomXmlInsRangeEnd( oReader );
					else if ( _T("w:customXmlInsRangeStart") == sName )
						pItem = new Logic::CCustomXmlInsRangeStart( oReader );
					else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
						pItem = new Logic::CCustomXmlMoveFromRangeEnd( oReader );
					else if ( _T("w:customXmlMoveFromRangeStart") == sName )
						pItem = new Logic::CCustomXmlMoveFromRangeStart( oReader );
					else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
						pItem = new Logic::CCustomXmlMoveToRangeEnd( oReader );
					else if ( _T("w:customXmlMoveToRangeStart") == sName )
						pItem = new Logic::CCustomXmlMoveToRangeStart( oReader );
					else if ( _T("w:del") == sName )
						pItem = new Logic::CDel( oReader );
					else if ( _T("w:ins") == sName )
						pItem = new Logic::CIns( oReader );
					else if ( _T("w:moveFrom") == sName )
						pItem = new Logic::CMoveFrom( oReader );
					else if ( _T("w:moveFromRangeEnd") == sName )
						pItem = new Logic::CMoveFromRangeEnd( oReader );
					else if ( _T("w:moveFromRangeStart") == sName )
						pItem = new Logic::CMoveFromRangeStart( oReader );
					else if ( _T("w:moveTo") == sName )
						pItem = new Logic::CMoveTo( oReader );
					else if ( _T("w:moveToRangeEnd") == sName )
						pItem = new Logic::CMoveToRangeEnd( oReader );
					else if ( _T("w:moveToRangeStart") == sName )
						pItem = new Logic::CMoveToRangeStart( oReader );
					else if ( _T("m:oMath") == sName )
						pItem = new Logic::COMath( oReader );
					else if ( _T("m:oMathPara") == sName )
						pItem = new Logic::COMathPara( oReader );
					else if ( _T("w:p") == sName )
						pItem = new Logic::CParagraph( oReader );
					else if ( _T("w:permEnd") == sName )
						pItem = new Logic::CPermEnd( oReader );
					else if ( _T("w:permStart") == sName )
						pItem = new Logic::CPermStart( oReader );
					else if ( _T("w:proofErr") == sName )
						pItem = new Logic::CProofErr( oReader );
					else if ( _T("w:sdt") == sName )
						pItem = new Logic::CSdt( oReader );
					else if ( _T("w:tbl") == sName )
						pItem = new Logic::CTbl( oReader );

					if ( pItem )
						m_arrItems.push_back( pItem );
				}
			}
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			std::wstring sXml;
			
			if ( et_w_ftr == m_eType )
				sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<w:ftr xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
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
				sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<w:hdr xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
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
				return;

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

			CDirectory::SaveToFile( oFilePath.GetPath(), sXml );

			oContent.Registration( type().OverrideType(), oDirectory, oFilePath );
			IFileContainer::Write( oFilePath, oDirectory, oContent );
		}

	public:
		virtual const OOX::FileType type() const
		{
			if ( et_w_hdr == m_eType )
				return FileTypes::Header;
			else if ( et_w_ftr == m_eType )
				return FileTypes::Footer;

			return FileTypes::Unknow;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& GetReadPath()
		{
			return m_oReadPath;
		}

	public:

		void AddParagraph(Logic::CParagraph *pPara)
		{
			m_arrItems.push_back( (WritingElement*)pPara );
		}
	public:
		CPath							m_oReadPath;
		OOX::EElementType				m_eType;

        std::vector<WritingElement* >	m_arrItems;
	};

} // namespace OOX

#endif // OOX_HEADER_FOOTER_INCLUDE_H_
