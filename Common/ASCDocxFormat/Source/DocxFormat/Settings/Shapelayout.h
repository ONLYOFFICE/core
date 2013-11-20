#pragma once
#ifndef OOX_SETTINGS_SHAPE_LAYOUT_INCLUDE_H_
#define OOX_SETTINGS_SHAPE_LAYOUT_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "IdMap.h"
#include "Rules.h"


namespace OOX
{
	namespace Settings
	{
		class ShapeLayout : public WritingElement
		{
		public:
			ShapeLayout();
			virtual ~ShapeLayout();
			explicit ShapeLayout(const XML::XNode& node);
			const ShapeLayout& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string			m_ext;
			IdMap				m_idMap;
			Rules				m_rules;
		};
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_SHAPE_LAYOUT_INCLUDE_H_