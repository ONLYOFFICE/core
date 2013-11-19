#include "Scale.h"

namespace PPTX
{
	namespace nsViewProps
	{
		Scale::Scale()
		{
		}

		Scale::~Scale()
		{
		}

		Scale::Scale(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Scale& Scale::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Scale::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			sx = element.element("sx");
			sy = element.element("sy");

			FillParentPointersForChilds();
		}

		const XML::XNode Scale::toXML() const
		{
			return	XML::XElement(ns.p + "scale",
				XML::Write(sx) +
				XML::Write(sy)
			);
		}

		void Scale::FillParentPointersForChilds()
		{
			sx->SetParentPointer(*this);
			sy->SetParentPointer(*this);
		}
	} // namespace nsViewProps
} // namespace PPTX