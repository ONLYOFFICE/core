#include "BuChar.h"

namespace PPTX
{
	namespace Logic
	{

		BuChar::BuChar()
		{
		}


		BuChar::~BuChar()
		{
		}


		BuChar::BuChar(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BuChar& BuChar::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BuChar::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			Char = element.attribute("char").value();
		}


		const XML::XNode BuChar::toXML() const
		{
			return XML::XElement(ns.a + "buChar",
				XML::XAttribute("char", Char)
			);
		}

	} // namespace Logic
} // namespace PPTX