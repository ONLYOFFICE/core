#include "Gradients.h"
#include <algorithm>
#include <boost/bind.hpp>

namespace Odp
{
	namespace Styles
	{
		Gradients::Gradients()
		{
		}

		Gradients::~Gradients()
		{
		}

		Gradients::Gradients(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Gradients& Gradients::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Gradients::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "gradient");
		}

		const bool Gradients::exist(const std::string& name) const
		{
			return std::find_if(m_items.begin(), m_items.end(), boost::bind(&Gradient::Name, _1) == name) != m_items.end();
		}

		const Gradient& Gradients::operator[](const std::string& name) const
		{
			return find(name);
		}

		Gradient& Gradients::operator[](const std::string& name)
		{
			return find(name);
		}

		const Gradient& Gradients::find(const std::string& name) const
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&Gradient::Name, _1) == name);
		}
		
		Gradient& Gradients::find(const std::string& name)
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&Gradient::Name, _1) == name);
		}
	} // namespace Styles
} // namespace Odp