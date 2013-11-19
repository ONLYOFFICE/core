#include "AlphaInv.h"

namespace PPTX
{
	namespace Logic
	{
		AlphaInv::AlphaInv()
		{
		}

		AlphaInv::~AlphaInv()
		{
		}

		AlphaInv::AlphaInv(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AlphaInv& AlphaInv::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AlphaInv::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Color.GetColorFrom(element);
			FillParentPointersForChilds();
		}

		const XML::XNode AlphaInv::toXML() const
		{
			return XML::XElement(ns.a + "alphaInv",
					XML::Write(Color)
				);
		}

		void AlphaInv::FillParentPointersForChilds()
		{
			if(Color.is_init())
				Color.SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX