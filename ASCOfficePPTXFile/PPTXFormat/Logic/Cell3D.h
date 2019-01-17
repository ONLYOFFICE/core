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
#ifndef PPTX_LOGIC_CELL3D_INCLUDE_H_
#define PPTX_LOGIC_CELL3D_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/Material.h"
#include "Bevel.h"
#include "LightRig.h"

namespace PPTX
{
	namespace Logic
	{
		class Cell3D : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Cell3D)

			Cell3D()
			{
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;
					
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

					if (_T("bevel") == strName)
						bevel = oReader;
					else if (_T("lightRig") == strName)
						lightRig = oReader;
				}
				FillParentPointersForChilds();
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
					WritingElement_ReadAttributes_ReadSingle ( oReader, _T("prstMaterial"), prstMaterial )
				WritingElement_ReadAttributes_End	( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlMacroReadAttributeBase(node, L"prstMaterial", prstMaterial);

				bevel		= node.ReadNodeNoNS(_T("bevel"));
				lightRig	= node.ReadNodeNoNS(_T("lightRig"));

				FillParentPointersForChilds();
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("prstMaterial"), prstMaterial);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(bevel);
				oValue.WriteNullable(lightRig);

				return XmlUtils::CreateNode(_T("a:cell3D"), oAttr, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				// TODO:
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				// TODO:
			}

		public:
			nullable_limit<Limit::Material>		prstMaterial;

			nullable<Bevel>						bevel;
			nullable<LightRig>					lightRig;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(bevel.IsInit())
					bevel->SetParentPointer(this);
				if(lightRig.IsInit())
					lightRig->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CELL3D_INCLUDE_H_
