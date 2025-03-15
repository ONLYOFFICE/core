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
	CSolutionFile::CSolutionFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	CSolutionFile::CSolutionFile(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain)
	{
		read(uri.GetDirectory(), uri);
	}
	CSolutionFile::CSolutionFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	CSolutionFile::~CSolutionFile()
	{
	}
	void CSolutionFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CSolutionFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		NSFile::CFileBinary::ReadAllTextUtf8A(oFilePath.GetPath(), m_sXmlA);
	}
	void CSolutionFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
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
	const OOX::FileType CSolutionFile::type() const
	{
		return FileTypes::Solution;
	}
	const OOX::CPath CSolutionFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath CSolutionFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CSolutionFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
	void CSolutionFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter)
	{
		pWriter->StartRecord(0);
		pWriter->WriteStringA(m_sXmlA);
		pWriter->EndRecord();
	}
//-----------------------------------------------------------------------------------------------------------------------------
	CSolutionsFile::CSolutionsFile(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
	}
	CSolutionsFile::CSolutionsFile(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
		read(uri.GetDirectory(), uri);
	}
	CSolutionsFile::CSolutionsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;

		read(oRootPath, oPath);
	}
	CSolutionsFile::~CSolutionsFile()
	{
	}
	void CSolutionsFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CSolutionsFile::read(const CPath& oRootPath, const CPath& oFilePath)
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
	void CSolutionsFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}
	const OOX::FileType CSolutionsFile::type() const
	{
		return FileTypes::Solutions;
	}
	const OOX::CPath CSolutionsFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath CSolutionsFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CSolutionsFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
	void CSolutionsFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		smart_ptr<OOX::IFileContainer> rels_old = pWriter->GetRels();
		pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>((CSolutionsFile*)this));
		
		pWriter->WriteRecord2(0, Solutions);

		pWriter->SetRels(rels_old);
	}
	void CSolutionsFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
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
//-----------------------------------------------------------------------------------------------------------------------------------------
	CValidationFile::CValidationFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	CValidationFile::CValidationFile(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain)
	{
		read(uri.GetDirectory(), uri);
	}
	CValidationFile::CValidationFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	CValidationFile::~CValidationFile()
	{
	}
	void CValidationFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CValidationFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		std::wstring sName = oReader.GetName();
		if (L"Validation" == sName && !oReader.IsEmptyNode())
		{
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"ValidationProperties" == sName)
				{
					ValidationProperties = oReader;
				}
				else if (L"RuleSets" == sName)
				{
					RuleSets = oReader;
				}
				else if (L"Issues" == sName)
				{
					Issues = oReader;
				}
			}
		}
	}
	void CValidationFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
	}
	void CValidationFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Validation");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.microsoft.com/office/visio/2012/main");
		pWriter->WriteAttribute2(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->WriteAttribute2(L"xml:space", L"preserve");
		pWriter->EndAttributes();

		if (ValidationProperties.IsInit())
			ValidationProperties->toXmlWriter(pWriter);

		if (RuleSets.IsInit())
			RuleSets->toXmlWriter(pWriter);

		if (Issues.IsInit())
			Issues->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Validation");
	}
	const OOX::FileType CValidationFile::type() const
	{
		return FileTypes::Validation;
	}
	const OOX::CPath CValidationFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath CValidationFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CValidationFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				ValidationProperties.Init();
				ValidationProperties->fromPPTY(pReader);
			}break;
			case 1:
			{
				RuleSets.Init();
				RuleSets->fromPPTY(pReader);
			}break;
			case 2:
			{
				Issues.Init();
				Issues->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CValidationFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter)
	{
		pWriter->WriteRecord2(0, ValidationProperties);
		pWriter->WriteRecord2(1, RuleSets);
		pWriter->WriteRecord2(2, Issues);


	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CIssue::getType() const
	{
		return et_dr_Issue;
	}
	void CIssue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Ignored", Ignored)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CIssue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"IssueTarget" == sName)
			{
				IssueTarget = oReader;
			}
			else if (L"RuleInfo" == sName)
			{
				RuleInfo = oReader;
			}
		}
	}
	void CIssue::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteBool2(1, Ignored);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, IssueTarget);
		pWriter->WriteRecord2(1, RuleInfo);
	}
	void CIssue::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				ID = pReader->GetULong();
			}break;
			case 1:
			{
				Ignored = pReader->GetBool();
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
				IssueTarget.Init();
				IssueTarget->fromPPTY(pReader);
			}break;
			case 1:
			{
				RuleInfo.Init();
				RuleInfo->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CIssue::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Issue");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute(L"Ignored", Ignored);
		pWriter->EndAttributes();

		if (IssueTarget.IsInit())
			IssueTarget->toXmlWriter(pWriter);

		if (RuleInfo.IsInit())
			RuleInfo->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Issue");
	}
	EElementType CIssues::getType() const
	{
		return et_dr_Issues;
	}
	void CIssues::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"Issue" == sName)
			{
				CIssue* pItem = new CIssue();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	void CIssues::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CIssue());
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
	void CIssues::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CIssues::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Issues");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Issues");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CRule::getType() const
	{
		return et_dr_Rule;
	}
	void CRule::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Category", Category)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "NameU", NameU)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Description", Description)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Ignored", Ignored)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "RuleTarget", RuleTarget)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CRule::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"RuleFilter" == sName)
			{
				RuleFilter = oReader;
			}
			else if(L"RuleTest" == sName)
			{
				RuleTest = oReader;
			}
		}
	}
	void CRule::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteString2(1, Category);
		pWriter->WriteString2(2, NameU);
		pWriter->WriteBool2(3, Ignored);
		pWriter->WriteString2(4, Description);
		pWriter->WriteInt2(5, RuleTarget);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		
		pWriter->WriteRecord2(0, RuleFilter);
		pWriter->WriteRecord2(1, RuleTest);
	}
	void CRule::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				ID = pReader->GetULong();
			}break;
			case 1:
			{
				Category = pReader->GetString2();
			}break;
			case 2:
			{
				NameU = pReader->GetString2();
			}break;
			case 3:
			{
				Ignored = pReader->GetBool();
			}break;
			case 4:
			{
				Description = pReader->GetString2();
			}break;
			case 5:
			{
				RuleTarget = pReader->GetLong();
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
				RuleFilter.Init();
				RuleFilter->node_name = L"RuleFilter";
				RuleFilter->fromPPTY(pReader);
			}break;
			case 1:
			{
				RuleTest.Init();
				RuleFilter->node_name = L"RuleTest";
				RuleTest->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CRule::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Rule");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute2(L"Category", Category);
		pWriter->WriteAttribute2(L"NameU", NameU);
		pWriter->WriteAttribute2(L"Description", Description);
		pWriter->WriteAttribute(L"Ignored", Ignored);
		pWriter->WriteAttribute(L"RuleTarget", RuleTarget);
		pWriter->EndAttributes();

		if (RuleFilter.IsInit())
			RuleFilter->toXmlWriter(pWriter);

		if (RuleTest.IsInit())
			RuleTest->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Rule");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CRuleSet::getType() const
	{
		return et_dr_RuleSet;
	}
	void CRuleSet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "NameU", NameU)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Name", Name)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Description", Description)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Enabled", Enabled)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CRuleSet::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"RuleSetFlags" == sName)
			{
				RuleSetFlags = oReader;
			}
			else if (L"Rule" == sName)
			{
				CRule* pItem = new CRule();
				
				if (pItem)
				{
					pItem->fromXML(oReader);
					m_arrItems.push_back(pItem);
				}
			}
		}
	}
	void CRuleSet::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteString2(1, NameU);
		pWriter->WriteString2(2, Name);
		pWriter->WriteString2(3, Description);
		pWriter->WriteBool2(4, Enabled);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, RuleSetFlags);
		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			pWriter->WriteRecord2(1, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
		}
	}
	void CRuleSet::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				ID = pReader->GetULong();
			}break;
			case 1:
			{
				NameU = pReader->GetString2();
			}break;
			case 2:
			{
				Name = pReader->GetString2();
			}break;
			case 3:
			{
				Description = pReader->GetString2();
			}break;
			case 8:
			{
				Enabled = pReader->GetBool();
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
				RuleSetFlags.Init();
				RuleSetFlags->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_arrItems.push_back(new CRule());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CRuleSet::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"RuleSet");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute2(L"NameU", NameU);
		pWriter->WriteAttribute2(L"Name", Name);
		pWriter->WriteAttribute2(L"Description", Description);
		pWriter->WriteAttribute(L"Enabled", Enabled);
		pWriter->EndAttributes();

		if (RuleSetFlags.IsInit())
			RuleSetFlags->toXmlWriter(pWriter);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"RuleSet");
	}
	EElementType CRuleSets::getType() const
	{
		return et_dr_RuleSets;
	}
	void CRuleSets::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();
			if (L"RuleSet" == sName)
			{
				CRuleSet* pItem = new CRuleSet();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	void CRuleSets::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CRuleSet());
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
	void CRuleSets::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CRuleSets::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"RuleSets");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"RuleSets");
	}
	EElementType CValidationProperties::getType() const
	{
		return et_dr_ValidationProperties;
	}
	void CValidationProperties::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ShowIgnored", ShowIgnored)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "LastValidated", LastValidated)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CValidationProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CValidationProperties::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteBool2(0, ShowIgnored);
		pWriter->WriteString2(1, LastValidated);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CValidationProperties::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				ShowIgnored = pReader->GetBool();
			}break;
			case 1:
			{
				LastValidated = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CValidationProperties::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"ValidationProperties");
		pWriter->StartAttributes();
		pWriter->WriteAttribute(L"ShowIgnored", ShowIgnored);
		pWriter->WriteAttribute2(L"LastValidated", LastValidated);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"ValidationProperties");
	}
	EElementType CIssueTarget::getType() const
	{
		return et_dr_IssueTarget;
	}
	void CIssueTarget::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "PageID", PageID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ShapeID", ShapeID)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CIssueTarget::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CIssueTarget::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, PageID);
		pWriter->WriteUInt2(1, ShapeID);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CIssueTarget::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				PageID = pReader->GetULong();
			}break;
			case 1:
			{
				ShapeID = pReader->GetULong();
			}break;

			}
		}
		pReader->Seek(_end_rec);
	}
	void CIssueTarget::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"IssueTarget");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"PageID", PageID);
		pWriter->WriteAttribute2(L"ShapeID", ShapeID);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"IssueTarget");
	}
	EElementType CRuleInfo::getType() const
	{
		return et_dr_RuleInfo;
	}
	void CRuleInfo::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "RuleID", RuleID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "RuleSetID", RuleSetID)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CRuleInfo::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CRuleInfo::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, RuleID);
		pWriter->WriteUInt2(1, RuleSetID);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CRuleInfo::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				RuleID = pReader->GetULong();
			}break;
			case 1:
			{
				RuleSetID = pReader->GetULong();
			}break;

			}
		}
		pReader->Seek(_end_rec);
	}
	void CRuleInfo::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"RuleInfo");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"RuleID", RuleID);
		pWriter->WriteAttribute2(L"RuleSetID", RuleSetID);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"RuleInfo");
	}
	EElementType CRuleFormula::getType() const
	{
		return et_dr_CRuleFormula;
	}
	void CRuleFormula::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "Formula", Formula)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CRuleFormula::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CRuleFormula::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, Formula);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CRuleFormula::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				Formula = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CRuleFormula::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter)
	{
		if (node_name.empty()) node_name = L"RuleFilter";
		pWriter->StartNode(node_name);
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"Formula", Formula);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(node_name);
	}
	EElementType CRuleSetFlags::getType() const
	{
		return et_dr_RuleSetFlags;
	}
	void CRuleSetFlags::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "Hidden", Hidden)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CRuleSetFlags::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CRuleSetFlags::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteBool2(0, Hidden);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CRuleSetFlags::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				Hidden = pReader->GetBool();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CRuleSetFlags::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"RuleSetFlags");
		pWriter->StartAttributes();
		pWriter->WriteAttribute(L"Hidden", Hidden);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"RuleSetFlags");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	CCommentsFile::CCommentsFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	CCommentsFile::CCommentsFile(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain)
	{
		read(uri.GetDirectory(), uri);
	}
	CCommentsFile::CCommentsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	CCommentsFile::~CCommentsFile()
	{
	}
	void CCommentsFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CCommentsFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		Comments = oReader;
	}
	void CCommentsFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		
		if (Comments.IsInit())
			Comments->toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
	}
	const OOX::FileType CCommentsFile::type() const
	{
		return FileTypes::Comments;
	}
	const OOX::CPath CCommentsFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath CCommentsFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CCommentsFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				Comments.Init();
				Comments->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void CCommentsFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		smart_ptr<OOX::IFileContainer> rels_old = pWriter->GetRels();
		pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>((CSolutionsFile*)this));

		pWriter->WriteRecord2(0, Comments);

		pWriter->SetRels(rels_old);
	}
	void CCommentsFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		if (Comments.IsInit())
			Comments->toXmlWriter(pWriter);
	}
