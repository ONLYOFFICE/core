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

#include "VbaRecordType.h"
#include "VbaBinary.h"


namespace VBA
{
	const std::wstring convert_string_icu(const char* buffer, const unsigned int& size, _UINT32 nCodePage);
	const std::wstring  guid2bstr(const _GUID_ guid);

	class BaseRecord;
	typedef boost::shared_ptr<BaseRecord> BaseRecordPtr;
	
	class BaseRecord
	{
	public:
		virtual BaseRecordPtr clone() = 0;
		virtual void load(CVbaFileStreamPtr stream) = 0;
		virtual RecordType get_type() = 0;
		virtual const std::string & getClassName() const = 0;
	};

#define BASE_STRUCTURE_DEFINE_CLASS_NAME(class_name)\
	private: \
	static const RecordType type = rt_##class_name; \
	public: \
        const std::string & getClassName() const { static std::string  str(#class_name); return str;}\
	virtual RecordType get_type() { return type; }

//--------------------------------------------------------------------------------------------------
	class AnsiString : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(AnsiString)
	public:
		AnsiString() {}
		AnsiString(CVbaFileStreamPtr stream);
		~AnsiString() {}

		BaseRecordPtr clone();
		virtual void load(CVbaFileStreamPtr stream);

		std::wstring value;
	};
	typedef boost::shared_ptr<AnsiString> AnsiStringPtr;
	
//--------------------------------------------------------------------------------------------------
	class Utf16String : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(Utf16String)
	public:
		Utf16String() {}
		Utf16String(CVbaFileStreamPtr stream);
		~Utf16String() {}

		BaseRecordPtr clone();
		virtual void load(CVbaFileStreamPtr stream);

		std::wstring value;
	};
	typedef boost::shared_ptr<Utf16String> Utf16StringPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTSYSKIND : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULESTREAMNAME)
	public:
		PROJECTSYSKIND(CVbaFileStreamPtr stream);
		~PROJECTSYSKIND() {}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		_UINT32 SysKind;
	};
	typedef boost::shared_ptr<PROJECTSYSKIND> PROJECTSYSKINDPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTCOMPATVERSION : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTCOMPATVERSION)
	public:
		PROJECTCOMPATVERSION(CVbaFileStreamPtr stream);
		~PROJECTCOMPATVERSION();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		_UINT32 CompatVersion;
	};
	typedef boost::shared_ptr<PROJECTCOMPATVERSION> PROJECTCOMPATVERSIONPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTLCID : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTLCID)
	public:
		PROJECTLCID(CVbaFileStreamPtr stream);
		~PROJECTLCID();
		BaseRecordPtr clone();


		virtual void load(CVbaFileStreamPtr stream);

		_UINT32 Lcid;
	};
	typedef boost::shared_ptr<PROJECTLCID> PROJECTLCIDPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTLCIDINVOKE : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTLCIDINVOKE)
	public:
		PROJECTLCIDINVOKE(CVbaFileStreamPtr stream);
		~PROJECTLCIDINVOKE();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		_UINT32 LcidInvoke;
	};
	typedef boost::shared_ptr<PROJECTLCIDINVOKE> PROJECTLCIDINVOKEPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTCODEPAGE : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTCODEPAGE)
	public:
		PROJECTCODEPAGE(CVbaFileStreamPtr stream);
		~PROJECTCODEPAGE();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		unsigned short CodePage;
	};
	typedef boost::shared_ptr<PROJECTCODEPAGE> PROJECTCODEPAGEPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTNAME : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTNAME)
	public:
		PROJECTNAME(CVbaFileStreamPtr stream);
		~PROJECTNAME();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		AnsiString ProjectName;
	};
	typedef boost::shared_ptr<PROJECTNAME> PROJECTNAMEPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTDOCSTRING : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTDOCSTRING)
	public:
		PROJECTDOCSTRING(CVbaFileStreamPtr stream);
		~PROJECTDOCSTRING();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		AnsiString aDocString;
		Utf16String uDocString;
	};
	typedef boost::shared_ptr<PROJECTDOCSTRING> PROJECTDOCSTRINGPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTHELPFILEPATH : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTHELPFILEPATH)
	public:
		PROJECTHELPFILEPATH(CVbaFileStreamPtr stream);
		~PROJECTHELPFILEPATH();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		Utf16String HelpFile1;
		Utf16String HelpFile2;
	};
	typedef boost::shared_ptr<PROJECTHELPFILEPATH> PROJECTHELPFILEPATHPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTHELPCONTEXT : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTHELPCONTEXT)
	public:
		PROJECTHELPCONTEXT(CVbaFileStreamPtr stream);
		~PROJECTHELPCONTEXT();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		_UINT32 HelpContext;
	};
	typedef boost::shared_ptr<PROJECTHELPCONTEXT> PROJECTHELPCONTEXTPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTLIBFLAGS : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTLIBFLAGS)
	public:
		PROJECTLIBFLAGS(CVbaFileStreamPtr stream);
		~PROJECTLIBFLAGS();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		_UINT32 ProjectLibFlags;
	};
	typedef boost::shared_ptr<PROJECTLIBFLAGS> PROJECTLIBFLAGSPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTVERSION : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTVERSION)
	public:
		PROJECTVERSION(CVbaFileStreamPtr stream);
		~PROJECTVERSION();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		_UINT32 VersionMajor;
		_UINT16 VersionMinor;
	};
	typedef boost::shared_ptr<PROJECTVERSION> PROJECTVERSIONPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTCONSTANTS : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTCONSTANTS)
	public:
		PROJECTCONSTANTS(CVbaFileStreamPtr stream);
		~PROJECTCONSTANTS();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		AnsiString aConstants;
		Utf16String uConstants;
	};
	typedef boost::shared_ptr<PROJECTCONSTANTS> PROJECTCONSTANTSPtr;
