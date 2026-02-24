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

#include "../../Base/Nullable.h"
#include "../WritingElement.h"
#include "../../DocxFormat/IFileContainer.h"

namespace SimpleTypes
{
	namespace Spreadsheet
	{
		class CSortMethod;
	}
}
namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}
}
namespace OOX
{
	namespace Spreadsheet
	{
		class CDxf;
		class CFilterColumn;
		class CSortCondition;
	}
}

namespace OOX
{
	namespace Spreadsheet
	{
		//todo CRichSortCondition
		class CSortRule : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSortRule)
			CSortRule(OOX::Document* pMain = NULL) : WritingElement(pMain) {}
			virtual ~CSortRule(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_SortRule;
			}
			nullable_uint m_oColId;
			nullable_string m_oId;
			nullable<CDxf> m_oDxf;
//			nullable<CRichSortCondition> m_oRichSortCondition;
			nullable<CSortCondition> m_oSortCondition;
		};
		class CSortRules : public WritingElementWithChilds<CSortRule>
		{
		public:
			WritingElement_AdditionMethods(CSortRules)
			CSortRules(OOX::Document* pMain = NULL) : WritingElementWithChilds<CSortRule>(pMain) {}
			virtual ~CSortRules(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_SortRules;
			}

			nullable<SimpleTypes::Spreadsheet::CSortMethod> m_oSortMethod;//none
			nullable_bool m_oCaseSensitive;//False

			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
		class CColumnFilter : public WritingElementWithChilds<CFilterColumn>
		{
		public:
			WritingElement_AdditionMethods(CColumnFilter)
			CColumnFilter(OOX::Document* pMain = NULL) : WritingElementWithChilds<CFilterColumn>(pMain) {}
			virtual ~CColumnFilter(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_ColumnFilter;
			}
			//Attributes
			nullable_uint m_oColId;
			nullable_string m_oId;
			//Members
			nullable<CDxf> m_oDxf;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
		class CNsvFilter : public WritingElementWithChilds<CColumnFilter>
		{
		public:
			WritingElement_AdditionMethods(CNsvFilter)
			CNsvFilter(OOX::Document* pMain = NULL) : WritingElementWithChilds<CColumnFilter>(pMain) {}
			virtual ~CNsvFilter(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_NsvFilter;
			}
			nullable_string m_oFilterId;
			nullable_string m_oRef;
			nullable_uint m_oTableId;

			nullable<CSortRules> m_oSortRules;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
		class CNamedSheetView : public WritingElementWithChilds<CNsvFilter>
		{
		public:
			WritingElement_AdditionMethods(CNamedSheetView)
			CNamedSheetView(OOX::Document* pMain = NULL) : WritingElementWithChilds<CNsvFilter>(pMain) {}
			virtual ~CNamedSheetView(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_NamedSheetView;
			}
			//Attributes
			nullable_string m_oName;
			nullable_string m_oId;
			//Members
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
		class CNamedSheetViews : public WritingElementWithChilds<CNamedSheetView>
		{
		public:
			WritingElement_AdditionMethods(CNamedSheetViews)
			CNamedSheetViews(OOX::Document* pMain = NULL) : WritingElementWithChilds<CNamedSheetView>(pMain) {}
			virtual ~CNamedSheetViews(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_NamedSheetViews;
			}
			//Members
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};

		class CNamedSheetViewFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CNamedSheetViewFile(OOX::Document* pMain);
			CNamedSheetViewFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			nullable<CNamedSheetViews> m_oNamedSheetViews;

		private:
			CPath m_oReadPath;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

	} //Spreadsheet
} // namespace OOX
