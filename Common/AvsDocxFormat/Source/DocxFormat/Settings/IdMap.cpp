
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "IdMap.h"


namespace OOX
{
	namespace Settings
	{

		IdMap::IdMap()
		{
		}


		IdMap::~IdMap()
		{
		}


		IdMap::IdMap(const XML::XNode& node)
		{
			fromXML(node);
		}


		const IdMap& IdMap::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void IdMap::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_ext =  element.attribute("ext").value();
			m_data = element.attribute("data").value();
		}


		const XML::XNode IdMap::toXML() const
		{
			return
				XML::XElement(ns.o + "idmap", 
					XML::XAttribute(ns.v + "ext", m_ext) + 
					XML::XAttribute("data", m_data)
				);
		}

	} // namespace Settings
} // namespace OOX