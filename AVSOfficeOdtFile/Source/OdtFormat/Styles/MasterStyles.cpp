
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "MasterStyles.h"
#include <algorithm>
#include <boost/bind.hpp>


namespace Odt
{
	namespace Styles
	{

		MasterStyles::MasterStyles()
		{
		}


		MasterStyles::~MasterStyles()
		{
		}


		MasterStyles::MasterStyles(const XML::XNode& node)
		{
			fromXML(node);
		}


		const MasterStyles& MasterStyles::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void MasterStyles::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_pages, element);
		}


		const XML::XNode MasterStyles::toXML() const
		{
			return XML::XElement(ns.office + "master-styles", XML::Write(m_pages));
		}


		const bool MasterStyles::exist(const std::string& name) const
		{
			return std::find_if(m_pages.begin(), m_pages.end(), boost::bind(&MasterPage::Name, _1) == name) != m_pages.end();
		}

		
		const MasterPage& MasterStyles::find(const std::string& name) const
		{
			return *Odt::find_if(m_pages.begin(), m_pages.end(), boost::bind(&MasterPage::Name, _1) == name);
			//return *std::find_if(m_pages.begin(), m_pages.end(), boost::bind(&MasterPage::Name, _1) == name);
		}
		
		
		MasterPage& MasterStyles::find(const std::string& name)
		{
			return *Odt::find_if(m_pages.begin(), m_pages.end(), boost::bind(&MasterPage::Name, _1) == name);
			//return *std::find_if(m_pages.begin(), m_pages.end(), boost::bind(&MasterPage::Name, _1) == name);
		}

	} // namespace Styles
} // namespace Odt