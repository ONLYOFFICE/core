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
#ifndef PPTX_LOGIC_TABLESTYLE_INCLUDE_H_
#define PPTX_LOGIC_TABLESTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "TableBgStyle.h"
#include "TablePartStyle.h"
#include "../Theme.h"

namespace PPTX
{
	namespace Logic
	{
		class TableStyle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TableStyle)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

		public:
			std::wstring styleId;
			std::wstring styleName;

			nullable<TableBgStyle>	 tblBg;
			nullable<TablePartStyle> wholeTbl;
			nullable<TablePartStyle> band1H;
			nullable<TablePartStyle> band2H;
			nullable<TablePartStyle> band1V;
			nullable<TablePartStyle> band2V;
			nullable<TablePartStyle> lastCol;
			nullable<TablePartStyle> firstCol;
			nullable<TablePartStyle> lastRow;
			nullable<TablePartStyle> seCell;
			nullable<TablePartStyle> swCell;
			nullable<TablePartStyle> firstRow;
			nullable<TablePartStyle> neCell;
			nullable<TablePartStyle> nwCell;

		public:
			std::wstring m_name;

		protected:
			virtual void FillParentPointersForChilds();

		public:
			void SetTheme(const smart_ptr<PPTX::Theme> theme);

		private:
			smart_ptr<PPTX::Theme> m_Theme;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLESTYLE_INCLUDE_H
