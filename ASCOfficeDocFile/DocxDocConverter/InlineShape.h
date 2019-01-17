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

#include "Run.h"
#include "ParagraphItem.h"
#include "CP.h"

#include "ShapeBuilder.h"
/*

http://support.microsoft.com/kb/276862

TODO : надо разобраться каким образом идет запись инлайн объектов

*/

namespace ASCDocFileFormat
{
	class InlineShape : public IParagraphItem
	{
	public:
		typedef list<ParagraphItem>::const_iterator const_iterator;
		typedef list<ParagraphItem>::iterator iterator;
		typedef list<ParagraphItem>::const_reverse_iterator const_reverse_iterator;
		typedef list<ParagraphItem>::reverse_iterator reverse_iterator;
		typedef list<ParagraphItem>::const_reference const_reference;
		typedef list<ParagraphItem>::reference reference;

	public:
	
		InlineShape();
		InlineShape (const vector<Run>& oRuns);		
		virtual ~InlineShape();

	public:

		void AddRun(const Run& oRun);
		void SetShape(const ASCDocFileFormat::CShapeRun& oShape);
		void SetPicture(const ASCDocFileFormat::Picture& oPicture);

		void RemoveAllRuns ();
		void BuildRuns() const;

		virtual unsigned long GetTextSize() const;
		virtual std::wstring GetAllText() const;
		virtual vector<Chpx> GetRunProperties( vector<unsigned int>* runOffsets ) const;
		virtual unsigned int PrlSize() const;
		virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* runOffsets ) const;
		virtual IVirtualConstructor* New() const;
		virtual IVirtualConstructor* Clone() const;
		std::vector<CP> GetFieldCharactersPositions() const;
		std::vector<Fld> GetFieldCharactersProperties() const;
		std::wstring GetInnerText() const;
	
	public:

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

	private:
	
		std::list<ParagraphItem>			m_oTextRuns;
		ASCDocFileFormat::CShapeRun			m_oShape;
		ASCDocFileFormat::Picture			m_oPicture;
		mutable std::vector<ParagraphItem>	m_oSpecialRuns;
	};
}