#pragma once

#include "Run.h"
#include "ParagraphItem.h"
#include "CP.h"

#include "ShapeBuilder.h"
/*

http://support.microsoft.com/kb/276862

TODO : надо разобраться каким образом идет запись инлайн объектов

*/

namespace AVSDocFileFormat
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
		void SetShape(const AVSDocFileFormat::CShapeRun& oShape);
		void SetPicture(const AVSDocFileFormat::Picture& oPicture);

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
		AVSDocFileFormat::CShapeRun			m_oShape;
		AVSDocFileFormat::Picture			m_oPicture;
		mutable std::vector<ParagraphItem>	m_oSpecialRuns;
	};
}