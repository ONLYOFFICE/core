#include "AlphaMod.h"

namespace PPTX
{
	namespace Logic
	{
		AlphaMod::AlphaMod()
		{
		}

		AlphaMod::~AlphaMod()
		{
		}

		AlphaMod::AlphaMod(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AlphaMod& AlphaMod::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AlphaMod::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cont = element.element("cont");
			FillParentPointersForChilds();
		}

		const XML::XNode AlphaMod::toXML() const
		{
			return XML::XElement(ns.a + "alphaMod",
					XML::Write(cont)
				);
		}

		void AlphaMod::FillParentPointersForChilds()
		{
			cont->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX