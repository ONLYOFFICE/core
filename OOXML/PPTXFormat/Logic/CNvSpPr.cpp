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

#include "CNvSpPr.h"

namespace PPTX
{
	namespace Logic
	{
		CNvSpPr::CNvSpPr(std::wstring ns)
		{
			m_namespace = ns;
		}
		CNvSpPr& CNvSpPr::operator=(const CNvSpPr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			txBox				= oSrc.txBox;
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
			noTextEdit			= oSrc.noTextEdit;

			return *this;
		}
		void CNvSpPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes(oReader);

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (strName == L"a:spLocks")
				{
					ReadAttributesLocks(oReader);
				}
			}
		}
		void CNvSpPr::ReadAttributesLocks(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("noAdjustHandles"),	noAdjustHandles)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noGrp"),	noGrp)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noMove"), noMove)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noRot"), noRot)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noResize"), noResize)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noTextEdit"), noTextEdit)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noSelect"), noSelect)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noChangeArrowheads"), noChangeArrowheads)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noChangeShapeType"), noChangeShapeType)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noChangeAspect"), noChangeAspect)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noEditPoints"), noEditPoints)
			WritingElement_ReadAttributes_End( oReader )
		}
		void CNvSpPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("txBox"),	txBox)
			WritingElement_ReadAttributes_End( oReader )
		}
		void CNvSpPr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlMacroReadAttributeBase(node, L"txBox", txBox);

			XmlUtils::CXmlNode l_Locks;
			if (node.GetNode(_T("a:spLocks"), l_Locks))
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
				XmlMacroReadAttributeBase(l_Locks, L"noTextEdit", noTextEdit);
			}
		}
		std::wstring CNvSpPr::toXML() const
		{
			XmlUtils::CAttribute oAttr1;
			oAttr1.Write(_T("txBox"), txBox);

			XmlUtils::CAttribute oAttr2;
			oAttr2.Write(_T("noAdjustHandles"), noAdjustHandles);
			oAttr2.Write(_T("noChangeArrowheads"), noChangeArrowheads);
			oAttr2.Write(_T("noChangeAspect"), noChangeAspect);
			oAttr2.Write(_T("noChangeShapeType"), noChangeShapeType);
			oAttr2.Write(_T("noEditPoints"), noEditPoints);
			oAttr2.Write(_T("noGrp"), noGrp);
			oAttr2.Write(_T("noMove"), noMove);
			oAttr2.Write(_T("noResize"), noResize);
			oAttr2.Write(_T("noRot"), noRot);
			oAttr2.Write(_T("noSelect"), noSelect);
			oAttr2.Write(_T("noTextEdit"), noTextEdit);

			if (_T("") == oAttr2.m_strValue)
				return XmlUtils::CreateNode(m_namespace + L":cNvSpPr", oAttr1);

			return XmlUtils::CreateNode(m_namespace + L":cNvSpPr", oAttr1, XmlUtils::CreateNode(_T("a:spLocks"), oAttr2));
		}
		void CNvSpPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_ = m_namespace;

			if		(pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
					 pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)	namespace_ = L"wps";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			namespace_ = L"xdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		namespace_ = L"a";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			namespace_ = L"dgm";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		namespace_ = L"dsp";

			pWriter->StartNode(namespace_ + L":cNvSpPr");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("txBox"), txBox);
			pWriter->EndAttributes();

			if (noAdjustHandles.is_init() ||
				noChangeArrowheads.is_init() ||
				noChangeAspect.is_init() ||
				noChangeShapeType.is_init() ||
				noEditPoints.is_init() ||
				noGrp.is_init() ||
				noMove.is_init() ||
				noResize.is_init() ||
				noRot.is_init() ||
				noSelect.is_init() ||
				noTextEdit.is_init())
			{
				pWriter->StartNode(_T("a:spLocks"));

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
				pWriter->WriteAttribute(_T("noTextEdit"), noTextEdit);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("a:spLocks"));
			}

			pWriter->EndNode(namespace_ + L":cNvSpPr");
		}
		void CNvSpPr::toXmlWriter2(const std::wstring& strNS, NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(strNS + _T(":cNvSpPr"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("txBox"), txBox);
			pWriter->EndAttributes();

			if (noAdjustHandles.is_init() ||
				noChangeArrowheads.is_init() ||
				noChangeAspect.is_init() ||
				noChangeShapeType.is_init() ||
				noEditPoints.is_init() ||
				noGrp.is_init() ||
				noMove.is_init() ||
				noResize.is_init() ||
				noRot.is_init() ||
				noSelect.is_init() ||
				noTextEdit.is_init())
			{
				pWriter->StartNode(_T("a:spLocks"));

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
				pWriter->WriteAttribute(_T("noTextEdit"), noTextEdit);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("a:spLocks"));
			}

			pWriter->EndNode(strNS + _T(":cNvSpPr"));
		}
		void CNvSpPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, txBox);
			pWriter->WriteBool2(1, noAdjustHandles);
			pWriter->WriteBool2(2, noChangeArrowheads);
			pWriter->WriteBool2(3, noChangeAspect);
			pWriter->WriteBool2(4, noChangeShapeType);
			pWriter->WriteBool2(5, noEditPoints);
			pWriter->WriteBool2(6, noGrp);
			pWriter->WriteBool2(7, noMove);
			pWriter->WriteBool2(8, noResize);
			pWriter->WriteBool2(9, noRot);
			pWriter->WriteBool2(10, noSelect);
			pWriter->WriteBool2(11, noTextEdit);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CNvSpPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						txBox = pReader->GetBool();
						break;
					}
					case 1:
					{
						noAdjustHandles = pReader->GetBool();
						break;
					}
					case 2:
					{
						noChangeArrowheads = pReader->GetBool();
						break;
					}
					case 3:
					{
						noChangeAspect = pReader->GetBool();
						break;
					}
					case 4:
					{
						noChangeShapeType = pReader->GetBool();
						break;
					}
					case 5:
					{
						noEditPoints = pReader->GetBool();
						break;
					}
					case 6:
					{
						noGrp = pReader->GetBool();
						break;
					}
					case 7:
					{
						noMove = pReader->GetBool();
						break;
					}
					case 8:
					{
						noResize = pReader->GetBool();
						break;
					}
					case 9:
					{
						noRot = pReader->GetBool();
						break;
					}
					case 10:
					{
						noSelect = pReader->GetBool();
						break;
					}
					case 11:
					{
						noTextEdit = pReader->GetBool();
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void CNvSpPr::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
