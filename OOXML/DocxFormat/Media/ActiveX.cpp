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

#include "../../../DesktopEditor/common/File.h"
#include "../../../MsBinaryFile/DocFile/MemoryStream.h"
#include "../../../MsBinaryFile/Common/Vba/Records.h"

#define	GetB(nRGB) ((unsigned char )nRGB)
#define	GetG(nRGB) ((unsigned char )(nRGB>>8))
#define	GetR(nRGB) ((unsigned char )(nRGB>>16))

namespace OOX
{
	struct VariousPropertiesBitfield
	{
		VariousPropertiesBitfield(_UINT32 flag)
		{
			Enabled = GETBIT(flag, 1);
			Locked = GETBIT(flag, 2);
			BackStyle = GETBIT(flag, 3);
			ColumnHeads = GETBIT(flag, 10);
			IntegralHeight = GETBIT(flag, 11);
			MatchRequired = GETBIT(flag, 12);
			Alignment = GETBIT(flag, 13);
			Editable = GETBIT(flag, 14);
			unsigned char IMEMode = GETBITS(flag, 15, 18);
			DragBehavior = GETBIT(flag, 19);
			EnterKeyBehavior = GETBIT(flag, 20);
			EnterFieldBehavior = GETBIT(flag, 21);
			TabKeyBehavior = GETBIT(flag, 22);
			WordWrap = GETBIT(flag, 23);
			BordersSuppress = GETBIT(flag, 25);
			SelectionMargin = GETBIT(flag, 26);
			AutoWordSelect = GETBIT(flag, 27);
			AutoSize = GETBIT(flag, 28);
			HideSelection = GETBIT(flag, 29);
			AutoTab = GETBIT(flag, 30);
			MultiLine = GETBIT(flag, 31);
		}
		VariousPropertiesBitfield & operator = (_UINT32 flag)
		{
			*this = VariousPropertiesBitfield(flag);
		}
		bool Enabled;
		bool Locked;
		bool BackStyle;
		bool ColumnHeads;
		bool IntegralHeight;
		bool MatchRequired;
		bool Alignment;
		bool Editable;
		unsigned char IMEMode;
		bool DragBehavior;
		bool EnterKeyBehavior;
		bool EnterFieldBehavior;
		bool TabKeyBehavior;
		bool WordWrap;
		bool BordersSuppress;
		bool SelectionMargin;
		bool AutoWordSelect;
		bool AutoSize;
		bool HideSelection;
		bool AutoTab;
		bool MultiLine;
	};
	struct LabelPropMask
	{
		LabelPropMask(_UINT32 flag)
		{
			fForeColor = GETBIT(flag, 0);
			fBackColor = GETBIT(flag, 1);
			fVariousPropertyBits = GETBIT(flag, 2);
			fCaption = GETBIT(flag, 3);
			fPicturePosition = GETBIT(flag, 4);
			fSize = GETBIT(flag, 5);
			fMousePointer = GETBIT(flag, 6);
			fBorderColor = GETBIT(flag, 7);
			fBorderStyle = GETBIT(flag, 8);
			fSpecialEffect = GETBIT(flag, 9);
			fPicture = GETBIT(flag, 10);
			fAccelerator = GETBIT(flag, 11);
			fMouseIcon = GETBIT(flag, 12);
		}
		LabelPropMask & operator = (_UINT32 flag)
		{
			*this = LabelPropMask(flag);
		}
		bool fForeColor;
		bool fBackColor;
		bool fVariousPropertyBits;
		bool fCaption;
		bool fPicturePosition;
		bool fSize;
		bool fMousePointer;
		bool fBorderColor;
		bool fBorderStyle;
		bool fSpecialEffect;
		bool fPicture;
		bool fAccelerator;
		bool fMouseIcon;
	};
	struct ImagePropMask
	{
		ImagePropMask(_UINT32 flag)
		{
			fAutoSize = GETBIT(flag, 2);
			fBorderColor = GETBIT(flag, 3);
			fBackColor = GETBIT(flag, 4);
			fBorderStyle = GETBIT(flag, 5);
			fMousePointer = GETBIT(flag, 6);
			fPictureSizeMode = GETBIT(flag, 7);
			fSpecialEffect = GETBIT(flag, 8);
			fSize = GETBIT(flag, 9);
			fPicture = GETBIT(flag, 10);
			fPictureAlignment = GETBIT(flag, 11);
			fPictureTiling = GETBIT(flag, 12);
			fVariousPropertyBits = GETBIT(flag, 13);
			fMouseIcon = GETBIT(flag, 14);
		}
		ImagePropMask & operator = (_UINT32 flag)
		{
			*this = ImagePropMask(flag);
		}
		bool fAutoSize;
		bool fBorderColor;
		bool fBackColor;
		bool fBorderStyle;
		bool fMousePointer;
		bool fPictureSizeMode;
		bool fSpecialEffect;
		bool fSize;
		bool fPicture;
		bool fPictureAlignment;
		bool fPictureTiling;
		bool fVariousPropertyBits;
		bool fMouseIcon;
	};

