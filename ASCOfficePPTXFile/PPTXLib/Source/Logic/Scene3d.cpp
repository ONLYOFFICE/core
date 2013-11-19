#include "Scene3d.h"

namespace PPTX
{
	namespace Logic
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

			camera = element.element("camera");
			lightRig = element.element("lightRig");
			backdrop = element.element("backdrop");
			
			FillParentPointersForChilds();
		}


		const XML::XNode Scene3d::toXML() const
		{
			return XML::XElement(ns.a + "scene3d",
					XML::Write(camera) +
					XML::Write(lightRig) +
					XML::Write(backdrop)
				);
		}

		void Scene3d::FillParentPointersForChilds()
		{
			camera->SetParentPointer(*this);
			lightRig->SetParentPointer(*this);
			if(backdrop.is_init())
				backdrop->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX