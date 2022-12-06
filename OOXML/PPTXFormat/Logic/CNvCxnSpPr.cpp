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

#include "CNvCxnSpPr.h"

namespace PPTX
{
	namespace Logic
	{
		CNvCxnSpPr::CNvCxnSpPr(std::wstring ns)
		{
			m_namespace = ns;
		}
		CNvCxnSpPr& CNvCxnSpPr::operator=(const CNvCxnSpPr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			noAdjustHandles		= oSrc.noAdjustHandles;
			noChangeArrowheads	= oSrc.noChangeArrowheads;
			noChangeAspect		= oSrc.noChangeAspect;
			noChangeShapeType	= oSrc.noChangeShapeType;
			noEditPoints		= oSrc.noEditPoints;
			noGrp				= oSrc.noGrp;
			noMove				= oSrc.noMove;
			noResize			= oSrc.noResize;
			noRot				= oSrc.noRot;
			noSelect			= oSrc.noSelect;

			stCxn_id			= oSrc.stCxn_id;
			stCxn_idx			= oSrc.stCxn_idx;
			endCxn_id			= oSrc.endCxn_id;
			endCxn_idx			= oSrc.endCxn_idx;

			return *this;
		}
		void CNvCxnSpPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (strName == L"a:cxnSpLocks")
				{
					ReadAttributesLocks(oReader);
				}
				else if (strName == L"a:stCxn")
				{
					ReadAttributes(oReader, stCxn_id, stCxn_idx);
				}
				else if (strName == L"a:endCxn")
				{
					ReadAttributes(oReader, endCxn_id, endCxn_idx);
				}
			}
		}
		void CNvCxnSpPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader, nullable_int & id, nullable_int & idx )
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("id"),	id)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("idx"),	idx)
			WritingElement_ReadAttributes_End( oReader )
		}
		void CNvCxnSpPr::ReadAttributesLocks(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("noAdjustHandles"),	noAdjustHandles)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noChangeArrowheads"),	noChangeArrowheads)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noChangeAspect"), noChangeAspect)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noChangeShapeType"), noChangeShapeType)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noEditPoints"), noEditPoints)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noGrp"), noGrp)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noMove"), noMove)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noResize"), noResize)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noRot"), noRot)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noSelect"), noSelect)
			WritingElement_ReadAttributes_End( oReader )
		}
		void CNvCxnSpPr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlUtils::CXmlNode l_Locks;
			if (node.GetNode(_T("a:cxnSpLocks"), l_Locks))
			{
				XmlMacroReadAttributeBase(l_Locks, L"noAdjustHandles", noAdjustHandles);
				XmlMacroReadAttributeBase(l_Locks, L"noChangeArrowheads", noChangeArrowheads);
				XmlMacroReadAttributeBase(l_Locks, L"noChangeAspect", noChangeAspect);
				XmlMacroReadAttributeBase(l_Locks, L"noChangeShapeType", noChangeShapeType);
				XmlMacroReadAttributeBase(l_Locks, L"noEditPoints", noEditPoints);
				XmlMacroReadAttributeBase(l_Locks, L"noGrp", noGrp);
				XmlMacroReadAttributeBase(l_Locks, L"noMove", noMove);
				XmlMacroReadAttributeBase(l_Locks, L"noResize", noResize);
				XmlMacroReadAttributeBase(l_Locks, L"noRot", noRot);
				XmlMacroReadAttributeBase(l_Locks, L"noSelect", noSelect);
			}

			XmlUtils::CXmlNode l_Cxn;
			if (node.GetNode(_T("a:stCxn"), l_Cxn))
			{
				XmlMacroReadAttributeBase(l_Cxn, L"id", stCxn_id);
				XmlMacroReadAttributeBase(l_Cxn, L"idx", stCxn_idx);
			}

			XmlUtils::CXmlNode l_endCxn;
			if (node.GetNode(_T("a:endCxn"), l_endCxn))
			{
				XmlMacroReadAttributeBase(l_endCxn, L"id", endCxn_id);
				XmlMacroReadAttributeBase(l_endCxn, L"idx", endCxn_idx);
			}
		}
		std::wstring CNvCxnSpPr::toXML() const
		{
			XmlUtils::CAttribute oAttr1;
			oAttr1.Write(_T("noAdjustHandles"), noAdjustHandles);
			oAttr1.Write(_T("noChangeArrowheads"), noChangeArrowheads);
			oAttr1.Write(_T("noChangeAspect"), noChangeAspect);
			oAttr1.Write(_T("noChangeShapeType"), noChangeShapeType);
			oAttr1.Write(_T("noEditPoints"), noEditPoints);
			oAttr1.Write(_T("noGrp"), noGrp);
			oAttr1.Write(_T("noMove"), noMove);
			oAttr1.Write(_T("noResize"), noResize);
			oAttr1.Write(_T("noRot"), noRot);
			oAttr1.Write(_T("noSelect"), noSelect);

			XmlUtils::CAttribute oAttr2;
			oAttr2.Write(_T("id"), stCxn_id);
			oAttr2.Write(_T("idx"), stCxn_idx);

			XmlUtils::CAttribute oAttr3;
			oAttr3.Write(_T("id"), endCxn_id);
			oAttr3.Write(_T("idx"), endCxn_idx);

			XmlUtils::CNodeValue oValue;
			oValue.m_strValue += XmlUtils::CreateNode(_T("a:cxnSpLocks"), oAttr1);

			if (_T("") != oAttr2.m_strValue)
				oValue.m_strValue += XmlUtils::CreateNode(_T("a:stCxn"), oAttr2);
			if (_T("") != oAttr3.m_strValue)
				oValue.m_strValue += XmlUtils::CreateNode(_T("a:endCxn"), oAttr3);

			return XmlUtils::CreateNode(m_namespace + L":cNvCxnSpPr", oValue);
		}
		void CNvCxnSpPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_ = m_namespace;

			if		(pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
					 pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)	namespace_ = L"wps";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			namespace_ = L"xdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		namespace_ = L"a";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			namespace_ = L"dgm";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		namespace_ = L"dsp";

			pWriter->StartNode(namespace_ + L":cNvCxnSpPr");

			pWriter->EndAttributes();

			pWriter->StartNode(_T("a:cxnSpLocks"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("noAdjustHandles"), noAdjustHandles);
			pWriter->WriteAttribute(_T("noChangeArrowheads"), noChangeArrowheads);
			pWriter->WriteAttribute(_T("noChangeAspect"), noChangeAspect);
			pWriter->WriteAttribute(_T("noChangeShapeType"), noChangeShapeType);
			pWriter->WriteAttribute(_T("noEditPoints"), noEditPoints);
			pWriter->WriteAttribute(_T("noGrp"), noGrp);
			pWriter->WriteAttribute(_T("noMove"), noMove);
			pWriter->WriteAttribute(_T("noResize"), noResize);
			pWriter->WriteAttribute(_T("noRot"), noRot);
			pWriter->WriteAttribute(_T("noSelect"), noSelect);
			pWriter->EndAttributes();
			pWriter->EndNode(_T("a:cxnSpLocks"));

			if (stCxn_id.is_init() || stCxn_idx.is_init())
			{
				pWriter->StartNode(_T("a:stCxn"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("id"), stCxn_id);
				pWriter->WriteAttribute(_T("idx"), stCxn_idx);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:stCxn"));
			}

			if (endCxn_id.is_init() || endCxn_idx.is_init())
			{
				pWriter->StartNode(_T("a:endCxn"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("id"), endCxn_id);
				pWriter->WriteAttribute(_T("idx"), endCxn_idx);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:endCxn"));
			}

			pWriter->EndNode(namespace_ + L":cNvCxnSpPr");
		}
		void CNvCxnSpPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, noAdjustHandles);
			pWriter->WriteBool2(1, noChangeArrowheads);
			pWriter->WriteBool2(2, noChangeAspect);
			pWriter->WriteBool2(3, noChangeShapeType);
			pWriter->WriteBool2(4, noEditPoints);
			pWriter->WriteBool2(5, noGrp);
			pWriter->WriteBool2(6, noMove);
			pWriter->WriteBool2(7, noResize);
			pWriter->WriteBool2(8, noRot);
			pWriter->WriteBool2(9, noSelect);

			pWriter->WriteInt2(10, stCxn_id);
			pWriter->WriteInt2(11, stCxn_idx);
			pWriter->WriteInt2(12, endCxn_id);
			pWriter->WriteInt2(13, endCxn_idx);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CNvCxnSpPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						noAdjustHandles = pReader->GetBool();
						break;
					}
					case 1:
					{
						noChangeArrowheads = pReader->GetBool();
						break;
					}
					case 2:
					{
						noChangeAspect = pReader->GetBool();
						break;
					}
					case 3:
					{
						noChangeShapeType = pReader->GetBool();
						break;
					}
					case 4:
					{
						noEditPoints = pReader->GetBool();
						break;
					}
					case 5:
					{
						noGrp = pReader->GetBool();
						break;
					}
					case 6:
					{
						noMove = pReader->GetBool();
						break;
					}
					case 7:
					{
						noResize = pReader->GetBool();
						break;
					}
					case 8:
					{
						noRot = pReader->GetBool();
						break;
					}
					case 9:
					{
						noSelect = pReader->GetBool();
						break;
					}
					case 10:
					{
						stCxn_id = pReader->GetLong();
						break;
					}
					case 11:
					{
						stCxn_idx = pReader->GetLong();
						break;
					}
					case 12:
					{
						endCxn_id = pReader->GetLong();
						break;
					}
					case 13:
					{
						endCxn_idx = pReader->GetLong();
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void CNvCxnSpPr::FillParentPointersForChilds()
		{
		}
		void CNvCxnSpPr::Normalize()
		{
			stCxn_id.normalize_positive();
			stCxn_idx.normalize_positive();
			endCxn_id.normalize_positive();
			endCxn_idx.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX
