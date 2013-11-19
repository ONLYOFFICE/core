#pragma once
#ifndef ODP_META_FILE_INCLUDE_H_
#define ODP_META_FILE_INCLUDE_H_

#include "OdtFormat/File.h"
#include "Statistic.h"
#include "UserDefined.h"
#include "nullable_property.h"

namespace Odp
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
			property<std::string> version;
			nullable_property<std::string> creation_date;
			nullable_property<std::string> editing_duration;
			nullable_property<std::string> editing_cycles;
			nullable_property<std::string> date;
			nullable_property<std::string> generator;
			nullable_property<Statistic> statistic;
			property<std::vector<UserDefined> > user_defined;
		};
	} // namespace Meta
} // namespace Odp

#endif // ODP_META_FILE_INCLUDE_H_