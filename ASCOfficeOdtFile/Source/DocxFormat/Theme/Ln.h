#pragma once
#ifndef OOX_THEME_LN_INCLUDE_H_
#define OOX_THEME_LN_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "SolidFill.h"


namespace OOX
{
	namespace Theme
	{
		class Ln : public WritingElement
		{
		public:
			Ln();
			virtual ~Ln();
			explicit Ln(const XML::XNode& node);
			const Ln& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int						m_w;
			std::string		m_cap;
			std::string		m_cmpd;
			std::string		m_algn;
			SolidFill			m_solidFill;
			std::string		m_prstDash;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_LN_INCLUDE_H_