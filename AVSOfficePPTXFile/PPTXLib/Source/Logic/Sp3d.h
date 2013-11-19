#pragma once
#ifndef PPTX_LOGIC_SP3D_INCLUDE_H_
#define PPTX_LOGIC_SP3D_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Limit/Material.h"
#include "Bevel.h"
#include "UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class Sp3d : public WrapperWritingElement
		{
		public:
			Sp3d();
			virtual ~Sp3d();
			explicit Sp3d(const XML::XNode& node);
			const Sp3d& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int, setter::only_positive<int> > contourW;
			nullable_property<int, setter::only_positive<int> > extrusionH;
			nullable_property<std::string, Limit::Material> prstMaterial;
			nullable_property<int> z;

			nullable_property<Bevel> bevelT;
			nullable_property<Bevel> bevelB;
			UniColor extrusionClr;
			UniColor contourClr;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SP3D_INCLUDE_H_