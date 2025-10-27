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
#include "VisioConnections.h"
#include "Shapes.h"
#include "../../DesktopEditor/common/SystemUtils.h"
#include "../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../Binary/Presentation/XmlWriter.h"

namespace OOX
{
namespace Draw
{
	OOX::Draw::CConnectionsFile::CConnectionsFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	OOX::Draw::CConnectionsFile::CConnectionsFile(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain)
	{
		read(uri.GetDirectory(), uri);
	}
	OOX::Draw::CConnectionsFile::CConnectionsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	OOX::Draw::CConnectionsFile::~CConnectionsFile()
	{
	}
	void OOX::Draw::CConnectionsFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void OOX::Draw::CConnectionsFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		DataConnections = oReader;
	}
	void OOX::Draw::CConnectionsFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
	}
	const OOX::FileType OOX::Draw::CConnectionsFile::type() const
	{
		return FileTypes::Connections;
	}
	const OOX::CPath OOX::Draw::CConnectionsFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath OOX::Draw::CConnectionsFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void OOX::Draw::CConnectionsFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				DataConnections.Init();
				DataConnections->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void OOX::Draw::CConnectionsFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteRecord2(0, DataConnections);
	}
	void OOX::Draw::CConnectionsFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		if (DataConnections.IsInit())
			DataConnections->toXmlWriter(pWriter);
	}
//-----------------------------------------------------------------------------------------------------------------------------
	OOX::Draw::CRecordsetFile::CRecordsetFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	OOX::Draw::CRecordsetFile::CRecordsetFile(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain)
	{
		read(uri.GetDirectory(), uri);
	}
	OOX::Draw::CRecordsetFile::CRecordsetFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	OOX::Draw::CRecordsetFile::~CRecordsetFile()
	{
	}
	void OOX::Draw::CRecordsetFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void OOX::Draw::CRecordsetFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		NSFile::CFileBinary::ReadAllTextUtf8A(oFilePath.GetPath(), m_sXmlA);
	}
	void OOX::Draw::CRecordsetFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
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
	const OOX::FileType OOX::Draw::CRecordsetFile::type() const
	{
		return FileTypes::Recordset;
	}
	const OOX::CPath OOX::Draw::CRecordsetFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath OOX::Draw::CRecordsetFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void OOX::Draw::CRecordsetFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
	void OOX::Draw::CRecordsetFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter)
	{
		pWriter->StartRecord(0);
		pWriter->WriteStringA(m_sXmlA);
		pWriter->EndRecord();
	}