//--------------------------------------------------------------------------------------------------
	class REFERENCENAME : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(REFERENCENAME)
	public:
		REFERENCENAME(CVbaFileStreamPtr stream);
		~REFERENCENAME();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		AnsiString aName;
		Utf16String uName;
	};
	typedef boost::shared_ptr<REFERENCENAME> REFERENCENAMEPtr;
//--------------------------------------------------------------------------------------------------
	class REFERENCEORIGINAL : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(REFERENCEORIGINAL)
	public:
		REFERENCEORIGINAL(CVbaFileStreamPtr stream);
		~REFERENCEORIGINAL();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		AnsiString LibidOriginal;
	};
	typedef boost::shared_ptr<REFERENCEORIGINAL> REFERENCEORIGINALPtr;
//--------------------------------------------------------------------------------------------------
	class REFERENCEREGISTERED : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(REFERENCEREGISTERED)
	public:
		REFERENCEREGISTERED(CVbaFileStreamPtr stream);
		~REFERENCEREGISTERED();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		AnsiString Libid;
	};
	typedef boost::shared_ptr<REFERENCEREGISTERED> REFERENCEREGISTEREDPtr;
//--------------------------------------------------------------------------------------------------
	class REFERENCEPROJECT : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(REFERENCEPROJECT)
	public:
		REFERENCEPROJECT(CVbaFileStreamPtr stream);
		~REFERENCEPROJECT();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		_UINT32 MajorVersion;
		_UINT16 MinorVersion;

		AnsiString LibidAbsolute;
		AnsiString LibidRelative;
	};
	typedef boost::shared_ptr<REFERENCEPROJECT> REFERENCEPROJECTPtr;
//--------------------------------------------------------------------------------------------------
	class REFERENCECONTROL : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(REFERENCECONTROL)
	public:
		REFERENCECONTROL(CVbaFileStreamPtr stream);
		~REFERENCECONTROL();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);
		
		AnsiString LibidTwiddled;
		AnsiString LibidExtended;

		REFERENCENAMEPtr NameRecordExtended;
		_GUID_ OriginalTypeLib;
	};
	typedef boost::shared_ptr<REFERENCECONTROL> REFERENCECONTROLPtr;
//--------------------------------------------------------------------------------------------------
	class PROJECTCOOKIE : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTCOOKIE)
	public:
		PROJECTCOOKIE(CVbaFileStreamPtr stream);
		~PROJECTCOOKIE();
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		unsigned short Cookie;
	};
	typedef boost::shared_ptr<PROJECTCOOKIE> PROJECTCOOKIEPtr;
//-------------------------------------------------------------------------------------------------
	class MODULENAME : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULENAME)
	public:
		MODULENAME(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		AnsiString ModuleName;
	};
	typedef boost::shared_ptr<MODULENAME> MODULENAMEPtr;
//-----------------------------------------------------------------------------
	class MODULENAMEUNICODE : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULENAMEUNICODE)
	public:
		MODULENAMEUNICODE(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		Utf16String ModuleNameUnicode;
	};
	typedef boost::shared_ptr<MODULENAMEUNICODE> MODULENAMEUNICODEPtr;
//-----------------------------------------------------------------------------
	class MODULESTREAMNAME : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULESTREAMNAME)
	public:
		MODULESTREAMNAME(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		AnsiString StreamName;
		Utf16String StreamNameUnicode;
	};
	typedef boost::shared_ptr<MODULESTREAMNAME> MODULESTREAMNAMEPtr;
//-----------------------------------------------------------------------------
	class MODULEDOCSTRING : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULEDOCSTRING)
	public:
		MODULEDOCSTRING(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		AnsiString DocString;
		Utf16String DocStringUnicode;
	};
	typedef boost::shared_ptr<MODULEDOCSTRING> MODULEDOCSTRINGPtr;
//-----------------------------------------------------------------------------
	class MODULEOFFSET : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULEOFFSET)
	public:
		MODULEOFFSET(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		_UINT32 TextOffset = 0xffffffff;
	};
	typedef boost::shared_ptr<MODULEOFFSET> MODULEOFFSETPtr;
//-----------------------------------------------------------------------------
	class MODULEHELPCONTEXT : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULEHELPCONTEXT)
	public:
		MODULEHELPCONTEXT(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		_UINT32 HelpContext;
	};
	typedef boost::shared_ptr<MODULEHELPCONTEXT> MODULEHELPCONTEXTPtr;
//-----------------------------------------------------------------------------
	class MODULECOOKIE : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULECOOKIE)
	public:
		MODULECOOKIE(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);
	};
	typedef boost::shared_ptr<MODULECOOKIE> MODULECOOKIEPtr;
//-----------------------------------------------------------------------------
	class MODULETYPE : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULETYPE)
	public:
		MODULETYPE(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);
	};
	typedef boost::shared_ptr<MODULETYPE> MODULETYPEPtr;
//-----------------------------------------------------------------------------
	class MODULEREADONLY : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULEREADONLY)
	public:
		MODULEREADONLY(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);
	};
	typedef boost::shared_ptr<MODULEREADONLY> MODULEREADONLYPtr;
//-----------------------------------------------------------------------------
	class MODULEPRIVATE : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULEPRIVATE)
	public:
		MODULEPRIVATE(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);
	};
	typedef boost::shared_ptr<MODULEPRIVATE> MODULEPRIVATEPtr;
//------------------------------------------------------------------------------------
	class MODULE : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(MODULE)
	public:
		MODULE(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();
		virtual void load(CVbaFileStreamPtr stream);

		MODULENAMEPtr NameRecord;
		MODULENAMEUNICODEPtr NameUnicodeRecord;
		MODULESTREAMNAMEPtr StreamNameRecord;
		MODULEDOCSTRINGPtr DocStringRecord;
		MODULEOFFSETPtr OffsetRecord;
		MODULEHELPCONTEXTPtr HelpContextRecord;

		bool bReadOnly = false;
		bool bPrivate = false;
		bool bProceduralModule = false;
	};
	typedef boost::shared_ptr<MODULE> MODULEPtr;
//----------------------------------------------------------------------------------------
	class PROJECTINFORMATION : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTINFORMATION)
	public:
		PROJECTINFORMATION(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		PROJECTSYSKINDPtr		SysKindRecord;
		PROJECTCOMPATVERSIONPtr	CompatVersionRecord;
		PROJECTLCIDPtr			LcidRecord;
		PROJECTLCIDINVOKEPtr	LcidInvokeRecord;
		PROJECTCODEPAGEPtr		CodePageRecord;
		PROJECTNAMEPtr			NameRecord;
		PROJECTDOCSTRINGPtr		DocStringRecord;
		PROJECTHELPFILEPATHPtr	HelpFilePathRecord;
		PROJECTHELPCONTEXTPtr	HelpContextRecord;
		PROJECTLIBFLAGSPtr		LibFlagsRecord;
		PROJECTVERSIONPtr		VersionRecord;
		PROJECTCONSTANTSPtr		ConstantsRecord;
	};
	typedef boost::shared_ptr<PROJECTINFORMATION> PROJECTINFORMATIONPtr;
//-----------------------------------------------------------------------------
	class REFERENCE : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(REFERENCE)
	public:
		REFERENCE(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		REFERENCENAMEPtr NameRecord;
		BaseRecordPtr ReferenceRecord;
	};
	typedef boost::shared_ptr<REFERENCE> REFERENCEPtr;
	class PROJECTREFERENCES : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTINFORMATION)
	public:
		PROJECTREFERENCES(CVbaFileStreamPtr stream)
		{
			load(stream);
		}
		BaseRecordPtr clone();

		virtual void load(CVbaFileStreamPtr stream);

		std::vector<REFERENCEPtr>	ReferenceArray;
	};
	typedef boost::shared_ptr<PROJECTREFERENCES> PROJECTREFERENCESPtr;
//----------------------------------------------------------------------------------------------
	class PROJECTMODULES : public BaseRecord
	{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(PROJECTMODULES)
	public:
		PROJECTMODULES(CVbaFileStreamPtr stream);
		~PROJECTMODULES();
		BaseRecordPtr clone();
		virtual void load(CVbaFileStreamPtr stream);

		std::vector<MODULEPtr> modules;
		PROJECTCOOKIEPtr ProjectCookieRecord;
	};
	typedef boost::shared_ptr<PROJECTMODULES> PROJECTMODULESPtr;
} // namespace VBA

