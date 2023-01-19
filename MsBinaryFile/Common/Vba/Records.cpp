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
#include "Records.h"

#include "../../../DesktopEditor/common/File.h"
#include "../../../UnicodeConverter/UnicodeConverter.h"

#include <boost/make_shared.hpp>
#include <map>
#include <locale>

#define GETBITS(from, numL, numH) ((from & (((1 << (numH - numL + 1)) - 1) << numL)) >> numL)
#define GETBIT(from, num) ((from & (1 << num)) != 0)

namespace VBA
{
	const std::string int2str(const int val, const int radix)
	{
		static char num_buf[10] = {};
#if defined(_WIN32) || defined(_WIN64)
		_itoa_s(val, num_buf, 9, radix);
#else
		sprintf(num_buf, "%d", val);
#endif
		return num_buf;
	}
	const std::wstring int2wstr(const int val, const int radix)
	{
#if defined(_WIN32) || defined(_WIN64)
		static wchar_t num_buf[20] = {};
		_itow_s(val, num_buf, 19, radix);
		return std::wstring(num_buf);
#else
		static char num_buf[20] = {};
		sprintf(num_buf, "%d", val);
		std::string a_str(num_buf);
		return std::wstring(a_str.begin(), a_str.end());
#endif
	}
	const std::wstring int2hex_wstr(const int val, const size_t size_of)
	{
		if (size_of > 4) return L"";

#if defined(_WIN32) || defined(_WIN64)
		static wchar_t num_buf[10] = {};
		std::wstring wstr = int2wstr(size_of << 1, 10);
		swprintf_s(num_buf, 9, (L"%0" + wstr + L"X").c_str(), val);
		return std::wstring(num_buf);
#else
		char num_buf[10] = {};
		std::string str = int2str(size_of << 1, 10);
		snprintf(num_buf, 9, ("%0" + str + "X").c_str(), val);
		std::string res(num_buf);
		return std::wstring(res.begin(), res.end());
#endif
	}
	const std::wstring  guid2bstr(_GUID_ & guid)
	{
		std::wstring  guid_ret = L"{";

		guid_ret += int2hex_wstr(guid.Data1, 4) + L"-" +
			int2hex_wstr(guid.Data2, 2) + L"-" +
			int2hex_wstr(guid.Data3, 2) + L"-" +
			int2hex_wstr((guid.getData4())[0], 1) + int2hex_wstr((guid.getData4())[1], 1) + L"-" +
			int2hex_wstr((guid.getData4())[2], 1) + int2hex_wstr((guid.getData4())[3], 1) +
			int2hex_wstr((guid.getData4())[4], 1) + int2hex_wstr((guid.getData4())[5], 1) +
			int2hex_wstr((guid.getData4())[6], 1) + int2hex_wstr((guid.getData4())[7], 1);
		return guid_ret + L"}";
	}
	const std::wstring convert_string_icu(const char* buffer, const unsigned int& size, _UINT32 nCodePage)
	{
		if (!buffer || size < 1) return L"";

		std::string sCodePage;
		std::map<int, std::string>::const_iterator pFind = NSUnicodeConverter::mapEncodingsICU.find(nCodePage);
		if (pFind != NSUnicodeConverter::mapEncodingsICU.end())
		{
			sCodePage = pFind->second;
		}
		if (!sCodePage.empty())
		{
			NSUnicodeConverter::CUnicodeConverter oConverter;
			return oConverter.toUnicode(buffer, size, sCodePage.c_str(), true);
		}
		else if (nCodePage != 0)
		{
			NSUnicodeConverter::CUnicodeConverter oConverter;
			return oConverter.toUnicode(buffer, size, nCodePage, true);
		}
		else
		{//текущая локаль

			std::locale loc("");
			std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);

			std::wstring result;
			result.resize(size);

			facet.widen(buffer, buffer + size, &result[0]);
			return result;
		}
	}
	std::wstring readString(unsigned char *pData, _UINT32 & size)
	{
		if (!pData) return L"";

		bool fCompressed = GETBIT(size, 31);
		size = GETBITS(size, 0, 30);

		std::wstring result;

		if (fCompressed)
			result = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(pData, size);
		else
			result = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pData, size / 2);

		return result;
	}
	std::wstring readStringPadding(CVbaFileStreamPtr stream, _UINT32 & size)
	{
		if (!stream) return L"";

		std::wstring result = readString(stream->getDataCurrent(), size);

		_INT32 count_padding = 4 - (size % 4);
		stream->skipBytes(size + ((count_padding > 0 && count_padding < 4) ? count_padding : 0));

		return result;
	}
	std::pair<boost::shared_array<unsigned char>, _UINT32> readStdPicture(CVbaFileStreamPtr stream)
	{
		boost::shared_array<unsigned char> empty;
		if (!stream) return std::make_pair(empty, 0);

		_UINT32 Preamble;
		*stream >> Preamble;

		if (Preamble != 0x0000746C) return std::make_pair(empty, 0);

		_UINT32 size;
		*stream >> size;

		unsigned char* buf = new unsigned char[size];
		stream->read(buf, size);

		boost::shared_array<unsigned char> _array(buf);

		return std::make_pair(_array, size);
	}
