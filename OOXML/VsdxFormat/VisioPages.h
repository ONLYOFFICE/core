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

#include "../Base/Nullable.h"

#include "FileTypes_Draw.h"
#include "../DocxFormat/IFileContainer.h"

namespace OOX
{
	namespace Draw
	{
		class CConnects;
		class CShapes;
		class CRel;
		class CIcon;

		class CPageFile : public OOX::IFileContainer, public OOX::File
		{
		public:
			CPageFile(OOX::Document* pMain);
			CPageFile(OOX::Document* pMain, const CPath& uri);
			CPageFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CPageFile();

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

			nullable<CShapes> Shapes;
			nullable<CConnects> Connects;
// from pages
			nullable_uint ID;
		};
		class CMasterFile : public OOX::IFileContainer, public OOX::File
		{
		public:
			CMasterFile(OOX::Document* pMain);
			CMasterFile(OOX::Document* pMain, const CPath& uri);
			CMasterFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CMasterFile();

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

			nullable<CShapes> Shapes;
			nullable<CConnects> Connects;
// from masters
			nullable_uint ID;
			nullable_string UniqueID;
		};
		//---------------------------------------------------------------------------------------------------------------------------------
		class CPageSheet : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CPageSheet)
			CPageSheet() {}
			virtual ~CPageSheet() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_string UniqueID;
			nullable_uint LineStyle;
			nullable_uint FillStyle;
			nullable_uint TextStyle;
		};
		class CPage : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CPage)
			CPage(OOX::Document* pMain = NULL);
			virtual ~CPage();

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint ID;
			nullable_string Name;
			nullable_string NameU;
			nullable_bool IsCustomName;
			nullable_bool IsCustomNameU;
			nullable_bool Background;
			nullable_uint BackPage;
			nullable_double ViewScale;
			nullable_double ViewCenterX;
			nullable_double ViewCenterY;
			nullable_uint ReviewerID;
			nullable_uint AssociatedPage;

			nullable<CPageSheet> PageSheet;
			nullable<CRel> Rel;
		};
		class CMaster : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CMaster)
			CMaster(OOX::Document* pMain = NULL);
			virtual ~CMaster();

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint ID;
			nullable_string Name;
			nullable_string BaseID;
			nullable_string UniqueID;
			nullable_bool MatchByName;
			nullable_string NameU;
			nullable_bool IsCustomName;
			nullable_bool IsCustomNameU;
			nullable_uint IconSize;
			nullable_uint PatternFlags;
			nullable_string Prompt;
			nullable_bool Hidden;
			nullable_bool IconUpdate;
			nullable_uint AlignName;
			nullable_uint MasterType;

			nullable<CPageSheet> PageSheet;
			nullable<CRel> Rel;
			nullable<CIcon> Icon;
		};
		class CPagesFile : public OOX::IFileContainer, public OOX::File, public WritingElementWithChilds<CPage>
		{
		public:
			CPagesFile(OOX::Document* pMain);
			CPagesFile(OOX::Document* pMain, const CPath& uri);
			CPagesFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CPagesFile();

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual EElementType getType() const;

			virtual void read(const CPath& oFilePath);
			virtual void read(const CPath& oRootPath, const CPath& oFilePath);
			virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			std::wstring m_strFilename;
		};
		class CMastersFile : public OOX::IFileContainer, public OOX::File, public WritingElementWithChilds<CMaster>
		{
		public:
			CMastersFile(OOX::Document* pMain);
			CMastersFile(OOX::Document* pMain, const CPath& uri);
			CMastersFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CMastersFile();

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual EElementType getType() const;

			virtual void read(const CPath& oFilePath);
			virtual void read(const CPath& oRootPath, const CPath& oFilePath);
			virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			std::wstring m_strFilename;
		};

	} //Draw
} // namespace OOX
