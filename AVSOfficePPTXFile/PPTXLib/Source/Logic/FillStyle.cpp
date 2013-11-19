#include "FillStyle.h"

namespace PPTX
{
	namespace Logic
	{

		FillStyle::FillStyle()
		{
		}

		FillStyle::~FillStyle(){
		}

		FillStyle::FillStyle(const XML::XNode& node)
		{
			fromXML(node);
		}

		const FillStyle& FillStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void FillStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Fill.GetFillFrom(element);
			FillParentPointersForChilds();
		}

		const XML::XNode FillStyle::toXML() const
		{
			return XML::XElement(ns.a + "fill",
					XML::Write(Fill)
				);
		}

		void FillStyle::FillParentPointersForChilds()
		{
			Fill.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX