#pragma once
#ifndef OOX_RELS_RELATION_SHIP_INCLUDE_H_
#define OOX_RELS_RELATION_SHIP_INCLUDE_H_

#include "./../WritingElement.h"
#include "./../RId.h"
#include "./../External/External.h"

namespace PPTX
{
	namespace Rels
	{
		class RelationShip : public WritingElement
		{
		public:
			RelationShip(const PPTX::RId& rId, const CString& type, const OOX::CPath& filename) : m_rId(rId), m_target(filename), m_type(type)
			{
				m_target.m_strFilename.Replace(_T(" "), _T("_"));
			}
			RelationShip(const PPTX::RId& rId, const smart_ptr<External> external): m_rId(rId), m_target(external->Uri()), 
				m_type(external->type().RelationType())
			{
				m_mode = new CString(_T("External"));
			}
			virtual ~RelationShip()
			{
			}
			explicit RelationShip(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
			}
			const RelationShip& operator =(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
				return *this;
			}
			
		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_rId		= node.GetAttribute(_T("Id"));
				m_target	= node.GetAttribute(_T("Target"));
				m_type		= node.GetAttribute(_T("Type"));
				m_mode		= node.GetAttribute(_T("TargetMode"), _T("Internal"));
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("Id"), m_rId.ToString());
				oAttr.Write(_T("Type"), m_type);
				oAttr.Write(_T("Target"), m_target.m_strFilename);
				oAttr.Write(_T("TargetMode"), m_mode);
				
				return XmlUtils::CreateNode(_T("Relationship"), oAttr);
			}

		public:
			const bool operator <(const RelationShip& rhs) const
			{
				return m_rId < rhs.m_rId;
			}

		public:
			const CString type() const
			{
				return m_type;
			}
			const OOX::CPath filename() const
			{
				return m_target;
			}
			const OOX::CPath target() const
			{
				return m_target;
			}
			const bool isExternal()const
			{
				if (!m_mode.IsInit())
					return false;
                return (*m_mode == _T("External"));
			}
			const RId rId() const
			{
				return m_rId;
			}

		private:
			RId						m_rId;
			OOX::CPath					m_target;
			CString					m_type;
			nullable_string			m_mode;
		};
	} // namespace Rels
} // namespace PPTX

#endif // OOX_RELS_RELATION_SHIP_INCLUDE_H_
