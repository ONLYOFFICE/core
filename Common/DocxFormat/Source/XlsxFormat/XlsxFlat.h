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

#include "../DocxFormat/IFileContainer.h"
#include "FileTypes_Spreadsheet.h"
#include "WritingElement.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CWorksheet;
		class CWorkbook;
		class CSharedStrings;
		class CStyles;

		class CXlsxFlat : public OOX::Document, public OOX::File, public WritingElement
		{
		public:

			CXlsxFlat();
			CXlsxFlat(const CPath& oFilePath);
			virtual ~CXlsxFlat();

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
				return OOX::Spreadsheet::FileTypes::SpreadsheetFlat;
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const
			{
				std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";

				return sXml;
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			virtual EElementType getType() const
			{
				return et_x_SpreadsheetFlat;
			}
	//-----------------------------------------------------------------------

			nullable<CWorkbook>			m_pWorkbook;
			std::vector<CWorksheet*>	m_arWorksheets;
			nullable<CStyles>			m_pStyles;
            nullable<CSharedStrings>    m_pSharedStrings;
		
			int	m_nLastReadRow;
			int	m_nLastReadCol;
		};

	}
}