//------------------------------------------------------------------------------------------
	DX_MODE::DX_MODE(_UINT32 flag)
	{
		fInheritDesign = GETBIT(flag, 0);
		fDesign = GETBIT(flag, 1);
		fInheritShowToolbox = GETBIT(flag, 2);
		fShowToolbox = GETBIT(flag, 3);
		fInheritShowGrid = GETBIT(flag, 4);
		fShowGrid = GETBIT(flag, 5);
		fInheritSnapToGrid = GETBIT(flag, 6);
		fSnapToGrid = GETBIT(flag, 7);
		fInheritGridX = GETBIT(flag, 8);
		fInheritGridY = GETBIT(flag, 9);
		fInheritClickControl = GETBIT(flag, 10);
		fInheritDblClickControl = GETBIT(flag, 11);
		fInheritShowInvisible = GETBIT(flag, 12);
		fInheritShowTooltips = GETBIT(flag, 13);
		fShowTooltips = GETBIT(flag, 14);
		fInheritLayoutImmediate = GETBIT(flag, 15);
		fLayoutImmediate = GETBIT(flag, 16);
	}
	SITE_FLAG::SITE_FLAG(_UINT32 flag)
	{
		fTabStop = GETBIT(flag, 0);
		fVisible = GETBIT(flag, 1);
		fDefault = GETBIT(flag, 2);
		fCancel = GETBIT(flag, 3);
		fStreamed = GETBIT(flag, 4);
		fAutoSize = GETBIT(flag, 5);
		fPreserveHeight = GETBIT(flag, 8);
		fFitToParent = GETBIT(flag, 9);
		fSelectChild = GETBIT(flag, 13);
		fPromoteControls = GETBIT(flag, 18);
	}
	TextPropsPropMask::TextPropsPropMask(_UINT32 mask)
	{
		fFontName = GETBIT(mask, 0);
		fFontEffects = GETBIT(mask, 1);
		fFontHeight = GETBIT(mask, 2);
		fFontCharSet = GETBIT(mask, 4);
		fFontPitchAndFamily = GETBIT(mask, 5);
		fParagraphAlign = GETBIT(mask, 6);
		fFontWeight = GETBIT(mask, 7);
	}
	FormPropMask::FormPropMask(_UINT32 mask)
	{
		fBackColor = GETBIT(mask, 1);
		fForeColor = GETBIT(mask, 2);
		fNextAvailableID = GETBIT(mask, 3);
		fBooleanProperties = GETBIT(mask, 6);
		fBorderStyle = GETBIT(mask, 7);
		fMousePointer = GETBIT(mask, 8);
		fScrollBars = GETBIT(mask, 9);
		fDisplayedSize = GETBIT(mask, 10);
		fLogicalSize = GETBIT(mask, 11);
		fScrollPosition = GETBIT(mask, 12);
		fGroupCnt = GETBIT(mask, 13);
		Reserved = GETBIT(mask, 14);
		fMouseIcon = GETBIT(mask, 15);
		fCycle = GETBIT(mask, 16);
		fSpecialEffect = GETBIT(mask, 17);
		fBorderColor = GETBIT(mask, 18);
		fCaption = GETBIT(mask, 19);
		fFont = GETBIT(mask, 20);
		fPicture = GETBIT(mask, 21);
		fZoom = GETBIT(mask, 22);
		fPictureAlignment = GETBIT(mask, 23);
		fPictureTiling = GETBIT(mask, 24);
		fPictureSizeMode = GETBIT(mask, 25);
		fShapeCookie = GETBIT(mask, 26);
		fDrawBuffer = GETBIT(mask, 27);
	}
	ClassInfoPropMask::ClassInfoPropMask(_UINT32 mask)
	{
		fClsID = GETBIT(mask, 0);
		fDispEvent = GETBIT(mask, 1);
		fDefaultProg = GETBIT(mask, 3);
		fClassFlags = GETBIT(mask, 4);
		fCountOfMethods = GETBIT(mask, 5);
		fDispidBind = GETBIT(mask, 6);
		fGetBindIndex = GETBIT(mask, 7);
		fPutBindIndex = GETBIT(mask, 8);
		fBindType = GETBIT(mask, 9);
		fGetValueIndex = GETBIT(mask, 10);
		fPutValueIndex = GETBIT(mask, 11);
		fValueType = GETBIT(mask, 12);
		fDispidRowset = GETBIT(mask, 13);
		fSetRowset = GETBIT(mask, 14);
	}
	SitePropMask::SitePropMask(_UINT32 mask)
	{
		fName = GETBIT(mask, 0);
		fTag = GETBIT(mask, 1);
		fID = GETBIT(mask, 2);
		fHelpContextID = GETBIT(mask, 3);
		fBitFlags = GETBIT(mask, 4);
		fObjectStreamSize = GETBIT(mask, 5);
		fTabIndex = GETBIT(mask, 6);
		fClsidCacheIndex = GETBIT(mask, 7);
		fPosition = GETBIT(mask, 8);
		fGroupID = GETBIT(mask, 9);
		fControlTipText = GETBIT(mask, 11);
		fRuntimeLicKey = GETBIT(mask, 12);
		fControlSource = GETBIT(mask, 13);
		fRowSource = GETBIT(mask, 14);
	}
	DesignExtenderPropMask::DesignExtenderPropMask(_UINT32 mask)
	{
		fBitFlags = GETBIT(mask, 0);
		fGridX = GETBIT(mask, 1);
		fGridY = GETBIT(mask, 2);
		fClickControlMode = GETBIT(mask, 2);
		fDblClickControlMode = GETBIT(mask, 2);
	};

//------------------------------------------------------------------------------------------
	StdFont::StdFont(CVbaFileStreamPtr stream) { load(stream); }
	BaseRecordPtr StdFont::clone()
	{
		return BaseRecordPtr(new StdFont(*this));
	}
	void StdFont::load(CVbaFileStreamPtr stream)
	{
		unsigned char Version = 0;
		unsigned char bFlags = 0;
		unsigned char bFaceLen = 0;

		unsigned short sCharset = 0;
		*stream >> Version >> sCharset >> bFlags >> FontWeight >> FontHeight >> bFaceLen;

		FontCharSet = sCharset;

		bFontBold = GETBIT(bFlags, 0);
		bFontItalic = GETBIT(bFlags, 1);
		bFontUnderline = GETBIT(bFlags, 2);
		bFontStrikeout = GETBIT(bFlags, 3);

		if (bFaceLen > 0)
		{
			char *buf = new char[bFaceLen];
			if (buf)
			{
				stream->read(buf, bFaceLen);
				sFontName = convert_string_icu(buf, bFaceLen, stream->CodePage);
				delete []buf;
			}
		}
	}
//------------------------------------------------------------------------------------------
	TextProps::TextProps(CVbaFileStreamPtr stream) { load(stream); }
	BaseRecordPtr TextProps::clone()
	{
		return BaseRecordPtr(new TextProps(*this));
	}
	void TextProps::load(CVbaFileStreamPtr stream)
	{
		unsigned char MinorVersion, MajorVersion;
		_UINT16 cbTextProps;
		_UINT32 flag2, flag3;

		*stream >> MinorVersion >> MajorVersion >> cbTextProps;
		
		_UINT32 pos = stream->GetDataPos();
		*stream >> flag2;
		TextPropsPropMask propMask(flag2);

		_UINT32 FontNameSize = 0;

		if (propMask.fFontName)
			*stream >> FontNameSize;
		if (propMask.fFontEffects)
		{
			*stream >> flag3;
			bFontBold = GETBIT(flag3, 0);
			bFontItalic = GETBIT(flag3, 1);
			bFontUnderline = GETBIT(flag3, 2);
			bFontStrikeout = GETBIT(flag3, 3);
			bFontAutoColor = GETBIT(flag3, 30);
		}
		if (propMask.fFontHeight)
			*stream >> FontHeight;
		
		if (propMask.fFontCharSet)
		{
			*stream >> FontCharSet;
		} 
		if (propMask.fFontPitchAndFamily)
		{
			*stream >> FontPitchAndFamily;
		}
		if (propMask.fParagraphAlign)
		{
			*stream >> FontPitchAndFamily;
		}
		if (propMask.fFontWeight)
		{
			*stream >> FontWeight;
		}

		stream->Align(4);

		if (FontNameSize > 0)
		{
			sFontName = readStringPadding(stream, FontNameSize);
		}
	}
