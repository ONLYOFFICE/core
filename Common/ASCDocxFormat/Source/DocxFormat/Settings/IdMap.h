#pragma once
#ifndef OOX_SETTINGS_ID_MAP_INCLUDE_H_
#define OOX_SETTINGS_ID_MAP_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>


namespace OOX
{
	namespace Settings
	{
		class IdMap : public WritingElement
		{
		public:
			IdMap();
			virtual ~IdMap();
			explicit IdMap(const XML::XNode& node);
			const IdMap& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_ext;
			int			m_data; 
		};
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_SHAPE_LAYOUT_INCLUDE_H_