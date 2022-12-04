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

#include "CNvGraphicFramePr.h"

namespace PPTX
{
	namespace Logic
	{
		CNvGraphicFramePr::CNvGraphicFramePr(std::wstring ns)
		{
			m_namespace = ns;
		}
		CNvGraphicFramePr& CNvGraphicFramePr::operator=(const CNvGraphicFramePr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			noChangeAspect	= oSrc.noChangeAspect;
			noDrilldown		= oSrc.noDrilldown;
			noGrp			= oSrc.noGrp;
			noMove			= oSrc.noMove;
			noResize		= oSrc.noResize;
			noSelect		= oSrc.noSelect;

			m_namespace		= oSrc.m_namespace;

			return *this;
		}
		void CNvGraphicFramePr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (strName == L"graphicFrameLocks")
				{
					ReadAttributesLocks(oReader);
				}
			}
		}
		void CNvGraphicFramePr::ReadAttributesLocks(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("noChangeAspect"),	noChangeAspect)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noGrp"),	noGrp)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noMove"), noMove)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noResize"), noResize)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noDrilldown"), noDrilldown)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noSelect"), noSelect)
			WritingElement_ReadAttributes_End( oReader )
		}
		void CNvGraphicFramePr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlUtils::CXmlNode oNode = node.ReadNodeNoNS(L"graphicFrameLocks");
			if (oNode.IsValid())
			{
				XmlMacroReadAttributeBase(oNode, L"noChangeAspect",	noChangeAspect);
				XmlMacroReadAttributeBase(oNode, L"noDrilldown",		noDrilldown);
				XmlMacroReadAttributeBase(oNode, L"noGrp",			noGrp);
				XmlMacroReadAttributeBase(oNode, L"noMove",			noMove);
				XmlMacroReadAttributeBase(oNode, L"noResize",		noResize);
				XmlMacroReadAttributeBase(oNode, L"noSelect",		noSelect);
			}
		}
		std::wstring CNvGraphicFramePr::toXML() const
		{
			std::wstring namespaceLocks = L"a";
			std::wstring namespaceLocksLink = PPTX::g_Namespaces.a.m_strLink;
			//if (m_namespace == L"wp") namespaceLocks = L"wp";

			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("noChangeAspect"),	noChangeAspect);
			oAttr.Write(_T("noDrilldown"),		noDrilldown);
			oAttr.Write(_T("noGrp"),			noGrp);
			oAttr.Write(_T("noMove"),			noMove);
			oAttr.Write(_T("noResize"),			noResize);
			oAttr.Write(_T("noSelect"),			noSelect);

			bool isAttrEmpty = oAttr.m_strValue.empty();
			oAttr.Write(_T("xmlns:") + namespaceLocks, namespaceLocksLink);

			return XmlUtils::CreateNode(m_namespace + L":cNvGraphicFramePr", isAttrEmpty ? L"" : XmlUtils::CreateNode(namespaceLocks + L":graphicFrameLocks", oAttr));
		}
		void CNvGraphicFramePr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_		= m_namespace;
			std::wstring namespaceLock_ = L"a";
			std::wstring namespaceLockLink_ = PPTX::g_Namespaces.a.m_strLink;

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)	namespace_ = L"xdr";
			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)
			{
				namespaceLock_	= L"a";
				namespaceLockLink_ = PPTX::g_Namespaces.a.m_strLink;
				namespace_		= L"wp";
			}
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		namespace_ = L"a";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			namespace_ = L"dgm";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		namespace_ = L"dsp";

			pWriter->StartNode(namespace_ + L":cNvGraphicFramePr");

			pWriter->EndAttributes();

			pWriter->StartNode(namespaceLock_ + L":graphicFrameLocks");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("xmlns:") + namespaceLock_, namespaceLockLink_);
			pWriter->WriteAttribute(_T("noChangeAspect"), noChangeAspect);
			pWriter->WriteAttribute(_T("noDrilldown"), noDrilldown);
			pWriter->WriteAttribute(_T("noGrp"), noGrp);
			pWriter->WriteAttribute(_T("noMove"), noMove);
			pWriter->WriteAttribute(_T("noResize"), noResize);
			pWriter->WriteAttribute(_T("noSelect"), noSelect);

			pWriter->EndAttributes();

			pWriter->EndNode(namespaceLock_ + L":graphicFrameLocks");

			pWriter->EndNode(namespace_ + L":cNvGraphicFramePr");
		}
		void CNvGraphicFramePr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, noChangeAspect);
			pWriter->WriteBool2(1, noDrilldown);
			pWriter->WriteBool2(2, noGrp);
			pWriter->WriteBool2(3, noMove);
			pWriter->WriteBool2(4, noResize);
			pWriter->WriteBool2(5, noSelect);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CNvGraphicFramePr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						noChangeAspect = pReader->GetBool();
						break;
					}
					case 1:
					{
						noDrilldown = pReader->GetBool();
						break;
					}
					case 2:
					{
						noGrp = pReader->GetBool();
						break;
					}
					case 3:
					{
						noMove = pReader->GetBool();
						break;
					}
					case 4:
					{
						noResize = pReader->GetBool();
						break;
					}
					case 5:
					{
						noSelect = pReader->GetBool();
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void CNvGraphicFramePr::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
