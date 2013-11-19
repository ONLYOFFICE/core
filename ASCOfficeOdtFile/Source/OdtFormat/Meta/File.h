#pragma once
#ifndef ODT_META_FILE_INCLUDE_H_
#define ODT_META_FILE_INCLUDE_H_

#include "./../File.h"
#include "./../DateTime.h"
#include "property.h"
#include "nullable.h"
#include "Statistic.h"


namespace Odt
{
	namespace Meta
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

		public:
			property<Statistic>	Statistic;

		private:
			std::string							m_version;
			Odt::DateTime						m_creation;
			Odt::DateTime						m_date;
			std::string							m_editingDuration;
			int											m_editingCicles;
			nullable<std::string>		m_generation;
		};
	} // namespace Meta
} // namespace Odt

#endif // ODT_META_FILE_INCLUDE_H_