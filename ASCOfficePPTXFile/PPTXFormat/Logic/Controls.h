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

#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Drawing.h"
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

			void AddObjectTo (const std::vector<SpTreeElem> *spTreeElements, NSShapeImageGen::CMediaManager* pMediaManager) const ;
			
			std::wstring GetVmlXmlBySpid(std::wstring spid, smart_ptr<OOX::IFileContainer> & rels) const ;
			
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const ;

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name	= node.GetAttribute(_T("name"));
				spid	= node.GetAttribute(_T("spid"));
				
                XmlMacroReadAttributeBase(node, L"rId"		, rId);
                XmlMacroReadAttributeBase(node, L"width"		, width);
                XmlMacroReadAttributeBase(node, L"height"	, height);
                XmlMacroReadAttributeBase(node, L"showAsIcon", showAsIcon);

				pic = node.ReadNode(_T("p:pic"));

				FillParentPointersForChilds();
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("name"),		name);
				
				if (rId.IsInit())	oAttr.Write(_T("rId"),		rId->ToString());
				if (width.IsInit())	oAttr.Write(_T("width"),	width->ToString());
				if (height.IsInit())oAttr.Write(_T("height"),	height->ToString());

				oAttr.Write(_T("spid"),		spid);

				return XmlUtils::CreateNode(_T("p:control"), oAttr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:control"));

				pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("name")		, name);
					pWriter->WriteAttribute(_T("showAsIcon"), showAsIcon);
					pWriter->WriteAttribute(_T("spid")		, spid);
					
					if (rId.IsInit())  pWriter->WriteAttribute(_T("rId"), rId->ToString());
					if (width.IsInit())pWriter->WriteAttribute(_T("width"), width->ToString());
					if (height.IsInit())pWriter->WriteAttribute(_T("height"), height->ToString());
				pWriter->EndAttributes();

				pWriter->Write(pic);

				pWriter->EndNode(_T("p:control"));
			}

			nullable_string									name;
			nullable_string									spid;
			nullable_bool									showAsIcon;
			nullable<SimpleTypes::CPositiveCoordinate<0>>	height;
			nullable<SimpleTypes::CPositiveCoordinate<0>>	width;
			nullable<OOX::RId>								rId;

			nullable<PPTX::Logic::Pic>						pic;
/// todooo
//extLst (Extension List) 19.2.1.12
		protected:			

			virtual void FillParentPointersForChilds()
			{
			}
		};
		
		// 19.3.1.15 controls (List of controls)
		class Controls : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Controls)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				arrControls.clear();

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (strName == _T("control"))
						{
							Control elem(oNode);
							arrControls.push_back(elem);
						}
					}
				}

				FillParentPointersForChilds();
			}
			virtual std::wstring toXML() const
			{
				XmlUtils::CNodeValue oValue;

				oValue.WriteArray(arrControls);				

				return XmlUtils::CreateNode(_T("p:controls"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:controls"));
				pWriter->EndAttributes();

				for (size_t i = 0; i < arrControls.size(); ++i)
					arrControls[i].toXmlWriter(pWriter);

				pWriter->EndNode(_T("p:controls"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const 
			{
				for (size_t i = 0; i <arrControls.size(); i++)
					arrControls[i].toPPTY(pWriter);
			}	
			void AddObjectsTo (const std::vector<PPTX::Logic::SpTreeElem> *spTreeElements, NSShapeImageGen::CMediaManager* pMediaManager) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
			}

		public:
			std::vector<Control>	arrControls;

		protected:
			virtual void FillParentPointersForChilds()
			{
				for (size_t i = 0; i <arrControls.size(); i++)
					arrControls[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

