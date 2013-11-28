#pragma once

#include "IRunItem.h"
#include "Constants.h"
#include "Plcfld.h"

namespace AVSDocFileFormat
{
	static const WCHAR* FldCharTypeBegin	=	_T("begin");
	static const WCHAR* FldCharTypeSeparate =	_T("separate");
	static const WCHAR* FldCharTypeEnd		=	_T("end");

	class FldChar : public IRunItem
	{
	public:
		static const byte FldCharBegin			=	0x13;
		static const byte FldCharSeparate		=	0x14;
		static const byte FldCharEnd			=	0x15;
	public:

		FldChar () : fld()
		{
			text		=	wstring(&TextMark::FieldBeginMark);
			properties.push_back(Prl((short)DocFileFormat::sprmCFSpec, (byte*)(&CFSpec)));
		}

		explicit FldChar (const wstring& charType, byte grffld = 0)
		{
			if (charType == wstring (FldCharTypeBegin))
			{
				text	=	wstring (&TextMark::FieldBeginMark); 
				fld		=	Fld (FldCharBegin, grffld); 
			}

			if (charType == wstring (FldCharTypeSeparate))
			{
				text	=	wstring (&TextMark::FieldSeparator);
				fld		=	Fld (FldCharSeparate, grffld);
			}

			if (charType == wstring (FldCharTypeEnd))
			{
				text	=	wstring(&TextMark::FieldEndMark);
				fld		=	Fld (FldCharEnd, grffld);
			}

			properties.push_back (Prl((short)DocFileFormat::sprmCFSpec, (byte*)(&CFSpec)));
		}

		virtual ~FldChar()
		{

		}

		inline const Fld& GetField() const
		{
			return fld;
		}

		inline byte CharType () const
		{
			return fld.GetFieldCharacterType();
		}

		inline void SetFieldCharacterProperties (byte grffld)
		{
			fld.SetProperties(grffld);
		}

		// IRunItem

		virtual IVirtualConstructor* New() const
		{
			return new FldChar();
		}

		virtual IVirtualConstructor* Clone() const
		{
			return new FldChar(*this);
		}

		virtual unsigned long GetTextSize() const
		{
			return text.size();
		}

		virtual wstring GetAllText() const
		{
			return text;
		}

		virtual vector<Prl> GetRunProperties() const
		{
			vector<Prl> prls;

			for (list<Prl>::const_iterator iter = properties.begin(); iter != this->properties.end(); ++iter)
				prls.push_back( *iter );

			return prls;
		}

		virtual unsigned int PrlSize () const
		{
			return (unsigned int)properties.size();
		}

	private:
		static const byte CFSpec = 1;

		wstring		text;
		list<Prl>	properties;
		Fld			fld;
	};
}