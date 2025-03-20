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
#include "CustomsXML.h"

#include "../FileTypes_Spreadsheet.h"

#include "../../../DesktopEditor/common/SystemUtils.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../../Binary/Presentation/XmlWriter.h"

namespace OOX
{
namespace Spreadsheet
{
	CXmlMapsFile::CXmlMapsFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	CXmlMapsFile::CXmlMapsFile(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain)
	{
		read(uri.GetDirectory(), uri);
	}
	CXmlMapsFile::CXmlMapsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	CXmlMapsFile::~CXmlMapsFile()
	{
	}
	void CXmlMapsFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CXmlMapsFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_MapInfo = oReader;
	}
	void CXmlMapsFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_XLSX;

		if (m_MapInfo.IsInit())
			m_MapInfo->toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
	}
	const OOX::FileType CXmlMapsFile::type() const
	{
		return FileTypes::XmlMaps;
	}
	const OOX::CPath CXmlMapsFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath CXmlMapsFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CXmlMapsFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				m_MapInfo.Init();
				m_MapInfo->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void CXmlMapsFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter)
	{
		smart_ptr<OOX::IFileContainer> rels_old = pWriter->GetRels();
		pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>((CMapInfo*)this));

		pWriter->WriteRecord2(0, m_MapInfo);

		pWriter->SetRels(rels_old);
	}
