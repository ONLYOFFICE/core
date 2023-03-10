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
		std::list<ITableCellElementPtr> cellElements;

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
		std::list<TableCell> cells;

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
		std::list<TableRow> rows;

		DocumentMapping* documentMapping;
	};
}
