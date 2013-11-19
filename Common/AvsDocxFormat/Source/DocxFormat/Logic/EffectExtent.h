#pragma once
#ifndef OOX_LOGIC_EFFECT_EXTENT_INCLUDE_H_
#define OOX_LOGIC_EFFECT_EXTENT_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "./../Unit.h"


namespace OOX
{
	namespace Logic
	{
		class EffectExtent : public WritingElement
		{
		public:
			EffectExtent();
			virtual ~EffectExtent();			
			explicit EffectExtent(const XML::XNode& node);
			const EffectExtent& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		// TODO попробовать добавить тип Padding или Margin
		public:
			property<UnitSx> Left;
			property<UnitSx> Top;
			property<UnitSx> Right;
			property<UnitSx> Bottom;				
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_EFFECT_EXTENT_INCLUDE_H