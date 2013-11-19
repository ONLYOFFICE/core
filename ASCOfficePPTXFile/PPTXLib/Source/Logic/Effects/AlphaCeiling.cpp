#include "AlphaCeiling.h"

namespace PPTX
{
	namespace Logic
	{

		AlphaCeiling::AlphaCeiling()
		{
		}

		AlphaCeiling::~AlphaCeiling()
		{
		}

		AlphaCeiling::AlphaCeiling(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AlphaCeiling& AlphaCeiling::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AlphaCeiling::fromXML(const XML::XNode& node)
		{
		}

		const XML::XNode AlphaCeiling::toXML() const
		{
			return XML::XElement(ns.a + "alphaCeiling");
		}
	} // namespace Logic
} // namespace PPTX