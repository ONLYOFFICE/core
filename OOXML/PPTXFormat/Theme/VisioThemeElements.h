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

#include "./../WrapperWritingElement.h"
#include "../Logic/UniColor.h"

namespace PPTX
{
	namespace nsTheme
	{		
		class LineEx : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(LineEx)
			LineEx& operator=(const LineEx& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable_uint rndg;
			nullable_uint start;
			nullable_uint startSize;
			nullable_uint end;
			nullable_uint endSize;	
			nullable_uint pattern;
		};
		class Sketch : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Sketch)
			Sketch& operator=(const Sketch& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable_uint lnAmp; //PositiveFixedPercentage
			nullable_uint fillAmp;
			nullable_uint lnWeight; //PositiveCoordinate
			nullable_uint numPts;
		};
		class LineStyle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(LineStyle)
			LineStyle& operator=(const LineStyle& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			LineEx lineEx;
			nullable<Sketch> sketch;
		protected:
			virtual void FillParentPointersForChilds();
		};
		class SchemeLineStyles : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SchemeLineStyles)
			SchemeLineStyles& operator=(const SchemeLineStyles& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			std::wstring node_name;
			std::vector<LineStyle> m_arrItems;

		protected:
			virtual void FillParentPointersForChilds();
		};
		class LineStyles : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(LineStyles)
			LineStyles& operator=(const LineStyles& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			SchemeLineStyles fmtConnectorSchemeLineStyles;
			SchemeLineStyles fmtSchemeLineStyles;

		protected:
			virtual void FillParentPointersForChilds();
		};
		class FillProps : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(FillProps)
			FillProps& operator=(const FillProps& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable_uint pattern;
			//extLst
		};
		class FillStyles : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(FillStyles)
			FillStyles& operator=(const FillStyles& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			std::vector<FillProps> m_arrItems; //min 3 ??? 
		protected:
			virtual void FillParentPointersForChilds();
		};
		class FontProps : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(FontProps)
			FontProps& operator=(const FontProps& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable_uint style;
			Logic::UniColor color;
			//extLst
		};
		class FontStyles : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(FontStyles)
			FontStyles& operator=(const FontStyles& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			std::wstring node_name;
			std::vector<FontProps> m_arrItems; //min 3
		protected:
			virtual void FillParentPointersForChilds();
		};
		class FontStylesGroup : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(FontStylesGroup)
			FontStylesGroup& operator=(const FontStylesGroup& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			FontStyles connectorFontStyles;
			FontStyles fontStyles;

		protected:
			virtual void FillParentPointersForChilds();
		};		
		class VarStyle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(VarStyle)
			VarStyle& operator=(const VarStyle& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable_uint fillIdx;
			nullable_uint lineIdx;
			nullable_uint effectIdx;
			nullable_uint fontIdx;
			//extLst
		};
		class VariationStyleScheme : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(VariationStyleScheme)
			VariationStyleScheme& operator=(const VariationStyleScheme& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable_uint embellishment;
			std::vector<VarStyle> m_arrItems; //min 4
		protected:
			virtual void FillParentPointersForChilds();
		};
		class VariationStyleSchemeLst : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(VariationStyleSchemeLst)
			VariationStyleSchemeLst& operator=(const VariationStyleSchemeLst& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			std::vector<VariationStyleScheme> m_arrItems; //min 4
		protected:
			virtual void FillParentPointersForChilds();
		};
		class VariationClrScheme : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(VariationClrScheme)
			VariationClrScheme& operator=(const VariationClrScheme& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			Logic::UniColor varColor[7];
		};
		class VariationClrSchemeLst : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(VariationClrSchemeLst)
			VariationClrSchemeLst& operator=(const VariationClrSchemeLst& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			std::vector<VariationClrScheme> m_arrItems;
		protected:
			virtual void FillParentPointersForChilds();
		};
		class Bkgnd : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Bkgnd)
			Bkgnd& operator=(const Bkgnd& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			Logic::UniColor color;
		};
	} // namespace nsTheme
} // namespace PPTX
