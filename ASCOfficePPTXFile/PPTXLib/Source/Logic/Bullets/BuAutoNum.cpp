#include "BuAutoNum.h"

namespace PPTX
{
	namespace Logic
	{

		BuAutoNum::BuAutoNum()
		{
		}


		BuAutoNum::~BuAutoNum()
		{
		}


		BuAutoNum::BuAutoNum(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BuAutoNum& BuAutoNum::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BuAutoNum::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			type = element.attribute("type").value();
			startAt = element.attribute("startAt").value();
		}


		const XML::XNode BuAutoNum::toXML() const
		{
			return XML::XElement(ns.a + "buAutoNum",
				XML::XAttribute("type", type) +
				XML::XAttribute("startAt", startAt)
			);
		}

	} // namespace Logic
} // namespace PPTX