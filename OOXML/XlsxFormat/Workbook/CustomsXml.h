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

#include "../Table/Autofilter.h"
#include "../../DocxFormat/IFileContainer.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CMapInfo : public OOX::Spreadsheet::WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CMapInfo)
			CMapInfo() {}
			virtual ~CMapInfo() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }
			virtual  void toXML(NSStringUtils::CStringBuilder& writer) const {}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual EElementType getType() const;

			nullable_string SelectionNamespaces;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//------------------------------------------------------------------------------------------------------------------------
		class CXmlMapsFile : public OOX::File
		{
		public:
			CXmlMapsFile(OOX::Document* pMain);
			CXmlMapsFile(OOX::Document* pMain, const CPath& uri);
			CXmlMapsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CXmlMapsFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter);
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			const CPath& GetReadPath();

			nullable<CMapInfo> m_MapInfo;
		private:
			CPath m_oReadPath;
		};
		class CDataBinding : public OOX::Spreadsheet::WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDataBinding)
			CDataBinding() {}
			virtual ~CDataBinding() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const {}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter);
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint ConnectionID;
			nullable_string DataBindingName;
			nullable_string FileBindingName;
			nullable_string SchemaID;
			nullable_bool FileBinding;
			nullable_uint DataBindingLoadMode;	
			nullable_string content;
		};
		class CMap : public OOX::Spreadsheet::WritingElement
		{
		public:
			WritingElement_AdditionMethods(CMap)
			CMap() {}
			virtual ~CMap() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const {}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint ID;
			nullable_string Name;
			nullable_string RootElement;
			nullable_string SchemaID;
			nullable_bool ShowImportExportValidationErrors;
			nullable_bool AutoFit;
			nullable_bool Append;
			nullable_bool PreserveSortAFLayout;
			nullable_bool PreserveFormat;
		
			nullable<CDataBinding> DataBinding;
		};
		class CSchema : public OOX::Spreadsheet::WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSchema)
			CSchema() {}
			virtual ~CSchema() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const {}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_string ID;
			nullable_string SchemaRef;
			nullable_string Namespace;
			nullable_string SchemaLanguage;
			nullable_string content;
		};
	} //Spreadsheet
} // namespace OOX
