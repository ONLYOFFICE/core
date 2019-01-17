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

#include "IParagraphItem.h"
#include "RunItem.h"
#include "Text.h"
#include "Picture.h"
#include "EndnoteRef.h"
#include "EndnoteReference.h"
#include "FootnoteRef.h"
#include "FootnoteReference.h"
#include "FldChar.h"

namespace ASCDocFileFormat
{
	class Run : public IParagraphItem
	{
	protected:
		std::list<RunItem> items;
		std::list<Prl> properties;

	public:
		typedef list<RunItem>::const_iterator const_iterator;
		typedef list<RunItem>::iterator iterator;
		typedef list<RunItem>::const_reverse_iterator const_reverse_iterator;
		typedef list<RunItem>::reverse_iterator reverse_iterator;
		typedef list<RunItem>::const_reference const_reference;
		typedef list<RunItem>::reference reference;

		const_iterator begin() const;
		const_iterator end() const;
		iterator begin();
		iterator end();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;
		reverse_iterator rbegin();
		reverse_iterator rend();
		const_reference front() const;
		const_reference back() const;
		reference front();
		reference back();

	public:
		Run();
		Run( const IRunItem& _runItem );
		Run( const Run& _run );
		virtual ~Run();
		Run& operator = ( const Run& _run );
		void AddRunItem( const IRunItem& _runItem );
		virtual unsigned long GetTextSize() const;
		virtual wstring GetAllText() const;
		virtual vector<Chpx> GetRunProperties( vector<unsigned int>* runOffsets ) const;
		virtual unsigned int PrlSize() const;
		virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* runOffsets ) const;
		virtual IVirtualConstructor* New() const;
		virtual IVirtualConstructor* Clone() const;
		void AddProperty( short sprm, void* operand );
		void AddProperty( const Prl& prl );
		void AddProperties( const vector<Prl>& prls );
		void AddOrReplaceProperties( const vector<Prl>& prls );
		void RemoveProperty( short sprm, void* operand );
		void RemovePropertyByCode( unsigned short sprm );
		void RemoveAllProperties();
	};
}