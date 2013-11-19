#include "BuNone.h"

namespace PPTX
{
	namespace Logic
	{

		BuNone::BuNone()
		{
		}


		BuNone::~BuNone()
		{
		}


		BuNone::BuNone(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BuNone& BuNone::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BuNone::fromXML(const XML::XNode& node)
		{
		}


		const XML::XNode BuNone::toXML() const
		{
			return XML::XElement(ns.a + "buNone");
		}

	} // namespace Logic
} // namespace PPTX