	struct TabStripPropMask
	{
		TabStripPropMask(_UINT32 flag)
		{
			fListIndex = GETBIT(flag, 0);
			fBackColor = GETBIT(flag, 1);
			fForeColor = GETBIT(flag, 2);
			fSize = GETBIT(flag, 4);
			fItems = GETBIT(flag, 5);
			fMousePointer = GETBIT(flag, 6);
			fTabOrientation = GETBIT(flag, 8);
			fTabStyle = GETBIT(flag, 9);
			fMultiRow = GETBIT(flag, 10);
			fTabFixedWidth = GETBIT(flag, 11);
			fTabFixedHeight = GETBIT(flag, 12);
			fTooltips = GETBIT(flag, 13);
			fTipStrings = GETBIT(flag, 15);
			fNames = GETBIT(flag, 17);
			fVariousPropertyBits = GETBIT(flag, 18);
			fNewVersion = GETBIT(flag, 19);
			fTabsAllocated = GETBIT(flag, 20);
			fTags = GETBIT(flag, 21);
			fTabData = GETBIT(flag, 22);
			fAccelerator = GETBIT(flag, 23);
			fMouseIcon = GETBIT(flag, 24);
		}
		TabStripPropMask & operator = (_UINT32 flag)
		{
			*this = TabStripPropMask(flag);
		}
		bool fListIndex;
		bool fBackColor;
		bool fForeColor;
		bool fSize;
		bool fItems;
		bool fMousePointer;
		bool fTabOrientation;
		bool fTabStyle;
		bool fMultiRow;
		bool fTabFixedWidth;
		bool fTabFixedHeight;
		bool fTooltips;
		bool fTipStrings;
		bool fNames;
		bool fVariousPropertyBits;
		bool fNewVersion;
		bool fTabsAllocated;
		bool fTags;
		bool fTabData;
		bool fAccelerator;
		bool fMouseIcon;
	};
	struct MorphDataPropMask
	{
		MorphDataPropMask(_UINT64 mask)
		{
			fVariousPropertyBits = GETBIT(mask, 0);
			fForeColor = GETBIT(mask, 1);
			fBackColor = GETBIT(mask, 2);
			fMaxLength = GETBIT(mask, 3);
			fBorderStyle = GETBIT(mask, 4);
			fScrollBars = GETBIT(mask, 5);
			fDisplayStyle = GETBIT(mask, 6);
			fMousePointer = GETBIT(mask, 7);
			fSize = GETBIT(mask, 8);
			fPasswordChar = GETBIT(mask, 9);
			fListWidth = GETBIT(mask, 10);
			fBoundColumn = GETBIT(mask, 11);
			fTextColumn = GETBIT(mask, 12);
			fColumnCount = GETBIT(mask, 13);
			fListRows = GETBIT(mask, 14);
			fcColumnInfo = GETBIT(mask, 15);
			fMatchEntry = GETBIT(mask, 16);
			fListStyle = GETBIT(mask, 17);
			fShowDropButtonWhen = GETBIT(mask, 18);
			fDropButtonStyle = GETBIT(mask, 20);
			fMultiSelect = GETBIT(mask, 21);
			fValue = GETBIT(mask, 22);
			fCaption = GETBIT(mask, 23);
			fPicturePosition = GETBIT(mask, 24);
			fBorderColor = GETBIT(mask, 25);
			fSpecialEffect = GETBIT(mask, 26);
			fMouseIcon = GETBIT(mask, 27);
			fPicture = GETBIT(mask, 28);
			fAccelerator = GETBIT(mask, 29);
			fGroupName = GETBIT(mask, 32);
		}
		MorphDataPropMask & operator = (_UINT32 flag)
		{
			*this = MorphDataPropMask(flag);
		}
		bool fVariousPropertyBits;
		bool fForeColor;
		bool fBackColor;
		bool fMaxLength;
		bool fBorderStyle;
		bool fScrollBars;
		bool fDisplayStyle;
		bool fMousePointer;
		bool fSize;
		bool fPasswordChar;
		bool fListWidth;
		bool fBoundColumn;
		bool fTextColumn;
		bool fColumnCount;
		bool fListRows;
		bool fcColumnInfo;
		bool fMatchEntry;
		bool fListStyle;
		bool fShowDropButtonWhen;
		bool fDropButtonStyle;
		bool fMultiSelect;
		bool fValue;
		bool fCaption;
		bool fPicturePosition;
		bool fBorderColor;
		bool fSpecialEffect;
		bool fMouseIcon;
		bool fPicture;
		bool fAccelerator;
		bool fGroupName;
	};

