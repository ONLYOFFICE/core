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

#include "PrstGeom.h"

namespace PPTX
{
	namespace Logic
	{
		PrstGeom& PrstGeom::operator=(const PrstGeom& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			prst = oSrc.prst;
			avLst = oSrc.avLst;

			return *this;
		}
		OOX::EElementType PrstGeom::getType() const
		{
			return OOX::et_a_prstGeom;
		}
		void PrstGeom::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if (sName == L"a:avLst")
				{
					if ( oReader.IsEmptyNode() )
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
					{
						std::wstring sName1 = oReader.GetName();

						if (sName1 == L"a:gd")
						{
							Gd gd;
							avLst.push_back(gd);
							avLst.back().fromXML(oReader);
						}
					}
				}
			}

			FillParentPointersForChilds();
		}
		void PrstGeom::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("prst"), prst)
			WritingElement_ReadAttributes_End( oReader )
		}
		void PrstGeom::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"prst", prst);

			XmlUtils::CXmlNode nodeList = node.ReadNode(_T("a:avLst"));
			XmlMacroLoadArray(nodeList, _T("a:gd"), avLst, Gd);

			FillParentPointersForChilds();
		}
		std::wstring PrstGeom::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("prst"), prst.get());

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(_T("a:avLst"), avLst);

			return XmlUtils::CreateNode(_T("a:prstGeom"), oAttr, oValue);
		}
		void PrstGeom::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:prstGeom"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("prst"), prst.get());
			pWriter->EndAttributes();

			if (avLst.size() == 0)
				pWriter->WriteString(_T("<a:avLst/>"));
			else
				pWriter->WriteArray(_T("a:avLst"), avLst);

			pWriter->EndNode(_T("a:prstGeom"));
		}
		void PrstGeom::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(GEOMETRY_TYPE_PRST);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, prst.get());
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecordArray(0, 1, avLst);

			pWriter->EndRecord();
		}
		void PrstGeom::FillParentPointersForChilds()
		{
			size_t count = avLst.size();
			for (size_t i = 0; i < count; ++i)
				avLst[i].SetParentPointer(this);
		}
		std::wstring PrstGeom::GetODString()const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("prst"), prst.get());

			std::wstring strXml = _T("");
			size_t nCount = avLst.size();
			for (size_t i = 0; i < nCount; ++i)
				strXml += avLst[i].GetODString();

			return XmlUtils::CreateNode(_T("prstGeom"), oAttr, strXml);
		}
	} // namespace Logic
} // namespace PPTX
