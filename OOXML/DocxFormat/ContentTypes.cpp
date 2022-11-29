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

#include "ContentTypes.h"
#include "../XML/XmlSimple.h"

namespace OOX
{
	namespace ContentTypes
	{
		CExtensionTable::CExtensionTable()
		{
			m_mTable.insert( std::make_pair( _T("bmp"),  _T("image/bmp")));
			m_mTable.insert( std::make_pair(  _T("gif"),  _T("image/gif")));
			m_mTable.insert( std::make_pair(  _T("png"),  _T("image/png")));
			m_mTable.insert( std::make_pair(  _T("tif"),  _T("image/tiff")));
			m_mTable.insert( std::make_pair(  _T("tiff"), _T("image/tiff")));
			m_mTable.insert( std::make_pair(  _T("jpeg"), _T("image/jpeg")));
			m_mTable.insert( std::make_pair(  _T("jpg"),  _T("image/jpeg")));
			m_mTable.insert( std::make_pair(  _T("jpe"),  _T("image/jpeg")));
			m_mTable.insert( std::make_pair(  _T("jfif"), _T("image/jpeg")));
			m_mTable.insert( std::make_pair(  _T("rels"), _T("application/vnd.openxmlformats-package.relationships+xml")));
			m_mTable.insert( std::make_pair(  _T("bin"),  _T("application/vnd.openxmlformats-officedocument.oleObject")));
			m_mTable.insert( std::make_pair(  _T("xml"),  _T("application/xml")));
			m_mTable.insert( std::make_pair(  _T("emf"),  _T("image/x-emf")));
			m_mTable.insert( std::make_pair(  _T("emz"),  _T("image/x-emz")));
			m_mTable.insert( std::make_pair(  _T("wmf"),  _T("image/x-wmf")));
			m_mTable.insert( std::make_pair(  _T("svg"),  _T("image/svg+xml")));
			m_mTable.insert( std::make_pair(  _T("svm"),  _T("image/svm")));
			m_mTable.insert( std::make_pair(  _T("wdp"),  _T("image/vnd.ms-photo")));
			m_mTable.insert( std::make_pair(  _T("wav"),  _T("audio/wav")));
			m_mTable.insert( std::make_pair(  _T("wma"),  _T("audio/x-wma")));
			m_mTable.insert( std::make_pair(  _T("m4a"),  _T("audio/unknown")));
			m_mTable.insert( std::make_pair(  _T("mp3"),  _T("audio/mpeg")));
			m_mTable.insert( std::make_pair(  _T("mp4"),  _T("video/unknown")));
			m_mTable.insert( std::make_pair(  _T("mov"),  _T("video/unknown")));
			m_mTable.insert( std::make_pair(  _T("m4v"),  _T("video/unknown")));
			m_mTable.insert( std::make_pair(  _T("mkv"),  _T("video/unknown")));
			m_mTable.insert( std::make_pair(  _T("avi"),  _T("video/avi")));
			m_mTable.insert( std::make_pair(  _T("flv"),  _T("video/x-flv")));
			m_mTable.insert( std::make_pair(  _T("wmv"),  _T("video/x-wmv")));
			m_mTable.insert( std::make_pair(  _T("webm"), _T("video/webm")));
			m_mTable.insert( std::make_pair(  _T("xls"),  _T("application/vnd.ms-excel")));
			m_mTable.insert( std::make_pair(  _T("xlsm"), _T("application/vnd.ms-excel.sheet.macroEnabled.12")));
			m_mTable.insert( std::make_pair(  _T("xlsb"), _T("application/vnd.ms-excel.sheet.binary.macroEnabled.12")));
			m_mTable.insert( std::make_pair(  _T("xlsx"), _T("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")));
			m_mTable.insert( std::make_pair(  _T("ppt"),  _T("application/vnd.ms-powerpoint")));
			m_mTable.insert( std::make_pair(  _T("pptm"), _T("application/vnd.ms-powerpoint.presentation.macroEnabled.12")));
			m_mTable.insert( std::make_pair(  _T("pptx"), _T("application/vnd.openxmlformats-officedocument.presentationml.presentation")));
			m_mTable.insert( std::make_pair(  _T("sldm"), _T("application/vnd.ms-powerpoint.slide.macroEnabled.12")));
			m_mTable.insert( std::make_pair(  _T("sldx"), _T("application/vnd.openxmlformats-officedocument.presentationml.slide")));
			m_mTable.insert( std::make_pair(  _T("doc"),  _T("application/msword")));
			m_mTable.insert( std::make_pair(  _T("docm"), _T("application/vnd.ms-word.document.macroEnabled.12")));
			m_mTable.insert( std::make_pair(  _T("docx"), _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document")));
			m_mTable.insert( std::make_pair(  _T("vml"),  _T("application/vnd.openxmlformats-officedocument.vmlDrawing")));
			m_mTable.insert( std::make_pair(  _T("vsd"),  _T("application/vnd.visio")));
			m_mTable.insert( std::make_pair(  _T("vsdx"), _T("application/vnd.ms-visio.drawing")));
		}
		const std::wstring CExtensionTable::operator[] (const std::wstring& sExtension) const
		{
			std::map<std::wstring, std::wstring>::const_iterator it = m_mTable.find(sExtension);
			if (it == m_mTable.end())
				return _T("application/unknown");

			return it->second;
		}

		CDefault::CDefault()
		{
			m_sExtension = _T("");
		}
		CDefault::CDefault(const std::wstring& sExtension) : m_sExtension(sExtension)
		{
		}
		CDefault::~CDefault()
		{
		}
		void CDefault::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_sExtension = oNode.GetAttribute(_T("Extension"));
		}
		void CDefault::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CDefault::toXML() const
		{
			static const CExtensionTable oTable;

			XmlUtils::CAttribute oAttr;
			oAttr.Write( _T("Extension"),   m_sExtension );
			oAttr.Write( _T("ContentType"), oTable[m_sExtension] );

			return XmlUtils::CreateNode(_T("Default"), oAttr );
		}
		EElementType CDefault::getType() const
		{
			return et_Default;
		}
		void CDefault::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, _T("Extension"), m_sExtension )
			WritingElement_ReadAttributes_End( oReader )
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
			m_oPart = oNode.GetAttribute(_T("PartName"));
			m_sType = oNode.GetAttribute(_T("ContentType"));
		}
		void COverride::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring COverride::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			std::wstring sPartName = m_oPart.m_strFilename;

