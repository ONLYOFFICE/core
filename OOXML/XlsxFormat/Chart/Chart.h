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

#include "ChartSerializeEx.h"
#include "../../DocxFormat/FileTypes.h"
#include "../../DocxFormat/Document.h"
#include "../Xlsx.h"

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

