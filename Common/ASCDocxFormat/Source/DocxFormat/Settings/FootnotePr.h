#pragma once
#ifndef OOX_SETTINGS_FOOTNOTE_PR_INCLUDE_H_
#define OOX_SETTINGS_FOOTNOTE_PR_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "Footnote.h"


namespace OOX
{
	namespace Settings
	{
		class FootnotePr : public WritingElement
		{
		public:
			FootnotePr();
			virtual ~FootnotePr();
			explicit FootnotePr(const XML::XNode& node);
			const FootnotePr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::vector<Footnote>	m_footnotes;		
		};	
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_FOOTNOTE_PR_INCLUDE_H_