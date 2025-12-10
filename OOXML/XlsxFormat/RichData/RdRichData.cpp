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
#include "RdRichData.h"

#include "../FileTypes_Spreadsheet.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

#include "../../DocxFormat/Drawing/DrawingExt.h"
#include "../../../DesktopEditor/common/File.h"

#include "../../Binary/Presentation/XmlWriter.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"

namespace OOX
{
namespace Spreadsheet
{
	CRdRichValueFile::CRdRichValueFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	CRdRichValueFile::CRdRichValueFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	CRdRichValueFile::~CRdRichValueFile()
	{
	}
	void CRdRichValueFile::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	const OOX::FileType CRdRichValueFile::type() const
	{
		return OOX::Spreadsheet::FileTypes::RdRichValue;
	}
	const CPath CRdRichValueFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CRdRichValueFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	const CPath& CRdRichValueFile::GetReadPath()
	{
		return m_oReadPath;
	}
	void CRdRichValueFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oPath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_oRvData = oReader;
	}
	void CRdRichValueFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if (false == m_oRvData.IsInit()) return;

		NSStringUtils::CStringBuilder sXml;

		sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		
		m_oRvData->toXML(sXml);

		std::wstring sPath = oPath.GetPath();
		NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

		oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
	}
//---------------------------------------------------------------------------------------------------------------------------
	CRdRichValueStructureFile::CRdRichValueStructureFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	CRdRichValueStructureFile::CRdRichValueStructureFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	CRdRichValueStructureFile::~CRdRichValueStructureFile()
	{
	}
	void CRdRichValueStructureFile::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	const OOX::FileType CRdRichValueStructureFile::type() const
	{
		return OOX::Spreadsheet::FileTypes::RdRichValueStructure;
	}
	const CPath CRdRichValueStructureFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CRdRichValueStructureFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	const CPath& CRdRichValueStructureFile::GetReadPath()
	{
		return m_oReadPath;
	}
	void CRdRichValueStructureFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oPath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_oRvStructures = oReader;
	}
	void CRdRichValueStructureFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if (false == m_oRvStructures.IsInit()) return;

		NSStringUtils::CStringBuilder sXml;

		sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		m_oRvStructures->toXML(sXml);

		std::wstring sPath = oPath.GetPath();
		NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

		oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
	}
//---------------------------------------------------------------------------------------------------------------------------
	CRdRichValueTypesFile::CRdRichValueTypesFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	CRdRichValueTypesFile::CRdRichValueTypesFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	CRdRichValueTypesFile::~CRdRichValueTypesFile()
	{
	}
	void CRdRichValueTypesFile::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	const OOX::FileType CRdRichValueTypesFile::type() const
	{
		return OOX::Spreadsheet::FileTypes::RdRichValueTypes;
	}
	const CPath CRdRichValueTypesFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CRdRichValueTypesFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	const CPath& CRdRichValueTypesFile::GetReadPath()
	{
		return m_oReadPath;
	}
	void CRdRichValueTypesFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oPath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_oRvTypesInfo = oReader;
	}
	void CRdRichValueTypesFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if (false == m_oRvTypesInfo.IsInit()) return;

		NSStringUtils::CStringBuilder sXml;

		sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		m_oRvTypesInfo->toXML(sXml);

		std::wstring sPath = oPath.GetPath();
		NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

		oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
	}
//-----------------------------------------------------------------------------------
	CRichValueData::CRichValueData() {}
	CRichValueData::~CRichValueData() {}
	void CRichValueData::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueData::toXML() const
	{
		return L"";
	}
	EElementType CRichValueData::getType() const
	{
		return et_x_RichValueData;
	}
	void CRichValueData::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"rv" == sName)
			{
				CRichValue* pRichValue = new CRichValue();
				if (pRichValue)
				{
					pRichValue->fromXML(oReader);
					m_arrItems.push_back(pRichValue);
				}
			}
			else if (L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CRichValueData::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if (m_arrItems.empty()) return;

		writer.WriteString(L"<rvData xmlns=\"http://schemas.microsoft.com/office/spreadsheetml/2017/richdata\" count=\"" + 
std::to_wstring(m_arrItems.size()) + L"\">");

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}
		writer.WriteString(L"</rvData>");
	}
//-----------------------------------------------------------------------------------
	CRichValueFallback::CRichValueFallback() {}
	CRichValueFallback::~CRichValueFallback() {}
	void CRichValueFallback::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueFallback::toXML() const
	{
		return L"";
	}
	EElementType CRichValueFallback::getType() const
	{
		return et_x_RichValueFallback;
	}
	void CRichValueFallback::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<fb");
		WritingStringNullableAttrString(L"t", m_oT, m_oT->ToString());
		
		if (m_sContent.empty())
			writer.WriteString(L"/>");
		else
		{
			writer.WriteString(L">");
			writer.WriteString(m_sContent);
			writer.WriteString(L"</fb>");
		}
	}
	void CRichValueFallback::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		m_sContent = oReader.GetText2();
	}
	void CRichValueFallback::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_ReadSingle(oReader, L"t", m_oT)
		WritingElement_ReadAttributes_End(oReader)
	}
