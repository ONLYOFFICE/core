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

#include "ContentTypes.h"
#include "../XML/XmlSimple.h"

namespace OOX
{
	namespace ContentTypes
	{
		CExtensionTable::CExtensionTable()
		{
			m_mTable.insert(std::make_pair(L"bmp", L"image/bmp"));
			m_mTable.insert(std::make_pair(L"gif", L"image/gif"));
			m_mTable.insert(std::make_pair(L"png", L"image/png"));
			m_mTable.insert(std::make_pair(L"tif", L"image/tiff"));
			m_mTable.insert(std::make_pair(L"tiff", L"image/tiff"));
			m_mTable.insert(std::make_pair(L"jpeg", L"image/jpeg"));
			m_mTable.insert(std::make_pair(L"jpg", L"image/jpeg"));
			m_mTable.insert(std::make_pair(L"jpe", L"image/jpeg"));
			m_mTable.insert(std::make_pair(L"jfif", L"image/jpeg"));
			m_mTable.insert(std::make_pair(L"rels", L"application/vnd.openxmlformats-package.relationships+xml"));
			m_mTable.insert(std::make_pair(L"bin", L"application/vnd.openxmlformats-officedocument.oleObject"));
			m_mTable.insert(std::make_pair(L"xml", L"application/xml"));
			m_mTable.insert(std::make_pair(L"emf", L"image/x-emf"));
			m_mTable.insert(std::make_pair(L"emz", L"image/x-emz"));
			m_mTable.insert(std::make_pair(L"wmf", L"image/x-wmf"));
			m_mTable.insert(std::make_pair(L"svg", L"image/svg+xml"));
			m_mTable.insert(std::make_pair(L"svm", L"image/svm"));
			m_mTable.insert(std::make_pair(L"wdp", L"image/vnd.ms-photo"));
			m_mTable.insert(std::make_pair(L"webp", L"image/webp"));
			m_mTable.insert(std::make_pair(L"wav", L"audio/wav"));
			m_mTable.insert(std::make_pair(L"wma", L"audio/x-wma"));
			m_mTable.insert(std::make_pair(L"m4a", L"audio/unknown"));
			m_mTable.insert(std::make_pair(L"mp3", L"audio/mpeg"));
			m_mTable.insert(std::make_pair(L"mp4", L"video/unknown"));
			m_mTable.insert(std::make_pair(L"mov", L"video/unknown"));
			m_mTable.insert(std::make_pair(L"m4v", L"video/unknown"));
			m_mTable.insert(std::make_pair(L"mkv", L"video/unknown"));
			m_mTable.insert(std::make_pair(L"avi", L"video/avi"));
			m_mTable.insert(std::make_pair(L"flv", L"video/x-flv"));
			m_mTable.insert(std::make_pair(L"wmv", L"video/x-wmv"));
			m_mTable.insert(std::make_pair(L"webm", L"video/webm"));
			m_mTable.insert(std::make_pair(L"xls", L"application/vnd.ms-excel"));
			m_mTable.insert(std::make_pair(L"xlsm", L"application/vnd.ms-excel.sheet.macroEnabled.12"));
			m_mTable.insert(std::make_pair(L"xlsb", L"application/vnd.ms-excel.sheet.binary.macroEnabled.12"));
			m_mTable.insert(std::make_pair(L"xlsx", L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
			m_mTable.insert(std::make_pair(L"ppt", L"application/vnd.ms-powerpoint"));
			m_mTable.insert(std::make_pair(L"pptm", L"application/vnd.ms-powerpoint.presentation.macroEnabled.12"));
			m_mTable.insert(std::make_pair(L"pptx", L"application/vnd.openxmlformats-officedocument.presentationml.presentation"));
			m_mTable.insert(std::make_pair(L"sldm", L"application/vnd.ms-powerpoint.slide.macroEnabled.12"));
			m_mTable.insert(std::make_pair(L"sldx", L"application/vnd.openxmlformats-officedocument.presentationml.slide"));
			m_mTable.insert(std::make_pair(L"doc", L"application/msword"));
			m_mTable.insert(std::make_pair(L"docm", L"application/vnd.ms-word.document.macroEnabled.12"));
			m_mTable.insert(std::make_pair(L"docx", L"application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
			m_mTable.insert(std::make_pair(L"vml", L"application/vnd.openxmlformats-officedocument.vmlDrawing"));
			m_mTable.insert(std::make_pair(L"vsd", L"application/vnd.visio"));
			m_mTable.insert(std::make_pair(L"vsdx", L"application/vnd.ms-visio.drawing"));
		}
		const std::wstring CExtensionTable::operator[] (const std::wstring& sExtension) const
		{
			std::map<std::wstring, std::wstring>::const_iterator it = m_mTable.find(sExtension);
			if (it == m_mTable.end())
				return L"application/unknown";

			return it->second;
		}

		CDefault::CDefault()
		{
		}
		CDefault::CDefault(const std::wstring& sExtension) : m_sExtension(sExtension)
		{
		}
		CDefault::~CDefault()
		{
		}
		void CDefault::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_sExtension = oNode.GetAttribute(L"Extension");
		}
		void CDefault::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		std::wstring CDefault::toXML() const
		{
			static const CExtensionTable oTable;

			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"Extension", m_sExtension);
			oAttr.Write(L"ContentType", oTable[m_sExtension]);

			return XmlUtils::CreateNode(L"Default", oAttr);
		}
		EElementType CDefault::getType() const
		{
			return et_Default;
		}
		void CDefault::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_ReadSingle(oReader, L"Extension", m_sExtension)
				WritingElement_ReadAttributes_End(oReader)
		}

		COverride::COverride()
		{
		}
		COverride::COverride(const std::wstring& sType, const CPath& oPath) : m_sType(sType), m_oPart(oPath)
		{
		}
		COverride::~COverride()
		{
		}
		void COverride::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_oPart = oNode.GetAttribute(L"PartName");
			m_sType = oNode.GetAttribute(L"ContentType");
		}
		void COverride::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		std::wstring COverride::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			std::wstring sPartName = m_oPart.m_strFilename;

