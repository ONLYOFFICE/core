
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "RelationTable.h"
#include <algorithm>


namespace OOX
{
	namespace Rels
	{
		
		RelationTable::RelationTable()
		{
		}
		

		RelationTable::~RelationTable()
		{
		}
		

		RelationTable::RelationTable(const XML::XNode& node)
		{
			fromXML(node);
		}
		
		
		const RelationTable& RelationTable::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void RelationTable::registration(const RId& rId, const std::string& type, const boost::filesystem::wpath& filename)
		{
			m_items.push_back(RelationShip(rId, type, filename));
		}


		void RelationTable::registration(const RId& rId, const boost::shared_ptr<OOX::External> external)
		{
			m_items.push_back(RelationShip(rId, external));
		}

	} // namespace Rels
} // namespace OOX