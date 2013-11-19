#include "TransitionBase.h"
#include "EmptyTransition.h"
#include "OrientationTransition.h"
#include "EightDirectionTransition.h"
#include "OptionalBlackTransition.h"
#include "SideDirectionTransition.h"
#include "CornerDirectionTransition.h"
#include "WheelTransition.h"
#include "SplitTransition.h"
#include "ZoomTransition.h"

namespace PPTX
{
	namespace Logic
	{

		TransitionBase::TransitionBase()
		{
		}

		TransitionBase::~TransitionBase()
		{
		}

		TransitionBase::TransitionBase(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TransitionBase& TransitionBase::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TransitionBase::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if (name == "random")
				base.reset(new Logic::EmptyTransition(node));
			else if (name == "circle")
				base.reset(new Logic::EmptyTransition(node));
			else if (name == "dissolve")
				base.reset(new Logic::EmptyTransition(node));
			else if (name == "diamond")
				base.reset(new Logic::EmptyTransition(node));
			else if (name == "newsflash")
				base.reset(new Logic::EmptyTransition(node));
			else if (name == "plus")
				base.reset(new Logic::EmptyTransition(node));
			else if (name == "wedge")
				base.reset(new Logic::EmptyTransition(node));
			else if (name == "blinds")
				base.reset(new Logic::OrientationTransition(node));
			else if (name == "checker")
				base.reset(new Logic::OrientationTransition(node));
			else if (name == "comb")
				base.reset(new Logic::OrientationTransition(node));
			else if (name == "randomBar")
				base.reset(new Logic::OrientationTransition(node));
			else if (name == "cover")
				base.reset(new Logic::EightDirectionTransition(node));
			else if (name == "pull")
				base.reset(new Logic::EightDirectionTransition(node));
			else if (name == "cut")
				base.reset(new Logic::OptionalBlackTransition(node));
			else if (name == "fade")
				base.reset(new Logic::OptionalBlackTransition(node));
			else if (name == "push")
				base.reset(new Logic::SideDirectionTransition(node));
			else if (name == "wipe")
				base.reset(new Logic::SideDirectionTransition(node));
			else if (name == "strips")
				base.reset(new Logic::CornerDirectionTransition(node));
			else if (name == "wheel")
				base.reset(new Logic::WheelTransition(node));
			else if (name == "split")
				base.reset(new Logic::SplitTransition(node));
			else if (name == "zoom")
				base.reset(new Logic::ZoomTransition(node));
			else base.reset();
		}

		void TransitionBase::GetTransitionTypeFrom(const XML::XElement& element){
			if(element.element("random").exist())
				base.reset(new Logic::EmptyTransition(element.element("random")));
			else if(element.element("circle").exist())
				base.reset(new Logic::EmptyTransition(element.element("circle")));
			else if(element.element("dissolve").exist())
				base.reset(new Logic::EmptyTransition(element.element("dissolve")));
			else if(element.element("diamond").exist())
				base.reset(new Logic::EmptyTransition(element.element("diamond")));
			else if(element.element("newsflash").exist())
				base.reset(new Logic::EmptyTransition(element.element("newsflash")));
			else if(element.element("plus").exist())
				base.reset(new Logic::EmptyTransition(element.element("plus")));
			else if(element.element("wedge").exist())
				base.reset(new Logic::EmptyTransition(element.element("wedge")));
			else if(element.element("blinds").exist())
				base.reset(new Logic::OrientationTransition(element.element("blinds")));
			else if(element.element("checker").exist())
				base.reset(new Logic::OrientationTransition(element.element("checker")));
			else if(element.element("comb").exist())
				base.reset(new Logic::OrientationTransition(element.element("comb")));
			else if(element.element("randomBar").exist())
				base.reset(new Logic::OrientationTransition(element.element("randomBar")));
			else if(element.element("cover").exist())
				base.reset(new Logic::EightDirectionTransition(element.element("cover")));
			else if(element.element("pull").exist())
				base.reset(new Logic::EightDirectionTransition(element.element("pull")));
			else if(element.element("cut").exist())
				base.reset(new Logic::OptionalBlackTransition(element.element("cut")));
			else if(element.element("fade").exist())
				base.reset(new Logic::OptionalBlackTransition(element.element("fade")));
			else if(element.element("push").exist())
				base.reset(new Logic::SideDirectionTransition(element.element("push")));
			else if(element.element("wipe").exist())
				base.reset(new Logic::SideDirectionTransition(element.element("wipe")));
			else if(element.element("strips").exist())
				base.reset(new Logic::CornerDirectionTransition(element.element("strips")));
			else if(element.element("wheel").exist())
				base.reset(new Logic::WheelTransition(element.element("wheel")));
			else if(element.element("split").exist())
				base.reset(new Logic::SplitTransition(element.element("split")));
			else if(element.element("zoom").exist())
				base.reset(new Logic::ZoomTransition(element.element("zoom")));
			else base.reset();
		}

		const XML::XNode TransitionBase::toXML() const
		{
			return XML::Write(base);
		}

	} // namespace Logic
} // namespace PPTX