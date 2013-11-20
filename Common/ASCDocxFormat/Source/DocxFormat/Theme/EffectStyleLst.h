#pragma once
#ifndef OOX_THEME_EFFECT_STYLE_LST_INCLUDE_H_
#define OOX_THEME_EFFECT_STYLE_LST_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "EffectStyle.h"


namespace OOX
{
	namespace Theme
	{
		class EffectStyleLst : public WritingElement
		{
		public:
			EffectStyleLst();
			virtual ~EffectStyleLst();
			explicit EffectStyleLst(const XML::XNode& node);
			const EffectStyleLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::vector<EffectStyle> m_effectStyles;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_EFFECT_STYLE_LST_INCLUDE_H_