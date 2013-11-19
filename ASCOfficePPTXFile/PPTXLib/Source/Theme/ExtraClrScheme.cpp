#include "ExtraClrScheme.h"

namespace PPTX
{
	namespace nsTheme
	{
		ExtraClrScheme::ExtraClrScheme()
		{
		}

		ExtraClrScheme::~ExtraClrScheme()
		{
		}

		ExtraClrScheme::ExtraClrScheme(const XML::XNode& node)
		{
			fromXML(node);
		}

		const ExtraClrScheme& ExtraClrScheme::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void ExtraClrScheme::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			clrScheme = element.element("clrScheme");
			clrMap = element.element("clrMap");

			FillParentPointersForChilds();
		}

		const XML::XNode ExtraClrScheme::toXML() const
		{
			return XML::XElement(ns.a + "extraClrScheme",
					XML::Write(clrScheme) +
					XML::Write(clrMap)
				);
		}

		void ExtraClrScheme::FillParentPointersForChilds()
		{
			clrScheme->SetParentPointer(*this);
			if(clrMap.is_init())
				clrMap->SetParentPointer(*this);
		}
	} // namespace nsTheme
} // namespace PPTX