#pragma once
#ifndef OOX_THEME_LIGHT_RIG_INCLUDE_H_
#define OOX_THEME_LIGHT_RIG_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "Rot.h"


namespace OOX
{
	namespace Theme
	{
		class LightRig : public WritingElement
		{
		public:
			LightRig();
			virtual ~LightRig();
			explicit LightRig(const XML::XNode& node);
			const LightRig& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_rig;
			std::string m_dir;
			Rot					m_rot;
		};
	} // namspace Theme
} // namespace OOX

#endif // OOX_THEME_LIGHT_RIG_INCLUDE_H_