	ActiveX_xml::~ActiveX_xml()
	{
		ClearItems();
	}
	void ActiveX_xml::ClearItems()
	{
		for (size_t nIndex = 0; nIndex < m_arrOcxPr.size(); ++nIndex)
		{
			if (m_arrOcxPr[nIndex]) delete m_arrOcxPr[nIndex];
			m_arrOcxPr[nIndex] = NULL;
		}
		m_arrOcxPr.clear();
	}
	void ActiveX_xml::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_oClassId);
		pWriter->WriteString2(1, m_oLicense);
		pWriter->WriteString2(2, m_oPersistence);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrOcxPr.size(); ++i)
		{
			pWriter->WriteRecord2(4, dynamic_cast<OOX::WritingElement*>(m_arrOcxPr[i]));
		}

		if (false == m_oObjectBinData.empty())
		{
			pWriter->StartRecord(5);
			pWriter->WriteBYTEArray(m_oObjectBinData.data(), m_oObjectBinData.size());
			pWriter->EndRecord();
		}
	}
	void ActiveX_xml::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_oClassId = pReader->GetString2();
			else if (1 == _at)	m_oLicense = pReader->GetString2();
			else if (2 == _at)	m_oPersistence = pReader->GetString2();
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 4:
			{
				m_arrOcxPr.push_back(new COcxPr());
				m_arrOcxPr.back()->fromPPTY(pReader);
			}break;
			case 5:
			{
				size_t size = pReader->GetRecordSize();
				m_oObjectBinData.resize(size);

				pReader->GetArray(m_oObjectBinData.data(), size);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
//-----------------------------------------------------
		if (false == m_oObjectBinData.empty())
		{
			smart_ptr<ActiveX_bin> activeX_bin(new ActiveX_bin(File::m_pMainDocument));

			activeX_bin->m_Data = m_oObjectBinData;

			smart_ptr<OOX::File> file = activeX_bin.smart_dynamic_cast<OOX::File>();
			OOX::RId rId = Add(file);
			m_oId = rId.get();
		}
	}
	void ActiveX_xml::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if		(oReader, L"ax:classid"		, m_oClassId)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"ax:persistence"	, m_oPersistence)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"r:id"			, m_oId)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"relationships:id", m_oId)
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

		m_oObjectBinData.resize(size_stream);

		if (false == m_oObjectBinData.empty())
		{
			file.ReadFile(m_oObjectBinData.data(), size_stream, size_stream);

			m_oObject = ActiveXObject::Create(*m_oClassId);
			
			if (m_oObject.IsInit())
			{
				m_oObject->Parse(m_oObjectBinData.data() + 2, size_stream - 2);
			}
		}
		file.CloseFile();
	}
	void ActiveX_xml::write(const OOX::CPath& oPath, const OOX::CPath& oDirectory, CContentTypes& oContent) const
	{
		NSStringUtils::CStringBuilder sXml;
		sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<ax:ocx \
xmlns:ax=\"http://schemas.microsoft.com/office/2006/activeX\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"");

		if (m_oClassId.IsInit())
			sXml.WriteString(L" ax:classid=\"" + *m_oClassId + L"\"");
		if (m_oPersistence.IsInit())
			sXml.WriteString(L" ax:persistence=\"" + *m_oPersistence + L"\"");
		if (m_oId.IsInit())
			sXml.WriteString(L" r:id=\"" + m_oId->ToString() + L"\"");
		if (m_oLicense.IsInit())
			sXml.WriteString(L" ax:license=\"" + *m_oLicense + L"\"");

		sXml.WriteString(L">");
		for (size_t i = 0; i < m_arrOcxPr.size(); ++i)
		{
			m_arrOcxPr[i]->toXML(sXml);
		}
		sXml.WriteString(L"</ax:ocx>");
		
		std::wstring sPath = oPath.GetPath();
		NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

		oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
		IFileContainer::Write(oPath, oDirectory, oContent);
	}
	//---------------------------------------------------------------------------------------------------------
	ActiveXObject* ActiveXObject::Create(_UINT16 type)
	{
		switch (type)

		{
		case 7:		return new ActiveXObjectFormControl();
		case 12:	return new ActiveXObjectImage();
		case 14:	return new ActiveXObjectFrame();
		case 16:	return new ActiveXObjectSpin();
		case 17:	return new ActiveXObjectButton();
		case 18:	return new ActiveXObjectTabStrip();
		case 21:	return new ActiveXObjectLabel();
		case 23:	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectEditBox);
		case 24:	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectList);
		case 25:	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectDrop);
		case 26:	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectCheckBox);
		case 27:	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectRadio);
		case 28:	return new ActiveXObjectMorphData(SimpleTypes::Spreadsheet::objectToggleButton);
		case 47:	return new ActiveXObjectScroll();
		//case 57:	MultiPage
		case 15:
		default:
			return new ActiveXObjectMorphData();
		}
	}
