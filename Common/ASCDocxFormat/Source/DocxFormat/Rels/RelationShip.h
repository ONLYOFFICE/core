#pragma once
#ifndef OOX_RELS_RELATION_SHIP_INCLUDE_H_
#define OOX_RELS_RELATION_SHIP_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include "./../RId.h"
#include <string>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>


namespace OOX {class External;}


namespace OOX
{
	namespace Rels
	{
		class RelationShip : public WritingElement
		{
		public:
			RelationShip(const RId& rId, const std::string& type, const boost::filesystem::wpath& filename);
			RelationShip(const RId& rId, const boost::shared_ptr<External> external);
			virtual ~RelationShip();
			explicit RelationShip(const XML::XNode& node);
			const RelationShip& operator =(const XML::XNode& node);
			
		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool operator <(const RelationShip& rhs) const;

		public:
			const std::string type() const;
			const boost::filesystem::wpath filename() const;
			const boost::filesystem::wpath target() const;
			const bool isExternal()const;
			const RId rId() const;

		private:
			RId							m_rId;
			boost::filesystem::wpath	m_target;
			std::string					m_type;
			nullable<std::string>		m_mode;
		};
	} // namespace Rels
} // namespace OOX

#endif // OOX_RELS_RELATION_SHIP_INCLUDE_H_