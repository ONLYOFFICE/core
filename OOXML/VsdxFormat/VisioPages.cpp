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
#include "Vsdx.h"
#include "VisioPages.h"
#include "Shapes.h"
#include "../../DesktopEditor/common/SystemUtils.h"
#include "../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../Binary/Presentation/XmlWriter.h"

namespace OOX
{
namespace Draw
{
	//-----------------------------------------------------------------------------------------------------------------------------
	CMasterFile::CMasterFile(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
	}
	CMasterFile::CMasterFile(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;

		read(uri.GetDirectory(), uri);
	}
	CMasterFile::CMasterFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;

		read(oRootPath, oPath);
	}
	CMasterFile::~CMasterFile()
	{
	}
	void CMasterFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CMasterFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		std::wstring sName = oReader.GetName();
		if (L"MasterContents" == sName && !oReader.IsEmptyNode())
		{
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"Shapes" == sName)
				{
					Shapes = oReader;
				}
				else if (L"Connects" == sName)
				{
					Connects = oReader;
				}
			}
		}
	}
	void CMasterFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
	}
	const OOX::FileType CMasterFile::type() const
	{
		return FileTypes::Master;
	}
	const OOX::CPath CMasterFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath CMasterFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CMasterFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		smart_ptr<OOX::IFileContainer> rels_old = pReader->GetRels();
		pReader->SetRels(dynamic_cast<OOX::IFileContainer*>((CMasterFile*)this));

		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				Shapes.Init();
				Shapes->fromPPTY(pReader);
			}break;
			case 1:
			{
				Connects.Init();
				Connects->fromPPTY(pReader);
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
	void CMasterFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		smart_ptr<OOX::IFileContainer> rels_old = pWriter->GetRels();
		pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>((CPageFile*)this));

		pWriter->WriteRecord2(0, Shapes);
		pWriter->WriteRecord2(1, Connects);

		pWriter->SetRels(rels_old);
	}
	void CMasterFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"MasterContents");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.microsoft.com/office/visio/2012/main");
		pWriter->WriteAttribute2(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->WriteAttribute2(L"xml:space", L"preserve");
		pWriter->EndAttributes();

		if (Shapes.IsInit())
			Shapes->toXmlWriter(pWriter);

		if (Connects.IsInit())
			Connects->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"MasterContents");
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	CPageFile::CPageFile(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
	}
	CPageFile::CPageFile(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
		read(uri.GetDirectory(), uri);
	}
	CPageFile::CPageFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
		read(oRootPath, oPath);
	}
	CPageFile::~CPageFile()
	{
	}
	void CPageFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CPageFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		std::wstring sName = oReader.GetName();
		if (L"PageContents" == sName && !oReader.IsEmptyNode())
		{
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"Shapes" == sName)
				{
					Shapes = oReader;
				}
				else if (L"Connects" == sName)
				{
					Connects = oReader;
				}
			}
		}
	}
	void CPageFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}
	const OOX::FileType CPageFile::type() const
	{
		return FileTypes::Page;
	}
	const OOX::CPath CPageFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath CPageFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CPageFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		std::vector<std::pair<nullable_uint, nullable_string>> arMasterRels;
		std::vector<nullable_uint> arPagesRels;

		smart_ptr<OOX::IFileContainer> rels_old = pReader->GetRels();
		pReader->SetRels(dynamic_cast<OOX::IFileContainer*>((CPageFile*)this));

		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				Shapes.Init();
				Shapes->fromPPTY(pReader);

			}break;
			case 1:
			{
				Connects.Init();
				Connects->fromPPTY(pReader);
			}break;
			case 2:
			{
				nullable_uint ID;
				nullable_string UniqueID;

				LONG _end_rec_2 = pReader->GetPos() + pReader->GetRecordSize() + 4;
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
							UniqueID = pReader->GetString2();
						}break;										
					}
				}
				pReader->Seek(_end_rec_2);
				arMasterRels.push_back(std::make_pair(ID, UniqueID));
			}break;
			case 3:
			{
				nullable_uint ID;

				LONG _end_rec_2 = pReader->GetPos() + pReader->GetRecordSize() + 4;
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
					}
				}
				pReader->Seek(_end_rec_2);
				arPagesRels.push_back(ID);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
		pReader->SetRels(rels_old);
