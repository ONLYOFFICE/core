
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "CoveredTableCell.h"


namespace Odt
{
	namespace Content
	{

		CoveredTableCell::CoveredTableCell()
			: CountHMerge(1)
		{
		}


		CoveredTableCell::~CoveredTableCell()
		{
		}


		CoveredTableCell::CoveredTableCell(const XML::XNode& node)
		{
			fromXML(node);
		}


		const CoveredTableCell& CoveredTableCell::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void CoveredTableCell::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			CountHMerge = 1;
		}


		const XML::XNode CoveredTableCell::toXML() const
		{
			XML::XContainer container;
			XML::XElement element = XML::XElement(ns.table + "covered-table-cell");
			for(int i = 1; i < CountHMerge; ++i)
				container.Add(element);
			return container;
		}


		const bool CoveredTableCell::hasNote() const
		{
			return false;
		}


		const bool CoveredTableCell::hasList() const
		{
			return false;
		}
	

		void CoveredTableCell::setNumCol(const int num)
		{
			m_numCol = num;
		}


		const int CoveredTableCell::getNumCol() const
		{
			return m_numCol;
		}

	} // namespace Content
} // namespace Odt