#pragma once
#ifndef OOX_SETTINGS_ENDNOTE_INCLUDE_H_
#define OOX_SETTINGS_ENDNOTE_INCLUDE_H_

#include "./../WritingElement.h"


namespace OOX
{
	namespace Settings
	{
		class Endnote : public WritingElement
		{
		public:
			Endnote();
			virtual ~Endnote();
			explicit Endnote(const XML::XNode& node);
			const Endnote& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int m_id;			
		};	
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_ENDNOTE_INCLUDE_H_