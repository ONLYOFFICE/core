#include "AnimationElement.h"
#include "Par.h"
#include "TransitionFilter.h"

namespace Odp
{
	namespace Content
	{
		AnimationElement::AnimationElement()
		{
		}

		AnimationElement::~AnimationElement()
		{
		}

		AnimationElement::AnimationElement(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AnimationElement& AnimationElement::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AnimationElement::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if(name == "par")
				m_item.reset(new Par(node));
			else if(name == "transitionFilter")
				m_item.reset(new TransitionFilter(node));
			else m_item.reset();
		}

		const XML::XNode AnimationElement::toXML() const
		{
			return XML::Write(m_item);
		}
	} // namespace Content
} // namespace Odp