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

#include "Document.h"
#include "FontTable.h"
#include "Numbering.h"
#include "Styles.h"
#include "Comments.h"

namespace OOX
{
	class CDocxFlat : public Document, public File, public WritingElement
	{
	public:

		CDocxFlat() : File(dynamic_cast<Document*>(this)), m_oComments(dynamic_cast<Document*>(this))
		{
		}
		CDocxFlat(const CPath& oFilePath) : File(this), m_oComments(dynamic_cast<Document*>(this))
		{
			read( oFilePath );
		}

		virtual void read(const CPath& oFilePath)
		{
			XmlUtils::CXmlLiteReader oReader;
			
			if ( !oReader.FromFile( oFilePath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			fromXML(oReader);
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			std::wstring sXml = toXML();
			
			NSFile::CFileBinary file;
			file.CreateFileW(oFilePath.GetPath());
			file.WriteStringUTF8(sXml);
			file.CloseFile();
		}
		virtual const OOX::FileType type() const
		{
			return FileTypes::DocumentFlat;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nStylesDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if ( L"w:body" == sName )
				{
					m_pDocument = new CDocument(dynamic_cast<Document*>(this));
					m_pDocument->fromXML(oReader);
				}
				else if ( L"w:fonts" == sName )
					m_pFontTable = oReader;
				else if ( L"w:lists" == sName )
					m_pNumbering = oReader;
				else if ( L"w:styles" == sName )
					m_pStyles = oReader;
				else if ( L"w:bgPict" == sName )
				{
					//m_oBgPict = oReader;
				}
			}
		}
        virtual std::wstring toXML() const
		{
			std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";

			return sXml;
		}
		virtual EElementType getType() const
		{
			return et_w_wordDocument;
		}
//-----------------------------------------------------------------------

		nullable<CDocument>				m_pDocument;
		CComments						m_oComments;
		nullable<CStyles>				m_pStyles;
		nullable<CFontTable>			m_pFontTable;
		nullable<CNumbering>			m_pNumbering;
		//nullable<Logic::CBgPict>		m_oBgPict;
	
	};


}
