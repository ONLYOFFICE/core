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

#include "ContentPart.h"
#include "../../DocxFormat/CustomXml.h"
#include "../../../DesktopEditor/common/Directory.h"

namespace PPTX
{
	namespace Logic
	{
		ContentPart::ContentPart()
		{
		}
		ContentPart& ContentPart::operator=(const ContentPart& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			m_namespace		= oSrc.m_namespace;
			xfrm			= oSrc.xfrm;
			nvPr			= oSrc.nvPr;
			nvContentPartPr	= oSrc.nvContentPartPr;
			id				= oSrc.id;

			return *this;
		}
		OOX::EElementType ContentPart::getType () const
		{
			return OOX::et_ContentPart;
		}
		void ContentPart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"xfrm" == sName) 
					xfrm = oReader;
				else if (L"nvContentPartPr" == sName)
					nvContentPartPr = oReader;
				else if (L"nvPr" == sName)
					nvPr = oReader;
			}
		}
		void ContentPart::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());
			XmlMacroReadAttributeBase(node, L"r:id", id);

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];
					std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"xfrm" == sName)
						xfrm = oNode;
					else if (L"nvContentPartPr" == sName)
						nvContentPartPr = oNode;
					else if (L"nvPr" == sName)
						nvPr = oNode;
				}
			}
		}
		void ContentPart::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (false == id.IsInit()) return;

			pWriter->StartRecord(SPTREE_TYPE_CONTENT_PART);

			pWriter->WriteRecord2(0, xfrm);
			pWriter->WriteRecord2(1, nvContentPartPr);
			pWriter->WriteRecord2(2, nvPr);

			OOX::IFileContainer* pDocumentRels = pWriter->GetRelsPtr();
			{
				smart_ptr<OOX::File> oFile;

				if (parentFileIs<OOX::IFileContainer>()) oFile = parentFileAs<OOX::IFileContainer>().Find(*id);
				else if (pDocumentRels != NULL) oFile = pDocumentRels->Find(id->get());

				OOX::CCustomXML * pCustomXML = dynamic_cast<OOX::CCustomXML*>(oFile.GetPointer());

				if (pCustomXML)
				{
					pWriter->SetRelsPtr(dynamic_cast<OOX::IFileContainer*>(oFile.GetPointer())); //??

					pWriter->StartRecord(3);
					pCustomXML->toPPTY(pWriter);
					pWriter->EndRecord();
				}
			}

			pWriter->SetRelsPtr(pDocumentRels);
			pWriter->EndRecord();
		}
		void ContentPart::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{			
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						xfrm.Init();
						xfrm->fromPPTY(pReader);
					}break;
					case 1:
					{
						nvContentPartPr.Init();
						nvContentPartPr->fromPPTY(pReader);
					}break;
					case 2:
					{
						nvPr.Init();
						nvPr->fromPPTY(pReader);
					}break;
					case 3:
					{
						pReader->Skip(4); // len
						smart_ptr<OOX::CInk> pInk = new OOX::CInk(NULL, pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);
						pInk->fromPPTY(pReader);

						if (pReader->m_pRels && pReader->m_pRels->m_pManager && pReader->m_pRels->m_pManager->m_pContentTypes)
						{
							std::wstring sMainFolder = L"/ppt/ink/";
							if (pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX ||
								pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)	sMainFolder = L"/word/ink/";
							else if (pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_XLSX)		sMainFolder = L"/xl/ink/";

							OOX::CPath sInkPath = pReader->m_pRels->m_pManager->GetDstFolder() + FILE_SEPARATOR_STR + L"ink";
							NSDirectory::CreateDirectory(sInkPath.GetPath());

							int nIndex = 1;
							while (NSFile::CFileBinary::Exists(sInkPath.GetPath() + FILE_SEPARATOR_STR + L"ink" + std::to_wstring(nIndex) + L".xml"))
								++nIndex;
							std::wstring sInkFileName = L"ink" + std::to_wstring(nIndex) + L".xml";

							OOX::CPath oInkRegPath = sMainFolder;
							pInk->write(sInkPath + FILE_SEPARATOR_STR + sInkFileName, oInkRegPath, *pReader->m_pRels->m_pManager->m_pContentTypes);

							std::wstring sInkRelsPath = (pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX) ? L"ink/" : L"../ink/";
							unsigned int nRId = pReader->m_pRels->WriteRels(OOX::FileTypes::Ink.RelationType(), sInkRelsPath + sInkFileName, L"");
							id = OOX::RId(nRId);
						}
						else if (OOX::IFileContainer *pRels = pReader->GetRelsPtr())
						{
							smart_ptr<OOX::File> pFile = pInk.smart_dynamic_cast<OOX::File>();
							id = pRels->Add(pFile);
						}
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		std::wstring ContentPart::toXML() const
		{
			if (!id.is_init())
				return L"";

			std::wstring name = (m_namespace.empty() ? L"" : m_namespace + L":") + L"contentPart";

			std::wstring strData = L"<" + name;
			strData += L" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"";
			strData += L" r:id=\"" + id->get() + L"\"";
			strData += L">";

			if (xfrm.IsInit())
				strData += xfrm->toXML();

			strData += L"</" + name + L">";
			return strData;
		}
		void ContentPart::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (id.IsInit() == false) return;

			std::wstring namespace_ = L"p", namespace_child = L"p14";

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)
			{
				namespace_child = L"w14";
				namespace_ = L"w14"; 
			}
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
			{
				namespace_child = L"xdr14";
				namespace_ = L"xdr";
			}

			pWriter->StartNode(namespace_ + L":contentPart");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			pWriter->WriteAttribute(L"r:id", id->get());
			pWriter->EndAttributes();

			if (nvContentPartPr.IsInit())
			{
				nvContentPartPr->toXmlWriter2(namespace_child, namespace_child == L"p14", pWriter);
			}
			if (nvPr.IsInit())
			{
				nvPr->toXmlWriter2(namespace_child, pWriter);
			}
			//else if (namespace_child == L"xdr14")
			//{
			//	pWriter->WriteString(L"<xdr14:nvPr macro=\"\"/>");
			//}
			if (xfrm.IsInit())
			{
				xfrm->toXmlWriter2(namespace_child, pWriter);
			}
			pWriter->EndNode(namespace_ + L":contentPart");
		}
		void ContentPart::FillParentPointersForChilds()
		{
		}
		void ContentPart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"r:id", id)
			WritingElement_ReadAttributes_End(oReader)
		}
//-------------------------------------------------------------------------------------------------------------
		CNvContentPartPr::CNvContentPartPr(std::wstring ns)
		{
			m_namespace = ns;
		}
		CNvContentPartPr& CNvContentPartPr::operator=(const CNvContentPartPr& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			isComment = oSrc.isComment;

			noAdjustHandles = oSrc.noAdjustHandles;
			noChangeArrowheads = oSrc.noChangeArrowheads;
			noChangeAspect = oSrc.noChangeAspect;
			noChangeShapeType = oSrc.noChangeShapeType;
			noEditPoints = oSrc.noEditPoints;
			noGrp = oSrc.noGrp;
			noMove = oSrc.noMove;
			noResize = oSrc.noResize;
			noRot = oSrc.noRot;
			noSelect = oSrc.noSelect;

			m_namespace = oSrc.m_namespace;

			return *this;
		}
		void CNvContentPartPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_ReadSingle(oReader, L"isComment", isComment)
			WritingElement_ReadAttributes_End(oReader)
		}
		void CNvContentPartPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"cpLocks" == strName)
				{
					ReadAttributesLocks(oReader);
				}
			}
		}
		void CNvContentPartPr::ReadAttributesLocks(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"noAdjustHandles", noAdjustHandles)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"noChangeArrowheads", noChangeArrowheads)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"noChangeAspect", noChangeAspect)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"noChangeShapeType", noChangeShapeType)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"noEditPoints", noEditPoints)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"noGrp", noGrp)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"noMove", noMove)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"noResize", noResize)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"noRot", noRot)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"noSelect", noSelect)
			WritingElement_ReadAttributes_End(oReader)
		}
		void CNvContentPartPr::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"isComment", isComment);

			XmlUtils::CXmlNode oNode;
			if (node.GetNode(L"a14:cpLocks", oNode))
			{
				XmlMacroReadAttributeBase(oNode, L"noAdjustHandles", noAdjustHandles);
				XmlMacroReadAttributeBase(oNode, L"noChangeArrowheads", noChangeArrowheads);
				XmlMacroReadAttributeBase(oNode, L"noChangeAspect", noChangeAspect);
				XmlMacroReadAttributeBase(oNode, L"noChangeShapeType", noChangeShapeType);
				XmlMacroReadAttributeBase(oNode, L"noEditPoints", noEditPoints);
				XmlMacroReadAttributeBase(oNode, L"noGrp", noGrp);
				XmlMacroReadAttributeBase(oNode, L"noMove", noMove);
				XmlMacroReadAttributeBase(oNode, L"noResize", noResize);
				XmlMacroReadAttributeBase(oNode, L"noRot", noRot);
				XmlMacroReadAttributeBase(oNode, L"noSelect", noSelect);
			}
		}
		std::wstring CNvContentPartPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"noAdjustHandles", noAdjustHandles);
			oAttr.Write(L"noChangeAspect", noChangeAspect);
			oAttr.Write(L"noChangeArrowheads", noChangeArrowheads);
			oAttr.Write(L"noChangeShapeType", noChangeShapeType);
			oAttr.Write(L"noEditPoints", noEditPoints);
			oAttr.Write(L"noGrp", noGrp);
			oAttr.Write(L"noMove", noMove);
			oAttr.Write(L"noResize", noResize);
			oAttr.Write(L"noRot", noRot);
			oAttr.Write(L"noSelect", noSelect);

			XmlUtils::CAttribute oAttr2;
			oAttr2.Write(L"isComment", isComment);

			if (L"" == oAttr.m_strValue)
			{
				return XmlUtils::CreateNode(m_namespace + L":cNvContentPartPr", oAttr2);
			}
			oAttr.Write(L"xmlns:a14", std::wstring(L"http://schemas.microsoft.com/office/drawing/2010/main"));

			return XmlUtils::CreateNode(m_namespace + L":cNvContentPartPr", oAttr2, XmlUtils::CreateNode(L"a14:cpLocks", oAttr));
		}
		void CNvContentPartPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_ = m_namespace;

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)		namespace_ = L"w14";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			namespace_ = L"xdr14";
			else																namespace_ = L"p14";

			toXmlWriter2(namespace_, pWriter);
		}
		void CNvContentPartPr::toXmlWriter2(const std::wstring& namespace_, NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(namespace_ + L":cNvContentPartPr");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"isComment", isComment);
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
				noSelect.is_init())
			{
				pWriter->StartNode(L"a14:cpLocks");
				pWriter->StartAttributes();
				pWriter->WriteAttribute(L"xmlns:a14", L"http://schemas.microsoft.com/office/drawing/2010/main");
				pWriter->WriteAttribute(L"noAdjustHandles", noAdjustHandles);
				pWriter->WriteAttribute(L"noChangeAspect", noChangeAspect);
				pWriter->WriteAttribute(L"noChangeArrowheads", noChangeArrowheads);
				pWriter->WriteAttribute(L"noChangeShapeType", noChangeShapeType);
				pWriter->WriteAttribute(L"noEditPoints", noEditPoints);
				pWriter->WriteAttribute(L"noGrp", noGrp);
				pWriter->WriteAttribute(L"noMove", noMove);
				pWriter->WriteAttribute(L"noResize", noResize);
				pWriter->WriteAttribute(L"noRot", noRot);
				pWriter->WriteAttribute(L"noSelect", noSelect);
				pWriter->EndAttributes();
				pWriter->EndNode(L"a14:cpLocks");
			}
			pWriter->EndNode(namespace_ + L":cNvContentPartPr");
		}
		void CNvContentPartPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, isComment);
			pWriter->WriteBool2(1, noAdjustHandles);
			pWriter->WriteBool2(2, noChangeArrowheads);
			pWriter->WriteBool2(3, noChangeAspect);
			pWriter->WriteBool2(4, noChangeShapeType);
			pWriter->WriteBool2(6, noEditPoints);
			pWriter->WriteBool2(7, noGrp);
			pWriter->WriteBool2(8, noMove);
			pWriter->WriteBool2(9, noResize);
			pWriter->WriteBool2(10, noRot);
			pWriter->WriteBool2(11, noSelect);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CNvContentPartPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					isComment = pReader->GetBool();					
				}break;
				case 1:
				{
					noAdjustHandles = pReader->GetBool();					
				}break;
				case 2:
				{
					noChangeArrowheads = pReader->GetBool();					
				}break;
				case 3:
				{
					noChangeAspect = pReader->GetBool();					
				}break;
				case 4:
				{
					noChangeShapeType = pReader->GetBool();					
				}break;
				case 6:
				{
					noEditPoints = pReader->GetBool();					
				}break;
				case 7:
				{
					noGrp = pReader->GetBool();					
				}break;
				case 8:
				{
					noMove = pReader->GetBool();					
				}break;
				case 9:
				{
					noResize = pReader->GetBool();					
				}break;
				case 10:
				{
					noRot = pReader->GetBool();					
				}break;
				case 11:
				{
					noSelect = pReader->GetBool();					
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CNvContentPartPr::FillParentPointersForChilds()
		{
		}
//---------------------------------------------------------------------------------------------
		NvContentPartPr::NvContentPartPr(std::wstring ns)
		{
			m_namespace = ns;
		}
		NvContentPartPr& NvContentPartPr::operator=(const NvContentPartPr& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			cNvPr = oSrc.cNvPr;
			cNvContentPartPr = oSrc.cNvContentPartPr;
			nvPr = oSrc.nvPr;

			m_namespace = oSrc.m_namespace;

			return *this;
		}
		void NvContentPartPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("cNvPr") == strName)
					cNvPr.fromXML(oReader);
				else if (_T("cNvContentPartPr") == strName)
					cNvContentPartPr.fromXML(oReader);
				else if (_T("nvPr") == strName)
					nvPr.fromXML(oReader);
			}
		}
		void NvContentPartPr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t nCount = oNodes.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("cNvPr") == strName)
						cNvPr = oNode;
					else if (_T("cNvContentPartPr") == strName)
						cNvContentPartPr = oNode;
					else if (_T("nvPr") == strName)
						nvPr = oNode;
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring NvContentPartPr::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(cNvPr);
			oValue.Write(cNvContentPartPr);
			oValue.Write(nvPr);

			return XmlUtils::CreateNode(m_namespace + L":nvContentPartPr", oValue);
		}
		void NvContentPartPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_ = m_namespace;

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)		namespace_ = L"w14";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			namespace_ = L"xdr14";
			else																namespace_ = L"p14";

			toXmlWriter2(namespace_, namespace_ == L"p14", pWriter);
		}
		void NvContentPartPr::toXmlWriter2(const std::wstring& namespace_, bool bWriteNvPr, NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(namespace_ + L":nvContentPartPr");
			pWriter->EndAttributes();

			if (namespace_ != L"w14" || false == cNvPr.name.empty())
				cNvPr.toXmlWriter2(namespace_, pWriter);
			cNvContentPartPr.toXmlWriter2(namespace_, pWriter);

			if (bWriteNvPr)
				nvPr.toXmlWriter2(namespace_, pWriter);

			pWriter->EndNode(namespace_ + L":nvContentPartPr");
		}
		void NvContentPartPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, cNvPr);
			pWriter->WriteRecord1(1, cNvContentPartPr);
			pWriter->WriteRecord1(2, nvPr);
		}
		void NvContentPartPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					cNvPr.fromPPTY(pReader);
					break;
				}
				case 1:
				{
					cNvContentPartPr.fromPPTY(pReader);
					break;
				}
				case 2:
				{
					nvPr.fromPPTY(pReader);
					break;
				}
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void NvContentPartPr::FillParentPointersForChilds()
		{
			cNvPr.SetParentPointer(this);
			cNvContentPartPr.SetParentPointer(this);
			nvPr.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
