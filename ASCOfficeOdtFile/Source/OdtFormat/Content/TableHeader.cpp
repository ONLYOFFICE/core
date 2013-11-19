
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableHeader.h"
#include "Exception/log_runtime_error.h"


namespace Odt
{
	namespace Content
	{

		TableHeader::TableHeader()
		{
			m_hasNote = false;
			m_hasList = false;
		}


		TableHeader::~TableHeader()
		{
		}


		TableHeader::TableHeader(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableHeader& TableHeader::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableHeader::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(Rows, element, "table-row");
			m_hasNote = false;
			m_hasList = false;
		}


		const XML::XNode TableHeader::toXML() const
		{
			return
				XML::XElement(ns.table + "table-header-rows",
					XML::Write(Rows)
				);
		}


		const bool TableHeader::hasNote() const
		{
			return m_hasNote;
		}


		const bool TableHeader::hasList() const
		{
			return m_hasList;
		}
	} // namespace Content
} // namespace Odt