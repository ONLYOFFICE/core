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

#include "ChartSerializeEx.h"
#include "../../DocxFormat/FileTypes.h"
#include "../../DocxFormat/Document.h"
#include "../Xlsx.h"

#include "ChartStyle.h"
#include "ChartColors.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CChartFile: public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CChartFile(OOX::Document* pMain, bool bDocument = true);
			CChartFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CChartFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual void write2(const std::wstring& sFilename) const;

			void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			CT_ChartSpace m_oChartSpace;

		private:
			CPath m_oReadPath;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			bool m_bDocument = true; //for upper/lower level rels (defaultDirectory)
		};

		class CChartExFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CChartExFile(OOX::Document* pMain, bool bDocument = true);
			CChartExFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CChartExFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual void write2(const std::wstring& sFilename) const;

			void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			ChartEx::CChartSpace m_oChartSpace;

		private:
			CPath m_oReadPath;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			bool m_bDocument = true; //for upper/lower level rels (defaultDirectory)
		};

		class CChartStyleFile : public OOX::FileGlobalEnumerated/*, public OOX::IFileContainer*/
		{
		public:
			CChartStyleFile(OOX::Document* pMain);
			CChartStyleFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CChartStyleFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual void write2(const std::wstring& sFilename) const;

			void toXML(NSStringUtils::CStringBuilder& writer) const;
			bool isValid() const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			ChartEx::CChartStyle m_oChartStyle;

		private:
			CPath m_oReadPath;
		};

		class CChartColorsFile: public OOX::FileGlobalEnumerated/*, public OOX::IFileContainer*/
		{
		public:
			CChartColorsFile(OOX::Document* pMain);
			CChartColorsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);

			virtual ~CChartColorsFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual void write2(const std::wstring& sFilename) const;

			void toXML(NSStringUtils::CStringBuilder& writer) const;
			bool isValid() const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			ChartEx::CColorStyle m_oColorStyle;

		private:
			CPath m_oReadPath;

		};
	} //Spreadsheet
} // namespace OOX

