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
#include "../Common/SimpleTypes_Draw.h"

namespace OOX
{
	namespace Draw
	{
		class CDocumentSettings : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDocumentSettings)
			CDocumentSettings(){}
			virtual ~CDocumentSettings(){}
			
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
 			
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint TopPage;
			nullable_uint DefaultTextStyle;
			nullable_uint DefaultLineStyle;
			nullable_uint DefaultFillStyle;
			nullable_uint DefaultGuideStyle;
			
			nullable_int GlueSettings;
			nullable_int SnapSettings;
			nullable_int SnapExtensions;
			nullable_int SnapAngles;
			nullable_bool DynamicGridEnabled;
			nullable_bool ProtectStyles;
			nullable_bool ProtectShapes;
			nullable_bool ProtectBkgnds;
			nullable_bool ProtectMasters;
			nullable_string CustomMenusFile;
			nullable_string CustomToolbarsFile;
			nullable_string AttachedToolbars; //base64
		};
		class CColorEntry : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CColorEntry)

			CColorEntry() {}
			virtual ~CColorEntry() {}

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
			nullable_uint IX;
			nullable_string RGB;
		};
		class CColors : public WritingElementWithChilds<CColorEntry>
		{
		public:
			WritingElement_AdditionMethods(CColors)

			CColors() {}
			virtual ~CColors() {}

			virtual std::wstring toXML() const { return L""; }
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};
		class CFaceName : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CFaceName)

			CFaceName() {}
			virtual ~CFaceName() {}

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
			nullable_string NameU;
			nullable_string UnicodeRanges;
			nullable_string CharSets;
			nullable_string Panos;
			nullable_string Panose;
			nullable_uint Flags;
		};
		class CFaceNames : public WritingElementWithChilds<CFaceName>
		{
		public:
			WritingElement_AdditionMethods(CFaceNames)

			CFaceNames() {}
			virtual ~CFaceNames() {}

			virtual std::wstring toXML() const { return L""; }
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};
		class CStyleSheet : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CStyleSheet)
			CStyleSheet() {}
			virtual ~CStyleSheet() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint ID;
			nullable_string NameU;
			nullable_string Name;
			nullable_bool IsCustomName;
			nullable_bool IsCustomNameU;
			nullable_uint LineStyle;
			nullable_uint FillStyle;
			nullable_uint TextStyle;
		};
		class CStyleSheets : public WritingElementWithChilds<CStyleSheet>
		{
		public:
			WritingElement_AdditionMethods(CStyleSheets)

			CStyleSheets() {}
			virtual ~CStyleSheets() {}

			virtual std::wstring toXML() const { return L""; }
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};
		class CDocumentSheet : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CDocumentSheet)
			CDocumentSheet() {}
			virtual ~CDocumentSheet() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_string Name;
			nullable_string NameU;
			nullable_bool IsCustomName;
			nullable_bool IsCustomNameU;
			nullable_string UniqueID;
			nullable_uint LineStyle;
			nullable_uint FillStyle;
			nullable_uint TextStyle;
		};
		class CEventItem : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CEventItem)

			CEventItem() {}
			virtual ~CEventItem() {}

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
			nullable_uint ID;
			nullable_uint Action;
			nullable_uint EventCode;
			nullable_bool Enabled;
			nullable_string Target;
			nullable_string TargetArgs;
		};
		class CEventList : public WritingElementWithChilds<CEventItem>
		{
		public:
			WritingElement_AdditionMethods(CEventList)
			CEventList() {}
			virtual ~CEventList() {}

			virtual std::wstring toXML() const { return L""; }
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};
		class CHeaderFooter : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CHeaderFooter)
			CHeaderFooter() {}
			virtual ~CHeaderFooter() {}

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
			nullable_string HeaderMargin;
			nullable_string FooterMargin;
			nullable_string HeaderLeft;
			nullable_string HeaderCenter;
			nullable_string HeaderRight;
			nullable_string FooterLeft;
			nullable_string FooterCenter;
			nullable_string FooterRight;

			//nullable<CHeaderFooterFont> HeaderFooterFont;
			nullable_string HeaderFooterColor;

		};
		class CWindow : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CWindow)
			CWindow() {}
			virtual ~CWindow() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint ID;
			nullable<SimpleTypes::Draw::CWindowType> WindowType;
			nullable_uint WindowState;
			nullable_int WindowLeft;
			nullable_int WindowTop;
			nullable_uint WindowWidth;
			nullable_uint WindowHeight;
			nullable<SimpleTypes::Draw::CContainerType> ContainerType;
			nullable_uint Container;
			nullable_uint Page;
			nullable_uint Sheet;
			nullable_double ViewScale;
			nullable_double ViewCenterX;
			nullable_double ViewCenterY;
			nullable_string Document;
			nullable_uint ParentWindow;
			nullable_bool ReadOnly;

			nullable_bool ShowRulers;
			nullable_bool ShowGrid;
			nullable_bool ShowPageBreaks;
			nullable_bool ShowGuides;
			nullable_bool ShowConnectionPoints;
			nullable_uint GlueSettings;
			nullable_uint SnapSettings;
			nullable_uint SnapExtensions;
			nullable_bool SnapAngles;
			nullable_bool DynamicGridEnabled;
			nullable_double TabSplitterPos;
			nullable_uint StencilGroup;
			nullable_uint StencilGroupPos;
		};
		class CWindows : public WritingElementWithChilds<CWindow>
		{
		public:
			WritingElement_AdditionMethods(CWindows)
			CWindows() {}
			virtual ~CWindows() {}

			virtual std::wstring toXML() const { return L""; }
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			nullable_uint ClientWidth;
			nullable_uint ClientHeight;
		};
		class CDocumentFile : public OOX::IFileContainer, public OOX::File
		{
		public:
			CDocumentFile(OOX::Document* pMain);
			CDocumentFile(OOX::Document* pMain, const CPath& uri);
			CDocumentFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CDocumentFile();

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

			nullable<CDocumentSettings> DocumentSettings;
			nullable<CColors> Colors;
			nullable<CFaceNames> FaceNames;
			nullable<CStyleSheets> StyleSheets;
			nullable<CDocumentSheet> DocumentSheet;
			nullable<CEventList> EventList;
			nullable<CHeaderFooter> HeaderFooter;
			//nullable<CPublishSettings> PublishSettings;
		};
		class CWindowsFile : public OOX::File
		{
		public:
			CWindowsFile(OOX::Document* pMain);
			CWindowsFile(OOX::Document* pMain, const CPath& uri);
			CWindowsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CWindowsFile();

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

			nullable<CWindows> Windows;
		};


	} //Draw
} // namespace OOX
