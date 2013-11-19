#include "Timing.h"

namespace PPTX
{
	namespace Logic
	{

		Timing::Timing()
		{
		}


		Timing::~Timing()
		{
		}
	

		Timing::Timing(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Timing& Timing::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Timing::fromXML(const XML::XNode& node)
		{
//			const XML::XElement element(node);
		}


		const XML::XNode Timing::toXML() const
		{
			return XML::XElement(ns.p + "timing");
		}

	} // namespace Logic
} // namespace PPTX