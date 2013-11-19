#include "AhBase.h"
#include "AhXY.h"
#include "AhPolar.h"

namespace PPTX
{
	namespace Logic
	{
		AhBase::AhBase()
		{
		}

		AhBase::~AhBase()
		{
		}

		AhBase::AhBase(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AhBase& AhBase::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AhBase::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if (name == "ahXY")
				ah.reset(new Logic::AhXY(node));
			else if (name == "ahPolar")
				ah.reset(new Logic::AhPolar(node));
			else ah.reset();
		}

		void AhBase::GetAdjustHandleFrom(const XML::XElement& element){
			if(element.element("ahXY").exist())
				ah.reset(new Logic::AhXY(element.element("ahXY")));
			else if(element.element("ahPolar").exist())
				ah.reset(new Logic::AhPolar(element.element("ahPolar")));
			else ah.reset();
		}

		const XML::XNode AhBase::toXML() const
		{
			return XML::Write(ah);
		}
	} // namespace Logic
} // namespace PPTX