//-----------------------------------------------------------------------------------------------------------------------------
	OOX::Draw::CRecordsetsFile::CRecordsetsFile(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
	}
	OOX::Draw::CRecordsetsFile::CRecordsetsFile(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;
		read(uri.GetDirectory(), uri);
	}
	OOX::Draw::CRecordsetsFile::CRecordsetsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bVisioPages = true;

		read(oRootPath, oPath);
	}
	OOX::Draw::CRecordsetsFile::~CRecordsetsFile()
	{
	}
	void OOX::Draw::CRecordsetsFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void OOX::Draw::CRecordsetsFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		DataRecordSets = oReader;
	}
	void OOX::Draw::CRecordsetsFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}
	const OOX::FileType OOX::Draw::CRecordsetsFile::type() const
	{
		return FileTypes::Recordsets;
	}
	const OOX::CPath OOX::Draw::CRecordsetsFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath OOX::Draw::CRecordsetsFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void OOX::Draw::CRecordsetsFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		OOX::IFileContainer* rels_old = pReader->GetRelsPtr();
		pReader->SetRelsPtr(dynamic_cast<OOX::IFileContainer*>((CRecordsetsFile*)this));

		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				DataRecordSets.Init();
				DataRecordSets->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
		pReader->SetRelsPtr(rels_old);
	}
	void OOX::Draw::CRecordsetsFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		OOX::IFileContainer* rels_old = pWriter->GetRelsPtr();
		pWriter->SetRelsPtr(dynamic_cast<OOX::IFileContainer*>((CRecordsetsFile*)this));
		
		pWriter->WriteRecord2(0, DataRecordSets);

		pWriter->SetRelsPtr(rels_old);
	}
	void OOX::Draw::CRecordsetsFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		if (DataRecordSets.IsInit())
			DataRecordSets->toXmlWriter(pWriter);
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CDataConnection::getType() const
	{
		return et_dr_DataConnection;
	}
	void CDataConnection::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FileName", FileName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ConnectionString", ConnectionString)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Command", Command)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FriendlyName", FriendlyName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Timeout", Timeout)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "AlwaysUseConnectionFile", AlwaysUseConnectionFile)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CDataConnection::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		
		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void CDataConnection::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteString2(1, FileName);
		pWriter->WriteString2(2, ConnectionString);
		pWriter->WriteString2(3, Command);
		pWriter->WriteString2(4, FriendlyName);
		pWriter->WriteUInt2(5, Timeout);
		pWriter->WriteBool2(6, AlwaysUseConnectionFile);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CDataConnection::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				FileName = pReader->GetString2();
			}break;
			case 2:
			{
				ConnectionString = pReader->GetString2();
			}break;
			case 3:
			{
				Command = pReader->GetString2();
			}break;
			case 4:
			{
				FriendlyName = pReader->GetString2();
			}break;
			case 5:
			{
				Timeout = pReader->GetULong();
			}break;
			case 6:
			{
				AlwaysUseConnectionFile = pReader->GetBool();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CDataConnection::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"DataConnection");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute2(L"FileName", FileName);
		pWriter->WriteAttribute2(L"ConnectionString", ConnectionString);
		pWriter->WriteAttribute2(L"Command", Command);
		pWriter->WriteAttribute2(L"FriendlyName", FriendlyName);
		pWriter->WriteAttribute2(L"Timeout", Timeout);
		pWriter->WriteAttribute(L"AlwaysUseConnectionFile", AlwaysUseConnectionFile);
		pWriter->EndAttributes();

		pWriter->WriteNodeEnd(L"DataConnection");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	void CDataConnections::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"DataConnection" == sName)
			{
				CDataConnection* pItem = new CDataConnection();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	void CDataConnections::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "NextID", NextID)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	EElementType CDataConnections::getType() const
	{
		return et_dr_DataConnections;
	}
	void CDataConnections::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
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
				NextID = pReader->GetULong();
			}break;
			}
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CDataConnection());
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
	void CDataConnections::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, NextID);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CDataConnections::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"DataConnections");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.microsoft.com/office/visio/2012/main");
		pWriter->WriteAttribute2(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->WriteAttribute2(L"xml:space", L"preserve");
		pWriter->WriteAttribute2(L"NextID", NextID);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"DataConnections");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CDataRecordSet::getType() const
	{
		return et_dr_DataRecordSet;
	}
	void CDataRecordSet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ConnectionID", ConnectionID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Options", Options)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Command", Command)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "TimeRefreshed", TimeRefreshed)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "NextRowID", NextRowID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Name", Name)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "RowOrder", RowOrder)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "RefreshOverwriteAll", RefreshOverwriteAll)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "RefreshNoReconciliationUI", RefreshNoReconciliationUI)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "RefreshInterval", RefreshInterval)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ReplaceLinks", ReplaceLinks)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Checksum", Checksum)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CDataRecordSet::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			
			WritingElement* pItem = NULL;
			if (strcmp("DataColumns", sName) == 0) //1
			{
				DataColumns = oReader;
			}
			else if (strcmp("Rel", sName) == 0) //1
			{
				Rel = oReader;
			}
			else if (strcmp("PrimaryKey", sName) == 0) //unbounded
			{
				pItem = new CPrimaryKey();
			}
			else if (strcmp("RowMap", sName) == 0) //unbounded
			{
				pItem = new CRowMap();
			}
			else if (strcmp("RefreshConflict", sName) == 0) //unbounded
			{
				pItem = new CRefreshConflict();
			}
			else if (strcmp("AutoLinkComparison", sName) == 0) //unbounded
			{
				pItem = new CAutoLinkComparison();
			}
			else if (strcmp("ADOData", sName) == 0) //unbounded ???
			{
			}
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CDataRecordSet::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteUInt2(1, ConnectionID);
		pWriter->WriteString2(2, Command);
		pWriter->WriteUInt2(3, Options);
		pWriter->WriteString2(4, TimeRefreshed);
		pWriter->WriteUInt2(5, NextRowID);
		pWriter->WriteString2(6, Name);
		pWriter->WriteBool2(7, RowOrder);
		pWriter->WriteBool2(8, RefreshOverwriteAll);
		pWriter->WriteBool2(9, RefreshNoReconciliationUI);
		pWriter->WriteUInt2(10, RefreshInterval);
		pWriter->WriteUInt2(11, ReplaceLinks);
		pWriter->WriteUInt2(12, Checksum);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		
		pWriter->WriteRecord2(0, DataColumns);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			int type = 0xff;					 //todooo predefine type for ???
			switch (m_arrItems[i]->getType())
			{
			case et_dr_PrimaryKey: type = 1; break;
			case et_dr_RowMap: type = 2; break;
			case et_dr_RefreshConflict: type = 3; break;
			case et_dr_AutoLinkComparison: type = 4; break;
			}
			if (type != 0xff)
				pWriter->WriteRecord2(type, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
		}
		if (Rel.IsInit() && Rel->Rid.IsInit() && pWriter->GetRelsPtr())
		{
			smart_ptr<OOX::File> pFile = pWriter->GetRelsPtr()->Find(Rel->Rid->GetValue());
			CRecordsetFile* pRecordset = dynamic_cast<CRecordsetFile*>(pFile.GetPointer());
			if (pRecordset)
			{
				pWriter->StartRecord(5);
				pRecordset->toPPTY(pWriter);
				pWriter->EndRecord();
			}
		}
	}
	void CDataRecordSet::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				ConnectionID = pReader->GetULong();
			}break;
			case 2:
			{
				Command = pReader->GetString2();
			}break;
			case 3:
			{
				Options = pReader->GetULong();
			}break;
			case 4:
			{
				TimeRefreshed = pReader->GetString2();
			}break;
			case 5:
			{
				NextRowID = pReader->GetULong();
			}break;
			case 6:
			{
				Name = pReader->GetString2();
			}break;
			case 7:
			{
				RowOrder = pReader->GetBool();
			}break;
			case 8:
			{
				RefreshOverwriteAll = pReader->GetBool();
			}break;
			case 9:
			{
				RefreshNoReconciliationUI = pReader->GetBool();
			}break;
			case 10:
			{
				RefreshInterval = pReader->GetULong();
			}break;
			case 11:
			{
				ReplaceLinks = pReader->GetULong();
			}break;
			case 12:
			{
				Checksum = pReader->GetULong();
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
				DataColumns.Init();
				DataColumns->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_arrItems.push_back(new CPrimaryKey());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 2:
			{
				m_arrItems.push_back(new CRowMap());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 3:
			{
				m_arrItems.push_back(new CRefreshConflict());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 4:
			{
				m_arrItems.push_back(new CAutoLinkComparison());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 5:
			{
				CRecordsetFile* pRecordset = new CRecordsetFile(NULL);
				pRecordset->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pRecordset);

				if (pReader->GetRelsPtr())
				{
					Rel.Init(); Rel->Rid.Init();
					Rel->Rid->SetValue(pReader->GetRelsPtr()->Add(oFile).get());
				}
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CDataRecordSet::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"DataRecordSet");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute2(L"ConnectionID", ConnectionID);
		pWriter->WriteAttribute2(L"Command", Command);
		pWriter->WriteAttribute2(L"Options", Options);
		pWriter->WriteAttribute2(L"TimeRefreshed", TimeRefreshed);
		pWriter->WriteAttribute2(L"NextRowID", NextRowID);
		pWriter->WriteAttribute2(L"Name", Name);
		pWriter->WriteAttribute(L"RowOrder", RowOrder);
		pWriter->WriteAttribute(L"RefreshOverwriteAll", RefreshOverwriteAll);
		pWriter->WriteAttribute(L"RefreshNoReconciliationUI", RefreshNoReconciliationUI);
		pWriter->WriteAttribute2(L"RefreshInterval", RefreshInterval);
		pWriter->WriteAttribute2(L"ReplaceLinks", ReplaceLinks);
		pWriter->WriteAttribute2(L"Checksum", Checksum);
		pWriter->EndAttributes();

		if (Rel.IsInit())
			Rel->toXmlWriter(pWriter);

		if (DataColumns.IsInit())
			DataColumns->toXmlWriter(pWriter);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"DataRecordSet");
	}
//-----------------------------------------------------------------------------------------------------------
	EElementType CDataRecordSets::getType() const
	{
		return et_dr_DataRecordSets;
	}
	void CDataRecordSets::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"DataRecordSet" == sName)
			{
				CDataRecordSet* pItem = new CDataRecordSet();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	void CDataRecordSets::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "NextID", NextID)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ActiveRecordsetID", ActiveRecordsetID)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "DataWindowOrder", DataWindowOrder)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CDataRecordSets::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
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
				NextID = pReader->GetULong();
			}break;
			case 1:
			{
				ActiveRecordsetID = pReader->GetULong();
			}break;
			case 2:
			{
				DataWindowOrder = pReader->GetString2();
			}break;
			}
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CDataRecordSet());
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
	void CDataRecordSets::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, NextID);
		pWriter->WriteUInt2(1, ActiveRecordsetID);
		pWriter->WriteString2(2, DataWindowOrder);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CDataRecordSets::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"DataRecordSets");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.microsoft.com/office/visio/2012/main");
		pWriter->WriteAttribute2(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->WriteAttribute2(L"xml:space", L"preserve");
		pWriter->WriteAttribute2(L"NextID", NextID);
		pWriter->WriteAttribute2(L"ActiveRecordsetID", ActiveRecordsetID);
		pWriter->WriteAttribute2(L"DataWindowOrder", DataWindowOrder);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"DataRecordSets");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CDataColumn::getType() const
	{
		return et_dr_DataColumn;
	}
	void CDataColumn::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ColumnNameID", ColumnNameID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Name", Name)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Label", Label)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "OrigLabel", OrigLabel)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "LangID", LangID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Calendar", Calendar)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "DataType", DataType)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "UnitType", UnitType)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Currency", Currency)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Degree", Degree)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "DisplayWidth", DisplayWidth)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "DisplayOrder", DisplayOrder)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Mapped", Mapped)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Hyperlink", Hyperlink)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CDataColumn::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void CDataColumn::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, ColumnNameID);
		pWriter->WriteString2(1, Name);
		pWriter->WriteString2(2, Label);
		pWriter->WriteString2(3, OrigLabel);
		pWriter->WriteString2(4, LangID);
		pWriter->WriteUInt2(5, Calendar);
		pWriter->WriteUInt2(6, DataType);
		pWriter->WriteString2(7, UnitType);
		pWriter->WriteUInt2(8, Currency);
		pWriter->WriteUInt2(9, Degree);
		pWriter->WriteUInt2(10, DisplayWidth);
		pWriter->WriteUInt2(11, DisplayOrder);
		pWriter->WriteBool2(12, Mapped);
		pWriter->WriteBool2(13, Hyperlink);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CDataColumn::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				ColumnNameID = pReader->GetString2();
			}break;
			case 1:
			{
				Name = pReader->GetString2();
			}break;
			case 2:
			{
				Label = pReader->GetString2();
			}break;
			case 3:
			{
				OrigLabel = pReader->GetString2();
			}break;
			case 4:
			{
				LangID = pReader->GetString2();
			}break;
			case 5:
			{
				Calendar = pReader->GetULong();
			}break;
			case 6:
			{
				DataType = pReader->GetULong();
			}break;
			case 7:
			{
				UnitType = pReader->GetString2();
			}break;
			case 8:
			{
				Currency = pReader->GetULong();
			}break;
			case 9:
			{
				Degree = pReader->GetULong();
			}break;
			case 10:
			{
				DisplayWidth = pReader->GetULong();
			}break;
			case 11:
			{
				DisplayOrder = pReader->GetULong();
			}break;
			case 12:
			{
				Mapped = pReader->GetBool();
			}break;
			case 13:
			{
				Hyperlink = pReader->GetBool();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CDataColumn::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"DataColumn");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ColumnNameID", ColumnNameID);
		pWriter->WriteAttribute2(L"Name", Name);
		pWriter->WriteAttribute2(L"Label", Label);
		pWriter->WriteAttribute2(L"OrigLabel", OrigLabel);
		pWriter->WriteAttribute2(L"LangID", LangID);
		pWriter->WriteAttribute2(L"Calendar", Calendar);
		pWriter->WriteAttribute2(L"DataType", DataType);
		pWriter->WriteAttribute2(L"UnitType", UnitType);
		pWriter->WriteAttribute2(L"Currency", Currency);
		pWriter->WriteAttribute2(L"Degree", Degree);
		pWriter->WriteAttribute2(L"DisplayWidth", DisplayWidth);
		pWriter->WriteAttribute2(L"DisplayOrder", DisplayOrder);
		pWriter->WriteAttribute(L"Mapped", Mapped);
		pWriter->WriteAttribute(L"Hyperlink", Hyperlink);
		pWriter->EndAttributes();

		pWriter->WriteNodeEnd(L"DataColumn");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	void CDataColumns::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"DataColumn" == sName)
			{
				CDataColumn* pItem = new CDataColumn();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	void CDataColumns::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "SortColumn", SortColumn)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "SortAsc", SortAsc)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	EElementType CDataColumns::getType() const
	{
		return et_dr_DataColumns;
	}
	void CDataColumns::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
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
				SortColumn = pReader->GetString2();
			}break;
			case 1:
			{
				SortAsc = pReader->GetBool();
			}break;
			}
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CDataColumn());
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
	void CDataColumns::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, SortColumn);
		pWriter->WriteBool2(1, SortAsc);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CDataColumns::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"DataColumns");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"SortColumn", SortColumn);
		pWriter->WriteAttribute(L"SortAsc", SortAsc);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"DataColumns");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CRowKeyValue::getType() const
	{
		return et_dr_RowKeyValue;
	}
	void CRowKeyValue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "RowID", RowID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Value", Value)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CRowKeyValue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void CRowKeyValue::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, RowID);
		pWriter->WriteString2(1, Value);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CRowKeyValue::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				RowID = pReader->GetULong();
			}break;
			case 1:
			{
				Value = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CRowKeyValue::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"RowKeyValue");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"RowID", RowID);
		pWriter->WriteAttribute2(L"Value", Value);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"RowKeyValue");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	void CPrimaryKey::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"RowKeyValue" == sName)
			{
				CRowKeyValue* pItem = new CRowKeyValue();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	void CPrimaryKey::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ColumnNameID", ColumnNameID)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	EElementType CPrimaryKey::getType() const
	{
		return et_dr_PrimaryKey;
	}
	void CPrimaryKey::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
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
				ColumnNameID = pReader->GetString2();
			}break;
			}
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CRowKeyValue());
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
	void CPrimaryKey::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, ColumnNameID);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CPrimaryKey::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"PrimaryKey");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ColumnNameID", ColumnNameID);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"PrimaryKey");
	}
	EElementType CRowMap::getType() const
	{
		return et_dr_RowMap;
	}
	void CRowMap::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "RowID", RowID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "PageID", PageID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ShapeID", ShapeID)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CRowMap::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void CRowMap::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, RowID);
		pWriter->WriteUInt2(1, PageID);
		pWriter->WriteUInt2(2, ShapeID);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CRowMap::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				RowID = pReader->GetULong();
			}break;
			case 1:
			{
				PageID = pReader->GetULong();
			}break;
			case 2:
			{
				ShapeID = pReader->GetULong();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CRowMap::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"RowMap");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"RowID", RowID);
		pWriter->WriteAttribute2(L"PageID", PageID);
		pWriter->WriteAttribute2(L"ShapeID", ShapeID);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"RowMap");
	}
	EElementType CRefreshConflict::getType() const
	{
		return et_dr_RefreshConflict;
	}
	void CRefreshConflict::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "RowID", RowID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "PageID", PageID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ShapeID", ShapeID)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CRefreshConflict::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void CRefreshConflict::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, RowID);
		pWriter->WriteUInt2(1, PageID);
		pWriter->WriteUInt2(2, ShapeID);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CRefreshConflict::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				RowID = pReader->GetULong();
			}break;
			case 1:
			{
				PageID = pReader->GetULong();
			}break;
			case 2:
			{
				ShapeID = pReader->GetULong();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CRefreshConflict::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"RefreshConflict");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"RowID", RowID);
		pWriter->WriteAttribute2(L"PageID", PageID);
		pWriter->WriteAttribute2(L"ShapeID", ShapeID);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"RefreshConflict");
	}
	EElementType CAutoLinkComparison::getType() const
	{
		return et_dr_AutoLinkComparison;
	}
	void CAutoLinkComparison::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ColumnName", ColumnName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ContextType", ContextType)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ContextTypeLabel", ContextTypeLabel)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CAutoLinkComparison::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void CAutoLinkComparison::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, ColumnName);
		pWriter->WriteUInt2(1, ContextType);
		pWriter->WriteString2(2, ContextTypeLabel);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CAutoLinkComparison::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				ColumnName = pReader->GetString2();
			}break;
			case 1:
			{
				ContextType = pReader->GetULong();
			}break;
			case 2:
			{
				ContextTypeLabel = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CAutoLinkComparison::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"AutoLinkComparison");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ColumnName", ColumnName);
		pWriter->WriteAttribute2(L"ContextType", ContextType);
		pWriter->WriteAttribute2(L"ContextTypeLabel", ContextTypeLabel);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"AutoLinkComparison");
	}
}
} // namespace OOX
