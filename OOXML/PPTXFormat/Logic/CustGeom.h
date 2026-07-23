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
#ifndef PPTX_LOGIC_CUSTGEOM_INCLUDE_H_
#define PPTX_LOGIC_CUSTGEOM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Gd.h"
#include "Rect.h"
#include "Path2D.h"
#include "AhBase.h"
#include "Cxn.h"

namespace PPTX
{
	namespace Logic
	{
		class CustGeom : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionMethods(CustGeom)
			PPTX_LOGIC_BASE2(CustGeom)

			CustGeom& operator=(const CustGeom& oSrc);
			virtual OOX::EElementType getType() const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			std::vector<Gd>			avLst;
			std::vector<Gd>			gdLst;
			std::vector<AhBase>		ahLst;
			std::vector<Cxn>		cxnLst;
			nullable<Rect>			rect;
			std::vector<Path2D>		pathLst;

		protected:
			virtual void FillParentPointersForChilds();

		public:
			virtual std::wstring GetODString() const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CUSTGEOM_INCLUDE_H
