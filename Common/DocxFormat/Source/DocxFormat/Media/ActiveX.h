/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef OOX_ACTIVEX_INCLUDE_H_
#define OOX_ACTIVEX_INCLUDE_H_

#include "Media.h"
#include "../../XlsxFormat/FileTypes_Spreadsheet.h"
#include "../../Common/SimpleTypes_Shared.h"
#include "../IFileContainer.h"

namespace OOX
{
	class COcxPr : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(COcxPr)
		COcxPr()
		{
		}
		virtual ~COcxPr()
		{
		}
		virtual void fromXML(XmlUtils::CXmlNode& node)
		{
		}
        virtual std::wstring toXML() const
		{
			return _T("");
		}
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<ocxPr>"));
			writer.WriteString(_T("</ocxPr>"));
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
				ReadAttributes(oReader);
				
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
		}

		virtual EElementType getType () const
		{
			return et_x_OcxPr;
		}

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)

			WritingElement_ReadAttributes_Read_if		(oReader, _T("ax:name")		, m_oName)
			WritingElement_ReadAttributes_Read_else_if	(oReader, _T("ax:value")	, m_oValue)

			WritingElement_ReadAttributes_End(oReader)
		}
		nullable<std::wstring> m_oName;
		nullable<std::wstring> m_oValue;

		//font
		//picture
	};
	class ActiveX_xml : public File, public OOX::IFileContainer
	{
	public:
		ActiveX_xml(OOX::Document *pMain) : File(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument = false;
		}
		ActiveX_xml(OOX::Document *pMain, const CPath& oRootPath, const CPath& filename) : File(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument = false;
			read( oRootPath, filename );
		}
		virtual ~ActiveX_xml()
		{
			ClearItems();
		}
		virtual void ClearItems()
		{
			for (size_t nIndex = 0; nIndex < m_arrOcxPr.size(); ++nIndex)
			{
				delete m_arrOcxPr[nIndex];
			}
			m_arrOcxPr.clear();
		}
		virtual void read(const CPath& oPath)
		{
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		virtual void read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			IFileContainer::Read( oRootPath, oPath );

			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oPath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if ( _T("ocx") == sName)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
				{
					int nDocumentDepth = oReader.GetDepth();
					while ( oReader.ReadNextSiblingNode( nDocumentDepth ) )
					{
						sName = XmlUtils::GetNameNoNS(oReader.GetName());

						if ( _T("ocxPr") == sName )
						{
							COcxPr* pOcxPr = new COcxPr(oReader);
							m_arrOcxPr.push_back(pOcxPr);
						}
					}
				}
			}
		}
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)

			WritingElement_ReadAttributes_Read_if		(oReader, L"ax:classid"		, m_oClassId)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"ax:persistence"	, m_oPersistence)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"r:id"			, m_oId)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"ax:license"		, m_oLicense)

			WritingElement_ReadAttributes_End(oReader)
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, CContentTypes& content) const
		{
		}
		virtual const FileType type() const
		{
			return OOX::FileTypes::ActiveX_xml;
		}
		virtual const CPath DefaultDirectory() const
		{
			if (m_bDocument) return type().DefaultDirectory();
			else	return L"../" + type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		bool									m_bDocument;
	protected:
		CPath									m_oReadPath;
		
		nullable<std::wstring>					m_oClassId;
		nullable<std::wstring>					m_oLicense;
		nullable<std::wstring>					m_oPersistence;	//(§3.6.2.1, ST_Persistence).
		nullable<SimpleTypes::CRelationshipId >	m_oId;
		std::vector<OOX::COcxPr*>				m_arrOcxPr;
	};

	class ActiveX_bin : public Media
	{
	public:
		ActiveX_bin(OOX::Document *pMain, bool bDocument = true) : Media (pMain, bDocument)
		{
		}
		ActiveX_bin(OOX::Document *pMain, const OOX::CPath& filename) : Media (pMain)
		{
			read(filename);
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, CContentTypes& content) const
		{
		}
		virtual const FileType type() const
		{
			return OOX::FileTypes::ActiveX_bin;
		}
		virtual const CPath DefaultDirectory() const
		{
			if (m_bDocument) return type().DefaultDirectory();
			else	return L"../" + type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return m_filename.GetFilename();
		}
		void set_filename_cache(const std::wstring & file_path)
		{
			m_filenameCache = file_path;
		}
		void set_filename_cache(CPath & file_path)
		{
			m_filenameCache = file_path;
		}
		CPath filename_cache()
		{
			return m_filenameCache;
		}
	protected:
		CPath	m_filenameCache; //image
	};

} // namespace OOX

#endif // OOX_ACTIVEX_INCLUDE_H_