//-------------------------------------------------------------------------------------------
		OOX::Draw::CVsdx* pVsdx = dynamic_cast<OOX::Draw::CVsdx*>(((OOX::File*)this)->m_pMainDocument);
		for (auto r : arMasterRels)
		{
			if (r.first.IsInit())
			{
				std::map<unsigned int, smart_ptr<OOX::File>>::iterator pFind = pVsdx->mapMastersByID.find(*r.first);
				if (pFind != pVsdx->mapMastersByID.end())
				{
					AddNoWrite(pFind->second, L"../masters");
				}
			}
			else if (r.second.IsInit())
			{
				//std::map<std::wstring, smart_ptr<OOX::File>>::iteratorpFind = map2.find(*r.first);
				//if (pFind != map.end())
				//{
				//	this->AddNoWrite(pFind->second, L"../masters");
				//}
			}
		}
		for (auto r : arPagesRels)
		{
			if (r.IsInit())
			{
				std::map<unsigned int, smart_ptr<OOX::File>>::iterator pFind = pVsdx->mapPagesByID.find(*r);
				if (pFind != pVsdx->mapPagesByID.end())
				{
					AddNoWrite(pFind->second, L"");
				}
			}
		}
	}
	void CPageFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		smart_ptr<OOX::IFileContainer> rels_old = pWriter->GetRels();
		pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>((CPageFile*)this));

		pWriter->WriteRecord2(0, Shapes);
		pWriter->WriteRecord2(1, Connects);

		pWriter->SetRels(rels_old);
//-------------------------------------------------------------------------------------------------------
		for (auto r : m_arContainer)
		{
			if (r->type() == OOX::Draw::FileTypes::Master)
			{
				smart_ptr<CMasterFile> master = r.smart_dynamic_cast<CMasterFile>();
				pWriter->StartRecord(2);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteUInt2(0, master->ID);
				pWriter->WriteString2(1, master->UniqueID);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
				pWriter->EndRecord();
			}
			else if (r->type() == OOX::Draw::FileTypes::Page)
			{
				smart_ptr<CPageFile> page = r.smart_dynamic_cast<CPageFile>();
				pWriter->StartRecord(3);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteUInt2(0, page->ID);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
				pWriter->EndRecord();
			}
		}
	}
	void CPageFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"PageContents");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.microsoft.com/office/visio/2012/main");
		pWriter->WriteAttribute2(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->WriteAttribute2(L"xml:space", L"preserve");
		pWriter->EndAttributes();

		if (Shapes.IsInit())
			Shapes->toXmlWriter(pWriter);

		if (Connects.IsInit())
			Connects->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"PageContents");
	}
	CMastersFile::CMastersFile(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
	}
	CMastersFile::CMastersFile(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
		read(uri.GetDirectory(), uri);
	}
	CMastersFile::CMastersFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;

		read(oRootPath, oPath);
	}
	CMastersFile::~CMastersFile()
	{
	}
	void CMastersFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CMastersFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		fromXML(oReader);
	}
	OOX::EElementType CMastersFile::getType() const
	{
		return et_dr_Masters;
	}
	void CMastersFile::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"Master" == sName)
			{
				CMaster* pItem = new CMaster();
				if (pItem)
				{
					*pItem = oReader;
					m_arrItems.push_back(pItem);

					if (pItem->Rel.IsInit())
					{
						smart_ptr<OOX::File> pFile = this->Find(pItem->Rel->Rid->GetValue());
						CMasterFile* pMasterContent = dynamic_cast<CMasterFile*>(pFile.GetPointer());
						if (pMasterContent)
						{
							pMasterContent->ID = pItem->ID;
							pMasterContent->UniqueID = pItem->UniqueID; 
						}
					}
				}
			}
		}
	}
	void CMastersFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}
	const OOX::FileType CMastersFile::type() const
	{
		return FileTypes::Masters;
	}
	const OOX::CPath CMastersFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath CMastersFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CMastersFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		smart_ptr<OOX::IFileContainer> rels_old = pReader->GetRels();
		pReader->SetRels(dynamic_cast<OOX::IFileContainer*>((CMastersFile*)this));

		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CMaster(((OOX::File*)this)->m_pMainDocument));
				m_arrItems.back()->fromPPTY(pReader);
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
	void CMastersFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		smart_ptr<OOX::IFileContainer> rels_old = pWriter->GetRels();
		pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>((CPageFile*)this));

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));

		pWriter->SetRels(rels_old);
	}
	void CMastersFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Masters");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.microsoft.com/office/visio/2012/main");
		pWriter->WriteAttribute2(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->WriteAttribute2(L"xml:space", L"preserve");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Masters");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	CPagesFile::CPagesFile(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
	}
	CPagesFile::CPagesFile(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
		read(uri.GetDirectory(), uri);
	}
	CPagesFile::CPagesFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
		read(oRootPath, oPath);
	}
	CPagesFile::~CPagesFile()
	{
	}
	void CPagesFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CPagesFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		fromXML(oReader);
	}
	OOX::EElementType CPagesFile::getType() const
	{
		return et_dr_Pages;
	}
	void CPagesFile::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"Page" == sName)
			{
				CPage* pItem = new CPage();

				if (pItem)
				{
					*pItem = oReader;
					m_arrItems.push_back(pItem);

					if (pItem->Rel.IsInit())
					{
						smart_ptr<OOX::File> pFile = this->Find(pItem->Rel->Rid->GetValue());
						CPageFile* pPageContent = dynamic_cast<CPageFile*>(pFile.GetPointer());
						if (pPageContent)
						{
							pPageContent->ID = pItem->ID;
						}
					}
				}
			}
		}
	}
	void CPagesFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}
	const OOX::FileType CPagesFile::type() const
	{
		return FileTypes::Pages;
	}
	const OOX::CPath CPagesFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath CPagesFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CPagesFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		smart_ptr<OOX::IFileContainer> rels_old = pReader->GetRels();
		pReader->SetRels(dynamic_cast<OOX::IFileContainer*>((CPagesFile*)this));

		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CPage(((OOX::File*)this)->m_pMainDocument));
				m_arrItems.back()->fromPPTY(pReader);
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
	void CPagesFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		smart_ptr<OOX::IFileContainer> rels_old = pWriter->GetRels();
		pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>((CPageFile*)this));

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));

		pWriter->SetRels(rels_old);
	}
	void CPagesFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Pages");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.microsoft.com/office/visio/2012/main");
		pWriter->WriteAttribute2(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->WriteAttribute2(L"xml:space", L"preserve");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Pages");
	}
