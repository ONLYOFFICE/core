#include "GradFill.h"

namespace PPTX
{
	namespace Logic
	{

		GradFill::GradFill()
		{
		}


		GradFill::~GradFill()
		{
		}


		GradFill::GradFill(const XML::XNode& node)
		{
			fromXML(node);
		}


		const GradFill& GradFill::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void GradFill::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_namespace = element.XName.get().Ns;

			flip = element.attribute("flip").value();
			rotWithShape = element.attribute("rotWithShape").value();

			XML::Fill(GsLst, element.element("gsLst"), "gs");
			path = element.element("path");
			lin = element.element("lin");
			tileRect = element.element("tileRect");

			FillParentPointersForChilds();
		}


		const XML::XNode GradFill::toXML() const
		{
			 return XML::XElement(m_namespace + "gradFill",
					XML::XAttribute("flip", flip) +
					XML::XAttribute("rotWithShape", rotWithShape) +
					XML::XElement(ns.a + "gsLst", XML::Write(GsLst)) +
					XML::Write(lin) +
					XML::Write(path) +
					XML::Write(tileRect)
				);
		}

		void GradFill::Merge(GradFill& fill)const
		{
			if(flip.is_init())
				fill.flip = *flip;
			if(rotWithShape.is_init())
				fill.rotWithShape = *rotWithShape;
			if(tileRect.is_init())
				fill.tileRect = tileRect;
			if(!GsLst->empty())
			{
				fill.GsLst->clear();
				for(std::list<Gs>::const_iterator i = GsLst->begin(); i != GsLst->end(); i++)
					fill.GsLst->push_back(*i);
			}

			if(lin.is_init())
			{
				fill.lin = lin;
				fill.path.reset();
			}
			if(path.is_init())
			{
				fill.path = path;
				fill.lin.reset();
			}
		}

		void GradFill::FillParentPointersForChilds()
		{
			for(std::list<Gs>::iterator i = GsLst->begin(); i != GsLst->end(); i++)
				i->SetParentPointer(*this);
			if(lin.is_init())
				lin->SetParentPointer(*this);
			if(path.is_init())
				path->SetParentPointer(*this);
			if(tileRect.is_init())
				tileRect->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX