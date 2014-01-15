
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Scene3d.h"


namespace OOX
{
	namespace Theme
	{

		Scene3d::Scene3d()
		{
		}


		Scene3d::~Scene3d()
		{
		}
	

		Scene3d::Scene3d(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Scene3d& Scene3d::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Scene3d::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_camera		=	element.element("camera");
			m_lightRig	=	element.element("lightRig");
		}


		const XML::XNode Scene3d::toXML() const
		{
			return 
				XML::XElement(ns.a + "scene3d", 
					XML::Write(m_camera) + 
					XML::Write(m_lightRig)
				);
		}

	} // namespace Theme
} // namespace OOX