//--------------------------------------------------------------------------------------------------------------
	EElementType CComments::getType() const
	{
		return et_dr_Comments;
	}
	void CComments::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ShowCommentTags", ShowCommentTags)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CComments::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			if (strcmp("AuthorList", sName) == 0)
			{
				AuthorList = oReader;
			}
			else if (strcmp("CommentList", sName) == 0)
			{
				CommentList = oReader;
			}
		}
	}
	void CComments::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteBool2(0, ShowCommentTags);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, AuthorList);
		pWriter->WriteRecord2(1, CommentList);
	}
	void CComments::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				ShowCommentTags = pReader->GetBool();
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
				AuthorList.Init();
				AuthorList->fromPPTY(pReader);
			}break;
			case 1:
			{
				CommentList.Init();
				CommentList->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CComments::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Comments");
		pWriter->StartAttributes();
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.microsoft.com/office/visio/2012/main");
		pWriter->WriteAttribute2(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->WriteAttribute2(L"xml:space", L"preserve");
		pWriter->WriteAttribute(L"ShowCommentTags", ShowCommentTags);
		pWriter->EndAttributes();

		if (AuthorList.IsInit())
			AuthorList->toXmlWriter(pWriter);

		if (CommentList.IsInit())
			CommentList->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Comments");
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CCommentList::getType() const
	{
		return et_dr_CommentList;
	}
	void CCommentList::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"CommentEntry" == sName)
			{
				CCommentEntry* pItem = new CCommentEntry();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	void CCommentList::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CCommentEntry());
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
	void CCommentList::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CCommentList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"CommentList");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"CommentList");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CAuthorList::getType() const
	{
		return et_dr_AuthorList;
	}
	void CAuthorList::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"AuthorEntry" == sName)
			{
				CAuthorEntry* pItem = new CAuthorEntry();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	void CAuthorList::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CAuthorEntry());
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
	void CAuthorList::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CAuthorList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"AuthorList");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"AuthorList");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CCommentEntry::getType() const
	{
		return et_dr_CommentEntry;
	}
	void CCommentEntry::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "AuthorID", AuthorID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "PageID", PageID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ShapeID", ShapeID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Date", Date)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "EditDate", EditDate)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Done", Done)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "CommentID", CommentID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "AutoCommentType", AutoCommentType)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CCommentEntry::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		content = oReader.GetText2();
	}
	void CCommentEntry::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, AuthorID);
		pWriter->WriteUInt2(1, PageID);
		pWriter->WriteUInt2(2, ShapeID);
		pWriter->WriteString2(3, Date);
		pWriter->WriteString2(4, EditDate);
		pWriter->WriteUInt2(5, CommentID);
		pWriter->WriteUInt2(6, AutoCommentType);
		pWriter->WriteString1(7, content);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CCommentEntry::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				AuthorID = pReader->GetULong();
			}break;
			case 1:
			{
				PageID = pReader->GetULong();
			}break;
			case 2:
			{
				ShapeID = pReader->GetULong();
			}break;
			case 3:
			{
				Date = pReader->GetString2();
			}break;
			case 4:
			{
				EditDate = pReader->GetString2();
			}break;
			case 5:
			{
				CommentID = pReader->GetULong();
			}break;
			case 6:
			{
				AutoCommentType = pReader->GetULong();
			}break;
			case 7:
			{
				content = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CCommentEntry::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"CommentEntry");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"AuthorID", AuthorID);
		pWriter->WriteAttribute2(L"PageID", PageID);
		pWriter->WriteAttribute2(L"ShapeID", ShapeID);
		pWriter->WriteAttribute2(L"Date", Date);
		pWriter->WriteAttribute2(L"EditDate", EditDate);
		pWriter->WriteAttribute2(L"CommentID", CommentID);
		pWriter->WriteAttribute2(L"AutoCommentType", AutoCommentType);
		pWriter->EndAttributes();
		pWriter->WriteStringXML(content);
		pWriter->WriteNodeEnd(L"CommentEntry");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CAuthorEntry::getType() const
	{
		return et_dr_AuthorEntry;
	}
	void CAuthorEntry::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Name", Name)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Initials", Initials)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ResolutionID", ResolutionID)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CAuthorEntry::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CAuthorEntry::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteString2(1, Name);
		pWriter->WriteString2(2, Initials);
		pWriter->WriteString2(3, ResolutionID);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CAuthorEntry::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				ID = pReader->GetULong();
			}break;
			case 1:
			{
				Name = pReader->GetString2();
			}break;
			case 2:
			{
				Initials = pReader->GetString2();
			}break;
			case 3:
			{
				ResolutionID = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CAuthorEntry::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"AuthorEntry");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute2(L"Name", Name);
		pWriter->WriteAttribute2(L"Initials", Initials);
		pWriter->WriteAttribute2(L"ResolutionID", ResolutionID);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"AuthorEntry");
	}

//-----------------------------------------------------------------------------------------------------------------------------
}
} // namespace OOX
