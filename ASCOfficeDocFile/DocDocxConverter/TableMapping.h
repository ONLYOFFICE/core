#pragma once

#include "DocumentMapping.h"
#include "../Common/IVirtualConstructor.h"

namespace DocFileFormat
{
	struct ITableCellElement: public IVirtualConstructor, public IVisitable
	{
		virtual int GetCPStart() const = 0;
		virtual int GetCPEnd() const = 0;
		virtual ~ITableCellElement() {}
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
		void Convert( IMapping* mapping, TablePropertyExceptions* tapx, const vector<short>* grid, int& gridIndex, int cellIndex );
		~TableCell();

	private:

		int cp;
		unsigned int depth;
		list<ITableCellElementPtr> cellElements;

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
		void Convert( IMapping* mapping, const vector<short>* grid );
		~TableRow();

	private:

		int cp;
		unsigned int depth;
		list<TableCell> cells;

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

		bool IsCellMarker( int _cp );
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

	private:

		int cpStart;
		int cpEnd;
		unsigned int depth;
		list<TableRow> rows;

		DocumentMapping* documentMapping;
	};
}