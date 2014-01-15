#pragma once
#ifndef OOX_SETTINGS_FOOTNOTE_INCLUDE_H_
#define OOX_SETTINGS_FOOTNOTE_INCLUDE_H_

#include "./../WritingElement.h"


namespace OOX
{
	namespace Settings
	{
		class Footnote : public WritingElement
		{
		public:
			Footnote();
			virtual ~Footnote();
			explicit Footnote(const XML::XNode& node);
			const Footnote& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int	m_id;			
		};	
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_FOOTNOTE_INCLUDE_H_