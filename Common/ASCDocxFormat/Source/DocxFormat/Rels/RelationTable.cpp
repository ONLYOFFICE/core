
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
		
		RelationTable::RelationTable(XmlUtils::CXmlNode& oNode)
		{
			fromXML(oNode);
		}		
		
		const RelationTable& RelationTable::operator =(XmlUtils::CXmlNode& oNode)
		{
			fromXML(oNode);
			return *this;
		}

		void RelationTable::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNodes oList;
			oNode.GetNodes( _T("Relationship"), oList );

			for ( int nIndex = 0; nIndex < oList.GetCount(); ++nIndex )
			{
				XmlUtils::CXmlNode oRelNode;
				if ( oList.GetAt( nIndex, oRelNode ) )
				{
					RelationShip oRelationShip (oRelNode);
					m_items.push_back( oRelationShip );
				}
			}
		}

		void RelationTable::registration(const RId& rId, const std::string& type, const OOX::CPath& filename)
		{
			m_items.push_back(RelationShip(rId, type, filename));
		}

		void RelationTable::registration(const RId& rId, const NSCommon::smart_ptr<OOX::External> external)
		{
			m_items.push_back(RelationShip(rId, external));
		}

	} // namespace Rels
} // namespace OOX