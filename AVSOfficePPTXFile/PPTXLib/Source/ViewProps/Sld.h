#pragma once
#ifndef PPTX_VIEWPROPS_SLD_INCLUDE_H_
#define PPTX_VIEWPROPS_SLD_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/Orient.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Sld : public WrapperWritingElement
		{
		public:
			Sld();
			virtual ~Sld();
			explicit Sld(const XML::XNode& node);
			const Sld& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> id;
			nullable_property<bool> collapse;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_SLD_INCLUDE_H_