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

#include "DiagramDrawing.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"

namespace OOX
{
	CDiagramDrawing::CDiagramDrawing(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
	{
	}

	CDiagramDrawing::CDiagramDrawing(OOX::Document* pMain, const CPath& uri) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
	{
		read(uri.GetDirectory(), uri);
	}

	CDiagramDrawing::CDiagramDrawing(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
	{
		read( oRootPath, oPath );
	}

	CDiagramDrawing::~CDiagramDrawing()
	{
	}

	void CDiagramDrawing::read(const CPath& oFilePath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}

    void CDiagramDrawing::read(const CPath& oRootPath, const CPath& oFilePath)
    {
		m_oReadPath = oFilePath;
        IFileContainer::Read( oRootPath, oFilePath );

		XmlUtils::CXmlNode oNodeDW;
        if (oNodeDW.FromXmlFile(oFilePath.GetPath() ))
		{
			m_oShapeTree = oNodeDW.ReadNodeNoNS(L"spTree");
		}
    }
    void CDiagramDrawing::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
    {
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		
		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_DSP_DRAWING;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}

	const OOX::FileType CDiagramDrawing::type() const
	{
		return FileTypes::DiagramDrawing;
	}
	const CPath CDiagramDrawing::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CDiagramDrawing::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	const CPath& CDiagramDrawing::GetReadPath()
	{
		return m_oReadPath;
	}

	void CDiagramDrawing::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_oShapeTree.Init();
					m_oShapeTree->m_namespace = L"dsp";
					m_oShapeTree->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}
	void CDiagramDrawing::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteRecord2(0, m_oShapeTree);
	}
	void CDiagramDrawing::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dsp:drawing");

		pWriter->StartAttributes();
			pWriter->WriteAttribute(L"xmlns:dsp", L"http://schemas.microsoft.com/office/drawing/2008/diagram");
			pWriter->WriteAttribute(L"xmlns:dgm", L"http://schemas.openxmlformats.org/drawingml/2006/diagram");
			pWriter->WriteAttribute(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
			pWriter->WriteAttribute(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->EndAttributes();

		if (m_oShapeTree.IsInit())
			m_oShapeTree->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dsp:drawing");
	}
} // namespace OOX
