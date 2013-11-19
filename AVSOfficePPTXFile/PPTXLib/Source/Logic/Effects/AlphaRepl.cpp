#include "AlphaRepl.h"

namespace PPTX
{
	namespace Logic
	{

		AlphaRepl::AlphaRepl()
		{
		}

		AlphaRepl::~AlphaRepl()
		{
		}

		AlphaRepl::AlphaRepl(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AlphaRepl& AlphaRepl::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AlphaRepl::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			a = element.attribute("a").value();
		}

		const XML::XNode AlphaRepl::toXML() const
		{
			return XML::XElement(ns.a + "alphaRepl",
				XML::XAttribute("a", a)
				);
		}
	} // namespace Logic
} // namespace PPTX