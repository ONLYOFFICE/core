#include "FillEffect.h"

namespace PPTX
{
	namespace Logic
	{
		FillEffect::FillEffect()
		{
		}

		FillEffect::~FillEffect()
		{
		}

		FillEffect::FillEffect(const XML::XNode& node)
		{
			fromXML(node);
		}

		const FillEffect& FillEffect::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void FillEffect::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Fill.GetFillFrom(element);
			FillParentPointersForChilds();
		}

		const XML::XNode FillEffect::toXML() const
		{
			return XML::XElement(ns.a + "fill",
					XML::Write(Fill)
				);
		}

		void FillEffect::FillParentPointersForChilds()
		{
			Fill.SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX