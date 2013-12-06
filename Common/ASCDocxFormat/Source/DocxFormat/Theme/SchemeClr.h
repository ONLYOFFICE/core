#pragma once
#ifndef OOX_THEME_SCHEME_CLR_INCLUDE_H_
#define OOX_THEME_SCHEME_CLR_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "nullable.h"


namespace OOX
{
	namespace Theme
	{
		class SchemeClr : public WritingElement
		{
		public:
			SchemeClr();
			virtual ~SchemeClr();
			explicit SchemeClr(const XML::XNode& node);
			const SchemeClr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string				m_value;
			nullable__<int>			m_tint;
			nullable__<int>			m_shade;
			nullable__<int>			m_satMod;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_SCHEME_CLR_INCLUDE_H_