#pragma once
#ifndef OOX_SETTINGS_ZOOM_INCLUDE_H_
#define OOX_SETTINGS_ZOOM_INCLUDE_H_

#include "./../WritingElement.h"
#include "nullable.h"


namespace OOX
{
	namespace Settings
	{
		class Zoom : public WritingElement
		{
		public:
			Zoom();
			virtual ~Zoom();
			explicit Zoom(const XML::XNode& node);
			const Zoom& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			nullable__<std::string>	m_value;
			int										m_percent;
		};
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_ZOOM_INCLUDE_H_