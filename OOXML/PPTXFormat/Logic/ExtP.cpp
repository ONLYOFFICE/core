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

#include "ExtP.h"

namespace PPTX
{
	namespace Logic
	{
		Ext::Ext()
		{
		}
		void Ext::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (strName == L"media")
				{
					ReadAttributes1(oReader);
					if ( oReader.IsEmptyNode() )
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
					{
						std::wstring strName1 = XmlUtils::GetNameNoNS(oReader.GetName());

						if (strName1 == L"trim")
						{
							ReadAttributes2(oReader);
						}
					}
				}
				else if (strName == L"compatExt")
				{
					ReadAttributes3(oReader);
				}
			}
		}
		void Ext::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, L"uri",	uri)
			WritingElement_ReadAttributes_End( oReader )
		}
		void Ext::ReadAttributes1(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"r:embed",	link)
			WritingElement_ReadAttributes_End( oReader )
		}
		void Ext::ReadAttributes2(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, L"st",	st)
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"end",	end)
			WritingElement_ReadAttributes_End( oReader )
		}
		void Ext::ReadAttributes3(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader );
				WritingElement_ReadAttributes_ReadSingle ( oReader, L"spid",	spid)
			WritingElement_ReadAttributes_End( oReader )
		}
		void Ext::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"media" == strName)
					{
						link = oNode.GetAttribute(L"r:embed");

						XmlUtils::CXmlNode trim = oNode.ReadNodeNoNS(L"trim");
						if (trim.IsValid())
						{
							XmlMacroReadAttributeBase(trim, L"st", st);
							XmlMacroReadAttributeBase(trim, L"end", end);
						}
					}
					else if (L"compatExt" == strName)
					{
						spid = oNode.GetAttribute(L"spid");
					}
					else if (L"sectionLst" == strName)
					{
						sectionLst = oNode;
					}
				}
			}
		}
		std::wstring Ext::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			XmlUtils::CNodeValue oValue;

			return XmlUtils::CreateNode(L"p:ext", oValue);
		}
		void Ext::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_ext = L"p";
			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)	namespace_ext= L"a";

			if (link.IsInit())
			{
				std::wstring namespace_link = L"p14";
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
					pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)	namespace_link= L"wp15";

				pWriter->StartNode(namespace_ext + L":ext");
					pWriter->StartAttributes();
					pWriter->WriteAttribute(L"uri", std::wstring(L"{DAA4B4D4-6D71-4841-9C94-3DE7FCFB9230}"));
					pWriter->EndAttributes();

					pWriter->StartNode(namespace_link + L":media");
						pWriter->StartAttributes();
						if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
							pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)
							pWriter->WriteAttribute(L"xmlns:wp15", std::wstring(L"http://schemas.microsoft.com/office/word/2012/wordprocessingDrawing"));
						else
							pWriter->WriteAttribute(L"xmlns:p14", std::wstring(L"http://schemas.microsoft.com/office/powerpoint/2010/main"));
						pWriter->WriteAttribute(L"r:embed", link->get());
						pWriter->EndAttributes();
					pWriter->EndNode(namespace_link + L":media");
				pWriter->EndNode(namespace_ext + L":ext");
			}
			if (sectionLst.IsInit())
			{
				pWriter->StartNode(namespace_ext + L":ext");
					pWriter->StartAttributes();
					pWriter->WriteAttribute(L"uri", std::wstring(L"{521415D9-36F7-43E2-AB2F-B90AF26B5E84}"));
					pWriter->EndAttributes();

					sectionLst->toXmlWriter(pWriter);
				pWriter->EndNode(namespace_ext + L":ext");
			}
		}
		void Ext::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			if (link.IsInit())
				link->toPPTY(0, pWriter);
			pWriter->WriteDouble2(1, st);
			pWriter->WriteDouble2(2, end);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Ext::FillParentPointersForChilds()
		{
		}
	} 
} 
