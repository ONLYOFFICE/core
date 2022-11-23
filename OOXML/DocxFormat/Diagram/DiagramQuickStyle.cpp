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

#include "DiagramQuickStyle.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"

namespace OOX
{
	CDiagramQuickStyle::CDiagramQuickStyle(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
	{
	}

	CDiagramQuickStyle::CDiagramQuickStyle(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
	{
		read(uri.GetDirectory(), uri);
	}

	CDiagramQuickStyle::CDiagramQuickStyle(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
	{
		read(oRootPath, oPath);
	}

	CDiagramQuickStyle::~CDiagramQuickStyle()
	{
	}

	void CDiagramQuickStyle::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}

	void CDiagramQuickStyle::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		std::wstring sName = oReader.GetName();
		if (L"dgm:styleDef" == sName)
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
						m_arStyleLbl.push_back(new Diagram::CStyleLbl(oReader));
					}
					else if (L"dgm:scene3d" == sName)
					{
						m_oScene3d = oReader;
					}
					else if (L"dgm:extLst" == sName)
					{
						m_oExtLst = oReader;
					}

				}
			}
		}
	}

	void CDiagramQuickStyle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"uniqueId", m_sUniqueId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"minVer", m_sMinVer)
		WritingElement_ReadAttributes_End(oReader)
	}

	void CDiagramQuickStyle::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		
		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_DIAGRAM;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}

	const OOX::FileType CDiagramQuickStyle::type() const
	{
		return FileTypes::DiagramQuickStyle;
	}

	const CPath CDiagramQuickStyle::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const CPath CDiagramQuickStyle::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	void CDiagramQuickStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				m_oTitle.Init(); m_oTitle->node_name = L"dgm:title";
				m_oTitle->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_oDesc.Init(); m_oDesc->node_name = L"dgm:desc";
				m_oDesc->fromPPTY(pReader);
			}break;
			case 2:
			{
				m_oCatLst.Init();
				m_oCatLst->fromPPTY(pReader);
			}break;

			case 3:
			{
				m_oScene3d.Init();  m_oScene3d->m_namespace = L"dgm";
				m_oScene3d->fromPPTY(pReader);
			}break;
			case 4:
			{
				m_arStyleLbl.push_back(new Diagram::CStyleLbl());
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

	void CDiagramQuickStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sUniqueId);
			pWriter->WriteString2(1, m_sMinVer);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oTitle);
		pWriter->WriteRecord2(1, m_oDesc);
		pWriter->WriteRecord2(2, m_oCatLst);
		pWriter->WriteRecord2(3, m_oScene3d);

		for (size_t i = 0; i < m_arStyleLbl.size(); ++i)
			pWriter->WriteRecord2(4, dynamic_cast<OOX::WritingElement*>(m_arStyleLbl[i]));
	}

	void CDiagramQuickStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:styleDef");

		pWriter->StartAttributes();
		pWriter->WriteAttribute(L"xmlns:dgm", L"http://schemas.openxmlformats.org/drawingml/2006/diagram");
		pWriter->WriteAttribute(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");

		pWriter->WriteAttribute(L"uniqueId", m_sUniqueId);
		pWriter->WriteAttribute(L"minVer", m_sMinVer);
		pWriter->EndAttributes();

		if (m_oTitle.IsInit()) m_oTitle->toXmlWriter(pWriter);
		if (m_oDesc.IsInit()) m_oDesc->toXmlWriter(pWriter);
		if (m_oCatLst.IsInit()) m_oCatLst->toXmlWriter(pWriter);
		if (m_oScene3d.IsInit()) m_oScene3d->toXmlWriter(pWriter);

		for (size_t i = 0; i < m_arStyleLbl.size(); ++i)
			m_arStyleLbl[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:styleDef");
	}

//-------------------------------------------------------------------------------------------

	Diagram::CStyleLbl::CStyleLbl() {}

	Diagram::CStyleLbl::~CStyleLbl() {}

	std::wstring Diagram::CStyleLbl::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	void Diagram::CStyleLbl::fromXML(XmlUtils::CXmlNode& node)
	{
	}

	EElementType Diagram::CStyleLbl::getType() const
	{
		return et_dgm_styleLbl;
	}

	void Diagram::CStyleLbl::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"name", m_sName)
		WritingElement_ReadAttributes_End(oReader)
	}

	void Diagram::CStyleLbl::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:scene3d" == sName)
			{
				m_oScene3d = oReader;
			}
			else if (L"dgm:sp3d" == sName)
			{
				m_oSp3d = oReader;
			}
			else if (L"dgm:style" == sName)
			{
				m_oStyle = oReader;
			}
			else if (L"dgm:txPr" == sName)
			{
				m_oTxPr = oReader;
			}
		}
	}

	void Diagram::CStyleLbl::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					m_oScene3d.Init(); m_oScene3d->m_namespace = L"dgm";
					m_oScene3d->fromPPTY(pReader);
				}break;
				case 1:
				{
					m_oSp3d.Init(); m_oSp3d->m_namespace = L"dgm";
					m_oSp3d->fromPPTY(pReader);
				}break;
				case 2:
				{
					m_oStyle.Init(); m_oStyle->m_namespace = L"dgm"; m_oStyle->fromPPTY(pReader);
				}break;
				case 3:
				{
					m_oTxPr.Init(); m_oTxPr->m_name = L"dgm:txPr";  m_oTxPr->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}

	void Diagram::CStyleLbl::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sName);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oScene3d);
		pWriter->WriteRecord2(1, m_oSp3d);
		pWriter->WriteRecord2(2, m_oStyle);
		pWriter->WriteRecord2(3, m_oTxPr);
	}

	void Diagram::CStyleLbl::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:styleLbl");
			pWriter->WriteAttribute(L"name", m_sName);
		pWriter->EndAttributes();

		if (m_oScene3d.IsInit())
			m_oScene3d->toXmlWriter(pWriter);
		if (m_oSp3d.IsInit())
			m_oSp3d->toXmlWriter(pWriter);
		if (m_oTxPr.IsInit())
			m_oTxPr->toXmlWriter(pWriter);
		if (m_oStyle.IsInit())
			m_oStyle->toXmlWriter(pWriter);
		pWriter->WriteNodeEnd(L"dgm:styleLbl");
	}

} // namespace OOX