//-------------------------------------------------------------------------------------
	CRichValue::CRichValue() {}
	CRichValue::~CRichValue() {}
	void CRichValue::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValue::toXML() const
	{
		return L"";
	}
	EElementType CRichValue::getType() const
	{
		return et_x_RichValue;
	}
	void CRichValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<rv");
		WritingStringNullableAttrInt2(L"s", m_oS);

		if (m_arrV.empty() && false == m_oFb.IsInit())
			writer.WriteString(L"/>");
		else
		{
			writer.WriteString(L">");
			for (auto v : m_arrV)
			{
				writer.WriteString(L"<v>" + v + L"</v>");
			}
			if (m_oFb.IsInit())
				m_oFb->toXML();
			writer.WriteString(L"</rv>");
		}
	}
	void CRichValue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"v" == sName)
			{
				m_arrV.push_back(oReader.GetText2());
			}
			else if (L"fb" == sName)
			{
				m_oFb = oReader;
			}
		}
	}
	void CRichValue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_ReadSingle(oReader, L"s", m_oS)
		WritingElement_ReadAttributes_End(oReader)
	}
//-------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
	CRichValueStructures::CRichValueStructures() {}
	CRichValueStructures::~CRichValueStructures() {}
	void CRichValueStructures::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueStructures::toXML() const
	{
		return L"";
	}
	EElementType CRichValueStructures::getType() const
	{
		return et_x_RichValueStructures;
	}
	void CRichValueStructures::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"s" == sName)
			{
				CRichValueStructure* pStructure = new CRichValueStructure();
				if (pStructure)
				{
					pStructure->fromXML(oReader);
					m_arrItems.push_back(pStructure);
				}
			}
			else if (L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CRichValueStructures::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if (m_arrItems.empty()) return;

		writer.WriteString(L"<rvStructures xmlns=\"http://schemas.microsoft.com/office/spreadsheetml/2017/richdata\" count=\"" +
			std::to_wstring(m_arrItems.size()) + L"\">");

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}
		writer.WriteString(L"</rvStructures>");
	}
//-----------------------------------------------------------------------------------
	CRichValueStructure::CRichValueStructure() {}
	CRichValueStructure::~CRichValueStructure() {}
	void CRichValueStructure::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueStructure::toXML() const
	{
		return L"";
	}
	EElementType CRichValueStructure::getType() const
	{
		return et_x_RichValueStructure;
	}
	void CRichValueStructure::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<s");
			WritingStringNullableAttrEncodeXmlString2(L"t", m_oT);
		writer.WriteString(L">");

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}
		writer.WriteString(L"</s>");
	}
	void CRichValueStructure::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"k" == sName)
			{
				CRichValueKey* pKey = new CRichValueKey();
				if (pKey)
				{
					pKey->fromXML(oReader);
					m_arrItems.push_back(pKey);
				}
			}
		}
	}
	void CRichValueStructure::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_ReadSingle(oReader, L"t", m_oT)
		WritingElement_ReadAttributes_End(oReader)
	}
//-------------------------------------------------------------------------------------
	CRichValueKey::CRichValueKey() {}
	CRichValueKey::~CRichValueKey() {}
	void CRichValueKey::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueKey::toXML() const
	{
		return L"";
	}
	EElementType CRichValueKey::getType() const
	{
		return et_x_RichValueKey;
	}
	void CRichValueKey::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<k");
			WritingStringNullableAttrEncodeXmlString2(L"n", m_oN);
			WritingStringNullableAttrString(L"t", m_oT, m_oT->ToString());
		writer.WriteString(L"/>");
	}
	void CRichValueKey::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void CRichValueKey::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"n", m_oN)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"t", m_oT)
		WritingElement_ReadAttributes_End(oReader)
	}
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
	CRichValueTypesInfo::CRichValueTypesInfo() {}
	CRichValueTypesInfo::~CRichValueTypesInfo() {}
	void CRichValueTypesInfo::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueTypesInfo::toXML() const
	{
		return L"";
	}
	EElementType CRichValueTypesInfo::getType() const
	{
		return et_x_RichValueTypesInfo;
	}
	void CRichValueTypesInfo::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"global" == sName)
			{
				m_oGlobal = oReader;
			}
			else if (L"types" == sName)
			{
				m_oTypes = oReader;
			}
			else if (L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CRichValueTypesInfo::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<rvTypesInfo xmlns=\"http://schemas.microsoft.com/office/spreadsheetml/2017/richdata2\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"x\" \
xmlns:x=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
>");
		if (m_oGlobal.IsInit())
			m_oGlobal->toXML(writer);

		if (m_oTypes.IsInit())
			m_oTypes->toXML(writer);

		writer.WriteString(L"</rvTypesInfo>");
	}
