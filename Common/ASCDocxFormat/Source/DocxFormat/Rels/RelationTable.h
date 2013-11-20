#pragma once
#ifndef OOX_RELS_RELATION_TABLE_INCLUDE_H_
#define OOX_RELS_RELATION_TABLE_INCLUDE_H_

#include "./../WritingVector.h"
#include "RelationShip.h"
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include "./../RId.h"

namespace OOX {class External;}

namespace OOX
{
	namespace Rels
	{
		class RelationTable : public WritingVector<RelationShip>
		{
		public:
			RelationTable();
			virtual ~RelationTable();
			explicit RelationTable(const XML::XNode& node);
			const RelationTable& operator =(const XML::XNode& node);

		public:
			void registration(const RId& rId, const std::string& type, const boost::filesystem::wpath& filename);
			void registration(const RId& rId, const boost::shared_ptr<OOX::External> external);
		};
	} // namespace Rels
} // namespace OOX

#endif // OOX_RELS_RELATION_TABLE_INCLUDE_H_