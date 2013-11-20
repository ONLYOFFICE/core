
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "PageSize.h"

namespace OOX
{
	namespace Logic
	{
		PageSize::PageSize()
		{

		}	

		PageSize::~PageSize()
		{

		}

		PageSize::PageSize(const XML::XNode& node)
		{
			fromXML(node);
		}

		const PageSize& PageSize::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void PageSize::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Width		=	element.attribute("w").value();
			Height		=	element.attribute("h").value();
			Orient		=	element.attribute("orient").value();
		}

		const XML::XNode PageSize::toXML() const
		{
			return XML::XElement(ns.w + "pgSz",
				XML::XAttribute(ns.w + "w", Width) +
				XML::XAttribute(ns.w + "h", Height) +
				XML::XAttribute(ns.w + "orient", Orient));
		}

	} // namespace Logic
} // namespace OOX