#pragma once
#ifndef OOX_SETTINGS_MATH_PR_INCLUDE_H_
#define OOX_SETTINGS_MATH_PR_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "nullable.h"


namespace OOX
{
	namespace Settings
	{
		class MathPr : public WritingElement
		{
		public:
			MathPr();
			virtual ~MathPr();
			explicit MathPr(const XML::XNode& node);
			const MathPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string		m_mathFont;
			std::string		m_brkBin; 
			std::string		m_brkBinSub; 
			std::string		m_smallFrac; 
			nullable<std::string>	m_dispDef; 
			int						m_lMargin;
			int						m_rMargin; 
			std::string		m_defJc; 
			int						m_wrapIndent; 
			std::string		m_intLim; 
			std::string		m_naryLim;
		};	
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_MATH_PR_INCLUDE_H_