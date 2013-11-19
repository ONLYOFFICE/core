#pragma once
#ifndef PPTX_LOGIC_CELL3D_INCLUDE_H_
#define PPTX_LOGIC_CELL3D_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Limit/Material.h"
#include "Bevel.h"
#include "LightRig.h"

namespace PPTX
{
	namespace Logic
	{
		class Cell3D : public WrapperWritingElement
		{
		public:
			Cell3D();
			virtual ~Cell3D();
			explicit Cell3D(const XML::XNode& node);
			const Cell3D& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string, Limit::Material> prstMaterial;

			nullable_property<Bevel> bevel;
			nullable_property<LightRig> lightRig;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CELL3D_INCLUDE_H_