#include "Tab.h"

namespace PPTX
{
	namespace Logic
	{

		Tab::Tab()
		{
		}

		Tab::~Tab()
		{
		}

		Tab::Tab(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Tab& Tab::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Tab::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			pos = element.attribute("pos").value();
			algn = element.attribute("algn").value();

			FillParentPointersForChilds();
		}

		const XML::XNode Tab::toXML() const
		{
			return XML::XElement(ns.a + "tab",
					XML::XAttribute("pos", pos) +
					XML::XAttribute("algn", algn)
				);
		}

	} // namespace Logic
} // namespace PPTX