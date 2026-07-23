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

#include "CustGeom.h"

namespace PPTX
{
	namespace Logic
	{
		CustGeom& CustGeom::operator=(const CustGeom& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			avLst = oSrc.avLst;
			gdLst = oSrc.gdLst;
			ahLst = oSrc.ahLst;
			cxnLst = oSrc.cxnLst;
			pathLst = oSrc.pathLst;

			rect = oSrc.rect;

			return *this;
		}
		OOX::EElementType CustGeom::getType() const
		{
			return OOX::et_a_custGeom;
		}
		void CustGeom::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
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
				else if (sName == L"a:gdLst")
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
							gdLst.push_back(gd);
							gdLst.back().fromXML(oReader);
						}
					}
				}
				else if (sName == L"a:rect")
					rect = oReader;
				else if (sName == L"a:pathLst")
				{
					if ( oReader.IsEmptyNode() )
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
					{
						std::wstring sName1 = oReader.GetName();

						if (sName1 == L"a:path")
						{
							Path2D gd;
							pathLst.push_back(gd);
							pathLst.back().fromXML(oReader);
						}
					}
				}
				else if (sName == L"a:ahLst")
				{
					if ( oReader.IsEmptyNode() )
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
					{
						std::wstring sName1 = oReader.GetName();

						if (sName1 == L"a:ahPolar")
						{
							AhBase gd;
							ahLst.push_back(gd);
							ahLst.back().fromXML(oReader);
						}
						else if (sName1 == L"a:ahXY")
						{
							AhBase gd;
							ahLst.push_back(gd);
							ahLst.back().fromXML(oReader);
						}
					}
				}
				else if (sName == L"a:cxnLst")
				{
					if ( oReader.IsEmptyNode() )
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
					{
						std::wstring sName1 = oReader.GetName();

						if (sName1 == L"a:cxn")
						{
							Cxn gd;
							cxnLst.push_back(gd);
							cxnLst.back().fromXML(oReader);
						}
					}
				}
			}
			FillParentPointersForChilds();
		}
		void CustGeom::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("a:avLst"), oNode))
				XmlMacroLoadArray(oNode, _T("a:gd"), avLst, Gd);
			if (node.GetNode(_T("a:gdLst"), oNode))
				XmlMacroLoadArray(oNode, _T("a:gd"), gdLst, Gd);

			if (node.GetNode(_T("a:ahLst"), oNode))
			{
				XmlMacroLoadArray(oNode, _T("a:ahPolar"), ahLst, AhBase);
				XmlMacroLoadArray(oNode, _T("a:ahXY"), ahLst, AhBase);
			}

			if (node.GetNode(_T("a:cxnLst"), oNode))
				XmlMacroLoadArray(oNode, _T("a:cxn"), cxnLst, Cxn);

			if (node.GetNode(_T("a:pathLst"), oNode))
				XmlMacroLoadArray(oNode, _T("a:path"), pathLst, Path2D);

			rect = node.ReadNodeNoNS(_T("rect"));

			FillParentPointersForChilds();
		}
		std::wstring CustGeom::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(_T("a:avLst"), avLst);
			oValue.WriteArray(_T("a:gdLst"), gdLst);
			oValue.WriteArray(_T("a:ahLst"), ahLst);
			oValue.WriteArray(_T("a:cxnLst"), cxnLst);
			oValue.WriteNullable(rect);
			oValue.WriteArray(_T("a:pathLst"), pathLst);

			return XmlUtils::CreateNode(_T("a:custGeom"), oValue);
		}
		void CustGeom::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:custGeom"));
			pWriter->EndAttributes();

			if (avLst.size() == 0)
				pWriter->WriteString(_T("<a:avLst/>"));
			else
				pWriter->WriteArray(_T("a:avLst"), avLst);

			if (gdLst.size() == 0)
				pWriter->WriteString(_T("<a:gdLst/>"));
			else
				pWriter->WriteArray(_T("a:gdLst"), gdLst);

			if (ahLst.size() == 0)
				pWriter->WriteString(_T("<a:ahLst/>"));
			else
				pWriter->WriteArray(_T("a:ahLst"), ahLst);

			if (cxnLst.size() == 0)
				pWriter->WriteString(_T("<a:cxnLst/>"));
			else
				pWriter->WriteArray(_T("a:cxnLst"), cxnLst);
		
			if (rect.is_init())
				pWriter->Write(rect);
			else
				pWriter->WriteString(_T("<a:rect l=\"0\" t=\"0\" r=\"r\" b=\"b\"/>"));
			if (pathLst.size() == 0)
				pWriter->WriteString(_T("<a:pathLst/>"));
			else
				pWriter->WriteArray(_T("a:pathLst"), pathLst);

			pWriter->EndNode(_T("a:custGeom"));
		}
		void CustGeom::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(GEOMETRY_TYPE_CUSTOM);

			pWriter->WriteRecordArray(0, 1, avLst);
			pWriter->WriteRecordArray(1, 1, gdLst);
			pWriter->WriteRecordArray(2, 1, ahLst);
			pWriter->WriteRecordArray(3, 1, cxnLst);
			pWriter->WriteRecordArray(4, 1, pathLst);
			pWriter->WriteRecord2(5, rect);

			pWriter->EndRecord();
		}
		void CustGeom::FillParentPointersForChilds()
		{
			size_t count = 0;

			count = avLst.size();
			for (size_t i = 0; i < count; ++i)
				avLst[i].SetParentPointer(this);

			count = gdLst.size();
			for (size_t i = 0; i < count; ++i)
				gdLst[i].SetParentPointer(this);

			count = ahLst.size();
			for (size_t i = 0; i < count; ++i)
				ahLst[i].SetParentPointer(this);

			count = cxnLst.size();
			for (size_t i = 0; i < count; ++i)
				cxnLst[i].SetParentPointer(this);

			count = pathLst.size();
			for (size_t i = 0; i < count; ++i)
				pathLst[i].SetParentPointer(this);

			if (rect.IsInit())
				rect->SetParentPointer(this);
		}
		std::wstring CustGeom::GetODString() const
		{
			std::wstring strXml = _T("");

			size_t nCount  = 0;

			strXml += _T("<avLst>");
			nCount = avLst.size();
			for (size_t i = 0; i < nCount; ++i)
				strXml += avLst[i].GetODString();
			strXml += _T("</avLst>");

			strXml += _T("<gdLst>");
			nCount = gdLst.size();
			for (size_t i = 0; i < nCount; ++i)
				strXml += gdLst[i].GetODString();
			strXml += _T("</gdLst>");

			strXml += _T("<ahLst>");
			nCount = ahLst.size();
			for (size_t i = 0; i < nCount; ++i)
				strXml += ahLst[i].GetODString();
			strXml += _T("</ahLst>");

			strXml += _T("<cxnLst>");
			nCount = cxnLst.size();
			for (size_t i = 0; i < nCount; ++i)
				strXml += cxnLst[i].GetODString();
			strXml += _T("</cxnLst>");

			strXml += _T("<pathLst>");
			nCount = pathLst.size();
			for (size_t i = 0; i < nCount; ++i)
				strXml += pathLst[i].GetODString();
			strXml += _T("</pathLst>");

			if (rect.IsInit())
				strXml += rect->GetODString();

			return XmlUtils::CreateNode(_T("ooxml-shape"), strXml);
		}
	} // namespace Logic
} // namespace PPTX
