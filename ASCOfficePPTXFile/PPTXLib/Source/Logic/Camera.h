#pragma once
#ifndef PPTX_LOGIC_CAMERA_INCLUDE_H_
#define PPTX_LOGIC_CAMERA_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "Rot.h"
#include "./../Limit/CameraType.h"

namespace PPTX
{
	namespace Logic
	{

		class Camera : public WrapperWritingElement
		{
		public:
			Camera();
			virtual ~Camera();
			explicit Camera(const XML::XNode& node);
			const Camera& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<Rot> rot;

			property<std::string, Limit::CameraType> prst;
			nullable_property<int, setter::between<int, 0, 10800000> > fov;
			nullable_property<int, setter::only_positive<int> > zoom;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CAMERA_INCLUDE_H_