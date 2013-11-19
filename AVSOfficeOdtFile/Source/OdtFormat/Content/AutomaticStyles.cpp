
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "AutomaticStyles.h"
#include <algorithm>
#include <boost/bind.hpp>

// TODO устаревший
namespace Odt
{
	namespace Content
	{

		AutomaticStyles::AutomaticStyles()
		{
		}


		AutomaticStyles::~AutomaticStyles()
		{
		}


		const AutomaticStyles& AutomaticStyles::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void AutomaticStyles::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
//			m_styles->ReadFromContent(element);
		}


		const XML::XNode AutomaticStyles::toXML() const
		{
			return 
				XML::XElement(ns.office + "automatic-styles"//, 
//					m_styles->WriteToContent()
				);
		}

/*
		void AutomaticStyles::setStyles(const boost::shared_ptr<Logic::Styles>& styles)
		{
			m_styles = styles;
		}
*/
	} // namespace Content
} // namespace Odt