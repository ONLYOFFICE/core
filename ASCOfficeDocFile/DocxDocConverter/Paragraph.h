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

#include "ParagraphItem.h"
#include "TextItem.h"

namespace ASCDocFileFormat
{
	class Paragraph : public ITextItem
	{
		friend class TableCell;

	private:
		list<ParagraphItem> paragraphItems;
		unsigned long runsOffset;
		short style;
		list<Prl> properties;

		unsigned int depth;

	protected:
		void SetDepth( unsigned int _depth );
		unsigned int GetDepth() const;

	public:
		typedef list<ParagraphItem>::const_iterator const_iterator;
		typedef list<ParagraphItem>::iterator iterator;
		typedef list<ParagraphItem>::const_reverse_iterator const_reverse_iterator;
		typedef list<ParagraphItem>::reverse_iterator reverse_iterator;
		typedef list<ParagraphItem>::const_reference const_reference;
		typedef list<ParagraphItem>::reference reference;

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
		Paragraph( short _style = 0 );
		void AddParagraphItem( const IParagraphItem& _paragraphItem );
		unsigned long GetTextSize() const;
		virtual ~Paragraph();
		virtual wstring GetAllText() const;
		virtual operator wstring() const;
		virtual vector<Chpx> GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const;
		virtual vector<PapxInFkp> GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const;
		virtual vector<TextItemPtr> GetAllParagraphsCopy() const;
		virtual vector<ITextItem*> GetAllParagraphs();
		virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const;
		virtual vector<IParagraphItemPtr> GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const;
		virtual IVirtualConstructor* New() const;
		virtual IVirtualConstructor* Clone() const;
		void AddProperty( short sprm, void* operand );
		void AddProperty( const Prl& prl );
		void AddProperties( const vector<Prl>& prls );
		void AddOrReplaceProperties( const vector<Prl>& prls );
		void RemoveProperty( short sprm, void* operand );
		void RemovePropertyByCode( unsigned short sprm );
		void RemoveAllProperties();
		void SetStyle( short _style );
		short GetStyle() const;
		PapxInFkp GetProperties() const;
		unsigned int GetParagraphPropertiesCount() const;
	};
}