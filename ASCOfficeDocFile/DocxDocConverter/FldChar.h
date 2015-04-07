#pragma once

#include "IRunItem.h"
#include "Constants.h"
#include "Plcfld.h"

namespace ASCDocFileFormat
{
	static const wchar_t* FldCharTypeBegin	=	_T("begin");
	static const wchar_t* FldCharTypeSeparate =	_T("separate");
	static const wchar_t* FldCharTypeEnd		=	_T("end");

	class FldChar : public IRunItem
	{
	public:
		static const unsigned char FldCharBegin			=	0x13;
		static const unsigned char FldCharSeparate		=	0x14;
		static const unsigned char FldCharEnd			=	0x15;
	public:

		FldChar () : fld()
		{
			text		=	wstring(&TextMark::FieldBeginMark);
			properties.push_back(Prl((short)DocFileFormat::sprmCFSpec, (unsigned char*)(&CFSpec)));
		}

		explicit FldChar (const wstring& charType, unsigned char grffld = 0)
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

			properties.push_back (Prl((short)DocFileFormat::sprmCFSpec, (unsigned char*)(&CFSpec)));
		}

		virtual ~FldChar()
		{

		}

		inline const Fld& GetField() const
		{
			return fld;
		}

		inline unsigned char CharType () const
		{
			return fld.GetFieldCharacterType();
		}

		inline void SetFieldCharacterProperties (unsigned char grffld)
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
		static const unsigned char CFSpec = 1;

		wstring		text;
		list<Prl>	properties;
		Fld			fld;
	};
}