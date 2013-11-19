#include "XfrmEffect.h"

namespace PPTX
{
	namespace Logic
	{

		XfrmEffect::XfrmEffect()
		{
		}

		XfrmEffect::~XfrmEffect()
		{
		}

		XfrmEffect::XfrmEffect(const XML::XNode& node)
		{
			fromXML(node);
		}

		const XfrmEffect& XfrmEffect::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void XfrmEffect::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			kx = element.attribute("kx").value();
			ky = element.attribute("ky").value();
			sx = element.attribute("sx").value();
			sy = element.attribute("sy").value();
			tx = element.attribute("tx").value();
			ty = element.attribute("ty").value();
		}

		const XML::XNode XfrmEffect::toXML() const
		{
			return XML::XElement(ns.a + "xfrm",
				XML::XAttribute("sx", sx) +
				XML::XAttribute("sy", sy) +
				XML::XAttribute("kx", kx) +
				XML::XAttribute("ky", ky) +
				XML::XAttribute("tx", tx) +
				XML::XAttribute("ty", ty)
				);
		}
	} // namespace Logic
} // namespace PPTX