//--------------------------------------------------------------------------------------------------------------
	CPage::CPage(OOX::Document* pMain) : WritingElement(pMain)
	{
	}
	CPage::~CPage()
	{
	}
	EElementType CPage::getType() const
	{
		return et_dr_Page;
	}
	void CPage::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Name", Name)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "NameU", NameU)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IsCustomName", IsCustomName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IsCustomNameU", IsCustomNameU)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Background", Background)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "BackPage", BackPage)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ViewScale", ViewScale)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ViewCenterX", ViewCenterX)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ViewCenterY", ViewCenterY)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ReviewerID", ReviewerID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "AssociatedPage", AssociatedPage)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CPage::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			if (strcmp("PageSheet", sName) == 0)
			{
				PageSheet = oReader;
			}
			else if (strcmp("Rel", sName) == 0)
			{
				Rel = oReader;
			}
		}
	}
	void CPage::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteString2(1, Name);
		pWriter->WriteString2(2, NameU);
		pWriter->WriteBool2(3, IsCustomName);
		pWriter->WriteBool2(4, IsCustomNameU);
		pWriter->WriteBool2(5, Background);
		pWriter->WriteUInt2(6, BackPage);
		pWriter->WriteDoubleReal2(7, ViewScale);
		pWriter->WriteDoubleReal2(8, ViewCenterX);
		pWriter->WriteDoubleReal2(9, ViewCenterY);
		pWriter->WriteUInt2(10, ReviewerID);
		pWriter->WriteUInt2(11, AssociatedPage);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, PageSheet);

		if (Rel.IsInit() && Rel->Rid.IsInit())
		{
			smart_ptr<OOX::File> pFile = pWriter->GetRels()->Find(Rel->Rid->GetValue());
			CPageFile* pPage = dynamic_cast<CPageFile*>(pFile.GetPointer());
			if (pPage)
			{
				pWriter->StartRecord(1);
				pPage->toPPTY(pWriter);
				pWriter->EndRecord();
			}
		}
	}
	void CPage::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				NameU = pReader->GetString2();
			}break;
			case 3:
			{
				IsCustomName = pReader->GetBool();
			}break;
			case 4:
			{
				IsCustomNameU = pReader->GetBool();
			}break;
			case 5:
			{
				Background = pReader->GetBool();
			}break;
			case 6:
			{
				BackPage = pReader->GetULong();
			}break;
			case 7:
			{
				ViewScale = pReader->GetDoubleReal();
			}break;
			case 8:
			{
				ViewCenterX = pReader->GetDoubleReal();
			}break;
			case 9:
			{
				ViewCenterY = pReader->GetDoubleReal();
			}break;
			case 10:
			{
				ReviewerID = pReader->GetULong();
			}break;
			case 11:
			{
				AssociatedPage = pReader->GetULong();
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
				PageSheet.Init();
				PageSheet->fromPPTY(pReader);
			}break;
			case 1:
			{
				CPageFile* pPage = new CPageFile(m_pMainDocument);
				pPage->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pPage);

				Rel.Init(); Rel->Rid.Init();
				Rel->Rid->SetValue(pReader->GetRels()->Add(oFile).get());
				
				pPage->ID = ID;

				OOX::Draw::CVsdx* pVsdx = dynamic_cast<OOX::Draw::CVsdx*>(m_pMainDocument);
				pVsdx->mapPagesByID.insert(std::make_pair(*ID, oFile));
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CPage::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Page");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute2(L"Name", Name);
		pWriter->WriteAttribute2(L"NameU", NameU);
		pWriter->WriteAttribute(L"IsCustomName", IsCustomName);
		pWriter->WriteAttribute(L"IsCustomNameU", IsCustomNameU);
		pWriter->WriteAttribute(L"Background", Background);
		pWriter->WriteAttribute2(L"BackPage", BackPage);
		pWriter->WriteAttribute(L"ViewScale", ViewScale);
		pWriter->WriteAttribute(L"ViewCenterX", ViewCenterX);
		pWriter->WriteAttribute(L"ViewCenterY", ViewCenterY);
		pWriter->WriteAttribute2(L"ReviewerID", ReviewerID);
		pWriter->WriteAttribute2(L"AssociatedPage", AssociatedPage);
		pWriter->EndAttributes();

		if (PageSheet.IsInit())
			PageSheet->toXmlWriter(pWriter);

		if (Rel.IsInit())
			Rel->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Page");
	}
