#pragma once
#ifndef PPTX_LOGIC_SCENE3D_INCLUDE_H_
#define PPTX_LOGIC_SCENE3D_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "Camera.h"
#include "LightRig.h"
#include "Backdrop.h"

namespace PPTX
{
	namespace Logic
	{

		class Scene3d : public WrapperWritingElement
		{
		public:
			Scene3d();
			virtual ~Scene3d();
			explicit Scene3d(const XML::XNode& node);
			const Scene3d& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<Camera> camera;
			property<LightRig> lightRig;
			nullable_property<Backdrop>	backdrop;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SCENE3D_INCLUDE_H_