//---------------------------------------------------------------------------------------------------------
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
			return new ActiveXObjectMorphData(); //from DisplayStyle
	}
	void ActiveXObject::readArrayString(MemoryStream *stream, std::vector<std::wstring> &Array, size_t size_total)
	{
		size_t pos_start = stream->GetPosition();
		while(true)
		{
			if (stream->GetPosition() + 4 > pos_start + size_total)
				break;

			_UINT32 CountAndCompression = stream->ReadUInt32();
			Array.push_back(readString(stream, CountAndCompression));
		}
	}
	std::wstring ActiveXObject::readString(MemoryStream *stream, size_t CountOfCharsWithCompressionFlag)
	{
		if (!stream) return L"";
		
		bool fCompressed = GETBIT(CountOfCharsWithCompressionFlag, 31);
		size_t size = GETBITS(CountOfCharsWithCompressionFlag, 0, 30);

		if (stream->GetPosition() + size > stream->GetSize())
		{
			size = stream->GetSize() - stream->GetPosition();
		}
		
		std::wstring result;

		unsigned char* pData = stream->ReadBytes(size, true);
		if (pData)
		{
			if (fCompressed)
				result = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(pData, size);
			else
				result = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pData, size / 2);
			delete []pData;
		}
		stream->Align(4);

		return result;
	}
	void ActiveXObject::readStdFont(MemoryStream *stream)
	{
		if (!stream) return;	
		
		unsigned char Version = stream->ReadByte();
		short sCharset = stream->ReadInt16();
		unsigned char bFlags = stream->ReadByte();
		short sWeight = stream->ReadInt16();
		m_oFontHeight = stream->ReadUInt32();		
		unsigned char bFaceLen = stream->ReadByte();

		m_oFontBold = GETBIT(bFlags, 0);
		m_oFontItalic = GETBIT(bFlags, 1);
		m_oFontUnderline = GETBIT(bFlags, 2);
		m_oFontStrikeout = GETBIT(bFlags, 3);

		if (bFaceLen > 0)
		{
			unsigned char *buf = stream->ReadBytes(bFaceLen, true);
			
			m_oFontName = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(buf, bFaceLen);
			delete[]buf;
		}
	}
	void ActiveXObject::readTextProps(MemoryStream *stream)
	{
		if (!stream) return;

		_UINT32 sizeFontName = 0, nFontWeight = 0;
		bool bCaptionCompressed, bGroupNameCompressed, bValueCompressed, bFontNameCompressed;

		int nFontCharSet = 0, nFontPitch = 0, nFontFamily = 0, nParagraphAlign = 0;

		unsigned char MinorVersionText = stream->ReadByte();
		unsigned char MajorVersionText = stream->ReadByte();

		unsigned short cbTextProps = stream->ReadUInt16();

		VBA::TextPropsPropMask PropMask(stream->ReadUInt32());

		if (PropMask.fFontName) sizeFontName = stream->ReadUInt32();
		if (PropMask.fFontEffects)
		{
			_UINT32 nFontEffects = stream->ReadUInt32();
			bool bEnabled = !GETBIT(nFontEffects, 13);
			m_oFontBold = GETBIT(nFontEffects, 0);
			m_oFontItalic = GETBIT(nFontEffects, 1);
			m_oFontUnderline = GETBIT(nFontEffects, 3);
			m_oFontStrikeout = GETBIT(nFontEffects, 4);
			m_oFontAutoColor = GETBIT(nFontEffects, 30);
		}
		if (PropMask.fFontHeight) m_oFontHeight = stream->ReadUInt32() / 20; //twips to pt

		if (PropMask.fFontCharSet) nFontCharSet = stream->ReadByte();
		if (PropMask.fFontPitchAndFamily)
		{
			_UINT32 nFontPitchAndFamily = stream->ReadByte();
			nFontPitch = GETBITS(nFontPitchAndFamily, 0, 3);
			nFontFamily = GETBITS(nFontPitchAndFamily, 4, 7);
		}
		if (PropMask.fParagraphAlign) nParagraphAlign = stream->ReadByte();

		size_t size_test = GETBITS(sizeFontName, 0, 30);
		if (PropMask.fFontWeight && (stream->GetPosition() + size_test + 4 <= stream->GetSize()) )
		{
			nFontWeight = stream->ReadUInt16();
		}
		if (sizeFontName > 0)
		{
			stream->Align(4);
			m_oFontName = readString(stream, sizeFontName);
		}
	}
	_UINT32 ActiveXObject::readColumnInfo(MemoryStream *stream)
	{
		if (!stream) return 0;

		unsigned char MinorVersionText = stream->ReadByte();
		unsigned char MajorVersionText = stream->ReadByte();

		unsigned short cbColumnInfo = stream->ReadUInt16();

		_UINT32 flag = stream->ReadUInt32();

		bool fColumnWidth = GETBIT(flag, 0);

		if (fColumnWidth)
			return stream->ReadUInt32();
		return 0;
	}

	_GUID_ ActiveXObject::readGUID(MemoryStream *stream)
	{
		_GUID_ result;

		if (stream)
		{
			result.Data1 = stream->ReadUInt32();
			result.Data2 = stream->ReadUInt16();
			result.Data3 = stream->ReadUInt16();

			unsigned char *buf = stream->ReadBytes(8, true);
			if (buf)
			{
				memcpy(result.getData4(), buf, 8);
				delete[]buf;
			}
		}
		return result;
	}
	std::pair<boost::shared_array<unsigned char>, size_t> ActiveXObject::readStdPicture(MemoryStream *stream)
	{
		boost::shared_array<unsigned char> empty;
		if (!stream) return std::make_pair(empty, 0);

		_UINT32 Preamble = stream->ReadUInt32();

		if (Preamble != 0x0000746C) return std::make_pair(empty, 0);

		size_t Size = stream->ReadUInt32();

		boost::shared_array<unsigned char> buf(stream->ReadBytes(Size, true));

		return std::make_pair(buf, Size);
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
	std::wstring ActiveXObject::toXml()
	{
		std::wstringstream strm;
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"Common")
			{
				CP_XML_ATTR_NULLABLE2(L"ObjectType", m_oObjectType);
				CP_XML_ATTR_NULLABLE(L"Caption", m_oCaption);
				CP_XML_ATTR_NULLABLE(L"Width", m_oWidth);
				CP_XML_ATTR_NULLABLE(L"Height", m_oHeight);
				CP_XML_ATTR_NULLABLE(L"ForeColor", m_oForeColor);
				CP_XML_ATTR_NULLABLE(L"BackColor", m_oBackColor);
				CP_XML_ATTR_NULLABLE(L"BorderColor", m_oBorderColor);
				CP_XML_ATTR_NULLABLE(L"BorderStyle", m_oBorderStyle);
				CP_XML_ATTR_NULLABLE(L"Min", m_oMin);
				CP_XML_ATTR_NULLABLE(L"Max", m_oMax);
				CP_XML_ATTR_NULLABLE(L"Position", m_oPosition);
				CP_XML_ATTR_NULLABLE(L"SmallChange", m_oSmallChange);
				CP_XML_ATTR_NULLABLE(L"LargeChange", m_oLargeChange);
				CP_XML_ATTR_NULLABLE(L"Orientation", m_oOrientation);
				CP_XML_ATTR_NULLABLE(L"Delay", m_oDelay);
				CP_XML_ATTR_NULLABLE(L"Value", m_oValue);
				CP_XML_ATTR_NULLABLE(L"ListRows", m_oListRows);
				CP_XML_ATTR_NULLABLE(L"ScrollBarsType", m_oScrollBarsType);
				CP_XML_ATTR_NULLABLE(L"LockText", m_oLockText);
				CP_XML_ATTR_NULLABLE(L"MultiLine", m_oMultiLine);
			}
			CP_XML_NODE(L"Font")
			{
				CP_XML_ATTR_NULLABLE(L"Name", m_oFontName);
				CP_XML_ATTR_NULLABLE(L"Height", m_oFontHeight);
				CP_XML_ATTR_NULLABLE(L"Bold", m_oFontBold);
				CP_XML_ATTR_NULLABLE(L"Italic", m_oFontItalic);
				CP_XML_ATTR_NULLABLE(L"Underline", m_oFontUnderline);
				CP_XML_ATTR_NULLABLE(L"Strikeout", m_oFontStrikeout);
			}
		}
		return strm.str();
	}

