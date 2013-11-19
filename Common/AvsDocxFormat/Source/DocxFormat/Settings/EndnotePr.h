#pragma once
#ifndef OOX_SETTINGS_ENDNOTE_PR_INCLUDE_H_
#define OOX_SETTINGS_ENDNOTE_PR_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "Endnote.h"


namespace OOX
{
	namespace Settings
	{
		class EndnotePr : public WritingElement
		{
		public:
			EndnotePr();
			virtual ~EndnotePr();
			explicit EndnotePr(const XML::XNode& node);
			const EndnotePr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::vector<Endnote>	m_endnotes;		
		};	
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_ENDNOTE_PR_INCLUDE_H_