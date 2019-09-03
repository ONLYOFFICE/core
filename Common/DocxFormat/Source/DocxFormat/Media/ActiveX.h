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

#include "Media.h"
#include "../../XlsxFormat/FileTypes_Spreadsheet.h"
#include "../../XlsxFormat/ComplexTypes_Spreadsheet.h"
#include "../../Common/SimpleTypes_Shared.h"
#include "../IFileContainer.h"

#include <boost/smart_ptr/shared_array.hpp>

class MemoryStream;

namespace OOX
{	
	namespace Spreadsheet
	{
		class CFormControlPr;
	}

	class ActiveXObject
	{
	public:
		ActiveXObject() {}
		static ActiveXObject* Create(const std::wstring &class_id);
		virtual void Parse(unsigned char* pData, DWORD size) = 0;

		void toFormControlPr(OOX::Spreadsheet::CFormControlPr* pFormControlPr); 
		std::wstring ReadString(MemoryStream *stream, size_t size, bool bCompressed);

		nullable<SimpleTypes::Spreadsheet::CObjectType<>> m_oObjectType;

		nullable_int	m_oForeColor;
		nullable_int	m_oBackColor;
		nullable_int	m_oBorderColor;
		nullable_int	m_oBorderStyle;
		nullable_string	m_oCaption;
		nullable_int	m_oMin;
		nullable_int	m_oMax;
		nullable_int	m_oPosition;
		nullable_int	m_oSmallChange;
		nullable_int	m_oLargeChange;
		nullable_int	m_oOrientation;
		nullable_int	m_oDelay;
		nullable_string	m_oValue;
		nullable_bool	m_oPasswordEdit;
		nullable<SimpleTypes::Spreadsheet::CSelType<>> m_oSelType;
		nullable_bool	m_oLockText;

		nullable_int	m_oWidth;
		nullable_int	m_oHeight;
	};

	class ActiveXObjectScroll : public ActiveXObject
	{
	public:
		ActiveXObjectScroll()
		{
			m_oObjectType.Init();
			m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectScroll);
		}
		virtual void Parse(unsigned char* pData, DWORD size);
	};
	class ActiveXObjectButton : public ActiveXObject
	{
	public:
		ActiveXObjectButton()
		{
			m_oObjectType.Init();
			m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectButton);
		}
		virtual void Parse(unsigned char* pData, DWORD size);
	};
	class ActiveXObjectImage : public ActiveXObject
	{
	public:
		ActiveXObjectImage() : bTile(false), nImageSize(0)
		{
		}
		virtual void Parse(unsigned char* pData, DWORD size);

		size_t nImageSize;
		boost::shared_array<unsigned char> pImageData;
		bool bTile;
	};
	class ActiveXObjectLabel : public ActiveXObject
	{
	public:
		ActiveXObjectLabel()
		{
			m_oObjectType.Init();
			m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectLabel);
		}
		virtual void Parse(unsigned char* pData, DWORD size);
	};
	class ActiveXObjectSpin : public ActiveXObject
	{
	public:
		ActiveXObjectSpin()
		{
			m_oObjectType.Init();
			m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectSpin);
		}
		virtual void Parse(unsigned char* pData, DWORD size);
	};
	
	class ActiveXObjectFormControl : public ActiveXObject
	{
	public:
		ActiveXObjectFormControl()
		{
			m_oObjectType = SimpleTypes::Spreadsheet::objectDialog;
		}
		virtual void Parse(unsigned char* pData, DWORD size);
	};
	class ActiveXObjectMorphData : public ActiveXObject
	{
	public:
		ActiveXObjectMorphData(SimpleTypes::Spreadsheet::EObjectType type)
		{
			m_oObjectType = type;
		}
		virtual void Parse(unsigned char* pData, DWORD size);
	};
	//TabStrip

//-------------------------------------------------------------------------------------------------
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
			return L"";
		}
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<ocxPr>");
			writer.WriteString(L"</ocxPr>");
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

			WritingElement_ReadAttributes_Read_if		(oReader, L"ax:name", m_oName)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"ax:value", m_oValue)

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
		virtual void read(const CPath& oRootPath, const CPath& oPath);
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		void read_bin(const CPath& oPath);

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
		CPath									m_oReadPath;
		
		nullable_string							m_oClassId;
		nullable_string							m_oLicense;
		nullable_string							m_oPersistence;	//(§3.6.2.1, ST_Persistence).
		nullable<SimpleTypes::CRelationshipId >	m_oId;
		std::vector<OOX::COcxPr*>				m_arrOcxPr;
//---------bin
		nullable<ActiveXObject>					m_oObject;
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
		virtual void read(const CPath& filename)
		{
			Media::read(filename);
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

		CPath m_filenameCache; //image
	};

} // namespace OOX