//-------------------------------------------------------------------------------------------------------------------------------
	void ActiveXObjectScroll::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);
		
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
		if (fVariousPropertyBits)
		{
			_UINT32 flag = mem_stream.ReadUInt32();
			VariousPropertiesBitfield vars(flag);
			
			m_oLockText = vars.Locked;
			m_oMultiLine = vars.MultiLine;
		}
		if (fMousePointer)
		{
			/*MousePointer =*/ mem_stream.ReadByte();
			/*Padding1 =*/ mem_stream.ReadByte();
			/*Padding2 =*/ mem_stream.ReadUInt16();
		}
		mem_stream.Align(4);

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
		mem_stream.Align(4);
		if (fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		if (fMouseIcon)
		{
			readGUID(&mem_stream);
			m_oMouseIcon = readStdPicture(&mem_stream);
		}
		mem_stream.Align(4);
	}
	void ActiveXObjectSpin::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);

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
		if (fVariousPropertyBits)
		{
			_UINT32 flag = mem_stream.ReadUInt32();
			VariousPropertiesBitfield vars(flag);
		
			m_oLockText = vars.Locked;
			m_oMultiLine = vars.MultiLine;
		}
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
		mem_stream.Align(4);
		if (fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		if (fMouseIcon)
		{
			readGUID(&mem_stream);
			m_oMouseIcon = readStdPicture(&mem_stream);
		}
		mem_stream.Align(4);
	}
	void ActiveXObjectButton::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);

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
		_UINT32 sizeCaption = 0;
		int size_picture = 0, pos_picture = 0;

		if (fForeColor)				m_oForeColor = mem_stream.ReadUInt32();
		if (fBackColor)				m_oBackColor = mem_stream.ReadUInt32();
		if (fVariousPropertyBits)
		{
			_UINT32 flag = mem_stream.ReadUInt32();
			VariousPropertiesBitfield vars(flag);

			m_oLockText = vars.Locked;
			m_oMultiLine = vars.MultiLine;
		}
		if (fCaption)			sizeCaption = mem_stream.ReadUInt32();
		if (fPicturePosition)	pos_picture = mem_stream.ReadUInt32();
		if (fMousePointer) /*MousePointer =*/ mem_stream.ReadByte();
 		if (fPicture) mem_stream.ReadUInt16(); //0 or 0xffff
 		if (fAccelerator) /*m_oAccelerator = */mem_stream.ReadUInt16();
		if (fMouseIcon)	mem_stream.ReadUInt16(); //0 or oxffff

		mem_stream.Align(4);
		if (sizeCaption > 0)
		{
			m_oCaption = readString(&mem_stream, sizeCaption);
		}
		if (fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		int stream_size = mem_stream.GetSize() - mem_stream.GetPosition(); 

		if (fMouseIcon)
		{
			readGUID(&mem_stream);
			m_oMouseIcon = readStdPicture(&mem_stream);
		}
		if (fPicture)
		{
			readGUID(&mem_stream);
			m_oPicture = readStdPicture(&mem_stream);
		}
		mem_stream.Align(4);
		readTextProps(&mem_stream);
	}
	void ActiveXObjectImage::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);

		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cbImage = mem_stream.ReadUInt16();

		ImagePropMask PropMask(mem_stream.ReadUInt32());

		m_oTile = PropMask.fPictureTiling;

		if (PropMask.fBorderColor)	m_oBorderColor = mem_stream.ReadUInt32();
		if (PropMask.fBackColor)		m_oBackColor = mem_stream.ReadUInt32();

		if (PropMask.fBorderStyle)	m_oBorderStyle = mem_stream.ReadByte();
		if (PropMask.fMousePointer)/*m_oMousePointer = */mem_stream.ReadByte();
		if (PropMask.fPictureSizeMode) m_oPictureSizeMode = mem_stream.ReadByte();
		if (PropMask.fSpecialEffect)/*oSpecialEffect = */mem_stream.ReadByte();
		if (PropMask.fPicture)/*m_oPicture = */mem_stream.ReadUInt16();
		if (PropMask.fPictureAlignment)/*m_oMousePointer = */mem_stream.ReadByte();

		if (PropMask.fVariousPropertyBits)
		{
			mem_stream.Align(4);
			_UINT32 flag = mem_stream.ReadUInt32();
			VariousPropertiesBitfield vars(flag);
			
			m_oLockText = vars.Locked;
			m_oMultiLine = vars.MultiLine;
		}
		if (PropMask.fMouseIcon) /*m_oMouseIcon =*/ mem_stream.ReadUInt16();

		mem_stream.Align(4);
		if (PropMask.fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		if (PropMask.fPicture)
		{
			_GUID_ guid = readGUID(&mem_stream);

			int Preamble = mem_stream.ReadUInt32(); //MUST 0x0000746C

			nImageSize = mem_stream.ReadUInt32();
			pImageData = boost::shared_array<unsigned char>(mem_stream.ReadBytes(nImageSize, true));
		}
	}
	std::wstring ActiveXObjectImage::toXml()
	{
		std::wstringstream strm;
		CP_XML_WRITER(strm)
		{
			strm << ActiveXObject::toXml();

			CP_XML_NODE(L"Image")
			{
				CP_XML_ATTR_NULLABLE(L"Tile", m_oTile);
				CP_XML_ATTR_NULLABLE(L"PictureSizeMode", m_oPictureSizeMode);
			}
		}
		return strm.str();
	}
	void ActiveXObjectLabel::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);

		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cbLabel = mem_stream.ReadUInt16();

		LabelPropMask PropMask(mem_stream.ReadUInt32());

		_UINT32 sizeCaption = 0;
		int size_picture = 0, pos_picture = 0;

		if (PropMask.fForeColor)				m_oForeColor = mem_stream.ReadUInt32();
		if (PropMask.fBackColor)				m_oBackColor = mem_stream.ReadUInt32();
		if (PropMask.fVariousPropertyBits)
		{
			_UINT32 flag = mem_stream.ReadUInt32();
			VariousPropertiesBitfield vars(flag);

			m_oLockText = vars.Locked;
			m_oMultiLine = vars.MultiLine;
		}
		if (PropMask.fCaption) sizeCaption = mem_stream.ReadUInt32();
		if (PropMask.fPicturePosition) pos_picture = mem_stream.ReadUInt32();
		if (PropMask.fMousePointer) /*MousePointer =*/ mem_stream.ReadByte();

		if (PropMask.fBorderColor)
		{
			mem_stream.Align(4);
			m_oBorderColor = mem_stream.ReadUInt32();
		}
 		if (PropMask.fBorderStyle) m_oBorderStyle = mem_stream.ReadUInt16();
 		if (PropMask.fSpecialEffect) /*m_oEffectStyle = */ mem_stream.ReadUInt16();
 		if (PropMask.fPicture) /*m_oPicture = */ mem_stream.ReadUInt16();
 		if (PropMask.fAccelerator) /*m_oAccelerator = */mem_stream.ReadUInt16();
		if (PropMask.fMouseIcon) /*m_oMouseIcon =*/ mem_stream.ReadUInt16();
		mem_stream.Align(4);
		if (sizeCaption > 0)
		{
			m_oCaption = readString(&mem_stream, sizeCaption);
		}
		if (PropMask.fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		if (PropMask.fMouseIcon)
		{
			mem_stream.Seek(16, 2);//guid
			m_oMouseIcon = readStdPicture(&mem_stream);
		}
		if (PropMask.fPicture)
		{
			mem_stream.Seek(16, 2);//guid
			m_oPicture = readStdPicture(&mem_stream);
		}
		mem_stream.Align(4);
		readTextProps(&mem_stream);
	}	
	void ActiveXObjectFormControl::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);

		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cb = mem_stream.ReadUInt16();
