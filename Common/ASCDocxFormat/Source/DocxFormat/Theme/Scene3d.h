#pragma once
#ifndef OOX_THEME_SCENE3D_INCLUDE_H_
#define OOX_THEME_SCENE3D_INCLUDE_H_

#include "./../WritingElement.h"
#include "Camera.h"
#include "LightRig.h"


namespace OOX
{
	namespace Theme
	{
		class Scene3d : public WritingElement
		{
		public:
			Scene3d();
			virtual ~Scene3d();
			explicit Scene3d(const XML::XNode& node);
			const Scene3d& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			Camera		m_camera;
			LightRig	m_lightRig;
		};
	} // namespace Theme
} // namespace OOXFileTheme

#endif // OOX_THEME_SCENE3D_INCLUDE_H_