//------------------------------------------------------------------------------------------
	AnsiString::AnsiString(CVbaFileStreamPtr stream) { load(stream); }
	BaseRecordPtr AnsiString::clone()
	{
		return BaseRecordPtr(new AnsiString(*this));
	}
	void AnsiString::load(CVbaFileStreamPtr stream)
	{
		_UINT32 sizeOf;
		*stream >> sizeOf;
		
		if (sizeOf > 0)
		{
			char *buf = new char[sizeOf];
			if (buf)
			{
				stream->read(buf, sizeOf);
				value = convert_string_icu(buf, sizeOf, stream->CodePage);
				delete[]buf;
			}
		}
	}
//------------------------------------------------------------------------------------------
	Utf16String::Utf16String(CVbaFileStreamPtr stream) { load(stream); }
	BaseRecordPtr Utf16String::clone()
	{
		return BaseRecordPtr(new Utf16String(*this));
	}
	void Utf16String::load(CVbaFileStreamPtr stream)
	{
		_UINT32 sizeOf;
		*stream >> sizeOf;

		if (sizeOf > 0)
		{
			UTF16 *buf = new UTF16[sizeOf / 2];
			if (buf)
			{
				stream->read(buf, sizeOf);
				if (sizeof(wchar_t) == 2)
				{
					value = std::wstring((wchar_t*)buf, sizeOf / 2);
				}
				else
				{
					value = convertUtf16ToWString(buf, sizeOf / 2);
				}	
				delete[]buf;
			}
		}
	}
//------------------------------------------------------------------------------------------
	BaseRecordPtr MODULENAME::clone()
	{
		return BaseRecordPtr(new MODULENAME(*this));
	}
	void MODULENAME::load(CVbaFileStreamPtr stream)
	{
		ModuleName.load(stream);
	}
//--------------------------------------------------------------------------------
	BaseRecordPtr MODULENAMEUNICODE::clone()
	{
		return BaseRecordPtr(new MODULENAMEUNICODE(*this));
	}
	void MODULENAMEUNICODE::load(CVbaFileStreamPtr stream)
	{
		ModuleNameUnicode.load(stream);
	}
//--------------------------------------------------------------------------------
	BaseRecordPtr MODULESTREAMNAME::clone()
	{
		return BaseRecordPtr(new MODULESTREAMNAME(*this));
	}
	void MODULESTREAMNAME::load(CVbaFileStreamPtr stream)
	{
		_UINT16 Reserved;
		
		StreamName.load(stream);
		*stream >> Reserved;
		StreamNameUnicode.load(stream);
	}
//--------------------------------------------------------------------------------
	BaseRecordPtr MODULEDOCSTRING::clone()
	{
		return BaseRecordPtr(new MODULEDOCSTRING(*this));
	}
	void MODULEDOCSTRING::load(CVbaFileStreamPtr stream)
	{
		_UINT16 Reserved; 

		DocString.load(stream);			
		*stream >> Reserved;
		DocStringUnicode.load(stream);
	}
//--------------------------------------------------------------------------------
	BaseRecordPtr MODULEOFFSET::clone()
	{
		return BaseRecordPtr(new MODULEOFFSET(*this));
	}
	void MODULEOFFSET::load(CVbaFileStreamPtr stream)
	{
		_UINT32 SizeOf;
		*stream >> SizeOf;
		*stream >> TextOffset;
	}
//--------------------------------------------------------------------------------
	BaseRecordPtr MODULEHELPCONTEXT::clone()
	{
		return BaseRecordPtr(new MODULEHELPCONTEXT(*this));
	}
	void MODULEHELPCONTEXT::load(CVbaFileStreamPtr stream)
	{
		_UINT32 SizeOf;
		*stream >> SizeOf;
		*stream >> HelpContext;
	}
//--------------------------------------------------------------------------------
	BaseRecordPtr MODULECOOKIE::clone()
	{
		return BaseRecordPtr(new MODULECOOKIE(*this));
	}
	void MODULECOOKIE::load(CVbaFileStreamPtr stream)
	{
		_UINT32 SizeOf;
		_UINT16 Cookie;
		*stream >> SizeOf >> Cookie;
	}
//--------------------------------------------------------------------------------
	BaseRecordPtr MODULETYPE::clone()
	{
		return BaseRecordPtr(new MODULETYPE(*this));
	}
	void MODULETYPE::load(CVbaFileStreamPtr stream)
	{
		_UINT32 Reserved;
		*stream >> Reserved;
	}
//--------------------------------------------------------------------------------
	BaseRecordPtr MODULEREADONLY::clone()
	{
		return BaseRecordPtr(new MODULEREADONLY(*this));
	}
	void MODULEREADONLY::load(CVbaFileStreamPtr stream)
	{
		_UINT32 Reserved;
		*stream >> Reserved;
	}
//--------------------------------------------------------------------------------
	BaseRecordPtr MODULEPRIVATE::clone()
	{
		return BaseRecordPtr(new MODULEPRIVATE(*this));
	}
	void MODULEPRIVATE::load(CVbaFileStreamPtr stream)
	{
		_UINT32 Reserved;
		*stream >> Reserved;
	}
