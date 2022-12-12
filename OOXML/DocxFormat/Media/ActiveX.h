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
#include "../Document.h"

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
		virtual ~ActiveXObject() {}
		static ActiveXObject* Create(const std::wstring &class_id);
		static ActiveXObject* Create(_UINT16 type);
		virtual void Parse(unsigned char* pData, DWORD size) = 0;
		virtual std::wstring toXml();

		void toFormControlPr(OOX::Spreadsheet::CFormControlPr* pFormControlPr); 
		
		std::wstring readString(MemoryStream *stream, size_t CountOfCharsWithCompressionFlag);
		void readArrayString(MemoryStream *stream, std::vector<std::wstring> &Array, size_t size);		
		void readTextProps(MemoryStream *stream);
		_UINT32 readColumnInfo(MemoryStream *stream);

		std::pair<boost::shared_array<unsigned char>, size_t> readStdPicture(MemoryStream *stream);
		_GUID_ readGUID(MemoryStream *stream);
		void readStdFont(MemoryStream *stream);

		nullable<SimpleTypes::Spreadsheet::CObjectType> m_oObjectType;

		nullable_uint	m_oForeColor;
		nullable_uint	m_oBackColor;
		nullable_uint	m_oBorderColor;
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
		nullable<SimpleTypes::Spreadsheet::CSelType> m_oSelType;
		nullable_int	m_oListRows;
		nullable_int	m_oScrollBarsType;
		nullable_bool	m_oLockText;
		nullable_bool	m_oMultiLine;

		nullable_uint	m_oWidth;
		nullable_uint	m_oHeight;

		nullable_string	m_oFontName;
		nullable_uint	m_oFontHeight;
		nullable_bool	m_oFontBold;
		nullable_bool	m_oFontItalic;
		nullable_bool	m_oFontUnderline;
		nullable_bool	m_oFontStrikeout;
		nullable_bool	m_oFontAutoColor;

		nullable<std::pair<boost::shared_array<unsigned char>, size_t>> m_oPicture;
		nullable<std::pair<boost::shared_array<unsigned char>, size_t>> m_oMouseIcon;

		std::vector<_UINT32> m_arColumnInfo;
	};

	class ActiveXObjectScroll : public ActiveXObject
	{
	public:
		ActiveXObjectScroll()
		{
			m_oObjectType.Init();
			m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectScroll);
		}
		virtual ~ActiveXObjectScroll() {}
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
		virtual ~ActiveXObjectButton() {}
		virtual void Parse(unsigned char* pData, DWORD size);
	};
	class ActiveXObjectImage : public ActiveXObject
	{
	public:
		ActiveXObjectImage() : nImageSize(0)
		{
			m_oObjectType.Init();
			m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectImage);
		}
		virtual ~ActiveXObjectImage() {}
		virtual void Parse(unsigned char* pData, DWORD size);
		virtual std::wstring toXml();

		size_t nImageSize;
		boost::shared_array<unsigned char> pImageData;
		
		nullable_bool m_oTile;
		nullable_int m_oPictureSizeMode;
	};
	class ActiveXObjectLabel : public ActiveXObject
	{
	public:
		ActiveXObjectLabel()
		{
			m_oObjectType.Init();
			m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectLabel);
		}
		virtual ~ActiveXObjectLabel() {}
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
		virtual ~ActiveXObjectSpin() {}
		virtual void Parse(unsigned char* pData, DWORD size);
	};
	
	class ActiveXObjectFormControl : public ActiveXObject
	{
	public:
		ActiveXObjectFormControl()
		{
			m_oObjectType.Init();
			m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectDialog);
		}
		virtual ~ActiveXObjectFormControl() {}
		virtual void Parse(unsigned char* pData, DWORD size);
		
// FormDataBlock
		_CP_OPT(_UINT32) m_oNextAvailableID;
		_CP_OPT(unsigned char) m_oMousePointer;
		_CP_OPT(unsigned char) m_oScrollBars;
		_CP_OPT(_UINT32) m_oGroupCnt;
		_CP_OPT(unsigned char) m_oCycle;
		_CP_OPT(unsigned char) m_oSpecialEffect;
		_CP_OPT(_UINT32) m_oZoom;
		_CP_OPT(unsigned char) m_oPictureAlignment;
		_CP_OPT(unsigned char) m_oPictureSizeMode;
		_CP_OPT(_UINT32) m_oShapeCookie;
		_CP_OPT(_UINT32) m_oDrawBuffer;
