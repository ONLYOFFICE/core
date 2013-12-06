#pragma once
#ifndef OOX_RELS_RELATION_SHIP_INCLUDE_H_
#define OOX_RELS_RELATION_SHIP_INCLUDE_H_

#include <string>

#include "./../RId.h"
#include "./../External/External.h"

#include "../../../../../Common/DocxFormat/Source/Base/SmartPtr.h"
#include "../../../../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../../../../../Common/DocxFormat/Source/Xml/XmlUtils.h"

namespace OOX {class External;}

namespace OOX
{
	namespace Rels
	{
		class RelationShip
		{
		public:
			RelationShip(const RId& rId, const std::string& type, const OOX::CPath& filename);
			RelationShip(const RId& rId, const NSCommon::smart_ptr<External> external);
			virtual ~RelationShip();
			explicit RelationShip(XmlUtils::CXmlNode& oNode);
			const RelationShip& operator =(XmlUtils::CXmlNode& oNode);
			
		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);

		public:
			const bool operator <(const RelationShip& rhs) const;

		public:
			const std::wstring type() const;
			const OOX::CPath filename() const;
			const OOX::CPath target() const;
			const bool isExternal()const;
			const RId rId() const;

		private:
			RId									m_rId;
			OOX::CPath							m_target;
			std::wstring						m_type;
			NSCommon::nullable<std::wstring>	m_mode;
		};
	} // namespace Rels
} // namespace OOX

#endif // OOX_RELS_RELATION_SHIP_INCLUDE_H_