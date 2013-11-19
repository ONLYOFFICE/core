#pragma once
#ifndef PPTX_LOGIC_LIGHTRIG_INCLUDE_H_
#define PPTX_LOGIC_LIGHTRIG_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "Rot.h"
#include "./../Limit/LightRigType.h"
#include "./../Limit/RectAlign.h"

namespace PPTX
{
	namespace Logic
	{

		class LightRig : public WrapperWritingElement
		{
		public:
			LightRig();
			virtual ~LightRig();
			explicit LightRig(const XML::XNode& node);
			const LightRig& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<Rot> rot;

			property<std::string, Limit::RectAlign> dir;
			property<std::string, Limit::LightRigType> rig;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LIGHTRIG_INCLUDE_H_