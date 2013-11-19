#pragma once
#ifndef OOX_THEME_CAMERA_INCLUDE_H_
#define OOX_THEME_CAMERA_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "Rot.h"


namespace OOX
{
	namespace Theme
	{
		class Camera : public WritingElement
		{
		public:
			Camera();
			virtual ~Camera();
			explicit Camera(const XML::XNode& node);
			const Camera& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string	m_prst;
			Rot					m_rot;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_CAMERA_INCLUDE_H_