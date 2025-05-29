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
