/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "VisioOthers.h"
#include "Shapes.h"
#include "../../DesktopEditor/common/SystemUtils.h"
#include "../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../Binary/Presentation/XmlWriter.h"

namespace OOX
{
namespace Draw
{
	OOX::Draw::CSolutionFile::CSolutionFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	OOX::Draw::CSolutionFile::CSolutionFile(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain)
	{
		read(uri.GetDirectory(), uri);
	}
	OOX::Draw::CSolutionFile::CSolutionFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	OOX::Draw::CSolutionFile::~CSolutionFile()
	{
	}
	void OOX::Draw::CSolutionFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void OOX::Draw::CSolutionFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		NSFile::CFileBinary::ReadAllTextUtf8A(oFilePath.GetPath(), m_sXmlA);
	}
	void OOX::Draw::CSolutionFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSFile::CFileBinary oFile;
		if (true == oFile.CreateFileW(oFilePath.GetPath()))
		{
			if (false == m_sXmlA.empty())
				oFile.WriteFile((BYTE*)m_sXmlA.c_str(), m_sXmlA.length());
			oFile.CloseFile();
		}
		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
	}
	const OOX::FileType OOX::Draw::CSolutionFile::type() const
	{
		return FileTypes::Solution;
	}
	const OOX::CPath OOX::Draw::CSolutionFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath OOX::Draw::CSolutionFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void OOX::Draw::CSolutionFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				pReader->Skip(4); // len
				m_sXmlA = pReader->GetString2A();
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void OOX::Draw::CSolutionFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter)
	{
		pWriter->StartRecord(0);
		pWriter->WriteStringA(m_sXmlA);
		pWriter->EndRecord();
	}
//-----------------------------------------------------------------------------------------------------------------------------
	OOX::Draw::CSolutionsFile::CSolutionsFile(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
	}
	OOX::Draw::CSolutionsFile::CSolutionsFile(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
		read(uri.GetDirectory(), uri);
	}
	OOX::Draw::CSolutionsFile::CSolutionsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;

		read(oRootPath, oPath);
	}
	OOX::Draw::CSolutionsFile::~CSolutionsFile()
	{
	}
	void OOX::Draw::CSolutionsFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void OOX::Draw::CSolutionsFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		Solutions = oReader;
	}
	void OOX::Draw::CSolutionsFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}
	const OOX::FileType OOX::Draw::CSolutionsFile::type() const
	{
		return FileTypes::Solutions;
	}
	const OOX::CPath OOX::Draw::CSolutionsFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath OOX::Draw::CSolutionsFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void OOX::Draw::CSolutionsFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		smart_ptr<OOX::IFileContainer> rels_old = pReader->GetRels();
		pReader->SetRels(dynamic_cast<OOX::IFileContainer*>((CSolutionsFile*)this));

		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				Solutions.Init();
				Solutions->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
		pReader->SetRels(rels_old);
	}
	void OOX::Draw::CSolutionsFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		smart_ptr<OOX::IFileContainer> rels_old = pWriter->GetRels();
		pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>((CSolutionsFile*)this));
		
		pWriter->WriteRecord2(0, Solutions);

		pWriter->SetRels(rels_old);
	}
	void OOX::Draw::CSolutionsFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{		
		if (Solutions.IsInit())
			Solutions->toXmlWriter(pWriter);
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CSolution::getType() const
	{
		return et_dr_Solution;
	}
	void CSolution::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "Name", Name)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CSolution::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			if (strcmp("Rel", sName) == 0)
			{
				Rel = oReader;
			}
		}
	}
	void CSolution::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, Name);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		
		if (Rel.IsInit() && Rel->Rid.IsInit())
		{
			smart_ptr<OOX::File> pFile = pWriter->GetRels()->Find(Rel->Rid->GetValue());
			CSolutionFile* pSolution = dynamic_cast<CSolutionFile*>(pFile.GetPointer());
			if (pSolution)
			{
				pWriter->StartRecord(0);
				pSolution->toPPTY(pWriter);
				pWriter->EndRecord();
			}
		}
	}
	void CSolution::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				Name = pReader->GetString2();
			}break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				CSolutionFile* pSolution = new CSolutionFile(NULL);
				pSolution->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pSolution);

				Rel.Init(); Rel->Rid.Init();
				Rel->Rid->SetValue(pReader->GetRels()->Add(oFile).get());
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CSolution::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Solution");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"Name", Name);
		pWriter->EndAttributes();

		if (Rel.IsInit())
			Rel->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Solution");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	void CSolutions::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"Solution" == sName)
			{
				CSolution* pItem = new CSolution();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	EElementType CSolutions::getType() const
	{
		return et_dr_Solutions;
	}
	void CSolutions::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CSolution());
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
	void CSolutions::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CSolutions::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Solutions");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.microsoft.com/office/visio/2012/main");
		pWriter->WriteAttribute2(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->WriteAttribute2(L"xml:space", L"preserve");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Solutions");
	}
}
} // namespace OOX
