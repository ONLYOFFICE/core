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

#include "CNvPicPr.h"

namespace PPTX
{
	namespace Logic
	{
		CNvPicPr::CNvPicPr(std::wstring ns)
		{
			m_namespace = ns;
		}
		CNvPicPr& CNvPicPr::operator=(const CNvPicPr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			preferRelativeResize	= oSrc.preferRelativeResize;

			noAdjustHandles			= oSrc.noAdjustHandles;
			noChangeArrowheads		= oSrc.noChangeArrowheads;
			noChangeAspect			= oSrc.noChangeAspect;
			noChangeShapeType		= oSrc.noChangeShapeType;
			noCrop					= oSrc.noCrop;
			noEditPoints			= oSrc.noEditPoints;
			noGrp					= oSrc.noGrp;
			noMove					= oSrc.noMove;
			noResize				= oSrc.noResize;
			noRot					= oSrc.noRot;
			noSelect				= oSrc.noSelect;

			m_namespace				= oSrc.m_namespace;

			return *this;
		}
		void CNvPicPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("preferRelativeResize"), preferRelativeResize )
			WritingElement_ReadAttributes_End	( oReader )
		}
		void CNvPicPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("picLocks") == strName)
				{
					ReadAttributesLocks(oReader);
				}
			}
		}
		void CNvPicPr::ReadAttributesLocks(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("noAdjustHandles"),	noAdjustHandles)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noChangeArrowheads"),	noChangeArrowheads)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noChangeAspect"), noChangeAspect)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noChangeShapeType"), noChangeShapeType)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noCrop"), noCrop)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noEditPoints"), noEditPoints)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noGrp"), noGrp)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noMove"), noMove)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noResize"), noResize)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noRot"), noRot)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noSelect"), noSelect)
			WritingElement_ReadAttributes_End( oReader )
		}
		void CNvPicPr::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"preferRelativeResize", preferRelativeResize);

			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("a:picLocks"), oNode))
			{
				XmlMacroReadAttributeBase(oNode, L"noAdjustHandles", noAdjustHandles);
				XmlMacroReadAttributeBase(oNode, L"noChangeArrowheads", noChangeArrowheads);
				XmlMacroReadAttributeBase(oNode, L"noChangeAspect", noChangeAspect);
				XmlMacroReadAttributeBase(oNode, L"noChangeShapeType", noChangeShapeType);
				XmlMacroReadAttributeBase(oNode, L"noCrop", noCrop);
				XmlMacroReadAttributeBase(oNode, L"noEditPoints", noEditPoints);
				XmlMacroReadAttributeBase(oNode, L"noGrp", noGrp);
				XmlMacroReadAttributeBase(oNode, L"noMove", noMove);
				XmlMacroReadAttributeBase(oNode, L"noResize", noResize);
				XmlMacroReadAttributeBase(oNode, L"noRot", noRot);
				XmlMacroReadAttributeBase(oNode, L"noSelect", noSelect);
			}
		}
		std::wstring CNvPicPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("noAdjustHandles"), noAdjustHandles);
			oAttr.Write(_T("noChangeAspect"), noChangeAspect);
			oAttr.Write(_T("noChangeArrowheads"), noChangeArrowheads);
			oAttr.Write(_T("noChangeShapeType"), noChangeShapeType);
			oAttr.Write(_T("noEditPoints"), noEditPoints);
			oAttr.Write(_T("noGrp"), noGrp);
			oAttr.Write(_T("noMove"), noMove);
			oAttr.Write(_T("noResize"), noResize);
			oAttr.Write(_T("noRot"), noRot);
			oAttr.Write(_T("noSelect"), noSelect);
			oAttr.Write(_T("noCrop"), noCrop);

			XmlUtils::CAttribute oAttr2;
			oAttr2.Write(_T("preferRelativeResize"), preferRelativeResize);

			if (_T("") == oAttr.m_strValue)
			{
				return XmlUtils::CreateNode(_T("p:cNvPicPr"), oAttr2);
			}

			return XmlUtils::CreateNode(_T("p:cNvPicPr"), oAttr2, XmlUtils::CreateNode(_T("a:picLocks"), oAttr));
		}
		void CNvPicPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_ = m_namespace;

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)		namespace_ = L"pic";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			namespace_ = L"xdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		namespace_ = L"a";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			namespace_ = L"dgm";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		namespace_ = L"dsp";

			pWriter->StartNode(namespace_ + L":cNvPicPr");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("preferRelativeResize"), preferRelativeResize);
			pWriter->EndAttributes();

			if (noAdjustHandles.is_init() ||
				noChangeAspect.is_init() ||
				noChangeArrowheads.is_init() ||
				noChangeShapeType.is_init() ||
				noEditPoints.is_init() ||
				noGrp.is_init() ||
				noMove.is_init() ||
				noResize.is_init() ||
				noRot.is_init() ||
				noSelect.is_init() ||
				noCrop.is_init())
			{
				pWriter->StartNode(_T("a:picLocks"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("noAdjustHandles"), noAdjustHandles);
				pWriter->WriteAttribute(_T("noChangeAspect"), noChangeAspect);
				pWriter->WriteAttribute(_T("noChangeArrowheads"), noChangeArrowheads);
				pWriter->WriteAttribute(_T("noChangeShapeType"), noChangeShapeType);
				pWriter->WriteAttribute(_T("noEditPoints"), noEditPoints);
				pWriter->WriteAttribute(_T("noGrp"), noGrp);
				pWriter->WriteAttribute(_T("noMove"), noMove);
				pWriter->WriteAttribute(_T("noResize"), noResize);
				pWriter->WriteAttribute(_T("noRot"), noRot);
				pWriter->WriteAttribute(_T("noSelect"), noSelect);
				pWriter->WriteAttribute(_T("noCrop"), noCrop);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:picLocks"));
			}
			pWriter->EndNode(namespace_ + L":cNvPicPr");
		}
		void CNvPicPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, preferRelativeResize);
			pWriter->WriteBool2(1, noAdjustHandles);
			pWriter->WriteBool2(2, noChangeArrowheads);
			pWriter->WriteBool2(3, noChangeAspect);
			pWriter->WriteBool2(4, noChangeShapeType);
			pWriter->WriteBool2(5, noCrop);
			pWriter->WriteBool2(6, noEditPoints);
			pWriter->WriteBool2(7, noGrp);
			pWriter->WriteBool2(8, noMove);
			pWriter->WriteBool2(9, noResize);
			pWriter->WriteBool2(10, noRot);
			pWriter->WriteBool2(11, noSelect);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CNvPicPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						preferRelativeResize = pReader->GetBool();
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
						noCrop = pReader->GetBool();
						break;
					}
					case 6:
					{
						noEditPoints = pReader->GetBool();
						break;
					}
					case 7:
					{
						noGrp = pReader->GetBool();
						break;
					}
					case 8:
					{
						noMove = pReader->GetBool();
						break;
					}
					case 9:
					{
						noResize = pReader->GetBool();
						break;
					}
					case 10:
					{
						noRot = pReader->GetBool();
						break;
					}
					case 11:
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
		void CNvPicPr::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