//-----------------------------------------------------------------------------------
	CRichValueTypes::CRichValueTypes() {}
	CRichValueTypes::~CRichValueTypes() {}
	void CRichValueTypes::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueTypes::toXML() const
	{
		return L"";
	}
	EElementType CRichValueTypes::getType() const
	{
		return et_x_RichValueTypes;
	}
	void CRichValueTypes::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"types" == sName)
			{
				CRichValueType* pType = new CRichValueType();
				if (pType)
				{
					pType->fromXML(oReader);
					m_arrItems.push_back(pType);
				}
			}
		}
	}
	void CRichValueTypes::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if (m_arrItems.empty()) return;

		writer.WriteString(L"<types>");

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}
		writer.WriteString(L"</types>");
	}
//-----------------------------------------------------------------------------------
	CRichValueType::CRichValueType() {}
	CRichValueType::~CRichValueType() {}
	void CRichValueType::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueType::toXML() const
	{
		return L"";
	}
	EElementType CRichValueType::getType() const
	{
		return et_x_RichValueType;
	}
	void CRichValueType::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"keyFlags" == sName)
			{
				m_oKeyFlags = oReader;
			}
			else if (L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CRichValueType::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<type");
			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
		writer.WriteString(L">");

		if (m_oKeyFlags.IsInit())
			m_oKeyFlags->toXML(writer);

		writer.WriteString(L"</type>");
	}
	void CRichValueType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"name", m_oName)
		WritingElement_ReadAttributes_End(oReader)
	}
//-----------------------------------------------------------------------------------
	void CRichValueTypeKeyFlags::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueTypeKeyFlags::toXML() const
	{
		return L"";
	}
	EElementType CRichValueTypeKeyFlags::getType() const
	{
		return et_x_RichValueTypeKeyFlags;
	}
	void CRichValueTypeKeyFlags::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"key" == sName)
			{
				CRichValueTypeReservedKey* pType = new CRichValueTypeReservedKey();
				if (pType)
				{
					pType->fromXML(oReader);
					m_arrItems.push_back(pType);
				}
			}
		}
	}
	void CRichValueTypeKeyFlags::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if (m_arrItems.empty()) return;

		writer.WriteString(L"<keyFlags>");

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}
		writer.WriteString(L"</keyFlags>");
	}
//-----------------------------------------------------------------------------------
	CRichValueTypeReservedKey::CRichValueTypeReservedKey() {}
	CRichValueTypeReservedKey::~CRichValueTypeReservedKey() {}
	void CRichValueTypeReservedKey::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueTypeReservedKey::toXML() const
	{
		return L"";
	}
	EElementType CRichValueTypeReservedKey::getType() const
	{
		return et_x_RichValueTypeReservedKey;
	}
	void CRichValueTypeReservedKey::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"flag" == sName)
			{
				CRichValueTypeReservedKeyFlag* pFlag = new CRichValueTypeReservedKeyFlag();
				if (pFlag)
				{
					pFlag->fromXML(oReader);
					m_arrItems.push_back(pFlag);
				}
			}
		}
	}
	void CRichValueTypeReservedKey::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if (m_arrItems.empty()) return;

		writer.WriteString(L"<key ");
		WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);

		if (m_arrItems.empty())
			writer.WriteString(L"/>");
		else
		{
			writer.WriteString(L">");
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</key>");
		}
	}
	void CRichValueTypeReservedKey::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_ReadSingle(oReader, L"name", m_oName)
		WritingElement_ReadAttributes_End(oReader)
	}
//-------------------------------------------------------------------------------------
	CRichValueTypeReservedKeyFlag::CRichValueTypeReservedKeyFlag() {}
	CRichValueTypeReservedKeyFlag::~CRichValueTypeReservedKeyFlag() {}
	void CRichValueTypeReservedKeyFlag::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueTypeReservedKeyFlag::toXML() const
	{
		return L"";
	}
	EElementType CRichValueTypeReservedKeyFlag::getType() const
	{
		return et_x_RichValueTypeReservedKeyFlag;
	}
	void CRichValueTypeReservedKeyFlag::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<flag");
		WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
		WritingStringNullableAttrBool2(L"value", m_oValue);
		writer.WriteString(L"/>");
	}
	void CRichValueTypeReservedKeyFlag::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void CRichValueTypeReservedKeyFlag::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"name", m_oName)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"value", m_oValue)
		WritingElement_ReadAttributes_End(oReader)
	}
//-----------------------------------------------------------------------------------
	CRichValueGlobalType::CRichValueGlobalType() {}
	CRichValueGlobalType::~CRichValueGlobalType() {}
	void CRichValueGlobalType::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	std::wstring CRichValueGlobalType::toXML() const
	{
		return L"";
	}
	EElementType CRichValueGlobalType::getType() const
	{
		return et_x_RichValueGlobalType;
	}
	void CRichValueGlobalType::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"keyFlags" == sName)
			{
				m_oKeyFlags = oReader;
			}
			else if (L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CRichValueGlobalType::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<global>");

		if (m_oKeyFlags.IsInit())
			m_oKeyFlags->toXML(writer);

		writer.WriteString(L"</global>");
	}

}
}

