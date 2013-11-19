#include "Geometry.h"
#include "PrstGeom.h"
#include "CustGeom.h"

namespace PPTX
{
	namespace Logic
	{
		Geometry::Geometry()
		{
		}

		Geometry::~Geometry()
		{
		}

		Geometry::Geometry(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Geometry& Geometry::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Geometry::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			if (element->XName == "prstGeom")
				m_geometry.reset(new Logic::PrstGeom(node));
			else if (element->XName == "custGeom")
				m_geometry.reset(new Logic::CustGeom(node));
			else m_geometry.reset();
		}

		void Geometry::GetGeometryFrom(const XML::XElement& element){
			if(element.element("prstGeom").exist())
				m_geometry.reset(new Logic::PrstGeom(element.element("prstGeom")));
			else if(element.element("custGeom").exist())
				m_geometry.reset(new Logic::CustGeom(element.element("custGeom")));
			else m_geometry.reset();
		}

		const XML::XNode Geometry::toXML() const
		{
			return XML::Write(m_geometry);
		}

	} // namespace Logic
} // namespace PPTX