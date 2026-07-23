/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../DocxFormat/IFileContainer.h"
#include "FileTypes_Spreadsheet.h"
#include "WritingElement.h"
#include "../../DesktopEditor/common/File.h"

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

			virtual void read(const CPath& oFilePath);
			virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			virtual EElementType getType() const;

	//-----------------------------------------------------------------------

			nullable<CWorkbook>			m_pWorkbook;
			std::vector<CWorksheet*>	m_arWorksheets;
			nullable<CStyles>			m_pStyles;
            nullable<CSharedStrings>    m_pSharedStrings;
		
			int	m_nLastReadRow;
			int	m_nLastReadCol;
			std::pair<double, double> getMaxDigitSize();

			std::wstring m_strFontDirectory;

		private:
			std::pair<double, double> m_maxDigitSize;
			void ReadSettingAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

	}
}
