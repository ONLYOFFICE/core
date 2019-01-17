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