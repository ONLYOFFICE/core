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

#include "../Base/Nullable.h"

#include "FileTypes_Draw.h"
#include "../DocxFormat/IFileContainer.h"

namespace OOX
{
	namespace Draw
	{
		class CRel;

		class CAutoLinkComparison : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CAutoLinkComparison)
			CAutoLinkComparison() {}
			virtual ~CAutoLinkComparison() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string ColumnName;
			nullable_uint ContextType;
			nullable_string ContextTypeLabel;
		};
		class CRefreshConflict : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRefreshConflict)
			CRefreshConflict() {}
			virtual ~CRefreshConflict() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_uint RowID;
			nullable_uint PageID;
			nullable_uint ShapeID;
		};
		class CRowMap : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRowMap)
			CRowMap() {}
			virtual ~CRowMap() {}

			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_uint RowID;
			nullable_uint PageID;
			nullable_uint ShapeID;
		};
		class CRowKeyValue : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRowKeyValue)
			CRowKeyValue() {}
			virtual ~CRowKeyValue() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint RowID;
			nullable_string Value;
		};
		class CPrimaryKey : public WritingElementWithChilds<CRowKeyValue>
		{
		public:
			WritingElement_AdditionMethods(CPrimaryKey)
			CPrimaryKey() {}
			virtual ~CPrimaryKey() {}

			virtual std::wstring toXML() const { return L""; }
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			nullable_string ColumnNameID;
		};
		class CDataConnection : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDataConnection)
			CDataConnection() {}
			virtual ~CDataConnection() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint ID;
			nullable_string FileName;
			nullable_string ConnectionString;
			nullable_string Command;
			nullable_string FriendlyName;
			nullable_uint Timeout;
			nullable_bool AlwaysUseConnectionFile;
		};
		class CDataConnections : public WritingElementWithChilds<CDataConnection>
		{
		public:
			WritingElement_AdditionMethods(CDataConnections)
			CDataConnections() {}
			virtual ~CDataConnections() {}

			virtual std::wstring toXML() const { return L""; }
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			
			nullable_uint NextID;
		};
		class CDataColumn : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDataColumn)
			CDataColumn() {}
			virtual ~CDataColumn() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_string ColumnNameID;
			nullable_string Name;
			nullable_string Label;
			nullable_string OrigLabel;
			nullable_string LangID;
			nullable_uint Calendar;
			nullable_uint DataType;
			nullable_string UnitType;
			nullable_uint Currency;
			nullable_uint Degree;
			nullable_uint DisplayWidth;
			nullable_uint DisplayOrder;
			nullable_bool Mapped;
			nullable_bool Hyperlink;
		};
		class CDataColumns : public WritingElementWithChilds<CDataColumn>
		{
		public:
			WritingElement_AdditionMethods(CDataColumns)
			CDataColumns() {}
			virtual ~CDataColumns() {}

			virtual std::wstring toXML() const { return L""; }
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			nullable_string SortColumn;
			nullable_bool SortAsc;
		};
		class CDataRecordSet : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CDataRecordSet)
			CDataRecordSet() {}
			virtual ~CDataRecordSet() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint ID;
			nullable_uint ConnectionID;
			nullable_string Command;
			nullable_uint Options;
			nullable_string TimeRefreshed;
			nullable_uint NextRowID;
			nullable_string Name;
			nullable_bool RowOrder;
			nullable_bool RefreshOverwriteAll;
			nullable_bool RefreshNoReconciliationUI;
			nullable_uint RefreshInterval;
			nullable_uint ReplaceLinks;
			nullable_uint Checksum;

			nullable<CDataColumns> DataColumns;
			nullable<CRel> Rel;
		};
		class CDataRecordSets : public WritingElementWithChilds<CDataRecordSet>
		{
		public:
			WritingElement_AdditionMethods(CDataRecordSets)
			CDataRecordSets() {}
			virtual ~CDataRecordSets() {}

			virtual std::wstring toXML() const { return L""; }
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			nullable_uint NextID;
			nullable_uint ActiveRecordsetID;
			nullable_string DataWindowOrder;
		};
//------------------------------------------------------------------------------------------------------------------------------
		class CConnectionsFile : public OOX::File
		{
		public:
			CConnectionsFile(OOX::Document* pMain);
			CConnectionsFile(OOX::Document* pMain, const CPath& uri);
			CConnectionsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CConnectionsFile();

			virtual void read(const CPath& oFilePath);
			virtual void read(const CPath& oRootPath, const CPath& oFilePath);
			virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			std::wstring m_strFilename;

			nullable<CDataConnections> DataConnections;
		};
		class CRecordsetsFile : public OOX::IFileContainer, public OOX::File
		{
		public:
			CRecordsetsFile(OOX::Document* pMain);
			CRecordsetsFile(OOX::Document* pMain, const CPath& uri);
			CRecordsetsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CRecordsetsFile();

			virtual void read(const CPath& oFilePath);
			virtual void read(const CPath& oRootPath, const CPath& oFilePath);
			virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			std::wstring m_strFilename;

			nullable<CDataRecordSets> DataRecordSets;
		};
		class CRecordsetFile : public OOX::File
		{
		public:
			CRecordsetFile(OOX::Document* pMain);
			CRecordsetFile(OOX::Document* pMain, const CPath& uri);
			CRecordsetFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CRecordsetFile();

			virtual void read(const CPath& oFilePath);
			virtual void read(const CPath& oRootPath, const CPath& oFilePath);
			virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter);

			std::wstring m_strFilename;

			std::string m_sXmlA;
		};
	} //Draw
} // namespace OOX
