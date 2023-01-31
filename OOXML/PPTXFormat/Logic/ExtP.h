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

#ifndef PPTX_LOGIC_EXTP_PROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_EXTP_PROPERTIES_INCLUDE_H_

#include "../Presentation/SectionLst.h"

//--------------------------------------------------------------------------------------------------------------
//<p:ext uri="{DAA4B4D4-6D71-4841-9C94-3DE7FCFB9230}">
//<p14:media xmlns:p14="http://schemas.microsoft.com/office/powerpoint/2010/main" r:embed="rId2">
//<p14:trim st="93333.0816" end="96583.4816"/>
//</p14:media>
//</p:ext>
//--------------------------------------------------------------------------------------------------------------
//<ext uri="{05C60535-1F16-4fd2-B633-F4F36F0B64E0}" xmlns:x14="http://schemas.microsoft.com/office/spreadsheetml/2009/9/main">
// <x14:sparklineGroups xmlns:xm="http://schemas.microsoft.com/office/excel/2006/main">
//  <x14:sparklineGroup displayEmptyCellsAs="gap">
//   <x14:colorSeries theme="5" tint="-0.499984740745262"/>
//   <x14:colorNegative theme="6"/>
//   <x14:colorAxis rgb="FF000000"/>
//   <x14:colorMarkers theme="5" tint="-0.499984740745262"/>
//   <x14:colorFirst theme="5" tint="0.39997558519241921"/>
//   <x14:colorLast theme="5" tint="0.39997558519241921"/>
//   <x14:colorHigh theme="5"/>
//   <x14:colorLow theme="5"/>
//   <x14:sparklines>
//    <x14:sparkline>
//     <xm:f>Лист1!D10:D12</xm:f>
//     <xm:sqref>H12</xm:sqref>
//    </x14:sparkline>
//   </x14:sparklines>
//  </x14:sparklineGroup>
// </x14:sparklineGroups>
//</ext>
//--------------------------------------------------------------------------------------------------------------
//<a:ext uri="{63B3BB69-23CF-44E3-9099-C40C66FF867C}">
// <a14:compatExt spid="_x0000_s1025"/>
//</a:ext>
//--------------------------------------------------------------------------------------------------------------
 // <p:ext uri="{521415D9-36F7-43E2-AB2F-B90AF26B5E84}">
 //  <p14:sectionLst xmlns:p14="http://schemas.microsoft.com/office/powerpoint/2010/main">
 //   <p14:section name="123" id="{775558AA-D529-4C59-9762-B6F3CDAC8092}">
 //    <p14:sldIdLst>
 //     <p14:sldId id="256"/>
 //     <p14:sldId id="257"/>
 //    </p14:sldIdLst>
 //   </p14:section>
 //   <p14:section name="qwerty" id="{296A3C58-7D47-4848-A546-3C86980BE927}">
 //    <p14:sldIdLst>
 //     <p14:sldId id="258"/>
 //    </p14:sldIdLst>
 //   </p14:section>
 //   <p14:section name="Раздел без заголовка" id="{DA908BF8-2A15-4E46-B49B-C1BB4B4AAF31}">
 //    <p14:sldIdLst/>
 //   </p14:section>
 //  </p14:sectionLst>
 // </p:ext>
 // <p:ext uri="{EFAFB233-063F-42B5-8137-9DF3F51BA10A}">
 //  <p15:sldGuideLst xmlns:p15="http://schemas.microsoft.com/office/powerpoint/2012/main"/>
 // </p:ext>
 //</p:extLst>

namespace PPTX
{
	namespace Logic
	{
		class Ext : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Ext)

			Ext();

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes1(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes2(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes3(XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

//-------------------------------------------------------
			nullable_string			uri;
//media
			nullable<OOX::RId>		link;
			nullable_double			st;
			nullable_double			end;			
//compatExt
			nullable_string			spid;

			nullable<nsPresentation::SectionLst>	sectionLst;
//dataModelExt
//table
//sparklineGroups
		protected:
			virtual void FillParentPointersForChilds();
		};
	} 
} 

#endif 
