#pragma once
#ifndef ODT_LOGIC_POSITION_INCLUDE_H_
#define ODT_LOGIC_POSITION_INCLUDE_H_

#include "Common.h"
#include <string>
#include "./../Namespaces.h"


namespace Odt
{
	namespace Logic
	{
		class Position : public Common::Position
		{
		public:
			Position();
			Position(const Common::Position& position);
			Position(const std::string& value);
			Position(const int type);
			Position(const XML::XElement& element);
			const XML::XNode toXML(const Namespaces& ns) const;

			const Position& operator = (const Common::Position& position);
			const Position& operator = (const std::string& value);
			//const Position& operator = (const Common::ZIndex& zIndex);

		public:
			const std::string ToString() const;
			void fromString(const std::string& value);
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_POSITION_INCLUDE_H_