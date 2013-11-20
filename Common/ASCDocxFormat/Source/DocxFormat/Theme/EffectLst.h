#pragma once
#ifndef OOX_THEME_EFFECT_LST_INCLUDE_H_
#define OOX_THEME_EFFECT_LST_INCLUDE_H_

#include "./../WritingElement.h"
#include "OuterShdw.h"
#include "Scene3d.h"
#include "Sp3d.h"
#include "property.h"


namespace OOX
{
	namespace Theme
	{
		class EffectLst : public WritingElement
		{
		public:
			EffectLst();
			virtual ~EffectLst();
			explicit EffectLst(const XML::XNode& node);
			const EffectLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			OuterShdw					m_outerShdw;
			nullable<Scene3d> m_scene3d;
			nullable<Sp3d>		m_sp3d;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_EFFECT_LST_INCLUDE_H_