//-------------------------------------------------------------------------------
		_UINT32 flag = mem_stream.ReadUInt32();
		VBA::FormPropMask propMask(flag);

		if (propMask.fBackColor)
		{
			m_oBackColor = mem_stream.ReadUInt32();
		}
		if (propMask.fForeColor)
		{
			m_oForeColor = mem_stream.ReadUInt32();
		}
		if (propMask.fNextAvailableID)
		{
			m_oNextAvailableID = mem_stream.ReadUInt32();
		}
		if (propMask.fBooleanProperties)
		{
			_UINT32 flag2 = mem_stream.ReadUInt32();
			VBA::_BooleanProperties booleanProperties;
			
			booleanProperties.FORM_FLAG_ENABLED = GETBIT(flag2, 2);
			booleanProperties.FORM_FLAG_DESINKPERSISTED = GETBIT(flag2, 14);
			booleanProperties.FORM_FLAG_DONTSAVECLASSTABLE = GETBIT(flag2, 15);
		}
		if (propMask.fBorderStyle) m_oBorderStyle = mem_stream.ReadByte();
		if (propMask.fMousePointer) m_oMousePointer = mem_stream.ReadByte();
		if (propMask.fScrollBars) m_oScrollBars = mem_stream.ReadByte();
		if (propMask.fGroupCnt)
		{
			mem_stream.Align(4);
			m_oGroupCnt = mem_stream.ReadUInt32();
		}
		if (propMask.fMouseIcon)
		{
			_UINT16 MouseIcon = mem_stream.ReadUInt16();
		}
		if (propMask.fCycle) m_oCycle = mem_stream.ReadUInt16();
		if (propMask.fSpecialEffect) m_oSpecialEffect = mem_stream.ReadByte();

		if (propMask.fBorderColor)
		{
			mem_stream.Align(4);
			m_oBorderColor = mem_stream.ReadUInt32();
		}
		_UINT32 LengthAndCompression = 0;
		if (propMask.fCaption)
		{
			mem_stream.Align(4);
			LengthAndCompression = mem_stream.ReadUInt32();
		}
		if (propMask.fFont)
		{
			_UINT16 Font = mem_stream.ReadUInt16();
		}
		if (propMask.fPicture)
		{
			_UINT16 Picture = mem_stream.ReadUInt16();
		}
		if (propMask.fZoom)
		{
			mem_stream.Align(4);
			m_oZoom = mem_stream.ReadUInt32();
		}
		if (propMask.fPictureAlignment)
		{
			m_oPictureAlignment = mem_stream.ReadByte();
		}
		if (propMask.fPictureSizeMode)
		{
			m_oPictureSizeMode = mem_stream.ReadByte();
		}
		mem_stream.Align(4);
		if (propMask.fShapeCookie)
		{
			m_oShapeCookie = mem_stream.ReadUInt32();
		}
		if (propMask.fDrawBuffer)
		{
			m_oDrawBuffer = mem_stream.ReadUInt32();
		}
