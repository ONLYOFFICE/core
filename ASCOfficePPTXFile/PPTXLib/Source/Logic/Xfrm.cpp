#include "Xfrm.h"


namespace PPTX
{
	namespace Logic
	{

		Xfrm::Xfrm()
		{
		}


		Xfrm::~Xfrm()
		{
		}
	

		Xfrm::Xfrm(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Xfrm& Xfrm::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Xfrm::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_namespace = element.XName.get().Ns;

			flipH = element.attribute("flipH").value();
			flipV = element.attribute("flipV").value();
			rot = element.attribute("rot").value();

			if(element.element("off").exist()){
				offX = element.element("off").attribute("x").value();
				offY = element.element("off").attribute("y").value();
			}
			if(element.element("ext").exist()){
				extX = element.element("ext").attribute("cx").value();
				extY = element.element("ext").attribute("cy").value();
			}
			if(element.element("chOff").exist()){
				chOffX = element.element("chOff").attribute("x").value();
				chOffY = element.element("chOff").attribute("y").value();
			}
			if(element.element("chExt").exist()){
				chExtX = element.element("chExt").attribute("cx").value();
				chExtY = element.element("chExt").attribute("cy").value();
			}
		}


		const XML::XNode Xfrm::toXML() const
		{
			return XML::XElement(m_namespace + "xfrm",
				XML::XAttribute("rot", rot) +
				XML::XAttribute("flipH", flipH) +
				XML::XAttribute("flipV", flipV) +
				XML::WriteIf(XML::XElement(ns.a + "off", XML::XAttribute("x", offX) + XML::XAttribute("y", offY)), offX.is_init() || offY.is_init()) +
				XML::WriteIf(XML::XElement(ns.a + "ext", XML::XAttribute("cx", extX) + XML::XAttribute("cy", extY)), extX.is_init() || extY.is_init()) +
				XML::WriteIf(XML::XElement(ns.a + "chOff", XML::XAttribute("x", chOffX) + XML::XAttribute("y", chOffY)), chOffX.is_init() || chOffY.is_init()) +
				XML::WriteIf(XML::XElement(ns.a + "chExt", XML::XAttribute("cx", chExtX) + XML::XAttribute("cy", chExtY)), chExtX.is_init() || chExtY.is_init())
			);
		}

		void Xfrm::Merge(nullable_property<Xfrm>& xfrm)const
		{
			if(!xfrm.is_init())
				xfrm = Xfrm();
			if(offX.is_init())
				xfrm->offX = *offX;
			if(offY.is_init())
				xfrm->offY = *offY;
			if(extX.is_init())
				xfrm->extX = *extX;
			if(extY.is_init())
				xfrm->extY = *extY;
			if(chOffX.is_init())
				xfrm->chOffX = *chOffX;
			if(chOffY.is_init())
				xfrm->chOffY = *chOffY;
			if(chExtX.is_init())
				xfrm->chExtX = *chExtX;
			if(chExtY.is_init())
				xfrm->chExtY = *chExtY;
			if(flipH.is_init())
				xfrm->flipH = *flipH;
			if(flipV.is_init())
				xfrm->flipV = *flipV;
			if(rot.is_init())
				xfrm->rot = *rot;
		}
	} // namespace Logic
} // namespace PPTX