//--------------------------------------------------------------------------------
	BaseRecordPtr MODULE::clone()
	{
		return BaseRecordPtr(new MODULE(*this));
	}

	void MODULE::load(CVbaFileStreamPtr stream)
	{
		bool bEof = false;
		while (!bEof && stream->checkFitRead(6))
		{
			_UINT16 Id;
			*stream >> Id;

			switch (Id)
			{
			case rt_MODULENAME:			NameRecord = boost::make_shared<MODULENAME>(stream); break;
			case rt_MODULENAMEUNICODE:	NameUnicodeRecord = boost::make_shared<MODULENAMEUNICODE>(stream); break;
			case rt_MODULESTREAMNAME:	StreamNameRecord = boost::make_shared<MODULESTREAMNAME>(stream); break;
			case rt_MODULEDOCSTRING:	DocStringRecord = boost::make_shared<MODULEDOCSTRING>(stream); break;
			case rt_MODULEOFFSET:		OffsetRecord = boost::make_shared<MODULEOFFSET>(stream); break;
			case rt_MODULEHELPCONTEXT:	HelpContextRecord = boost::make_shared<MODULEHELPCONTEXT>(stream); break;
			case rt_MODULECOOKIE: { MODULECOOKIE Cookie(stream); } break;
			case rt_MODULEREADONLY: { bReadOnly = true;  MODULEREADONLY ReadOnly(stream); } break;
			case rt_MODULEPRIVATE: { bPrivate = true; MODULEPRIVATE Private(stream); } break;
			case rt_MODULETYPE_Procedural:
				bProceduralModule = true;
			case rt_MODULETYPE_Class: { MODULETYPE type(stream); } break;
			case rt_Terminator:
			{
				bEof = true;
				_UINT32 Reserved;
				*stream >> Reserved;
			}break;
			default:
				_UINT32 SizeOf;
				*stream >> SizeOf;
				stream->skipBytes(SizeOf);
				break;
			}
		}
	}
//-------------------------------------------------------------------------------------------------------
	PROJECTSYSKIND::PROJECTSYSKIND(CVbaFileStreamPtr stream) { load(stream); }
	BaseRecordPtr PROJECTSYSKIND::clone()
	{
		return BaseRecordPtr(new PROJECTSYSKIND(*this));
	}
	void PROJECTSYSKIND::load(CVbaFileStreamPtr stream)
	{
		_UINT32 sizeOf;
		*stream >> sizeOf >> SysKind;
	}
