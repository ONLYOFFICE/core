/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "DocumentMapping.h"
#include "../Common/Base/IVirtualConstructor.h"

namespace DocFileFormat
{
	struct ITableCellElement: public IVirtualConstructor, public IVisitable
	{
		virtual int GetCPStart() const = 0;
		virtual int GetCPEnd() const = 0;
		virtual ~ITableCellElement() {}

		virtual void AddCP(int _cpStart = 0, int _cpEnd = 0) = 0;
	};

	typedef NSCommon::smart_ptr<ITableCellElement> ITableCellElementPtr;

	class TableCell
	{
	public:
		explicit TableCell( DocumentMapping* _documentMapping, int _cp = 0, unsigned int _depth = 1 );
		TableCell( const TableCell& _tableCell );
		TableCell& operator = ( const TableCell& _tableCell );
		void SetCP( int _cp );
		int GetCP() const;
		void SetDepth( unsigned int _depth );
		unsigned int GetDepth() const;
		void AddItem( const ITableCellElement& _tableCellElement );
		bool IsEmpty() const;
		void Clear();
		void Convert( IMapping* mapping, TablePropertyExceptions* tapx, const std::vector<short>* grid, int& gridIndex, int cellIndex );
		~TableCell();

		ITableCellElementPtr GetLast() { return cellElements.back(); }

	private:

		int cp;
		unsigned int depth;
		std::vector<ITableCellElementPtr> cellElements;

		DocumentMapping* documentMapping;
	};

	class TableRow
	{
	public:
		explicit TableRow( DocumentMapping* _documentMapping, int _cp = 0, unsigned int _depth = 1 );
		void SetCP( int _cp );
		int GetCP() const;
		void SetDepth( unsigned int _depth );
		unsigned int GetDepth() const;
		void AddCell( const TableCell& _tableCell );
		bool IsEmpty() const;
		void Clear();
		void Convert( IMapping* mapping, const std::vector<short>* grid);
		~TableRow();

	private:

		int cp;
		unsigned int depth;
		std::vector<TableCell> cells;

		DocumentMapping* documentMapping;
	};

	class DocParagraph: public ITableCellElement
	{
	public:
		explicit DocParagraph( DocumentMapping* _documentMapping, int _cpStart = 0, int _cpEnd = 0 );
		virtual int GetCPStart() const;
		void SetCPStart( int _cpStart );
		virtual int GetCPEnd() const;
		void SetCPEnd( int _cpEnd );
		virtual IVirtualConstructor* New() const;
		virtual IVirtualConstructor* Clone() const;
		virtual void Convert( IMapping* mapping );
		virtual ~DocParagraph();
		virtual void AddCP(int _cpStart = 0, int _cpEnd = 0);

	private:

		int cpStart;
		int cpEnd;

		DocumentMapping* documentMapping;
	};

	class Table: public ITableCellElement
	{
	private:

		Table();

	protected:

		bool IsCellMarker( int _cp, bool & bBadMarker);
		bool IsRowMarker( int _cp );
		bool IsParagraphMarker( int _cp );

	public:

		explicit Table( DocumentMapping* _documentMapping, int _cp = 0, unsigned int _depth = 1 );
		virtual int GetCPStart() const;
		void SetCPStart( int _cpStart );
		virtual int GetCPEnd() const;
		void SetCPEnd( int _cpEnd );
		bool IsEmpty() const;
		void Clear();
		void SetDepth( unsigned int _depth );
		unsigned int GetDepth() const;
		int AddRow( const TableRow& _tableRow );
		virtual IVirtualConstructor* New() const;
		virtual IVirtualConstructor* Clone() const;
		virtual void Convert( IMapping* mapping );
		virtual ~Table();
		virtual void AddCP(int _cpStart = 0, int _cpEnd = 0);

	private:

		int cpStart;
		int cpEnd;
		unsigned int depth;
		std::vector<TableRow> rows;

		DocumentMapping* documentMapping;
	};
}
