
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "RelationShip.h"
#include "./../External/External.h"
#include <boost/algorithm/string/replace.hpp>


namespace OOX
{
	namespace Rels
	{

		RelationShip::RelationShip(const RId& rId, const std::string& type, const boost::filesystem::wpath& filename)
			: m_rId(rId),
				m_target(filename),
				m_type(type)
		{
			std::wstring newFilename = filename.string();
			boost::replace_all(newFilename, L" ", L"_");
			m_target = newFilename;
		}


		RelationShip::RelationShip(const RId& rId, const boost::shared_ptr<External> external)
			: m_rId(rId),
				m_target(external->Uri()),
				m_type(external->type().RelationType()),
				m_mode("External")
		{
		}


		RelationShip::~RelationShip()
		{
		}

	
		RelationShip::RelationShip(const XML::XNode& node)
		{
			fromXML(node);
		}


		const RelationShip& RelationShip::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void RelationShip::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_rId			= element.attribute("Id").value().ToString();
			m_target	= element.attribute("Target").value();
			m_type		= element.attribute("Type").value();
			m_mode		= element.attribute("TargetMode").value();
		}


		const XML::XNode RelationShip::toXML() const
		{
			return
				XML::XElement("Relationship",
					XML::XAttribute("Id", m_rId) +
					XML::XAttribute("Type", m_type) + 
					XML::XAttribute("Target", m_target) +
					XML::XAttribute("TargetMode", m_mode)
				);
		}


		const bool RelationShip::operator <(const RelationShip& rhs) const
		{
			return m_rId < rhs.m_rId;
		}


		const std::string RelationShip::type() const
		{
			return m_type;
		}


		const boost::filesystem::wpath RelationShip::filename() const
		{
			return m_target;
		}


		const boost::filesystem::wpath RelationShip::target() const
		{
			return m_target;
		}


		const bool RelationShip::isExternal()const
		{
			return (m_mode.get_value_or("Internal") == "External");
		}


		const RId RelationShip::rId() const
		{
			return m_rId;
		}

	} // namespace Rels
} // namespace OOX