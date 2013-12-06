#pragma once
#ifndef OOX_RELS_RELATION_TABLE_INCLUDE_H_
#define OOX_RELS_RELATION_TABLE_INCLUDE_H_

#include "RelationShip.h"
#include "./../RId.h"

namespace OOX {class External;}

namespace OOX
{
	namespace Rels
	{
		class RelationTable
		{
		public:
			RelationTable();
			virtual ~RelationTable();
			RelationTable(XmlUtils::CXmlNode& oNode);
			const RelationTable& operator =(XmlUtils::CXmlNode& oNode);
		
		public:
			void fromXML(XmlUtils::CXmlNode& oNode);

		public:
			void registration(const RId& rId, const std::string& type, const OOX::CPath& filename);
			void registration(const RId& rId, const NSCommon::smart_ptr<OOX::External> external);

		public:
			std::vector<RelationShip> m_items;
		};
	} // namespace Rels
} // namespace OOX

#endif // OOX_RELS_RELATION_TABLE_INCLUDE_H_