//--------------------------------------------------------------------------------------------------------------
	EElementType CMapInfo::getType() const
	{
		return et_x_MapInfo;
	}
	void CMapInfo::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "SelectionNamespaces", SelectionNamespaces)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CMapInfo::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetNameNoNS();

			OOX::Spreadsheet::WritingElement* pItem = NULL;
			if (L"Schema" == sName)
			{
				pItem = new OOX::Spreadsheet::CSchema();
			}
			else if (L"Map" == sName)
			{
				pItem = new OOX::Spreadsheet::CMap();
			}
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CMapInfo::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, SelectionNamespaces);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			int type = 0xff;					 //todooo predefine type for ???
			switch (m_arrItems[i]->getType())
			{
			case et_x_Schema: type = 0; break;
			case et_x_Map: type = 1; break;
			}
			if (type != 0xff)
				pWriter->WriteRecord2(type, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
		}
	}
	void CMapInfo::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				SelectionNamespaces = pReader->GetString2();
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
				m_arrItems.push_back(new CSchema());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_arrItems.push_back(new CMap());
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
	void CMapInfo::toXmlWriter(NSBinPptxRW::CXmlWriter * pWriter) const
	{
		pWriter->StartNode(L"MapInfo");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
		pWriter->WriteAttribute2(L"SelectionNamespaces", SelectionNamespaces);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"MapInfo");
	}
	EElementType CMap::getType() const
	{
		return et_x_Map;
	}
	void CMap::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Name", Name)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "RootElement", RootElement)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "SchemaID", SchemaID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ShowImportExportValidationErrors", ShowImportExportValidationErrors)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "AutoFit", AutoFit)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Append", Append)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "PreserveSortAFLayout", PreserveSortAFLayout)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "PreserveFormat", PreserveFormat)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CMap::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"DataBinding" == sName)
			{
				DataBinding = oReader;
			}
		}
	}
	void CMap::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteString2(1, Name);
		pWriter->WriteString2(2, RootElement);
		pWriter->WriteString2(3, SchemaID);
		pWriter->WriteBool2(4, ShowImportExportValidationErrors);
		pWriter->WriteBool2(5, AutoFit);
		pWriter->WriteBool2(6, Append);
		pWriter->WriteBool2(7, PreserveSortAFLayout);
		pWriter->WriteBool2(8, PreserveFormat);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		
		pWriter->WriteRecord2(0, DataBinding);
	}
	void CMap::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				RootElement = pReader->GetString2();
			}break;
			case 3:
			{
				SchemaID = pReader->GetString2();
			}break;
			case 4:
			{
				ShowImportExportValidationErrors = pReader->GetBool();
			}break;
			case 5:
			{
				AutoFit = pReader->GetBool();
			}break;
			case 6:
			{
				Append = pReader->GetBool();
			}break;
			case 7:
			{
				PreserveSortAFLayout = pReader->GetBool();
			}break;
			case 8:
			{
				PreserveFormat = pReader->GetBool();
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
				DataBinding.Init();
				DataBinding->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CMap::toXmlWriter(NSBinPptxRW::CXmlWriter * pWriter) const
	{
		pWriter->StartNode(L"Map");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute2(L"RootElement", RootElement);
		pWriter->WriteAttribute2(L"Name", Name);
		pWriter->WriteAttribute2(L"SchemaID", SchemaID);
		pWriter->WriteAttribute(L"ShowImportExportValidationErrors", ShowImportExportValidationErrors);
		pWriter->WriteAttribute(L"AutoFit", AutoFit);
		pWriter->WriteAttribute(L"Append", Append);
		pWriter->WriteAttribute(L"PreserveSortAFLayout", PreserveSortAFLayout);
		pWriter->WriteAttribute(L"PreserveFormat", PreserveFormat);
		pWriter->EndAttributes();

		if (DataBinding.IsInit())
			DataBinding->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Map");
	}
	EElementType CDataBinding::getType() const
	{
		return et_x_Map;
	}
	void CDataBinding::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ConnectionID", ConnectionID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "DataBindingName", DataBindingName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FileBindingName", FileBindingName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "SchemaID", SchemaID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FileBinding", FileBinding)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "DataBindingLoadMode", DataBindingLoadMode)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CDataBinding::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		content = oReader.GetInnerXml();
	}
	void CDataBinding::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter)
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ConnectionID);
		pWriter->WriteString2(1, DataBindingName);
		pWriter->WriteString2(2, FileBindingName);
		pWriter->WriteString2(3, SchemaID);
		pWriter->WriteBool2(4, FileBinding);
		pWriter->WriteUInt2(5, DataBindingLoadMode);
		pWriter->WriteString2(6, content);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CDataBinding::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				ConnectionID = pReader->GetULong();
			}break;
			case 1:
			{
				DataBindingName = pReader->GetString2();
			}break;
			case 2:
			{
				FileBindingName = pReader->GetString2();
			}break;
			case 3:
			{
				SchemaID = pReader->GetString2();
			}break;
			case 4:
			{
				FileBinding = pReader->GetBool();
			}break;
			case 5:
			{
				DataBindingLoadMode = pReader->GetULong();
			}break;
			case 6:
			{
				content = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CDataBinding::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"DataBinding");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ConnectionID", ConnectionID);
		pWriter->WriteAttribute2(L"DataBindingName", DataBindingName);
		pWriter->WriteAttribute2(L"FileBindingName", FileBindingName);
		pWriter->WriteAttribute2(L"SchemaID", SchemaID);
		pWriter->WriteAttribute(L"FileBinding", FileBinding);
		pWriter->WriteAttribute2(L"DataBindingLoadMode", DataBindingLoadMode);
		pWriter->EndAttributes();

		if (content.is_init())
		{
			pWriter->WriteString(*content);
		}
		pWriter->WriteNodeEnd(L"DataBinding");
	}
	EElementType CSchema::getType() const
	{
		return et_x_Schema;
	}
	void CSchema::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "SchemaRef", SchemaRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Namespace", Namespace)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "SchemaLanguage", SchemaLanguage)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CSchema::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		content = oReader.GetInnerXml();
	}
	void CSchema::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, ID);
		pWriter->WriteString2(1, SchemaRef);
		pWriter->WriteString2(2, Namespace);
		pWriter->WriteString2(3, SchemaLanguage);
		pWriter->WriteString2(4, content);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CSchema::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				ID = pReader->GetString2();
			}break;
			case 1:
			{
				SchemaRef = pReader->GetString2();
			}break;
			case 2:
			{
				Namespace = pReader->GetString2();
			}break;
			case 3:
			{
				SchemaLanguage = pReader->GetString2();
			}break;
			case 4:
			{
				content = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CSchema::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Schema");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute2(L"SchemaRef", SchemaRef);
		pWriter->WriteAttribute2(L"Namespace", Namespace);
		pWriter->WriteAttribute2(L"SchemaLanguage", SchemaLanguage);
		pWriter->EndAttributes();

		if (content.is_init())
		{
			pWriter->WriteString(*content);
		}

		pWriter->WriteNodeEnd(L"Schema");
	}
}
} // namespace OOX

