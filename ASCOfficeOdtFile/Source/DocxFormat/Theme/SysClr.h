#pragma once
#ifndef OOX_THEME_SYSCLR_INCLUDE_H_
#define OOX_THEME_SYSCLR_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>


namespace OOX
{
	namespace Theme
	{
		class SysClr : public WritingElement
		{
		public:
			SysClr();
			virtual ~SysClr();
			explicit SysClr(const XML::XNode& node);
			const SysClr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_value;
			std::string m_lastClr;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_SYSCLR_INCLUDE_H_