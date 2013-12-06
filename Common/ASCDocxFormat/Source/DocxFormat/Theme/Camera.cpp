
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Camera.h"


namespace OOX
{
	namespace Theme
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
			m_prst	= element.attribute("prst").value();
		  m_rot		=	element.element("rot");
		}


		const XML::XNode Camera::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX