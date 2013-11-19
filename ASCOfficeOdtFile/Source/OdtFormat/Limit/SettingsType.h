#pragma once
#ifndef ODT_LIMIT_SETTINGS_TYPE_INCLUDE_H_
#define ODT_LIMIT_SETTINGS_TYPE_INCLUDE_H_

#include "Setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class SettingsType : public setter::from<std::string>
		{
		public:
			SettingsType();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace List
} // namespace Odt

#endif // ODT_LIMIT_SETTINGS_TYPE_INCLUDE_H_