			XmlUtils::replace_all(sPartName, L"\\", L"/");

			oAttr.Write( _T("PartName"), _T("/") + sPartName);
			oAttr.Write( _T("ContentType"), m_sType );

			return XmlUtils::CreateNode(_T("Override"), oAttr);
		}
		EElementType COverride::getType() const
		{
			return et_Override;
		}
		void COverride::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("PartName"),    m_oPart )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("ContentType"), m_sType )
			WritingElement_ReadAttributes_End( oReader )
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
		//AddDefault(OOX::CPath(_T(".jpg")));
		//AddDefault(OOX::CPath(_T(".jpe")));
		//AddDefault(OOX::CPath(_T(".gif")));
		//AddDefault(OOX::CPath(_T(".emf")));
		//AddDefault(OOX::CPath(_T(".xlsx")));
	}
	CContentTypes::CContentTypes(const CPath& oPath)
	{
		Read( oPath );
	}
	CContentTypes::~CContentTypes()
	{
	}
	bool CContentTypes::Read (const CPath& oDirPath)
	{
		OOX::CPath oFullPath = oDirPath / c_oContentTypeFileName;

		XmlUtils::CXmlLiteReader oReader;
		if ( !oReader.FromFile( oFullPath.m_strFilename ) )
			return false;
		return ReadFromReader(oReader);
	}
	bool CContentTypes::ReadFromString (std::wstring& sXml)
	{
		XmlUtils::CXmlLiteReader oReader;
		if ( !oReader.FromString( sXml ) )
			return false;
		return ReadFromReader(oReader);
	}
	void CContentTypes::Merge(CContentTypes * pSrcContentTypes)
	{
		if (pSrcContentTypes == NULL) return;

		for (boost::unordered_map<std::wstring, ContentTypes::CDefault>::iterator it = pSrcContentTypes->m_mapDefaults.begin(); it != pSrcContentTypes->m_mapDefaults.end(); ++it)
		{
			AddDefault(it->first);
		}
		for (boost::unordered_map<std::wstring, ContentTypes::COverride>::iterator it = pSrcContentTypes->m_mapOverrides.begin(); it != pSrcContentTypes->m_mapOverrides.end(); ++it)
		{
			if (m_mapOverrides.find(it->first) == m_mapOverrides.end())
			{
				m_mapOverrides [it->first] = it->second;
			}
		}
	}
	bool CContentTypes::Write(const CPath& oDirPath) const
	{
		std::wstring sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">");

		for (boost::unordered_map<std::wstring, ContentTypes::CDefault>::const_iterator it = m_mapDefaults.begin(); it != m_mapDefaults.end(); ++it)
		{
			sXml += it->second.toXML();
		}

		for (boost::unordered_map<std::wstring, ContentTypes::COverride>::const_iterator it = m_mapOverrides.begin(); it != m_mapOverrides.end(); ++it)
		{
			sXml += it->second.toXML();
		}

		sXml += _T("</Types>");

		OOX::CPath oFullPath = oDirPath / c_oContentTypeFileName;
		XmlUtils::SaveToFile( oFullPath.m_strFilename, sXml );

		return true;
	}
	void CContentTypes::Registration(const std::wstring& sType, const CPath& oDirectory, const CPath& oFilename)
	{
		OOX::CPath oFullPath = oDirectory / oFilename;

		AddOverride( sType, oFullPath.m_strFilename );
		AddDefault ( oFullPath.GetExtention(false) );
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

		if (m_mapDefaults.find (sExt) == m_mapDefaults.end())
		{
			m_mapDefaults [sExt] = ContentTypes::CDefault( sExt );
		}
	}
	bool CContentTypes::ReadFromReader (XmlUtils::CXmlLiteReader& oReader)
	{
		std::wstring sName;
		if ( !oReader.ReadNextNode() || _T("Types") != ( sName = oReader.GetName() ) || oReader.IsEmptyNode() )
			return false;

		int nTypesDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nTypesDepth ) )
		{
			sName = oReader.GetName();

			if ( _T("Default") == sName )
			{
				ContentTypes::CDefault oDefault = oReader;
				m_mapDefaults [ oDefault.m_sExtension] = oDefault ;
			}
			else if ( _T("Override") == sName )
			{
				ContentTypes::COverride oOverride = oReader;
				m_mapOverrides [oOverride.filename().GetPath()] = oOverride;
			}
		}
		return true;
	}
	void CContentTypes::AddOverride(const std::wstring& sType, std::wstring sPath)
		{
			if (sPath[0] ==  FILE_SEPARATOR_CHAR)
			{
				sPath.erase(0, 1);
			}
			ContentTypes::COverride oOverride( sType, sPath );

			m_mapOverrides [oOverride.filename().GetPath()] = oOverride;
		}

} // namespace OOX