//- FormExtraDataBlock

		if (propMask.fDisplayedSize)
		{
			m_oDisplayedWidth = mem_stream.ReadUInt32();
			m_oDisplayedHeight = mem_stream.ReadUInt32();
		}
		if (propMask.fLogicalSize)
		{
			m_oLogicalWidth = mem_stream.ReadUInt32();
			m_oLogicalHeight = mem_stream.ReadUInt32();
		}
		if (propMask.fScrollPosition)
		{
			m_oScrollTop = mem_stream.ReadUInt32();
			m_oScrollLeft = mem_stream.ReadUInt32();
		}

		if (propMask.fCaption && LengthAndCompression > 0)
		{
			m_oCaption = readString(&mem_stream, LengthAndCompression);
		}
		//- FormStreamData
		if (propMask.fMouseIcon)
		{
			m_oMouseIconGUID = readGUID(&mem_stream);
			m_oMouseIcon = readStdPicture(&mem_stream);
		}
		mem_stream.Align(4);

		if (propMask.fFont)
		{
			m_oFontGUID = readGUID(&mem_stream);
			if (m_oFontGUID->Data1 == 0x0BE35203 && m_oFontGUID->Data2 == 0x8F91 && m_oFontGUID->Data3 == 0x11CE)
				readStdFont(&mem_stream);
			else if (m_oFontGUID->Data1 == 0xAFC20920 && m_oFontGUID->Data2 == 0xDA4E && m_oFontGUID->Data3 == 0x11CE)
				readTextProps(&mem_stream);
		}
		mem_stream.Align(4);
		if (propMask.fPicture)
		{
			m_oPictureGUID = readGUID(&mem_stream);
			m_oPicture = readStdPicture(&mem_stream);
		}
	}
	void ActiveXObjectTabStrip::Parse(unsigned char* pData, DWORD size)
	{
		MemoryStream mem_stream(pData, size, false);

		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cb = mem_stream.ReadUInt16();

//-------------------------------------------------------------------------------
		TabStripPropMask propMask(mem_stream.ReadUInt32());

		if (propMask.fListIndex) m_oListIndex = mem_stream.ReadUInt32();
		if (propMask.fBackColor) m_oBackColor = mem_stream.ReadUInt32();
		if (propMask.fForeColor) m_oForeColor = mem_stream.ReadUInt32();
		
		_UINT32 ItemsSize = 0, TipStringsSize = 0, TagsSize = 0, AcceleratorsSize = 0, NamesSize = 0, TabsAllocated = 0;

		if (propMask.fItems)
		{
			ItemsSize = mem_stream.ReadUInt32();
		}
		if (propMask.fMousePointer)
		{
			/*m_oMousePointer = */mem_stream.ReadByte();
			mem_stream.Seek(3, 2);
		}
		if (propMask.fTabOrientation) 
			m_oTabOrientation = mem_stream.ReadUInt32(); // fmTabOrientation

		if (propMask.fTabStyle)
			m_oTabStyle = mem_stream.ReadUInt32(); 

		if (propMask.fTabFixedWidth)
			m_oTabFixedWidth = mem_stream.ReadUInt32(); 

		if (propMask.fTabFixedHeight)
			m_oTabFixedHeight = mem_stream.ReadUInt32(); 

		if (propMask.fTipStrings)
			TipStringsSize = mem_stream.ReadUInt32();

		if (propMask.fNames)
			NamesSize = mem_stream.ReadUInt32();

		if (propMask.fVariousPropertyBits)
		{
			_UINT32 flag = mem_stream.ReadUInt32();
			VariousPropertiesBitfield vars(flag);

			m_oLockText = vars.Locked;
			m_oMultiLine = vars.MultiLine;
		}

		if (propMask.fTabsAllocated)
			TabsAllocated = mem_stream.ReadUInt32();
		
		if (propMask.fTags)
			TagsSize = mem_stream.ReadUInt32();
			
		if (propMask.fTabData)
			m_oTabData = mem_stream.ReadUInt32();
	
		if (propMask.fAccelerator)
			AcceleratorsSize = mem_stream.ReadUInt32();
			
		if (propMask.fMouseIcon)
		{
			mem_stream.ReadUInt16(); //0 or 0xFFFF
			/*Padding3 =*/ mem_stream.ReadUInt16();
		}
//TabStripExtraDataBlock
		if (propMask.fSize)
		{
			m_oWidth =  mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		readArrayString(&mem_stream, m_oItems, ItemsSize);
		readArrayString(&mem_stream, m_oTipStrings, TipStringsSize);
		readArrayString(&mem_stream, m_oNames, NamesSize);
		readArrayString(&mem_stream, m_oTags, TagsSize);
		readArrayString(&mem_stream, m_oAccelerators, AcceleratorsSize);

		if (propMask.fMouseIcon)
		{
			readGUID(&mem_stream);
			m_oMouseIcon = readStdPicture(&mem_stream);
		}
		mem_stream.Align(4);

		readTextProps(&mem_stream);

		if (propMask.fTabData)
		{
			for (size_t i = 0; i < ItemsSize; ++i)
			{
				_UINT32 flag = mem_stream.ReadUInt32();
				bool tabVisible = GETBIT(flag, 0);
				bool tabEnable = GETBIT(flag, 1);
				m_oTabStripTabFlags.push_back(std::make_pair(tabVisible, tabEnable));
			}			
		}
	}

	std::wstring ActiveXObjectTabStrip::toXml()
	{
		std::wstringstream strm;
		CP_XML_WRITER(strm)
		{
			strm << ActiveXObject::toXml();
			
			CP_XML_NODE(L"TabStrip")
			{
				CP_XML_ATTR_NULLABLE(L"listIndex", m_oListIndex);
				CP_XML_ATTR_NULLABLE(L"tabOrientation", m_oTabOrientation);
				CP_XML_ATTR_NULLABLE(L"tabStyle", m_oTabStyle);
				CP_XML_ATTR_NULLABLE(L"tabData", m_oTabData);
				CP_XML_ATTR_NULLABLE(L"tabFixedWidth", m_oTabFixedWidth);
				CP_XML_ATTR_NULLABLE(L"tabFixedHeight", m_oTabFixedHeight);

				std::wstring valueItems;
				for (size_t i = 0; i < m_oItems.size(); ++i)
					valueItems += m_oItems[i] + L";";
				
				CP_XML_ATTR_STR(L"valueItems", valueItems);

				std::wstring tipStrings;
				for (size_t i = 0; i < m_oTipStrings.size(); ++i)
					tipStrings += m_oTipStrings[i] + L";";
				
				CP_XML_ATTR_STR(L"tipStrings", tipStrings);

				std::wstring tabNames;
				for (size_t i = 0; i < m_oNames.size(); ++i)
					tabNames += m_oNames[i] + L";";
				
				CP_XML_ATTR_STR(L"tabNames", tabNames);

				std::wstring tags;
				for (size_t i = 0; i < m_oTags.size(); ++i)
					tags += m_oTags[i] + L";";

				CP_XML_ATTR_STR(L"tags", tags);
			}
		}
		return strm.str();
	}
	void ActiveXObjectMorphData::Parse(unsigned char* pData, DWORD size)
	{ 
//CheckBox, ComboBox, ListBox, OptionButton, TextBox, and ToggleButton
		MemoryStream mem_stream(pData, size, false);
		
		unsigned char MinorVersion = mem_stream.ReadByte();
		unsigned char MajorVersion = mem_stream.ReadByte();

		unsigned short cb = mem_stream.ReadUInt16(), cColumnInfo = 0, ColumnCount = 0;

		if (cb > size) return;

		MorphDataPropMask PropMask(mem_stream.ReadUInt64());
//MorphData-DataBlock
		int MaxLength = 0;
		if (PropMask.fVariousPropertyBits)
		{
			_UINT32 flag = mem_stream.ReadUInt32();
			VariousPropertiesBitfield vars(flag);

			m_oLockText = vars.Locked;
			m_oMultiLine = vars.MultiLine;
		}
		if (PropMask.fForeColor) m_oForeColor = mem_stream.ReadUInt32();
		if (PropMask.fBackColor) m_oBackColor = mem_stream.ReadUInt32();
		if (PropMask.fMaxLength) MaxLength = mem_stream.ReadUInt32();

		if (PropMask.fBorderStyle) m_oBorderStyle = mem_stream.ReadByte();
		if (PropMask.fScrollBars) m_oScrollBarsType = mem_stream.ReadByte();
		if (PropMask.fDisplayStyle)
		{
			if (false == m_oObjectType.IsInit()) m_oObjectType.Init();

			int nDisplayStyle = mem_stream.ReadByte(); 

			switch (nDisplayStyle)
			{
				case 0x01: m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectEditBox); break;
				case 0x02: m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectList); break;
				case 0x03: m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectDrop); break;
				case 0x04: m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectCheckBox); break;
				case 0x05: m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectRadio); break;
				case 0x06: m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectToggleButton); break;
				case 0x07: m_oObjectType->SetValue(SimpleTypes::Spreadsheet::objectList); break;
			}
		}