			XmlUtils::replace_all(sPartName, L"\\", L"/");

			oAttr.Write(L"PartName", L"/" + sPartName);
			oAttr.Write(L"ContentType", m_sType);

			return XmlUtils::CreateNode(L"Override", oAttr);
		}
		EElementType COverride::getType() const
		{
			return et_Override;
		}
		void COverride::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"PartName", m_oPart)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"ContentType", m_sType)
			WritingElement_ReadAttributes_End(oReader)
		}
		const std::wstring COverride::type() const
		{
			return m_sType;
		}
		const OOX::CPath COverride::filename() const
		{
			return m_oPart;
		}

	} // namespace ContentTypes

	CContentTypes::CContentTypes()
	{
		AddDefault(L"bin");
		AddDefault(L"rels");
		AddDefault(L"jpeg");
		AddDefault(L"png");
		AddDefault(L"wmf");
		//AddDefault(L"bmp");
		//AddDefault(OOX::CPath(L".jpg"));
		//AddDefault(OOX::CPath(L".jpe"));
		//AddDefault(OOX::CPath(L".gif"));
		//AddDefault(OOX::CPath(L".emf"));
		//AddDefault(OOX::CPath(L".xlsx"));
	}
	CContentTypes::CContentTypes(const CPath& oPath)
	{
		Read(oPath);
	}
	CContentTypes::~CContentTypes()
	{
	}
	bool CContentTypes::Read(const CPath& oDirPath)
	{
		OOX::CPath oFullPath = oDirPath / c_oContentTypeFileName;

		XmlUtils::CXmlLiteReader oReader;
		if (!oReader.FromFile(oFullPath.m_strFilename))
			return false;
		return ReadFromReader(oReader);
	}
	bool CContentTypes::ReadFromString(std::wstring& sXml)
	{
		XmlUtils::CXmlLiteReader oReader;
		if (!oReader.FromString(sXml))
			return false;
		return ReadFromReader(oReader);
	}
	void CContentTypes::Merge(CContentTypes* pSrcContentTypes)
	{
		if (pSrcContentTypes == NULL) return;

		for (std::unordered_map<std::wstring, ContentTypes::CDefault>::iterator it = pSrcContentTypes->m_mapDefaults.begin(); it != pSrcContentTypes->m_mapDefaults.end(); ++it)
		{
			AddDefault(it->first);
		}
		for (std::unordered_map<std::wstring, smart_ptr<ContentTypes::COverride>>::iterator it = pSrcContentTypes->m_mapOverrides.begin(); it != pSrcContentTypes->m_mapOverrides.end(); ++it)
		{
			if (m_mapOverrides.find(it->first) == m_mapOverrides.end())
			{
				m_mapOverrides[it->first] = it->second;
			}
		}
	}
	bool CContentTypes::Write(const CPath& oDirPath) const
	{
		std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">";

		for (std::unordered_map<std::wstring, ContentTypes::CDefault>::const_iterator it = m_mapDefaults.begin(); it != m_mapDefaults.end(); ++it)
		{
			sXml += it->second.toXML();
		}

		for (std::unordered_map<std::wstring, smart_ptr<ContentTypes::COverride>>::const_iterator it = m_mapOverrides.begin(); it != m_mapOverrides.end(); ++it)
		{
			if (it->second.IsInit())
			{
				sXml += it->second->toXML();
			}
		}

		sXml += L"</Types>";

		OOX::CPath oFullPath = oDirPath / c_oContentTypeFileName;
		XmlUtils::SaveToFile(oFullPath.m_strFilename, sXml);

		return true;
	}
	void CContentTypes::Registration(const std::wstring& sType, const CPath& oDirectory, const CPath& oFilename)
	{
		OOX::CPath oFullPath = oDirectory / oFilename;

		AddOverride(sType, oFullPath.m_strFilename);
		AddDefault(oFullPath.GetExtention(false));
	}
	void CContentTypes::Registration(const std::wstring& sType, const std::wstring& sDirectory, const std::wstring& sFilename)
	{
		Registration(sType, OOX::CPath(sDirectory), OOX::CPath(sFilename));
	}
	void CContentTypes::AddDefault(const std::wstring& sExtension)
	{
		if (sExtension.empty()) return;

		std::wstring sExt = XmlUtils::GetLower(sExtension);

		if (sExt[0] == L'.') sExt = sExt.substr(1);

		if (m_mapDefaults.find(sExt) == m_mapDefaults.end())
		{
			m_mapDefaults[sExt] = ContentTypes::CDefault(sExt);
		}
	}
	bool CContentTypes::ReadFromReader(XmlUtils::CXmlLiteReader& oReader)
	{
		std::wstring sName;
		if (!oReader.ReadNextNode() || L"Types" != (sName = oReader.GetName()) || oReader.IsEmptyNode())
			return false;

		int nTypesDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nTypesDepth))
		{
			sName = oReader.GetName();

			if (L"Default" == sName)
			{
				ContentTypes::CDefault oDefault;
				oDefault = oReader;
				m_mapDefaults[oDefault.m_sExtension] = oDefault;
			}
			else if (L"Override" == sName)
			{
				smart_ptr<ContentTypes::COverride> oOverride = new ContentTypes::COverride();

				if (oOverride.IsInit())
				{
					oOverride->fromXML(oReader);
					
					m_mapOverrides.insert(std::make_pair(oOverride->filename().GetPath(), oOverride));
					m_mapOverridesByType.insert(std::make_pair(oOverride->m_sType, oOverride));
				}
			}
		}
		return true;
	}
	void CContentTypes::AddOverride(const std::wstring& sType, std::wstring sPath)
	{
		if (sPath[0] == FILE_SEPARATOR_CHAR)
		{
			sPath.erase(0, 1);
		}
		smart_ptr<ContentTypes::COverride> oOverride = new ContentTypes::COverride(sType, sPath);

		m_mapOverrides.insert(std::make_pair(oOverride->filename().GetPath(), oOverride));
		m_mapOverridesByType.insert(std::make_pair(oOverride->m_sType, oOverride));

	}

} // namespace OOX
