﻿/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
#ifndef PPTX_LOGIC_TABLE_CELLPROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_TABLE_CELLPROPERTIES_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/TextVerticalType.h"
#include "./../../Limit/TextAnchor.h"
#include "./../../Limit/HorzOverflow.h"
#include "./../Ln.h"
#include "./../Cell3D.h"
#include "./../UniFill.h"

namespace PPTX
{
	namespace Logic
	{
		class TableCellProperties : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionMethods(TableCellProperties)

			TableCellProperties();

			TableCellProperties& operator=(const TableCellProperties& oSrc);

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable<Ln>		LnL;
			nullable<Ln>		LnR;
			nullable<Ln>		LnT;
			nullable<Ln>		LnB;
			nullable<Ln>		LnTlToBr;
			nullable<Ln>		LnBlToTr;
            nullable<Cell3D>	cell3D;
			UniFill				Fill;
//			<xsd:element name="headers" type="CT_Headers" minOccurs="0"/> 

			nullable_int							MarL;			//default="91440"
			nullable_int							MarR;			//default="91440"
			nullable_int							MarT;			//default="45720"
			nullable_int							MarB;			//default="45720"
			nullable_limit<Limit::TextVerticalType> Vert;			//default="horz"
			nullable_limit<Limit::TextAnchor>		Anchor;			//default="t"
			nullable_bool							AnchorCtr;		//default="false"
			nullable_limit<Limit::HorzOverflow>		HorzOverflow;	//default="clip"

			bool is_empty;

		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLE_CELLPROPERTIES_INCLUDE_H_
