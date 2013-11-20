#pragma once
#ifndef OOX_LOGIC_TEXT_INCLUDE_H_
#define OOX_LOGIC_TEXT_INCLUDE_H_

#include "RunItemBase.h"


namespace OOX
{
	namespace Logic
	{
		class Text : public RunItemBase
		{
		public:
			Text();
			virtual ~Text();
			explicit Text(const std::string& text);
			explicit Text(const XML::XNode& node);
			const Text& operator =(const XML::XNode& node);
			const Text& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		private:
			std::string m_text;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TEXT_INCLUDE_H_