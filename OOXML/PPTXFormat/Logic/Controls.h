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

#include "SpTreeElem.h"

#include "../../Common/SimpleTypes_Drawing.h"
#include "Pic.h"

namespace PPTX
{
	namespace Logic
	{
		//19.3.2.1 control (Embedded Control)
		class Control : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE_NC(Control)
			Control(OOX::Document *pMain = NULL) : WrapperWritingElement(pMain)
			{
			}
			std::wstring GetVmlXmlBySpid(std::wstring spid, OOX::IFileContainer*& rels) const ;
			
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const ;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			nullable_string			name;
			nullable_string			spid;
			nullable_bool			showAsIcon;
			nullable_uint			height;
			nullable_uint			width;
			nullable<OOX::RId>		rId;

			nullable<PPTX::Logic::Pic> pic;
//extLst (Extension List) 19.2.1.12
		protected:			

			virtual void FillParentPointersForChilds()
			{
				if (pic.IsInit()) pic->SetParentPointer(this);
			}
		};
		
		// 19.3.1.15 controls (List of controls)
		class Controls : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE_NC(Controls)
			
			Controls(OOX::Document *pMain = NULL) : WrapperWritingElement(pMain)
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node);
			
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			std::vector<Control> arrControls;

		protected:
			virtual void FillParentPointersForChilds()
			{
				for (size_t i = 0; i <arrControls.size(); i++)
					arrControls[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

