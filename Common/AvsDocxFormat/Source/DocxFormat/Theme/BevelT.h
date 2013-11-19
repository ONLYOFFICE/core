#pragma once
#ifndef OOX_THEME_BEVELT_INCLUDE_H_
#define OOX_THEME_BEVELT_INCLUDE_H_

#include "./../WritingElement.h"


namespace OOX
{
	namespace Theme
	{
		class BevelT : public WritingElement
		{
		public:
			BevelT();
			virtual ~BevelT();
			explicit BevelT(const XML::XNode& node);
			const BevelT& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int m_w;
			int m_h;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_BEVELT_INCLUDE_H_