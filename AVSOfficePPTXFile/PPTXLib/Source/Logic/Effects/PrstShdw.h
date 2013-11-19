#pragma once
#ifndef PPTX_LOGIC_PRSTSHDW_INCLUDE_H_
#define PPTX_LOGIC_PRSTSHDW_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniColor.h"
#include "./../../Limit/PresetShadowVal.h"

namespace PPTX
{
	namespace Logic
	{

		class PrstShdw : public WrapperWritingElement
		{
		public:
			PrstShdw();
			virtual ~PrstShdw();
			explicit PrstShdw(const XML::XNode& node);
			const PrstShdw& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniColor Color;

			property<std::string, Limit::PresetShadowVal> prst;
			nullable_property<int, setter::between<int, 0, 21600000> > dir;
			nullable_property<int, setter::only_positive<int> > dist;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PRSTSHDW_INCLUDE_H_