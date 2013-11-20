#pragma once
#ifndef OOX_THEME_EFFECT_STYLE_INCLUDE_H_
#define OOX_THEME_EFFECT_STYLE_INCLUDE_H_

#include "./../WritingElement.h"
#include "EffectLst.h"
#include "Scene3d.h"
#include "Sp3d.h"
#include "property.h"


namespace OOX
{
	namespace Theme
	{
		class EffectStyle : public WritingElement
		{
		public:
			EffectStyle();
			virtual ~EffectStyle();
			explicit EffectStyle(const XML::XNode& node);
			const EffectStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			EffectLst					m_effestLst;
			nullable<Scene3d>	m_scene3d;
			nullable<Sp3d>		m_sp3d;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_EFFECT_STYLE_INCLUDE_H_