#pragma once
#ifndef ODT_SETTINGS_FILE_INCLUDE_H_
#define ODT_SETTINGS_FILE_INCLUDE_H_

#include "./../File.h"
#include <string>
#include "Settings.h"


namespace Odt
{
	namespace Settings
	{
		class File : public Odt::File
		{
		public:
			File();
			virtual ~File();

		public:
			virtual void read(const boost::filesystem::wpath& path);
			virtual void write(const boost::filesystem::wpath& path) const;
			virtual const bool isValid(const boost::filesystem::wpath& path) const;

		public:
			static const bool exist(const boost::filesystem::wpath& path);

		private:
			std::string m_version;
			Settings		m_settings;
		};
	} // namespace Settings
} // namespace Odt

#endif // ODT_SETTING_FILE_INCLUDE_H_