#include "UniPath2D.h"
#include "Path2D/MoveTo.h"
#include "Path2D/LineTo.h"
#include "Path2D/Close.h"
#include "Path2D/CubicBezTo.h"
#include "Path2D/ArcTo.h"
#include "Path2D/QuadBezTo.h"

namespace PPTX
{
	namespace Logic
	{

		UniPath2D::UniPath2D()
		{
		}

		UniPath2D::~UniPath2D()
		{
		}

		UniPath2D::UniPath2D(const XML::XNode& node)
		{
			fromXML(node);
		}

		const UniPath2D& UniPath2D::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void UniPath2D::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if (name == "moveTo")
				Path2D.reset(new Logic::MoveTo(node));
			else if (name == "lnTo")
				Path2D.reset(new Logic::LineTo(node));
			else if (name == "cubicBezTo")
				Path2D.reset(new Logic::CubicBezTo(node));
			else if (name == "close")
				Path2D.reset(new Logic::Close());
			else if (name == "arcTo")
				Path2D.reset(new Logic::ArcTo());
			else if (name == "quadBezTo")
				Path2D.reset(new Logic::QuadBezTo(node));
			else Path2D.reset();
		}

		void UniPath2D::GetPath2DFrom(const XML::XElement& element){
			if(element.element("moveTo").exist())
				Path2D.reset(new Logic::MoveTo(element.element("moveTo")));
			else if(element.element("lnTo").exist())
				Path2D.reset(new Logic::LineTo(element.element("lnTo")));
			else if(element.element("cubicBezTo").exist())
				Path2D.reset(new Logic::CubicBezTo(element.element("cubicBezTo")));
			else if(element.element("close").exist())
				Path2D.reset(new Logic::Close());
			else if(element.element("arcTo").exist())
				Path2D.reset(new Logic::ArcTo(element.element("arcTo")));
			else if(element.element("quadBezTo").exist())
				Path2D.reset(new Logic::QuadBezTo(element.element("quadBezTo")));
			else Path2D.reset();
		}

		const XML::XNode UniPath2D::toXML() const
		{
			return XML::Write(Path2D);
		}
	} // namespace Logic
} // namespace PPTX