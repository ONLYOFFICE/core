#pragma once
#ifndef OOX_STYLES_SOLID_FILL_INCLUDE_H_
#define OOX_STYLES_SOLID_FILL_INCLUDE_H_

#include "./../WritingElement.h"
#include "SchemeClr.h"


namespace OOX
{
	namespace Theme
	{
		class SolidFill : public WritingElement
		{
		public:
			SolidFill();
			virtual ~SolidFill();
			explicit SolidFill(const XML::XNode& node);
			const SolidFill& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			SchemeClr m_schemeClr;
		};
	} // namespace Theme
} // OOX

#endif // OOX_STYLES_SOLID_FILL_INCLUDE_H_