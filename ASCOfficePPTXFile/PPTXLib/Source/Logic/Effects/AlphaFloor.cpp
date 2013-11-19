#include "AlphaFloor.h"

namespace PPTX
{
	namespace Logic
	{

		AlphaFloor::AlphaFloor()
		{
		}

		AlphaFloor::~AlphaFloor()
		{
		}

		AlphaFloor::AlphaFloor(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AlphaFloor& AlphaFloor::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AlphaFloor::fromXML(const XML::XNode& node)
		{
		}

		const XML::XNode AlphaFloor::toXML() const
		{
			return XML::XElement(ns.a + "alphaFloor");
		}
	} // namespace Logic
} // namespace PPTX