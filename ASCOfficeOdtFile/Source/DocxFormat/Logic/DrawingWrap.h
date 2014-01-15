#pragma once

#ifndef OOX_LOGIC_DRAWINGWRAP_INCLUDE_H_
#define OOX_LOGIC_DRAWINGWRAP_INCLUDE_H_

#include "property.h"
#include "nullable_property.h"
#include "./../WritingElement.h"
#include "./../Limit/TextWrapType.h"
#include "./../Limit/DrawingWrapType.h"
#include "WrapPath.h"
#include "EffectExtent.h"

namespace OOX
{
	namespace Logic
	{
		class DrawingWrap : public WritingElement
		{
		public:
			DrawingWrap();
			virtual ~DrawingWrap();
			explicit DrawingWrap(const XML::XNode& node);
			const DrawingWrap& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:

			property<std::string, Limit::DrawingWrapType>	Type;
			property<std::string, Limit::TextWrapType>		TextWrap;
			nullable_property<double>						DistT;
			nullable_property<double>						DistB;
			nullable_property<double>						DistL;
			nullable_property<double>						DistR;
			nullable_property<EffectExtent>					EffectExtent;
			nullable_property<WrapPath>						WrapPath;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DRAWINGWRAP_INCLUDE_H