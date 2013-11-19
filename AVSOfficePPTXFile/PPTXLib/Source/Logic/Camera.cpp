#include "Camera.h"

namespace PPTX
{
	namespace Logic
	{

		Camera::Camera()
		{
		}


		Camera::~Camera()
		{
		}


		Camera::Camera(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Camera& Camera::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Camera::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			prst = element.attribute("prst").value();
			fov = element.attribute("fov").value();
			zoom = element.attribute("zoom").value();

			rot = element.element("rot");
			FillParentPointersForChilds();
		}


		const XML::XNode Camera::toXML() const
		{
			return XML::XElement(ns.a + "camera",
					XML::XAttribute("prst", prst) +
					XML::XAttribute("fov", fov) +
					XML::XAttribute("zoom", zoom) +
					XML::Write(rot)
				);
		}

		void Camera::FillParentPointersForChilds()
		{
			if(rot.is_init())
				rot->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX