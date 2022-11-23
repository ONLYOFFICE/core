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

#include "DiagramColors.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"

namespace OOX
{
	void CDiagramColors::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		std::wstring sName = oReader.GetName();
		if (L"dgm:colorsDef" == sName)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
			{
				int nNumberingDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nNumberingDepth))
				{
					sName = oReader.GetName();

					if (L"dgm:catLst" == sName)
					{
						m_oCatLst = oReader;
					}
					else if (L"dgm:desc" == sName)
					{
						m_oDesc = oReader;
					}
					else if (L"dgm:title" == sName)
					{
						m_oTitle = oReader;
					}
					else if (L"dgm:styleLbl" == sName)
					{
						m_arStyleLbl.push_back(new Diagram::CColorStyleLbl(oReader));
					}
					else if (L"dgm:extLst" == sName)
					{
						m_oExtLst = oReader;
					}
				}
			}
		}
	}
	void CDiagramColors::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		
		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_DIAGRAM;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}
	void CDiagramColors::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"uniqueId", m_sUniqueId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"minVer", m_sMinVer)
		WritingElement_ReadAttributes_End(oReader)
	}
	void CDiagramColors::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sUniqueId = pReader->GetString2();
			else if (1 == _at)	m_sMinVer = pReader->GetString2();
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_oTitle.Init();
					m_oTitle->node_name = L"dgm:title";
					m_oTitle->fromPPTY(pReader);
				}break;
				case 1:
				{
					m_oDesc.Init();
					m_oDesc->node_name = L"dgm:desc";
					m_oDesc->fromPPTY(pReader);
				}break;
				case 2:
				{
					m_oCatLst.Init();
					m_oCatLst->fromPPTY(pReader);
				}break;
				case 3:
				{
					m_arStyleLbl.push_back(new Diagram::CColorStyleLbl());
					m_arStyleLbl.back()->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}
	void CDiagramColors::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sUniqueId);
			pWriter->WriteString2(1, m_sMinVer);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oTitle);
		pWriter->WriteRecord2(1, m_oDesc);
		pWriter->WriteRecord2(2, m_oCatLst);

		for (size_t i = 0; i < m_arStyleLbl.size(); ++i)
			pWriter->WriteRecord2(3, dynamic_cast<OOX::WritingElement*>(m_arStyleLbl[i]));
	}
	void CDiagramColors::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:colorsDef");
		pWriter->StartAttributes();
			pWriter->WriteAttribute(L"xmlns:dgm", L"http://schemas.openxmlformats.org/drawingml/2006/diagram");
			pWriter->WriteAttribute(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");

			pWriter->WriteAttribute(L"uniqueId", m_sUniqueId);
			pWriter->WriteAttribute(L"minVer", m_sMinVer);
		pWriter->EndAttributes();

		if (m_oTitle.IsInit()) m_oTitle->toXmlWriter(pWriter);
		if (m_oDesc.IsInit()) m_oDesc->toXmlWriter(pWriter);
		
		if (m_oCatLst.IsInit()) m_oCatLst->toXmlWriter(pWriter);

		for (size_t i = 0; i < m_arStyleLbl.size(); ++i)
			m_arStyleLbl[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:colorsDef");
	}
//-------------------------------------------------------------------------------------------
	void Diagram::CClrLst::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"hueDir", m_oHueDir)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"meth", m_oMeth)
		WritingElement_ReadAttributes_End(oReader)
	}
	void Diagram::CClrLst::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			m_arrItems.push_back(new PPTX::Logic::UniColor()); 
			m_arrItems.back()->fromXML(oReader);
		}
	}
	void Diagram::CClrLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)
			{
				m_oHueDir.Init();
				m_oHueDir->SetValueFromByte(pReader->GetUChar());
			}
			else if (1 == _at)
			{
				m_oMeth.Init();
				m_oMeth->SetValueFromByte(pReader->GetUChar());
			}
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
				case 0:
				{
					m_arrItems.push_back(new PPTX::Logic::UniColor());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}
	void Diagram::CClrLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			if (m_oHueDir.IsInit()) pWriter->WriteByte1(0, m_oHueDir->GetValue());
			if (m_oMeth.IsInit()) pWriter->WriteByte1(1, m_oMeth->GetValue());
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void Diagram::CClrLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(node_name);
			if (m_oHueDir.IsInit()) pWriter->WriteAttribute(L"hueDir", m_oHueDir->ToString());
			if (m_oMeth.IsInit()) pWriter->WriteAttribute(L"meth", m_oMeth->ToString());
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
				m_arrItems[i]->toXmlWriter(pWriter);
		}

		pWriter->WriteNodeEnd(node_name);
	}
//-------------------------------------------------------------------------------------------
	void Diagram::CColorStyleLbl::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"name", m_sName)
		WritingElement_ReadAttributes_End(oReader)
	}
	void Diagram::CColorStyleLbl::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:effectClrLst" == sName)
			{
				m_oEffectClrLst = oReader;
			}
			else if (L"dgm:fillClrLst" == sName)
			{
				m_oFillClrLst = oReader;
			}
			else if (L"dgm:linClrLst" == sName)
			{
				m_oLinClrLst = oReader;
			}
			else if (L"dgm:txEffectClrLst" == sName)
			{
				m_oTxEffectClrLst = oReader;
			}
			else if (L"dgm:txFillClrLst" == sName)
			{
				m_oTxFillClrLst = oReader;
			}
			else if (L"dgm:txLinClrLst" == sName)
			{
				m_oTxLinClrLst = oReader;
			}
		}
	}
	void Diagram::CColorStyleLbl::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sName = pReader->GetString2();
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
				case 0:
				{
					m_oEffectClrLst.Init(); m_oEffectClrLst->node_name = L"dgm:effectClrLst"; m_oEffectClrLst->fromPPTY(pReader);
				}break;
				case 1:
				{
					m_oFillClrLst.Init(); m_oFillClrLst->node_name = L"dgm:fillClrLst"; m_oFillClrLst->fromPPTY(pReader);
				}break;
				case 2:
				{
					m_oLinClrLst.Init(); m_oLinClrLst->node_name = L"dgm:linClrLst"; m_oLinClrLst->fromPPTY(pReader);
				}break;
				case 3:
				{
					m_oTxEffectClrLst.Init(); m_oTxEffectClrLst->node_name = L"dgm:txEffectClrLst"; m_oTxEffectClrLst->fromPPTY(pReader);
				}break;
				case 4:
				{
					m_oTxFillClrLst.Init(); m_oTxFillClrLst->node_name = L"dgm:txFillClrLst"; m_oTxFillClrLst->fromPPTY(pReader);
				}break;
				case 5:
				{
					m_oTxLinClrLst.Init(); m_oTxLinClrLst->node_name = L"dgm:txLinClrLst"; m_oTxLinClrLst->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}
	void Diagram::CColorStyleLbl::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sName);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oEffectClrLst);
		pWriter->WriteRecord2(1, m_oFillClrLst);
		pWriter->WriteRecord2(2, m_oLinClrLst);
		pWriter->WriteRecord2(3, m_oTxEffectClrLst);
		pWriter->WriteRecord2(4, m_oTxFillClrLst);
		pWriter->WriteRecord2(5, m_oTxLinClrLst);
	}
	void Diagram::CColorStyleLbl::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:styleLbl");
			pWriter->WriteAttribute(L"name", m_sName);
		pWriter->EndAttributes();
		
		if (m_oFillClrLst.IsInit())
			m_oFillClrLst->toXmlWriter(pWriter);
		if (m_oLinClrLst.IsInit())
			m_oLinClrLst->toXmlWriter(pWriter);
		if (m_oEffectClrLst.IsInit())
			m_oEffectClrLst->toXmlWriter(pWriter);		
		if (m_oTxLinClrLst.IsInit())
			m_oTxLinClrLst->toXmlWriter(pWriter);
		if (m_oTxFillClrLst.IsInit())
			m_oTxFillClrLst->toXmlWriter(pWriter);
		if (m_oTxEffectClrLst.IsInit())
			m_oTxEffectClrLst->toXmlWriter(pWriter);
		pWriter->WriteNodeEnd(L"dgm:styleLbl");
	}
} // namespace OOX
