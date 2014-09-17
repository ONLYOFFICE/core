#pragma once
#ifndef OOX_RELS_RELATION_TABLE_INCLUDE_H_
#define OOX_RELS_RELATION_TABLE_INCLUDE_H_

#include "./../WritingVector.h"
#include "RelationShip.h"
#include "./../RId.h"

namespace PPTX {class External;}

namespace PPTX
{
	namespace Rels
	{
		class RelationTable : public WritingVector<RelationShip>
		{
		public:
			RelationTable()
			{
			}
			virtual ~RelationTable()
			{
			}
			explicit RelationTable(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
			}
			const RelationTable& operator =(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
				return *this;
			}

		public:
			void registration(const RId& rId, const CString& type, const OOX::CPath& filename)
			{
				m_items.Add(RelationShip(rId, type, filename));
			}
			void registration(const RId& rId, const smart_ptr<PPTX::External> external)
			{
				m_items.Add(RelationShip(rId, external));
			}
		};
	} // namespace Rels
} // namespace PPTX

#endif // OOX_RELS_RELATION_TABLE_INCLUDE_H_