//-----------------------------------------------------------------------------------------------------------------
	CMaster::CMaster(OOX::Document* pMain) : WritingElement(pMain)
	{

	}
	CMaster::~CMaster()
	{
	}
	EElementType CMaster::getType() const
	{
		return et_dr_Master;
	}
	void CMaster::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Name", Name)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "NameU", NameU)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "BaseID", BaseID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "UniqueID", UniqueID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "MatchByName", MatchByName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IsCustomName", IsCustomName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IsCustomNameU", IsCustomNameU)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IconSize", IconSize)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "PatternFlags", PatternFlags)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Prompt", Prompt)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Hidden", Hidden)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IconUpdate", IconUpdate)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "AlignName", AlignName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "MasterType", MasterType)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CMaster::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			if (strcmp("PageSheet", sName) == 0)
			{
				PageSheet = oReader;
			}
			else if (strcmp("Icon", sName) == 0)
			{
				Icon = oReader;
			}
			else if (strcmp("Rel", sName) == 0)
			{
				Rel = oReader;
			}
		}
	}
	void CMaster::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteString2(1, Name);
		pWriter->WriteString2(2, NameU);
		pWriter->WriteString2(3, BaseID);
		pWriter->WriteString2(4, UniqueID);
		pWriter->WriteBool2(5, MatchByName);
		pWriter->WriteBool2(6, IsCustomName);
		pWriter->WriteBool2(7, IsCustomNameU);
		pWriter->WriteUInt2(8, IconSize);
		pWriter->WriteUInt2(9, PatternFlags);
		pWriter->WriteString2(10, Prompt);
		pWriter->WriteBool2(11, Hidden);
		pWriter->WriteBool2(12, IconUpdate);
		pWriter->WriteUInt2(13, AlignName);
		pWriter->WriteUInt2(14, MasterType);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, PageSheet);

		if (Icon.IsInit())
		{
			pWriter->StartRecord(1);
			pWriter->WriteString(Icon->content);
			pWriter->EndRecord();
		}
		if (Rel.IsInit() && Rel->Rid.IsInit())
		{
			smart_ptr<OOX::File> pFile = pWriter->GetRels()->Find(Rel->Rid->GetValue());
			CMasterFile* pMaster = dynamic_cast<CMasterFile*>(pFile.GetPointer());
			if (pMaster)
			{
				pWriter->StartRecord(2);
				pMaster->toPPTY(pWriter);
				pWriter->EndRecord();
			}
		}
	}
	void CMaster::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				NameU = pReader->GetString2();
			}break;
			case 3:
			{
				BaseID = pReader->GetString2();
			}break;
			case 4:
			{
				UniqueID = pReader->GetString2();
			}break;
			case 5:
			{
				MatchByName = pReader->GetBool();
			}break;
			case 6:
			{
				IsCustomName = pReader->GetBool();
			}break;
			case 7:
			{
				IsCustomNameU = pReader->GetBool();
			}break;
			case 8:
			{
				IconSize = pReader->GetULong();
			}break;
			case 9:
			{
				PatternFlags = pReader->GetULong();
			}break;
			case 10:
			{
				Prompt = pReader->GetString2();
			}break;
			case 11:
			{
				Hidden = pReader->GetBool();
			}break;
			case 12:
			{
				IconUpdate = pReader->GetBool();
			}break;
			case 13:
			{
				AlignName = pReader->GetULong();
			}break;
			case 14:
			{
				MasterType = pReader->GetULong();
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
				PageSheet.Init();
				PageSheet->fromPPTY(pReader);
			}break;
			case 1:
			{
				Icon.Init();
				Icon->fromPPTY(pReader);
			}break;
			case 2:
			{
				CMasterFile* pMaster = new CMasterFile(m_pMainDocument);
				pMaster->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pMaster);

				Rel.Init(); Rel->Rid.Init();
				Rel->Rid->SetValue(pReader->GetRels()->Add(oFile).get());

				pMaster->ID = ID;
				pMaster->UniqueID = UniqueID;

				OOX::Draw::CVsdx* pVsdx = dynamic_cast<OOX::Draw::CVsdx*>(m_pMainDocument);

				pVsdx->mapMastersByID.insert(std::make_pair(*ID, oFile));
				//pVsdx->mapMastersByUniqueID.insert(*UniqueID, oFile)
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CMaster::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Master");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute2(L"Name", Name);
		pWriter->WriteAttribute2(L"NameU", NameU);
		pWriter->WriteAttribute2(L"BaseID", BaseID);
		pWriter->WriteAttribute2(L"UniqueID", UniqueID);
		pWriter->WriteAttribute(L"MatchByName", MatchByName);
		pWriter->WriteAttribute(L"IsCustomName", IsCustomName);
		pWriter->WriteAttribute(L"IsCustomNameU", IsCustomNameU);
		pWriter->WriteAttribute2(L"IconSize", IconSize);
		pWriter->WriteAttribute2(L"PatternFlags", PatternFlags);
		pWriter->WriteAttribute2(L"Prompt", Prompt);
		pWriter->WriteAttribute(L"Hidden", Hidden);
		pWriter->WriteAttribute(L"IconUpdate", IconUpdate);
		pWriter->WriteAttribute2(L"AlignName", AlignName);
		pWriter->WriteAttribute2(L"MasterType", MasterType);
		pWriter->EndAttributes();

		if (PageSheet.IsInit())
			PageSheet->toXmlWriter(pWriter);

		if (Icon.IsInit())
			Icon->toXmlWriter(pWriter);

		if (Rel.IsInit())
			Rel->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Master");
	}
	EElementType CPageSheet::getType() const
	{
		return et_dr_PageSheet;
	}
	void CPageSheet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "UniqueID", UniqueID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "LineStyle", LineStyle)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FillStyle", FillStyle)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "TextStyle", TextStyle)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CPageSheet::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			WritingElement* pItem = NULL;
			if (L"Cell" == sName)
			{
				pItem = new CCell();
			}
			else if (L"Trigger" == sName)
			{
				pItem = new CTrigger();
			}
			else if (L"Section" == sName)
			{
				pItem = new CSection();
			}
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CPageSheet::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, UniqueID);
		pWriter->WriteUInt2(1, LineStyle);
		pWriter->WriteUInt2(2, FillStyle);
		pWriter->WriteUInt2(3, TextStyle);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			int type = 0xff;					 //todooo predefine type for ???
			switch (m_arrItems[i]->getType())
			{
			case et_dr_Cell: type = 0; break;
			case et_dr_Trigger: type = 1; break;
			case et_dr_Section: type = 2; break;
			}
			if (type != 0xff)
				pWriter->WriteRecord2(type, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
		}
	}
	void CPageSheet::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				UniqueID = pReader->GetString2();
			}break;
			case 1:
			{
				LineStyle = pReader->GetULong();
			}break;
			case 2:
			{
				FillStyle = pReader->GetULong();
			}break;
			case 3:
			{
				TextStyle = pReader->GetULong();
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
				m_arrItems.push_back(new CCell());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_arrItems.push_back(new CTrigger());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 2:
			{
				m_arrItems.push_back(new CSection());
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
	void CPageSheet::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"PageSheet");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"UniqueID", UniqueID);
		pWriter->WriteAttribute2(L"LineStyle", LineStyle);
		pWriter->WriteAttribute2(L"FillStyle", FillStyle);
		pWriter->WriteAttribute2(L"TextStyle", TextStyle);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"PageSheet");
	}
}
} // namespace OOX
