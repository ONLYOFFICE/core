#pragma once
#ifndef OOX_THEME_SRGBCLR_INCLUDE_H_
#define OOX_THEME_SRGBCLR_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "nullable.h"


namespace OOX
{
	namespace Theme
	{
		class SrgbClr : public WritingElement
		{
		public:
			SrgbClr();
			virtual ~SrgbClr();
			explicit SrgbClr(const XML::XNode& node);
			const SrgbClr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string			m_value;
			nullable<int>		m_alpha;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_SRGBCLR_INCLUDE_H_