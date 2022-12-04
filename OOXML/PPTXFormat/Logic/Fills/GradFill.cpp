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

#include "GradFill.h"

namespace PPTX
{
	namespace Logic
	{
		GradFill::GradFill(std::wstring ns)
		{
			m_namespace = ns;
		}
		GradFill& GradFill::operator=(const GradFill& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			flip		 = oSrc.flip;
			rotWithShape = oSrc.rotWithShape;

			GsLst = oSrc.GsLst;
			lin			= oSrc.lin;
			path		= oSrc.path;
			tileRect	= oSrc.tileRect;

			m_namespace	= oSrc.m_namespace;

			return *this;
		}
		OOX::EElementType GradFill::getType () const
		{
			return OOX::et_a_gradFill;
		}
		void GradFill::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("path") == strName)
					path = oReader;
				else if (_T("lin") == strName)
					lin = oReader;
				else if (_T("tileRect") == strName)
					tileRect = oReader;
				else if (_T("gsLst") == strName)
				{
					if ( oReader.IsEmptyNode() )
						continue;

					int nCurDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nCurDepth1 ) )
					{
						Gs g; GsLst.push_back(g);
						GsLst.back().fromXML(oReader);
					}
				}
			}
		}
		void GradFill::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("rotWithShape"), rotWithShape )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("flip"), flip )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void GradFill::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlMacroReadAttributeBase(node, L"flip", flip);
			XmlMacroReadAttributeBase(node, L"rotWithShape", rotWithShape);

			XmlUtils::CXmlNode oNode = node.ReadNodeNoNS(_T("gsLst"));
			if (oNode.IsValid())
				XmlMacroLoadArray(oNode, _T("*"), GsLst, Gs);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("path") == strName)
					{
						if (!path.IsInit())
							path = oNode;
					}
					else if (_T("lin") == strName)
					{
						if (!lin.IsInit())
							lin = oNode;
					}
					else if (_T("tileRect") == strName)
					{
						if (!tileRect.IsInit())
							tileRect = oNode;
					}
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring GradFill::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(_T("flip"), flip);
			oAttr.Write(_T("rotWithShape"), rotWithShape);

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(_T("a:gsLst"), GsLst);
			oValue.WriteNullable(path);
			oValue.WriteNullable(lin);
			oValue.WriteNullable(tileRect);

			std::wstring strName = m_namespace.empty() ? _T("gradFill") : (m_namespace + _T(":gradFill"));
			return XmlUtils::CreateNode(strName, oAttr, oValue);
		}
		void GradFill::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring sAttrNamespace;
			std::wstring strName;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				sAttrNamespace = _T("w14:");
				strName = _T("w14:gradFill");
			}
			else
			{
				strName = m_namespace.empty() ? _T("gradFill") : (m_namespace + _T(":gradFill"));
			}

			pWriter->StartNode(strName);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + _T("flip"), flip);
			pWriter->WriteAttribute(sAttrNamespace + _T("rotWithShape"), rotWithShape);
			pWriter->EndAttributes();

			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				pWriter->WriteArray(_T("w14:gsLst"), GsLst);
			else
				pWriter->WriteArray(_T("a:gsLst"), GsLst);
			pWriter->Write(path);
			pWriter->Write(lin);
			pWriter->Write(tileRect);

			pWriter->EndNode(strName);
		}
		void GradFill::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(FILL_TYPE_GRAD);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, flip);
			pWriter->WriteBool2(1, rotWithShape);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->StartRecord(0);

			ULONG len = (ULONG)GsLst.size();
			pWriter->WriteULONG(len);

			for (ULONG i = 0; i < len; ++i)
			{
				pWriter->WriteRecord1(0, GsLst[i]);
			}

			pWriter->EndRecord();

			pWriter->WriteRecord2(1, lin);
			pWriter->WriteRecord2(2, path);
			pWriter->WriteRecord2(3, tileRect);

			pWriter->EndRecord();
		}
		void GradFill::Merge(GradFill& fill)const
		{
			if(flip.IsInit())
				fill.flip = *flip;
			if(rotWithShape.IsInit())
				fill.rotWithShape = *rotWithShape;
			if(tileRect.IsInit())
				fill.tileRect = tileRect;
			if(0 != GsLst.size())
			{
				fill.GsLst.clear();
				fill.GsLst = GsLst;
			}

			if(lin.IsInit())
			{
				fill.lin = lin;
				fill.path.reset();
			}
			if(path.IsInit())
			{
				fill.path = path;
				fill.lin.reset();
			}
		}
		UniColor GradFill::GetFrontColor()const
		{
			if (0 == GsLst.size())
				return UniColor();
			return GsLst[0].color;
		}
		void GradFill::FillParentPointersForChilds()
		{
			size_t count = GsLst.size();
			for (size_t i = 0; i < count; ++i)
				GsLst[i].SetParentPointer(this);

			if(lin.IsInit())
				lin->SetParentPointer(this);
			if(path.IsInit())
				path->SetParentPointer(this);
			if(tileRect.IsInit())
				tileRect->SetParentPointer(this);
		}

	} // namespace Logic
} // namespace PPTX
