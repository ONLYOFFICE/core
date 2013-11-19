#pragma once
#ifndef OOX_STYLES_GS_INCLUDE_H_
#define OOX_STYLES_GS_INCLUDE_H_

#include "./../WritingElement.h"
#include "SchemeClr.h"


namespace OOX
{
	namespace Theme
	{
		class Gs : public WritingElement
		{
		public:
			Gs();
			virtual ~Gs();
			explicit Gs(const XML::XNode& node);
			const Gs& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int				m_pos;
			SchemeClr m_schemeClr;
		};
	} // namespace Theme
} // OOX

#endif // OOX_STYLES_GS_INCLUDE_H_