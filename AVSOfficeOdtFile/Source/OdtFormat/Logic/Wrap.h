#pragma once
#ifndef ODT_LOGIC_WRAP_INCLUDE_H_
#define ODT_LOGIC_WRAP_INCLUDE_H_

#include "Common.h"
#include <string>
#include "./../Namespaces.h"


namespace Odt
{
	namespace Logic
	{
		class Wrap : public Common::Wrap
		{
		public:
			Wrap();
			Wrap(const Common::Wrap& wrap);
			Wrap(const std::string& value);
			Wrap(const XML::XElement& element);
			const XML::XNode toXML(const Namespaces& ns) const;

			const Wrap& operator = (const Common::Wrap& wrap);
			const Wrap& operator = (const std::string& value);

		public:
			const std::string ToString() const;
			void fromString(const std::string& value);
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_WRAP_INCLUDE_H_