//----------------------------------------------------------------------------
		_UINT32 PasswordChar = 0, m_oListWidth = 0, BoundColumn = 0, TextColumn = 0, m_oMousePointer = 0;
		_UINT32 MatchEntry = 0, ListStyle = 0, ShowDropButtonWhen = 0, DropButtonStyle = 0, pos_picture = 0, m_oEffect = 0, m_oAccelerator = 0;
		
		if (PropMask.fMousePointer) m_oMousePointer = mem_stream.ReadByte();
		if (PropMask.fPasswordChar)
		{
			m_oPasswordEdit = true;
			PasswordChar = mem_stream.ReadUInt16(); 
		}
		if (PropMask.fListWidth)
		{
			mem_stream.Align(4);
			m_oListWidth = mem_stream.ReadUInt32();
		}
		if (PropMask.fBoundColumn)	BoundColumn = mem_stream.ReadUInt16();
		if (PropMask.fTextColumn)	TextColumn = mem_stream.ReadUInt16(); 
		if (PropMask.fColumnCount)	ColumnCount = mem_stream.ReadUInt16(); 
		if (PropMask.fListRows)	m_oListRows = mem_stream.ReadUInt16();
		if (PropMask.fcColumnInfo) cColumnInfo = mem_stream.ReadUInt16(); 
		if (PropMask.fMatchEntry) MatchEntry = mem_stream.ReadByte(); 
		if (PropMask.fListStyle) ListStyle = mem_stream.ReadByte(); 
		if (PropMask.fShowDropButtonWhen) ShowDropButtonWhen = mem_stream.ReadByte(); 
		if (PropMask.fDropButtonStyle) DropButtonStyle = mem_stream.ReadByte(); 
		if (PropMask.fMultiSelect) m_oSelType = (SimpleTypes::Spreadsheet::ESelType) mem_stream.ReadByte();

		_UINT32 sizeCaption = 0, sizeGroupName = 0, sizeValue = 0;

		if (PropMask.fValue)
		{
			mem_stream.Align(4);
			sizeValue = mem_stream.ReadUInt32();
		}
		if (PropMask.fCaption)
		{
			mem_stream.Align(4);
			sizeCaption = mem_stream.ReadUInt32();
		}
		if (PropMask.fPicturePosition)
		{
			pos_picture = mem_stream.ReadUInt32();
		}
		if (PropMask.fBorderColor)
		{
			mem_stream.Align(4);
			m_oBorderColor = mem_stream.ReadUInt32();
		}
		if (PropMask.fSpecialEffect)
		{
			mem_stream.Align(4);
			m_oEffect = mem_stream.ReadUInt32(); // fmSpecialEffect
		}		
		if (PropMask.fMouseIcon)
		{
			_UINT16 MouseIcon = mem_stream.ReadUInt16(); //0 or 0xFFFF
		}
		if (PropMask.fPicture)
		{
			_UINT16 Picture = mem_stream.ReadUInt16();//0 or 0xFFFF
		}
		if (PropMask.fAccelerator)
		{
			m_oAccelerator = mem_stream.ReadUInt16();
		}
		mem_stream.Align(4);
		if (PropMask.fGroupName) sizeGroupName = mem_stream.ReadUInt32();
		
//MorphData-ExtraDataBlock
		if (PropMask.fSize)
		{
			m_oWidth = mem_stream.ReadUInt32();
			m_oHeight = mem_stream.ReadUInt32();
		}
		if (sizeValue > 0)
		{
			m_oValue = readString(&mem_stream, sizeValue);
		}
		if (sizeCaption > 0)
		{
			m_oCaption = readString(&mem_stream, sizeCaption);
		}
		std::wstring oGroupName;
		if (sizeGroupName > 0 && sizeGroupName < 0xfff0)
		{
			oGroupName = readString(&mem_stream, sizeGroupName);
		}
//MorphData-StreamData
		int stream_size = mem_stream.GetSize() - mem_stream.GetPosition();

		if (PropMask.fMouseIcon)
		{
			readGUID(&mem_stream);
			m_oMouseIcon = readStdPicture(&mem_stream);
		}
		mem_stream.Align(4);
		if (PropMask.fPicture)
		{
			readGUID(&mem_stream);
			m_oPicture = readStdPicture(&mem_stream);
		}
		mem_stream.Align(4);
		readTextProps(&mem_stream);

		for (size_t i = 0; i < cColumnInfo; ++i)
		{
			m_arColumnInfo.push_back(readColumnInfo(&mem_stream));
		}
	}
} // namespace OOX
