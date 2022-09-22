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
#include <boost/make_shared.hpp>
#include <map>
#include <locale>
#include "../../../UnicodeConverter/UnicodeConverter.h"

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
	const std::wstring  guid2bstr(const _GUID_ guid)
	{
		std::wstring  guid_ret = L"{";

		guid_ret += int2hex_wstr(guid.Data1, 4) + L"-" +
			int2hex_wstr(guid.Data2, 2) + L"-" +
			int2hex_wstr(guid.Data3, 2) + L"-" +
			int2hex_wstr(guid.Data4[0], 1) + int2hex_wstr(guid.Data4[1], 1) + L"-" +
			int2hex_wstr(guid.Data4[2], 1) + int2hex_wstr(guid.Data4[3], 1) +
			int2hex_wstr(guid.Data4[4], 1) + int2hex_wstr(guid.Data4[5], 1) +
			int2hex_wstr(guid.Data4[6], 1) + int2hex_wstr(guid.Data4[7], 1);
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
//---------------------------------------------------------------------------------------		

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
} // namespace VBA