// FormExtraDataBlock
		_CP_OPT(_UINT32) m_oDisplayedWidth;
		_CP_OPT(_UINT32) m_oDisplayedHeight;
		_CP_OPT(_UINT32) m_oLogicalWidth;
		_CP_OPT(_UINT32) m_oLogicalHeight;
		_CP_OPT(_UINT32) m_oScrollTop;
		_CP_OPT(_UINT32) m_oScrollLeft;

// FormStreamData
		_CP_OPT(_GUID_) m_oMouseIconGUID;
		_CP_OPT(_GUID_) m_oFontGUID;
		_CP_OPT(_GUID_) m_oPictureGUID;

	};
	class ActiveXObjectFrame : public ActiveXObjectFormControl
	{
	public:
		ActiveXObjectFrame()
		{
			m_oObjectType.Init();
			m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectGBox);
		}
		virtual ~ActiveXObjectFrame() {}
	};
	class ActiveXObjectMorphData : public ActiveXObject
	{
	public:
		ActiveXObjectMorphData()
		{
		}
		ActiveXObjectMorphData(SimpleTypes::Spreadsheet::EObjectType type)
		{
			m_oObjectType.Init();
			m_oObjectType->SetValue(type);
		}
		virtual ~ActiveXObjectMorphData() {}
		virtual void Parse(unsigned char* pData, DWORD size);
	};
	class ActiveXObjectTabStrip : public ActiveXObject
	{
	public:
		ActiveXObjectTabStrip()
		{
			m_oObjectType.Init();
			m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectTabStrip);
		}
		virtual ~ActiveXObjectTabStrip() {}
		virtual void Parse(unsigned char* pData, DWORD size);
		virtual std::wstring toXml();

		std::vector<std::wstring> m_oItems;
		std::vector<std::wstring> m_oTipStrings;
		std::vector<std::wstring> m_oNames;
		std::vector<std::wstring> m_oTags;
		std::vector<std::wstring> m_oAccelerators;

		nullable_uint m_oListIndex;
		nullable_uint m_oTabOrientation;
		nullable_uint m_oTabStyle;
		nullable_uint m_oTabData;
		nullable_uint m_oTabFixedWidth;
		nullable_uint m_oTabFixedHeight;

		std::vector<std::pair<bool, bool>> m_oTabStripTabFlags;
	};
//-------------------------------------------------------------------------------------------------
	class COcxPr : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(COcxPr)
		COcxPr() {}
		virtual ~COcxPr() {}
		virtual void fromXML(XmlUtils::CXmlNode& node)
		{
		}
        virtual std::wstring toXML() const
		{
			return L"";
		}
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<ax:ocxPr");
			if (m_oName.IsInit())
				writer.WriteString(L" ax:name=\"" + *m_oName + L"\"");
			if (m_oValue.IsInit())
				writer.WriteString(L" ax:value=\"" + *m_oValue + L"\"");
			writer.WriteString(L"/>");
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
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	m_oName = pReader->GetString2();
				else if (1 == _at)	m_oValue = pReader->GetString2();
			}
			pReader->Seek(end);
		}
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, m_oName);
				pWriter->WriteString2(1, m_oValue);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}

		nullable_string m_oName;
		nullable_string m_oValue;

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
			m_bDocument = (NULL != dynamic_cast<OOX::CDocument*>(pMain));
			read( oRootPath, filename );
		}
		virtual ~ActiveX_xml();
		virtual void ClearItems();
		virtual void read(const CPath& oPath)
		{
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		virtual void read(const CPath& oRootPath, const CPath& oPath);
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		void read_bin(const CPath& oPath);

		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, CContentTypes& content) const;

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
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

		bool									m_bDocument;
		CPath									m_oReadPath;
		
		nullable_string							m_oClassId;
		nullable_string							m_oLicense;
		nullable_string							m_oPersistence;	//(§3.6.2.1, ST_Persistence).
		nullable<SimpleTypes::CRelationshipId >	m_oId;
		std::vector<OOX::COcxPr*>				m_arrOcxPr;
//---------bin

		std::vector<BYTE>		m_oObjectBinData;
		nullable<ActiveXObject>	m_oObject;
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
			return type().DefaultFileName();
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
