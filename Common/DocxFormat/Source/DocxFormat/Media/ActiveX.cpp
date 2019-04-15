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
#include "ActiveX.h"
#include "../../XlsxFormat/Controls/Controls.h"

#include "../../../../../DesktopEditor/common/File.h"
#include "../../../../../ASCOfficeDocFile/DocDocxConverter/MemoryStream.h"

#define	GetB(nRGB) ((unsigned char )nRGB)
#define	GetG(nRGB) ((unsigned char )(nRGB>>8))
#define	GetR(nRGB) ((unsigned char )(nRGB>>16))

namespace OOX
{
	void ActiveX_xml::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)

		WritingElement_ReadAttributes_Read_if		(oReader, L"ax:classid"		, m_oClassId)
		WritingElement_ReadAttributes_Read_else_if	(oReader, L"ax:persistence"	, m_oPersistence)
		WritingElement_ReadAttributes_Read_else_if	(oReader, L"r:id"			, m_oId)
		WritingElement_ReadAttributes_Read_else_if	(oReader, L"ax:license"		, m_oLicense)

		WritingElement_ReadAttributes_End(oReader)
	}
	void ActiveX_xml::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oPath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
		if ( L"ocx" == sName )
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
			{
				int nDocumentDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nDocumentDepth ) )
				{
					sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"ocxPr" == sName )
					{
						COcxPr* pOcxPr = new COcxPr(oReader);
						m_arrOcxPr.push_back(pOcxPr);
					}
				}
			}
		}
		if (m_oId.IsInit())
		{
			smart_ptr<OOX::File> pFileControlBin;
			pFileControlBin = this->Find(OOX::RId(m_oId->GetValue()));
	
			smart_ptr<OOX::ActiveX_bin> pActiveX_bin = pFileControlBin.smart_dynamic_cast<OOX::ActiveX_bin>();

			if (pActiveX_bin.IsInit())
			{
				read_bin(pActiveX_bin->filename());
			}
		}
	}
	void ActiveX_xml::read_bin(const CPath& oPath)
	{
		NSFile::CFileBinary file;

		if (false == m_oClassId.IsInit()) return;
		if (false == file.OpenFile(oPath.GetPath())) return;

		DWORD size_stream = file.GetFileSize();

		unsigned char* data_stream = new unsigned char[size_stream];
		if (data_stream)
		{
			file.ReadFile(data_stream, size_stream, size_stream);

			m_oObject = ActiveXObject::Create(*m_oClassId);
			
			if (m_oObject.IsInit())
			{
				m_oObject->Parse(data_stream, size_stream);
			}

			RELEASEARRAYOBJECTS(data_stream);
		}
		file.CloseFile();
	}
	ActiveXObject* ActiveXObject::Create(const std::wstring &class_id)
	{
			 if (class_id == L"{DFD181E0-5E2F-11CE-A449-00AA004A803D}") return new ActiveXObjectScroll();
		else if (class_id == L"{D7053240-CE69-11CD-A777-00DD01143C57}")	return new ActiveXObjectButton();
		else if (class_id == L"{8BD21D40-EC42-11CE-9E0D-00AA006002F3}")	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectCheckBox);
		else if (class_id == L"{8BD21D30-EC42-11CE-9E0D-00AA006002F3}")	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectDrop);
		else if (class_id == L"{C62A69F0-16DC-11CE-9E98-00AA00574A4F}")	return new ActiveXObjectFormControl();
	//	else if (class_id == L"{6E182020-F460-11CE-9BCD-00AA00608E01}")	return new ActiveXObjectGBox();
		else if (class_id == L"{8BD21D50-EC42-11CE-9E0D-00AA006002F3}")	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectRadio);
		else if (class_id == L"{978C9E23-D4B0-11CE-BF2D-00AA003F40D0}")	return new ActiveXObjectLabel();
		else if (class_id == L"{8BD21D20-EC42-11CE-9E0D-00AA006002F3}")	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectList);
		else if (class_id == L"{DFD181E0-5E2F-11CE-A449-00AA004A803D}")	return new ActiveXObjectScroll();
		else if (class_id == L"{79176FB0-B7F2-11CE-97EF-00AA006D2776}")	return new ActiveXObjectSpin();
		else if (class_id == L"{8BD21D10-EC42-11CE-9E0D-00AA006002F3}")	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectEditBox);
	//	else if (class_id == L"{EAE50EB0-4A62-11CE-BED6-00AA00611080}") return new ActiveXObjectTabStrip();
		else if (class_id == L"{4C599241-6926-101B-9992-00000B65C6F9}") return new ActiveXObjectImage();
		else if (class_id == L"{8BD21D60-EC42-11CE-9E0D-00AA006002F3}")	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectButton); //Toggle
		else
			return NULL;
	}
	std::wstring ActiveXObject::ReadString(MemoryStream *stream, size_t size, bool bCompressed)
	{
		if (!stream) return L"";
		
		std::wstring result;

		unsigned char* pData = stream->ReadBytes(size, true);
		if (pData)
		{
			if (bCompressed)
				result = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(pData, size);
			else
				result = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pData, size / 2);
			delete []pData;
		}
		int count_padding = 4 - (size % 4);
		if (count_padding > 0 && count_padding < 4)
			stream->Seek(stream->GetPosition() + count_padding);

		return result;
	}
	void ActiveXObject::toFormControlPr(OOX::Spreadsheet::CFormControlPr* pFormControlPr)
	{
		if (!pFormControlPr) return;

		pFormControlPr->m_oObjectType = m_oObjectType;

		if (m_oSmallChange.IsInit())
			pFormControlPr->m_oInc = *m_oSmallChange > 0 ? *m_oSmallChange : 0;
		if (m_oMin.IsInit())
			pFormControlPr->m_oMin = *m_oMin > 0 ? *m_oMin : 0;
		if (m_oMax.IsInit())
			pFormControlPr->m_oMax = *m_oMax > 0 ? *m_oMax : 0;
		if (m_oLargeChange.IsInit())
			pFormControlPr->m_oPage = *m_oLargeChange > 0 ? *m_oLargeChange : 0;
		if (m_oPosition.IsInit())
			pFormControlPr->m_oVal = *m_oPosition > 0 ? *m_oPosition : 0;
		if (m_oOrientation.IsInit())
		{
			if (*m_oOrientation == 1) pFormControlPr->m_oHoriz = true;
			if (*m_oOrientation == 0) pFormControlPr->m_oVerticalBar = true;
		}		
		pFormControlPr->m_oPasswordEdit = m_oPasswordEdit;
		pFormControlPr->m_oSelType = m_oSelType;
		pFormControlPr->m_oLockText = m_oLockText;

		if (m_oCaption.IsInit())
		{
			pFormControlPr->m_oText = m_oCaption;
		}
		if (m_oValue.IsInit())
		{
			pFormControlPr->m_oVal = XmlUtils::GetInteger(*m_oValue);
		}
		if (m_oBackColor.IsInit())
		{
			pFormControlPr->m_oFillColor.Init();
			pFormControlPr->m_oFillColor->SetRGB(GetR(*m_oBackColor), GetG(*m_oBackColor), GetB(*m_oBackColor));
		}
		if (m_oBorderColor.IsInit())
		{
			pFormControlPr->m_oBorderColor.Init();
			pFormControlPr->m_oBorderColor->SetRGB(GetR(*m_oBorderColor), GetG(*m_oBorderColor), GetB(*m_oBorderColor));
		}
	}

	void ActiveXObjectScroll::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);
		mem_stream.Seek(16);

		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cbScrollBar = mem_stream.ReadUInt16();

		_UINT32 PropMask = mem_stream.ReadUInt32();

		bool fForeColor				= GETBIT(PropMask, 0);
		bool fBackColor				= GETBIT(PropMask, 1);
		bool fVariousPropertyBits	= GETBIT(PropMask, 2);
		bool fSize					= GETBIT(PropMask, 3);
		bool fMousePointer			= GETBIT(PropMask, 4);
		bool fMin					= GETBIT(PropMask, 5);
		bool fMax					= GETBIT(PropMask, 6);
		bool fPosition				= GETBIT(PropMask, 7);
		bool fPrevEnabled			= GETBIT(PropMask, 9);
		bool fNextEnabled			= GETBIT(PropMask, 10);
		bool fSmallChange			= GETBIT(PropMask, 11);
		bool fLargeChange			= GETBIT(PropMask, 12);
		bool fOrientation			= GETBIT(PropMask, 13);
		bool fProportionalThumb		= GETBIT(PropMask, 14);
		bool fDelay					= GETBIT(PropMask, 15);
		bool fMouseIcon				= GETBIT(PropMask, 16);

		if (fForeColor)				m_oForeColor = mem_stream.ReadUInt32();
		if (fBackColor)				m_oBackColor = mem_stream.ReadUInt32();
		if (fVariousPropertyBits)	/*VariousPropertyBits =*/ mem_stream.ReadUInt32(); 
		if (fMousePointer)
		{
			/*MousePointer =*/ mem_stream.ReadByte();
			/*Padding1 =*/ mem_stream.ReadByte();
			/*Padding2 =*/ mem_stream.ReadUInt16();
		}
		m_oMin = fMin ? mem_stream.ReadInt32() : 0;
		m_oMax = fMax ? mem_stream.ReadInt32() : 0x00007FFF;

 		if (fPosition)			m_oPosition = mem_stream.ReadInt32();
 		if (fPrevEnabled)		/*PrevEnabled =*/ mem_stream.ReadUInt32() != 0;
 		if (fNextEnabled)		/*NextEnabled =*/ mem_stream.ReadUInt32() != 0;
 		if (fSmallChange)		m_oSmallChange = mem_stream.ReadInt32();
 		if (fLargeChange)		m_oLargeChange = mem_stream.ReadInt32();
 		
		m_oOrientation = fOrientation ? mem_stream.ReadUInt32() : 0x00000001; // horiz

		if (fProportionalThumb)	/*ProportionalThumb =*/ mem_stream.ReadUInt32();
		if (fDelay)				m_oDelay = mem_stream.ReadUInt32();
		if (fMouseIcon)	
		{
			/*m_oMouseIcon =*/ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
		if (fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		int stream_size = mem_stream.GetSize() - mem_stream.GetPosition(); 
		//MouseIcon
	}
	void ActiveXObjectSpin::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);
		mem_stream.Seek(16);

		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cbSpin = mem_stream.ReadUInt16();

		_UINT32 PropMask = mem_stream.ReadUInt32();

		bool fForeColor				= GETBIT(PropMask, 0);
		bool fBackColor				= GETBIT(PropMask, 1);
		bool fVariousPropertyBits	= GETBIT(PropMask, 2);
		bool fSize					= GETBIT(PropMask, 3);
		bool fMin					= GETBIT(PropMask, 5);
		bool fMax					= GETBIT(PropMask, 6);
		bool fPosition				= GETBIT(PropMask, 7);
		bool fPrevEnabled			= GETBIT(PropMask, 8);
		bool fNextEnabled			= GETBIT(PropMask, 9);
		bool fSmallChange			= GETBIT(PropMask, 10);
		bool fOrientation			= GETBIT(PropMask, 11);
		bool fDelay					= GETBIT(PropMask, 12);
		bool fMouseIcon				= GETBIT(PropMask, 13);
		bool fMousePointer			= GETBIT(PropMask, 14);

		if (fForeColor)				m_oForeColor = mem_stream.ReadUInt32();
		if (fBackColor)				m_oBackColor = mem_stream.ReadUInt32();
		if (fVariousPropertyBits)	/*VariousPropertyBits =*/ mem_stream.ReadUInt32(); 
 		
		m_oMin = fMin ? mem_stream.ReadInt32() : 0;
		m_oMax = fMax ? mem_stream.ReadInt32() : 100;
 		
		if (fPosition)				m_oPosition = mem_stream.ReadInt32();
 		if (fPrevEnabled)			/*PrevEnabled =*/ mem_stream.ReadUInt32() != 0;
 		if (fNextEnabled)			/*NextEnabled =*/ mem_stream.ReadUInt32() != 0;
 		if (fSmallChange)			m_oSmallChange = mem_stream.ReadInt32();
 		if (fOrientation)			m_oOrientation = mem_stream.ReadUInt32();
		if (fDelay)					m_oDelay = mem_stream.ReadUInt32();
		if (fMouseIcon)	
		{
			/*m_oMouseIcon =*/ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
		if (fMousePointer)
		{
			/*MousePointer =*/ mem_stream.ReadByte();
			/*Padding1 =*/ mem_stream.ReadByte();
			/*Padding2 =*/ mem_stream.ReadUInt16();
		}
		if (fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		int stream_size = mem_stream.GetSize() - mem_stream.GetPosition(); 
		//MouseIcon
	}
	void ActiveXObjectButton::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);
		mem_stream.Seek(16);

		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cbButton = mem_stream.ReadUInt16();

		_UINT32 PropMask = mem_stream.ReadUInt32();

		bool fForeColor				= GETBIT(PropMask, 0);
		bool fBackColor				= GETBIT(PropMask, 1);
		bool fVariousPropertyBits	= GETBIT(PropMask, 2);
		bool fCaption				= GETBIT(PropMask, 3);
		bool fPicturePosition		= GETBIT(PropMask, 4);
		bool fSize					= GETBIT(PropMask, 5);
		bool fMousePointer			= GETBIT(PropMask, 6);
		bool fPicture				= GETBIT(PropMask, 7);
		bool fAccelerator			= GETBIT(PropMask, 8);
		bool fTakeFocusOnClick		= GETBIT(PropMask, 9);
		bool fMouseIcon				= GETBIT(PropMask, 10);

		m_oLockText = true;

		bool bCaptionCompressed = false;
		int sizeCaption = 0;
		int size_picture = 0, pos_picture = 0;

		if (fForeColor)				m_oForeColor = mem_stream.ReadUInt32();
		if (fBackColor)				m_oBackColor = mem_stream.ReadUInt32();
		if (fVariousPropertyBits)	/*VariousPropertyBits =*/ mem_stream.ReadUInt32(); 
		if (fCaption)
		{
			sizeCaption = mem_stream.ReadUInt32();
			
			bCaptionCompressed = GETBIT(sizeCaption, 31);
			sizeCaption = GETBITS(sizeCaption, 0, 30);
		}
		if (fPicturePosition)		pos_picture = mem_stream.ReadUInt32();
		if (fMousePointer)
		{
			/*MousePointer =*/ mem_stream.ReadByte();
			/*Padding1 =*/ mem_stream.ReadByte();
			/*Padding2 =*/ mem_stream.ReadUInt16();
		}
 		if (fPicture)
		{
			/*m_oPicture = */ mem_stream.ReadUInt16();
			/*Padding4 =*/ mem_stream.ReadUInt16();
		}
 		if (fAccelerator)
		{
			/*m_oAccelerator = */mem_stream.ReadUInt16();
			/*Padding4 =*/ mem_stream.ReadUInt16();
		}
		if (fMouseIcon)	
		{
			/*m_oMouseIcon =*/ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
		if (sizeCaption > 0)
		{
			m_oCaption = ReadString(&mem_stream, sizeCaption, bCaptionCompressed);
		}
		if (fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		int stream_size = mem_stream.GetSize() - mem_stream.GetPosition(); 
		//MouseIcon
	}
	void ActiveXObjectImage::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);
		mem_stream.Seek(16);

		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cbImage = mem_stream.ReadUInt16();

		_UINT32 PropMask = mem_stream.ReadUInt32();

		bool fAutoSize				= GETBIT(PropMask, 2);
		bool fBorderColor			= GETBIT(PropMask, 3);
		bool fBackColor				= GETBIT(PropMask, 4);
		bool fBorderStyle			= GETBIT(PropMask, 5);
		bool fMousePointer			= GETBIT(PropMask, 6);
		bool fPictureSizeMode		= GETBIT(PropMask, 7);
		bool fSpecialEffect			= GETBIT(PropMask, 8);
		bool fSize					= GETBIT(PropMask, 9);
		bool fPicture				= GETBIT(PropMask, 10);
		bool fPictureAlignment		= GETBIT(PropMask, 11);
		bTile						= GETBIT(PropMask, 12);
		bool fVariousPropertyBits	= GETBIT(PropMask, 13);
		bool fMouseIcon				= GETBIT(PropMask, 14);

		if (fBorderColor)			m_oBorderColor = mem_stream.ReadUInt32();
		if (fBackColor)				m_oBackColor = mem_stream.ReadUInt32();
 		size_t padding = 4;
		if (fBorderStyle)
		{
			m_oBorderStyle = mem_stream.ReadByte(); padding--;
		}
		if (fMousePointer)
		{
			/*m_oMousePointer = */mem_stream.ReadByte(); padding--;
		}
		if (fPictureSizeMode)
		{
			/*oPictureSizeMode = */mem_stream.ReadByte(); padding--;
		}
		if (fSpecialEffect)
		{
			/*oSpecialEffect = */mem_stream.ReadByte(); padding--;
		}
		if (padding > 0 && padding < 4)
			mem_stream.Seek(mem_stream.GetPosition() + padding);

		padding = 4;
		if (fPicture)
		{
			/*m_oPicture = */mem_stream.ReadUInt16(); padding-=2;
		}
		if (fPictureAlignment)
		{
			/*m_oMousePointer = */mem_stream.ReadByte(); padding--;
		}
		if (padding > 0 && padding < 4)
			mem_stream.Seek(mem_stream.GetPosition() + padding);
		
		if (fVariousPropertyBits)	/*VariousPropertyBits =*/ mem_stream.ReadUInt32(); 
		if (fMouseIcon)	
		{
			/*m_oMouseIcon =*/ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
		if (fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		if (fPicture)
		{
			unsigned char* pData = mem_stream.ReadBytes(16, true);
			std::string sGuid = std::string((char*)pData, 16);
			delete []pData;

			int Preamble = mem_stream.ReadUInt32(); //MUST 0x0000746C

			nImageSize = mem_stream.ReadUInt32();
			pImageData = boost::shared_array<unsigned char>(mem_stream.ReadBytes(nImageSize, true));
		}
	}	
	void ActiveXObjectLabel::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);
		mem_stream.Seek(16);

		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cbLabel = mem_stream.ReadUInt16();

		_UINT32 PropMask = mem_stream.ReadUInt32();

		bool fForeColor				= GETBIT(PropMask, 0);
		bool fBackColor				= GETBIT(PropMask, 1);
		bool fVariousPropertyBits	= GETBIT(PropMask, 2);
		bool fCaption				= GETBIT(PropMask, 3);
		bool fPicturePosition		= GETBIT(PropMask, 4);
		bool fSize					= GETBIT(PropMask, 5);
		bool fMousePointer			= GETBIT(PropMask, 6);
		bool fBorderColor			= GETBIT(PropMask, 7);
		bool fBorderStyle			= GETBIT(PropMask, 8);
		bool fSpecialEffect			= GETBIT(PropMask, 9);
		bool fPicture				= GETBIT(PropMask, 10);
		bool fAccelerator			= GETBIT(PropMask, 11);
		bool fMouseIcon				= GETBIT(PropMask, 12);

		m_oLockText = true;
		bool bCaptionCompressed = false;
		int sizeCaption = 0;
		int size_picture = 0, pos_picture = 0;

		if (fForeColor)				m_oForeColor = mem_stream.ReadUInt32();
		if (fBackColor)				m_oBackColor = mem_stream.ReadUInt32();
		if (fVariousPropertyBits)	/*VariousPropertyBits =*/ mem_stream.ReadUInt32(); 
		if (fCaption)
		{
			sizeCaption = mem_stream.ReadUInt32();
			
			bCaptionCompressed = GETBIT(sizeCaption, 31);
			sizeCaption = GETBITS(sizeCaption, 0, 30);
		}
		if (fPicturePosition)		pos_picture = mem_stream.ReadUInt32();
		if (fMousePointer)
		{
			/*MousePointer =*/ mem_stream.ReadByte();
			/*Padding1 =*/ mem_stream.ReadByte();
			/*Padding2 =*/ mem_stream.ReadUInt16();
		}
 		if (fBorderColor)		m_oBorderColor = mem_stream.ReadUInt32();
 		if (fBorderStyle)
		{
			m_oBorderStyle = mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
 		if (fSpecialEffect)
		{
			/*m_oEffectStyle = */ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
 		if (fPicture)
		{
			/*m_oPicture = */ mem_stream.ReadUInt16();
			/*Padding4 =*/ mem_stream.ReadUInt16();
		}
 		if (fAccelerator)
		{
			/*m_oAccelerator = */mem_stream.ReadUInt16();
			/*Padding4 =*/ mem_stream.ReadUInt16();
		}
		if (fMouseIcon)	
		{
			/*m_oMouseIcon =*/ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
		if (sizeCaption > 0)
		{
			m_oCaption = ReadString(&mem_stream, sizeCaption, bCaptionCompressed);
		}
		if (fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		int stream_size = mem_stream.GetSize() - mem_stream.GetPosition(); 
		//MouseIcon
	}
	void ActiveXObjectFormControl::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);
		mem_stream.Seek(16);

		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cb = mem_stream.ReadUInt16();

		_UINT32 PropMask = mem_stream.ReadUInt32();
	}
	void ActiveXObjectMorphData::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);
		mem_stream.Seek(16);

		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cb = mem_stream.ReadUInt16();

		_UINT32 PropMask = mem_stream.ReadUInt32();
		_UINT32 PropMask2 = mem_stream.ReadUInt32();

		bool fVariousPropertyBits	= GETBIT(PropMask, 0);
		bool fForeColor				= GETBIT(PropMask, 1);
		bool fBackColor				= GETBIT(PropMask, 2);
		bool fMaxLength				= GETBIT(PropMask, 3);
		bool fBorderStyle			= GETBIT(PropMask, 4);
		bool fScrollBars			= GETBIT(PropMask, 5);
		bool fDisplayStyle			= GETBIT(PropMask, 6);
		bool fMousePointer			= GETBIT(PropMask, 7);
		bool fSize					= GETBIT(PropMask, 8);
		bool fPasswordChar			= GETBIT(PropMask, 9);
		bool fListWidth				= GETBIT(PropMask, 10);
		bool fBoundColumn			= GETBIT(PropMask, 11);
		bool fTextColumn			= GETBIT(PropMask, 12);
		bool fColumnCount			= GETBIT(PropMask, 13);
		bool fListRows				= GETBIT(PropMask, 14);
		bool fcColumnInfo			= GETBIT(PropMask, 15);
		bool fMatchEntry			= GETBIT(PropMask, 16);
		bool fListStyle				= GETBIT(PropMask, 17);
		bool fShowDropButtonWhen	= GETBIT(PropMask, 18);
		bool fDropButtonStyle		= GETBIT(PropMask, 20);
		bool fMultiSelect			= GETBIT(PropMask, 21);
		bool fValue					= GETBIT(PropMask, 22);
		bool fCaption				= GETBIT(PropMask, 23);
		bool fPicturePosition		= GETBIT(PropMask, 24);
		bool fBorderColor			= GETBIT(PropMask, 25);
		bool fSpecialEffect			= GETBIT(PropMask, 26);
		bool fMouseIcon				= GETBIT(PropMask, 27);
		bool fPicture				= GETBIT(PropMask, 28);
		bool fAccelerator			= GETBIT(PropMask, 29);
		bool fGroupName				= GETBIT(PropMask2, 0);

		int MaxLength = 0;
		if (fVariousPropertyBits)	/*VariousPropertyBits =*/ mem_stream.ReadUInt32(); 
		if (fForeColor)				m_oForeColor = mem_stream.ReadUInt32();
		if (fBackColor)				m_oBackColor = mem_stream.ReadUInt32();
		if (fMaxLength)				MaxLength = mem_stream.ReadUInt32();
		
		int count_padding = 4;
		if (fBorderStyle)
		{
 			m_oBorderStyle = mem_stream.ReadByte(); count_padding--;
		}
		if (fScrollBars)
		{
 			/*m_oScrollBars =*/ mem_stream.ReadByte(); count_padding--;
		}
		if (fDisplayStyle)
		{
 			/*m_oDisplayStyle =*/ mem_stream.ReadByte(); count_padding--;
		}
		if (fMousePointer)
		{
 			/*m_oMousePointer =*/ mem_stream.ReadByte(); count_padding--;
		}
		if (count_padding > 0 && count_padding < 4)
			mem_stream.Seek(mem_stream.GetPosition() + count_padding);
		if (fPasswordChar)	
		{
			m_oPasswordEdit = true;
			/*PasswordChar =*/ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
		if (fListWidth)	/*m_oListWidth =*/ mem_stream.ReadUInt32();
		if (fBoundColumn)	
		{
			/*BoundColumn =*/ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}		
		if (fTextColumn)	
		{
			/*TextColumn =*/ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}	
		if (fColumnCount)	
		{
			/*ColumnCount =*/ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
		if (fListRows)	
		{
			/*ListRows =*/ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
		count_padding = 8;
		if (fcColumnInfo)	
		{
			/*cColumnInfo =*/ mem_stream.ReadUInt16(); count_padding-=2;
		}
		if (fMatchEntry)	
		{
			/*MatchEntry =*/ mem_stream.ReadByte(); count_padding--;
		}
		if (fListStyle)	
		{
			/*ListStyle =*/ mem_stream.ReadByte(); count_padding--;
		}
		if (fShowDropButtonWhen)	
		{
			/*ShowDropButtonWhen =*/ mem_stream.ReadByte(); count_padding--;
		}
		if (fDropButtonStyle)	
		{
			/*DropButtonStyle =*/ mem_stream.ReadByte(); count_padding--;
		}
		if (fMultiSelect)	
		{
			m_oSelType = (SimpleTypes::Spreadsheet::ESelType) mem_stream.ReadByte(); count_padding--;
		}
		if (count_padding > 0 && count_padding < 8)
			mem_stream.Seek(mem_stream.GetPosition() + count_padding);
		
		int sizeCaption = 0, sizeGroupName = 0, sizeValue = 0;
		bool bCaptionCompressed, bGroupNameCompressed, bValueCompressed;

		if (fValue)
		{
			sizeValue = mem_stream.ReadUInt32();
			
			bValueCompressed = GETBIT(sizeValue, 31);
			sizeValue = GETBITS(sizeValue, 0, 30);
		}
		if (fCaption)
		{
			sizeCaption = mem_stream.ReadUInt32();
			
			bCaptionCompressed = GETBIT(sizeCaption, 31);
			sizeCaption = GETBITS(sizeCaption, 0, 30);
		}
		if (fPicturePosition)	/*pos_picture =*/ mem_stream.ReadUInt32();
 		if (fBorderColor)		m_oBorderColor = mem_stream.ReadUInt32();
		if (fSpecialEffect)		/*m_oEffect =*/ mem_stream.ReadUInt32();
		if (fMouseIcon)	
		{
			/*m_oMouseIcon =*/ mem_stream.ReadUInt16();
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
 		if (fPicture)
		{
			/*m_oPicture = */ mem_stream.ReadUInt16();
			/*Padding4 =*/ mem_stream.ReadUInt16();
		}
 		if (fAccelerator)
		{
			/*m_oAccelerator = */mem_stream.ReadUInt16();
			/*Padding4 =*/ mem_stream.ReadUInt16();
		}
		if (fGroupName)
		{
			sizeGroupName = mem_stream.ReadUInt32();
			
			bGroupNameCompressed = GETBIT(sizeGroupName, 31);
			sizeGroupName = GETBITS(sizeGroupName, 0, 30);
		}
		if (fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		if (sizeValue > 0)
		{
			m_oValue = ReadString(&mem_stream, sizeValue, bValueCompressed);
		}
		if (sizeCaption > 0)
		{
			m_oCaption = ReadString(&mem_stream, sizeCaption, bCaptionCompressed);
		}
		std::wstring oGroupName;
		if (sizeGroupName > 0)
		{
			oGroupName = ReadString(&mem_stream, sizeGroupName, bGroupNameCompressed);
		}
		int stream_size = mem_stream.GetSize() - mem_stream.GetPosition(); 
		//MouseIcon
	}
} // namespace OOX
