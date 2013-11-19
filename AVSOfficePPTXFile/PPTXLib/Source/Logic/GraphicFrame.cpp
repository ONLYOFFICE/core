#include "GraphicFrame.h"
#include "SpTree.h"
#include "..\Slide.h"
#include "..\SlideLayout.h"
#include "..\SlideMaster.h"

namespace PPTX
{
	namespace Logic
	{
		GraphicFrame::GraphicFrame()
		{
		}

		GraphicFrame::~GraphicFrame()
		{
		}

		GraphicFrame::GraphicFrame(const XML::XNode& node)
		{
			fromXML(node);
		}

		const GraphicFrame& GraphicFrame::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void GraphicFrame::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			nvGraphicFramePr = element.element("nvGraphicFramePr");
			xfrm = element.element("xfrm");
			if(element.element("graphic").exist())
			{
				XML::XElement elem = element.element("graphic");
				if(elem.element("graphicData").exist())
				{
					elem = elem.element("graphicData");
					if(elem.element("tbl").exist())
					{
						Table = elem.element("tbl");
					}
					else if(elem.element("oleObj").exist())
					{
						elem = elem.element("oleObj");
						spid = elem.attribute("spid").value();
			//			dm = elem.attribute("dm").ToString();
			//			lo = elem.attribute("lo").ToString();
			//			qs = elem.attribute("qs").ToString();
			//			cs = elem.attribute("cs").ToString();
					}
					else if(elem.element("AlternateContent").exist())
					{
						elem = elem.element("AlternateContent");
						if(elem.element("Choice").exist())
						{
							elem = elem.element("Choice");
							if(elem.element("oleObj").exist())
							{
								elem = elem.element("oleObj");
								spid = elem.attribute("spid").value();
							}
						}
					}
				}
			}

			FillParentPointersForChilds();
		}

		void GraphicFrame::GetRect(RECT& pRect)const
		{
			pRect.left = xfrm->offX.get();
			pRect.top = xfrm->offY.get();
			pRect.right = pRect.left + xfrm->extX.get();
			pRect.bottom = pRect.top + xfrm->extY.get();

			if(parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(pRect);
		}

		const XML::XNode GraphicFrame::toXML() const
		{
			XML::XNode node;
			//if(dm.is_init())
			//{
			//	return XML::XElement(ns.p + "graphicFrame",
			//			XML::Write(nvGraphicFramePr) +
			//			XML::Write(xfrm) +
			//			XML::XElement(ns.a + "graphic",
			//				XML::XElement(ns.a + "graphicData", //Возможно, здесь надо добавить ури
			//					XML::XElement(ns.dgm + "relIds",
			//						XML::XNamespace(ns.dgm) +
			//						XML::XNamespace(ns.r) +
			//						XML::XAttribute(ns.r + "dm", dm) +
			//						XML::XAttribute(ns.r + "lo", lo) +
			//						XML::XAttribute(ns.r + "qs", qs) +
			//						XML::XAttribute(ns.r + "cs", cs)
			//					)
			//				)
			//			)
			//		);
			//}
			return node;
		}

		void GraphicFrame::FillParentPointersForChilds()
		{
			xfrm->SetParentPointer(*this);
			nvGraphicFramePr->SetParentPointer(*this);
			if(Table.is_init())
				Table->SetParentPointer(*this);
		}

		const boost::filesystem::wpath GraphicFrame::GetPathBySpid()const
		{
			boost::filesystem::wpath filename = L"";
			if(parentFileIs<PPTX::Slide>())
				filename = parentFileAs<PPTX::Slide>().GetPathBySpid(spid.get_value_or(""));
			else if(parentFileIs<PPTX::SlideLayout>())
				filename = parentFileAs<PPTX::SlideLayout>().GetPathBySpid(spid.get_value_or(""));
			else if(parentFileIs<PPTX::SlideMaster>())
				filename = parentFileAs<PPTX::SlideMaster>().GetPathBySpid(spid.get_value_or(""));

			return filename;
		}
	} // namespace Logic
} // namespace PPTX