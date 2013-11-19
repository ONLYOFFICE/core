#include "Timing.h"

namespace PPTX
{
	namespace Logic
	{

		Timing::Timing()
		{
		}

		Timing::~Timing()
		{
		}

		Timing::Timing(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Timing& Timing::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Timing::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			tnLst = element.element("tnLst");
			bldLst = element.element("bldLst");
			FillParentPointersForChilds();
		}

		const XML::XNode Timing::toXML() const
		{
			return XML::XElement(ns.p + "timing",
					XML::Write(tnLst) +
					XML::Write(bldLst)
				);
		}

		void Timing::FillParentPointersForChilds()
		{
			if(tnLst.is_init())
				tnLst->SetParentPointer(*this);
			if(bldLst.is_init())
				bldLst->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX