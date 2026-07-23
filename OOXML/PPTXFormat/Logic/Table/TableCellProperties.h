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
