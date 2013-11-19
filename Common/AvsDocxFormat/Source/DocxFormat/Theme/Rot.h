#pragma once
#ifndef OOX_THEME_ROT_INCLUDE_H_
#define OOX_THEME_ROT_INCLUDE_H_

#include "./../WritingElement.h"


namespace OOX
{
	namespace Theme
	{
		class Rot : public WritingElement
		{
		public:
			Rot();
			virtual ~Rot();
			explicit Rot(const XML::XNode& node);
			const Rot& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int m_lat;
			int m_lon;
			int m_rev;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_ROT_INCLUDE_H_