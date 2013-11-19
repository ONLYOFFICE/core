#include "HF.h"

namespace PPTX
{
	namespace Logic
	{

		HF::HF()
		{
		}


		HF::~HF()
		{
		}


		HF::HF(const XML::XNode& node)
		{
			fromXML(node);
		}


		const HF& HF::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void HF::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			dt = element.attribute("dt").value();
			ftr = element.attribute("ftr").value();
			hdr = element.attribute("hdr").value();
			sldNum = element.attribute("sldNum").value();
		}


		const XML::XNode HF::toXML() const
		{
			return XML::XElement(ns.p + "hf",
				XML::XAttribute("dt", dt) +
				XML::XAttribute("ftr", ftr) +
				XML::XAttribute("hdr", hdr) +
				XML::XAttribute("sldNum", sldNum)
			);
		}

	} // namespace Logic
} // namespace PPTX