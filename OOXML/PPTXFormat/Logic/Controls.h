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
			PPTX_LOGIC_BASE(Control)
			Control(OOX::Document *pMain = NULL) : WrapperWritingElement(pMain)
			{
			}
			std::wstring GetVmlXmlBySpid(std::wstring spid, smart_ptr<OOX::IFileContainer> & rels) const ;
			
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
			PPTX_LOGIC_BASE(Controls)
			
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