//-------------------------------------------------------------------------------
	PROJECTCOMPATVERSION::PROJECTCOMPATVERSION(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTCOMPATVERSION::~PROJECTCOMPATVERSION() {}
	BaseRecordPtr PROJECTCOMPATVERSION::clone()
	{
		return BaseRecordPtr(new PROJECTCOMPATVERSION(*this));
	}
	void PROJECTCOMPATVERSION::load(CVbaFileStreamPtr stream)
	{
		_UINT32 sizeOf;
		*stream >> sizeOf >> CompatVersion;
	}
//-------------------------------------------------------------------------------
	PROJECTLCID::PROJECTLCID(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTLCID::~PROJECTLCID() {}
	BaseRecordPtr PROJECTLCID::clone()
	{
		return BaseRecordPtr(new PROJECTLCID(*this));
	}
	void PROJECTLCID::load(CVbaFileStreamPtr stream)
	{
		_UINT32 sizeOf;
		*stream >> sizeOf >> Lcid;
	}
//-------------------------------------------------------------------------------
	PROJECTLCIDINVOKE::PROJECTLCIDINVOKE(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTLCIDINVOKE::~PROJECTLCIDINVOKE() {}
	BaseRecordPtr PROJECTLCIDINVOKE::clone()
	{
		return BaseRecordPtr(new PROJECTLCIDINVOKE(*this));
	}
	void PROJECTLCIDINVOKE::load(CVbaFileStreamPtr stream)
	{
		_UINT32 sizeOf;
		*stream >> sizeOf >> LcidInvoke;
	}
//-------------------------------------------------------------------------------	
	PROJECTCODEPAGE::PROJECTCODEPAGE(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTCODEPAGE::~PROJECTCODEPAGE() {}
	BaseRecordPtr PROJECTCODEPAGE::clone()
	{
		return BaseRecordPtr(new PROJECTCODEPAGE(*this));
	}
	void PROJECTCODEPAGE::load(CVbaFileStreamPtr stream)
	{
		_UINT32 sizeOf;
		*stream >> sizeOf >> CodePage;

		stream->CodePage = CodePage;
	}
//-------------------------------------------------------------------------------
	PROJECTNAME::PROJECTNAME(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTNAME::~PROJECTNAME() {}
	BaseRecordPtr PROJECTNAME::clone()
	{
		return BaseRecordPtr(new PROJECTNAME(*this));
	}
	void PROJECTNAME::load(CVbaFileStreamPtr stream)
	{
		ProjectName.load(stream);
	}
//-------------------------------------------------------------------------------
	PROJECTDOCSTRING::PROJECTDOCSTRING(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTDOCSTRING::~PROJECTDOCSTRING() {}
	BaseRecordPtr PROJECTDOCSTRING::clone()
	{
		return BaseRecordPtr(new PROJECTDOCSTRING(*this));
	}
	void PROJECTDOCSTRING::load(CVbaFileStreamPtr stream)
	{
		unsigned short Reserved;
		aDocString.load(stream);
		*stream >> Reserved;
		uDocString.load(stream);
	}
//-------------------------------------------------------------------------------
	PROJECTHELPFILEPATH::PROJECTHELPFILEPATH(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTHELPFILEPATH::~PROJECTHELPFILEPATH() {}
	BaseRecordPtr PROJECTHELPFILEPATH::clone()
	{
		return BaseRecordPtr(new PROJECTHELPFILEPATH(*this));
	}
	void PROJECTHELPFILEPATH::load(CVbaFileStreamPtr stream)
	{
		unsigned short Reserved;
		HelpFile1.load(stream);
		*stream >> Reserved;
		HelpFile2.load(stream);
	}
//-------------------------------------------------------------------------------
	PROJECTHELPCONTEXT::PROJECTHELPCONTEXT(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTHELPCONTEXT::~PROJECTHELPCONTEXT() {}
	BaseRecordPtr PROJECTHELPCONTEXT::clone()
	{
		return BaseRecordPtr(new PROJECTHELPCONTEXT(*this));
	}
	void PROJECTHELPCONTEXT::load(CVbaFileStreamPtr stream)
	{
		_UINT32 sizeOf;
		*stream >> sizeOf >> HelpContext;
	}
//-------------------------------------------------------------------------------
	PROJECTLIBFLAGS::PROJECTLIBFLAGS(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTLIBFLAGS::~PROJECTLIBFLAGS() {}
	BaseRecordPtr PROJECTLIBFLAGS::clone()
	{
		return BaseRecordPtr(new PROJECTLIBFLAGS(*this));
	}
	void PROJECTLIBFLAGS::load(CVbaFileStreamPtr stream)
	{
		_UINT32 sizeOf;
		*stream >> sizeOf >> ProjectLibFlags;
	}
//-------------------------------------------------------------------------------
	PROJECTVERSION::PROJECTVERSION(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTVERSION::~PROJECTVERSION() {}
	BaseRecordPtr PROJECTVERSION::clone()
	{
		return BaseRecordPtr(new PROJECTVERSION(*this));
	}
	void PROJECTVERSION::load(CVbaFileStreamPtr stream)
	{
		_UINT32 sizeOf;
		*stream >> sizeOf >> VersionMajor >> VersionMinor;
	}
//-------------------------------------------------------------------------------
	//Constants = Constant *(" : " Constant) 
	//Constant = ConstantName " = " ConstantValue 
	//ConstantName = VbaIdentifier 
	//ConstantValue = ["-"] 1 * 5DIGIT

	PROJECTCONSTANTS::PROJECTCONSTANTS(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTCONSTANTS::~PROJECTCONSTANTS() {}
	BaseRecordPtr PROJECTCONSTANTS::clone()
	{
		return BaseRecordPtr(new PROJECTCONSTANTS(*this));
	}
	void PROJECTCONSTANTS::load(CVbaFileStreamPtr stream)
	{
		unsigned short Reserved;
		
		aConstants.load(stream);
		*stream >> Reserved;
		uConstants.load(stream);
	}
//-------------------------------------------------------------------------------
	REFERENCENAME::REFERENCENAME(CVbaFileStreamPtr stream) { load(stream); }
	REFERENCENAME::~REFERENCENAME() {}
	BaseRecordPtr REFERENCENAME::clone()
	{
		return BaseRecordPtr(new REFERENCENAME(*this));
	}
	void REFERENCENAME::load(CVbaFileStreamPtr stream)
	{
		unsigned short Reserved;
		aName.load(stream);
		*stream >> Reserved;
		uName.load(stream);
	}
//-------------------------------------------------------------------------------
	REFERENCEORIGINAL::REFERENCEORIGINAL(CVbaFileStreamPtr stream) { load(stream); }
	REFERENCEORIGINAL::~REFERENCEORIGINAL() {}
	BaseRecordPtr REFERENCEORIGINAL::clone()
	{
		return BaseRecordPtr(new REFERENCEORIGINAL(*this));
	}
	void REFERENCEORIGINAL::load(CVbaFileStreamPtr stream)
	{
		LibidOriginal.load(stream);
	}
//-------------------------------------------------------------------------------
	REFERENCEREGISTERED::REFERENCEREGISTERED(CVbaFileStreamPtr stream) { load(stream); }
	REFERENCEREGISTERED::~REFERENCEREGISTERED() {}
	BaseRecordPtr REFERENCEREGISTERED::clone()
	{
		return BaseRecordPtr(new REFERENCEREGISTERED(*this));
	}
	void REFERENCEREGISTERED::load(CVbaFileStreamPtr stream)
	{
		_UINT32 Reserved1, Size;
		_UINT16 Reserved2;
		*stream >> Size;
		Libid.load(stream);
		*stream >> Reserved1 >> Reserved2;
	}
//-------------------------------------------------------------------------------
	REFERENCEPROJECT::REFERENCEPROJECT(CVbaFileStreamPtr stream) { load(stream); }
	REFERENCEPROJECT::~REFERENCEPROJECT() {}
	BaseRecordPtr REFERENCEPROJECT::clone()
	{
		return BaseRecordPtr(new REFERENCEPROJECT(*this));
	}
	void REFERENCEPROJECT::load(CVbaFileStreamPtr stream)
	{
		_UINT32 sizeOf;
		*stream >> sizeOf;

		LibidAbsolute.load(stream);
		LibidRelative.load(stream);

		*stream >> MajorVersion >> MinorVersion;
	}
//-------------------------------------------------------------------------------
	REFERENCECONTROL::REFERENCECONTROL(CVbaFileStreamPtr stream) { load(stream); }
	REFERENCECONTROL::~REFERENCECONTROL() {}
	BaseRecordPtr REFERENCECONTROL::clone()
	{
		return BaseRecordPtr(new REFERENCECONTROL(*this));
	}
	void REFERENCECONTROL::load(CVbaFileStreamPtr stream)
	{
		_UINT32 Reserved1, Reserved4, SizeTwiddled, SizeExtended, Cookie;
		_UINT16 Reserved2, Reserved3, Reserved5, Id;

		*stream >> SizeTwiddled;
		LibidTwiddled.load(stream);
		*stream >> Reserved1 >> Reserved2;

		*stream >> Id;
		if (Id == 0x0016)
		{
			NameRecordExtended = boost::make_shared<REFERENCENAME>(stream);
			*stream >> Reserved3;
		}
		else
		{
			Reserved3 = Id; // = 0x0030
		}

		*stream >> SizeExtended;
		LibidExtended.load(stream);
		*stream >> Reserved4 >> Reserved5 >> OriginalTypeLib >> Cookie;
	}
//-------------------------------------------------------------------------------
	PROJECTCOOKIE::PROJECTCOOKIE(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTCOOKIE::~PROJECTCOOKIE() {}
	BaseRecordPtr PROJECTCOOKIE::clone()
	{
		return BaseRecordPtr(new PROJECTCOOKIE(*this));
	}
	void PROJECTCOOKIE::load(CVbaFileStreamPtr stream)
	{
		_UINT16 Id;
		_UINT32 sizeOf;
		*stream >> Id >> sizeOf >> Cookie;
	}
//-----------------------------------------------------------------------------------
	PROJECTMODULES::PROJECTMODULES(CVbaFileStreamPtr stream) { load(stream); }
	PROJECTMODULES::~PROJECTMODULES() {}
	BaseRecordPtr PROJECTMODULES::clone()
	{
		return BaseRecordPtr(new PROJECTMODULES(*this));
	}
	void PROJECTMODULES::load(CVbaFileStreamPtr stream)
	{
		_UINT16 Id, nCount;
		_UINT32 sizeOf;
		
		*stream >> Id >> sizeOf >> nCount;
		
		ProjectCookieRecord = PROJECTCOOKIEPtr(new PROJECTCOOKIE(stream));

		for (_UINT32 i = 0; i < nCount; i++)
		{
			modules.push_back(boost::make_shared<MODULE>(stream));
		}
	}
//----------------------------------------------------------------------------------
	BaseRecordPtr PROJECTINFORMATION::clone()
	{
		return BaseRecordPtr(new PROJECTINFORMATION(*this));
	}
	void PROJECTINFORMATION::load(CVbaFileStreamPtr stream)
	{
		while (stream->checkFitRead(6))
		{
			_UINT16 Id;
			*stream >> Id;

			if (Id == 0x0016)
			{//next union
				stream->RollBack(2);
				break;
			}

			switch (Id)
			{
			case rt_PROJECTSYSKIND:
			{
				SysKindRecord = boost::make_shared<PROJECTSYSKIND>(stream);

			}break;
			case rt_PROJECTCODEPAGE:
			{
				CodePageRecord = boost::make_shared<PROJECTCODEPAGE>(stream);
			}break;
			case rt_PROJECTCOMPATVERSION:
			{
				CompatVersionRecord = boost::make_shared<PROJECTCOMPATVERSION>(stream);
			}break;
			case rt_PROJECTLCID:
			{
				LcidRecord = boost::make_shared<PROJECTLCID>(stream);
			}break;
			case rt_PROJECTLCIDINVOKE:
			{
				LcidInvokeRecord = boost::make_shared<PROJECTLCIDINVOKE>(stream);
			}break;
			case rt_PROJECTNAME:
			{
				NameRecord = boost::make_shared<PROJECTNAME>(stream);
			}break;
			case rt_PROJECTDOCSTRING:
			{
				DocStringRecord = boost::make_shared<PROJECTDOCSTRING>(stream);
			}break;
			case rt_PROJECTHELPFILEPATH:
			{
				HelpFilePathRecord = boost::make_shared<PROJECTHELPFILEPATH>(stream);
			}break;
			case rt_PROJECTHELPCONTEXT:
			{
				HelpContextRecord = boost::make_shared<PROJECTHELPCONTEXT>(stream);
			}break;
			case rt_PROJECTLIBFLAGS:
			{
				LibFlagsRecord = boost::make_shared<PROJECTLIBFLAGS>(stream);
			}break;
			case rt_PROJECTVERSION:
			{
				VersionRecord = boost::make_shared<PROJECTVERSION>(stream);
			}break;
			case rt_PROJECTCONSTANTS:
			{
				ConstantsRecord = boost::make_shared<PROJECTCONSTANTS>(stream);
			}break;
			default://unknown .... skip					
			{
				_UINT32 SizeOf;
				*stream >> SizeOf;
				stream->skipBytes(SizeOf);
			}break;
			}
		}

	}
//---------------------------------------------------------------------------------------		

	BaseRecordPtr PROJECTREFERENCES::clone()
	{
		return BaseRecordPtr(new PROJECTREFERENCES(*this));
	}
	void PROJECTREFERENCES::load(CVbaFileStreamPtr stream)
	{
		while (stream->checkFitRead(6))
		{
			_UINT16 Id;
			*stream >> Id;

			if (Id == rt_REFERENCENAME)
			{
				stream->RollBack(2);
				ReferenceArray.push_back(boost::make_shared<REFERENCE>(stream));
			}
			else if (Id == rt_PROJECTMODULES)
			{//next union
				stream->RollBack(2);
				break;
			}
			else
			{
				_UINT32 SizeOf;
				*stream >> SizeOf;
				stream->skipBytes(SizeOf);
			}
		}
	}
//---------------------------------------------------------------------------------------		
	BaseRecordPtr REFERENCE::clone()
	{
		return BaseRecordPtr(new REFERENCE(*this));
	}
	void REFERENCE::load(CVbaFileStreamPtr stream)
	{
		while (stream->checkFitRead(6))
		{
			_UINT16 Id;
			*stream >> Id;

			if (Id == rt_PROJECTMODULES ||
				(NameRecord && Id == rt_REFERENCENAME))
			{//next union
				stream->RollBack(2);
				break;
			}

			switch (Id)
			{
			case rt_REFERENCENAME:			NameRecord = boost::make_shared<REFERENCENAME>(stream); break;
			case rt_REFERENCEORIGINAL:		ReferenceRecord = boost::make_shared<REFERENCEORIGINAL>(stream); break;
			case rt_REFERENCECONTROL:		ReferenceRecord = boost::make_shared<REFERENCECONTROL>(stream); break;
			case rt_REFERENCEREGISTERED:	ReferenceRecord = boost::make_shared<REFERENCEREGISTERED>(stream); break;
			case rt_REFERENCEPROJECT:		ReferenceRecord = boost::make_shared<REFERENCEPROJECT>(stream); break;
			default://unknown .... skip					
			{
				_UINT32 SizeOf;
				*stream >> SizeOf;
				stream->skipBytes(SizeOf);
			}break;
			}
		}
	}
//-------------------------------------------------------------------------------
	//PROJECTMODULES::PROJECTMODULES() {}
	//PROJECTMODULES::~PROJECTMODULES() {}
	//BaseObjectPtr PROJECTMODULES::clone()
	//{
	//	return BaseObjectPtr(new PROJECTMODULES(*this));
	//}
	//const bool PROJECTMODULES::loadContent(BinProcessor& proc)
	//{
	//	BiffAttributeSimple<unsigned short> nCount;

	//	proc.nCount;

	//	if (proc.optional<PROJECTCOOKIE>())
	//	{
	//		ProjectCookieRecord = boost::make_shared<>(stream);
	//		elements_.pop_back();
	//	}

	//	for (unsigned short i = 0; i < nCount; i++)
	//	{
	//		MODULE *m = new MODULE();
	//		m->loadContent(proc);
	//		modules.push_back(BaseObjectPtr(m));
	//	}
	//}
	BaseRecordPtr FormControl::clone()
	{
		return BaseRecordPtr(new FormControl(*this));
	}
	void FormControl::load(CVbaFileStreamPtr stream)
	{
		_UINT32 flag;
		*stream >> flag;
		FormPropMask propMask(flag);

		if (propMask.fBackColor) *stream >> BackColor;
		if (propMask.fForeColor) *stream >> ForeColor;
		if (propMask.fNextAvailableID) *stream >> NextAvailableID;

		if (propMask.fBooleanProperties)
		{
			*stream >> flag;
			BooleanProperties = _BooleanProperties();
			BooleanProperties->FORM_FLAG_ENABLED = GETBIT(flag, 2);
			BooleanProperties->FORM_FLAG_DESINKPERSISTED = GETBIT(flag, 14);
			BooleanProperties->FORM_FLAG_DONTSAVECLASSTABLE = GETBIT(flag, 15);
		}
		if (propMask.fBorderStyle) *stream >> BorderStyle;
		if (propMask.fMousePointer) *stream >> MousePointer;
		if (propMask.fScrollBars) *stream >> ScrollBars;
		if (propMask.fGroupCnt) *stream >> GroupCnt;
		if (propMask.fMouseIcon)
		{
			_UINT16 MouseIcon;
			*stream >> MouseIcon; // == 0xFFFF
		}
		if (propMask.fCycle) *stream >> Cycle;
		if (propMask.fSpecialEffect) *stream >> SpecialEffect;
		if (propMask.fBorderColor)
		{
			stream->Align(4);
			*stream >> BorderColor;
		}

		_UINT32 LengthAndCompression = 0;
		if (propMask.fCaption)
		{
			stream->Align(4);
			*stream >> LengthAndCompression;
		}
		if (propMask.fFont)
		{
			_UINT16 Font;
			*stream >> Font; // == 0xFFFF
		}		
		if (propMask.fPicture)
		{
			_UINT16 Picture;
			*stream >> Picture; // == 0xFFFF
		}
		stream->Align(4);
		if (propMask.fZoom) *stream >> Zoom;
		if (propMask.fPictureAlignment) *stream >> PictureAlignment;
		if (propMask.fPictureSizeMode) *stream >> PictureSizeMode;
		if (propMask.fShapeCookie) *stream >> ShapeCookie;
		if (propMask.fDrawBuffer) *stream >> DrawBuffer;
//- FormExtraDataBlock		
		if (propMask.fDisplayedSize)
		{
			*stream >> DisplayedSize;
		}
		if (propMask.fLogicalSize)
		{
			*stream >> LogicalSize;
		}
		if (propMask.fScrollPosition)
		{
			*stream >> ScrollPosition;
		}
		if (propMask.fCaption && LengthAndCompression > 0)
		{
			Caption = readStringPadding(stream, LengthAndCompression);
		}
//- FormStreamData
		if (propMask.fMouseIcon)
		{
			*stream >> MouseIconGUID;
			MouseIcon = readStdPicture(stream);
		}

		if (propMask.fFont)
		{
			*stream >> FontGUID;
			if (FontGUID->Data1 == 0x0BE35203 && FontGUID->Data2 == 0x8F91 && FontGUID->Data3 == 0x11CE)
				Font = BaseRecordPtr(new StdFont(stream));
			else if (FontGUID->Data1 == 0xAFC20920 && FontGUID->Data2 == 0xDA4E && FontGUID->Data3 == 0x11CE)
				Font = BaseRecordPtr(new TextProps(stream));
		}		
		if (propMask.fPicture)
		{
			*stream >> PictureGUID;
			Picture = readStdPicture(stream);
		}
	}
//------------------------------------------------------------------------------------------
	SiteClassInfo::SiteClassInfo(CVbaFileStreamPtr stream) { load(stream); }
	BaseRecordPtr SiteClassInfo::clone()
	{
		return BaseRecordPtr(new SiteClassInfo(*this));
	}
	void SiteClassInfo::load(CVbaFileStreamPtr stream)
	{
		_UINT16 Version, cbClassTable;
		_UINT32 flag;
		*stream >> Version >> cbClassTable >> flag;

		ClassInfoPropMask propMask(flag);
//ClassInfoDataBlock
		_UINT16 flag2 = 0, flag3 = 0, GetBindIndex = 0, PutBindIndex = 0, BindType = 0, GetValueIndex = 0, PutValueIndex = 0, ValueType = 0, SetRowset = 0;
		_UINT32 CountOfMethods = 0, DispidBind = 0xFFFFFFFF, DispidRowset = 0xFFFFFFFF;

		if (propMask.fClassFlags)
		{
			*stream >> flag2 >> flag3;
			//ClassTableFlags tableFlags(flag2);
			//VarFlags varFlags(flag3);
		}
		if (propMask.fCountOfMethods)
		{
			*stream >> CountOfMethods;
		}
		if (propMask.fDispidBind)
		{
			*stream >> DispidBind;
		}
		int count_padding = 0;
		if (propMask.fGetBindIndex)
		{
			*stream >> GetBindIndex;
			count_padding += 2;
		}
		if (propMask.fPutBindIndex)
		{
			*stream >> PutBindIndex;
			count_padding += 2;
		}
		if (propMask.fBindType)
		{
			*stream >> BindType;
			count_padding += 2;
		}
		if (propMask.fGetValueIndex)
		{
			*stream >> GetValueIndex;
			count_padding += 2;
		}
		if (propMask.fPutValueIndex)
		{
			*stream >> PutValueIndex;
			count_padding += 2;
		}
		if (propMask.fValueType)
		{
			*stream >> ValueType;
			count_padding += 2;
		}
		count_padding = 4 - (count_padding % 4);

		if (count_padding > 0 && count_padding < 4)
			stream->skipBytes(count_padding);
		
		if (propMask.fDispidRowset)
		{
			*stream >> DispidRowset;
		}
		count_padding = 4;
		if (propMask.fSetRowset)
		{
			*stream >> SetRowset;
			count_padding -= 2;
		}
		if (count_padding > 0 && count_padding < 4)
			stream->skipBytes(count_padding);
		
//ClassInfoExtraDataBlock
		if (propMask.fClsID)
		{
			*stream >> guidClsID;
		}
		if (propMask.fDispEvent)
		{
			*stream >> guidDispEvent;
		}
		if (propMask.fDefaultProg)
		{
			*stream >> guidDefaultProg;
		}
	}
//------------------------------------------------------------------------------------------
	FormObjectDepthTypeCount::FormObjectDepthTypeCount(CVbaFileStreamPtr stream) { load(stream); }
	BaseRecordPtr FormObjectDepthTypeCount::clone()
	{
		return BaseRecordPtr(new FormObjectDepthTypeCount(*this));
	}
	void FormObjectDepthTypeCount::load(CVbaFileStreamPtr stream)
	{
		unsigned char flag;
		*stream >> Depth >> flag;
		
		TypeOrCount = GETBITS(flag, 0, 6);
		fCount = GETBIT(flag, 7);
	}
//------------------------------------------------------------------------------------------
	OleSiteConcreteControl::OleSiteConcreteControl(CVbaFileStreamPtr stream) { load(stream); }
	BaseRecordPtr OleSiteConcreteControl::clone()
	{
		return BaseRecordPtr(new OleSiteConcreteControl(*this));
	}
	void OleSiteConcreteControl::load(CVbaFileStreamPtr stream)
	{
		_UINT16 Version, cbSite;
		_UINT32 flag;
		*stream >> Version >> cbSite >> flag;

		SitePropMask propMask(flag);
//SiteDataBlock

		_UINT32 NameLengthAndCompression = 0, TagLengthAndCompression = 0, ControlTipTextLengthAndCompression = 0,
			RuntimeLicKeyLengthAndCompression = 0, ControlSourceLengthAndCompression = 0, RowSourceLengthAndCompression = 0;
		if (propMask.fName)
		{
			*stream >> NameLengthAndCompression;
		}
		if (propMask.fTag)
		{
			*stream >> TagLengthAndCompression;
		}
		if (propMask.fID)
		{
			*stream >> ID;
		}
		if (propMask.fHelpContextID)
		{
			*stream >> HelpContextID;
		}
		if (propMask.fBitFlags)
		{
			*stream >> flag;
			BitFlags = SITE_FLAG(flag);
		}
		if (propMask.fObjectStreamSize)
		{
			*stream >> ObjectStreamSize;
		}
		_UINT32 pos1 = stream->GetDataPos();
		if (propMask.fTabIndex)
		{
			*stream >> TabIndex;
		}
		if (propMask.fClsidCacheIndex)
		{
			*stream >> ClsidCacheIndex;
		}
		if (propMask.fGroupID)
		{
			*stream >> GroupID;
		}
		_UINT32 pos2 = stream->GetDataPos();
		int count_padding = 4 - ((pos2 - pos1) % 4);

		if (count_padding > 0 && count_padding < 4)
			stream->skipBytes(count_padding);

		if (propMask.fControlTipText)
		{
			*stream >> ControlTipTextLengthAndCompression;
		}
		if (propMask.fRuntimeLicKey)
		{
			*stream >> RuntimeLicKeyLengthAndCompression;
		}
		if (propMask.fControlSource)
		{
			*stream >> ControlSourceLengthAndCompression;
		}
		if (propMask.fRowSource)
		{
			*stream >> RowSourceLengthAndCompression;
		}
//SiteExtraDataBlock
		if (propMask.fName && NameLengthAndCompression > 0)
		{
			Name = readStringPadding(stream, NameLengthAndCompression);
		}
		if (propMask.fTag && TagLengthAndCompression > 0)
		{
			Tag = readStringPadding(stream, TagLengthAndCompression);
		}
		if (propMask.fPosition)
		{
			*stream >> SitePosition;
		}
		if (propMask.fControlTipText && ControlTipTextLengthAndCompression > 0)
		{
			ControlTipText = readStringPadding(stream, ControlTipTextLengthAndCompression);
		}
		if (propMask.fRuntimeLicKey && RuntimeLicKeyLengthAndCompression > 0)
		{
			RuntimeLicKey = readStringPadding(stream, RuntimeLicKeyLengthAndCompression);
		}
		if (propMask.fControlSource && ControlSourceLengthAndCompression > 0)
		{
			ControlSource = readStringPadding(stream, ControlSourceLengthAndCompression);
		}
		if (propMask.fRowSource && RowSourceLengthAndCompression > 0)
		{
			RowSource = readStringPadding(stream, RowSourceLengthAndCompression);
		}
	}
//------------------------------------------------------------------------------------------
	FormSiteData::FormSiteData(CVbaFileStreamPtr stream) { load(stream); }
	FormSiteData::FormSiteData(CVbaFileStreamPtr stream, bool _bClassTableEnable) { bClassTableEnable = _bClassTableEnable; load(stream); }
	BaseRecordPtr FormSiteData::clone()
	{
		return BaseRecordPtr(new FormSiteData(*this));
	}
	void FormSiteData::load(CVbaFileStreamPtr stream)
	{
		if (bClassTableEnable)
		{
			_UINT16 CountOfSiteClassInfo = 0;
			*stream >> CountOfSiteClassInfo;

			for (_UINT16 i = 0; i < CountOfSiteClassInfo; ++i)
			{
				ClassTables.push_back(SiteClassInfoPtr(new SiteClassInfo(stream)));
			}
		}		
		_UINT32 CountOfSites = 0, CountOfBytes = 0;
		*stream >> CountOfSites >> CountOfBytes;		
		
		_UINT32 pos1 = stream->GetDataPos();

		int countSites = 0;
		for (_UINT16 i = 0; i < CountOfSites; ++i)
		{
			FormObjectDepthTypeCountPtr ptr = FormObjectDepthTypeCountPtr(new FormObjectDepthTypeCount(stream));
			SiteDepthsAndTypes.push_back(ptr);

			countSites += ptr->fCount ? ptr->TypeOrCount : 1;

			if (countSites >= CountOfSites)
				break;
		}
		_UINT32 pos2 = stream->GetDataPos();

		int count_padding = 4 - ((pos2 - pos1) % 4);
		if (count_padding > 0 && count_padding < 4)
			stream->skipBytes(count_padding);

		for (_UINT16 i = 0; i < CountOfSites; ++i)
		{
			Sites.push_back(OleSiteConcreteControlPtr(new OleSiteConcreteControl(stream)));
		}
	}
//------------------------------------------------------------------------------------------
	FormDesignExData::FormDesignExData(CVbaFileStreamPtr stream) { load(stream); }
	BaseRecordPtr FormDesignExData::clone()
	{
		return BaseRecordPtr(new FormDesignExData(*this));
	}
	void FormDesignExData::load(CVbaFileStreamPtr stream)
	{
		unsigned char MinorVersion, MajorVersion;
		_UINT16 cbDesignExtender;
		_UINT32 flag;

		*stream >> MinorVersion >> MajorVersion >> cbDesignExtender >> flag;
		
		DesignExtenderPropMask propMask(flag);
	//DesignExtenderPropMask
		if (propMask.fBitFlags)
		{
			*stream >> flag;
			BitFlags = DX_MODE(flag);
		}
		if (propMask.fGridX)
		{
			*stream >> GridX;
		}
		if (propMask.fGridY)
		{
			*stream >> GridY;
		}
		if (propMask.fClickControlMode)
		{
			unsigned char flag2; 
			*stream >> flag2;
			ClickControlMode = (fmClickControlMode)flag2;
		}
		if (propMask.fDblClickControlMode)
		{
			unsigned char flag2;
			*stream >> flag2;
			DblClickControlMode = (fmClickControlMode)flag2;
		}
		stream->Align(4);
	}

}// namespace VBA

