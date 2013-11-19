#pragma once
#ifndef ODP_CONTENT_FILE_INCLUDE_H_
#define ODP_CONTENT_FILE_INCLUDE_H_

#include "OdtFormat/File.h"
#include "OdtFormat/Logic/NamedStyles.h"
#include "OdtFormat/Logic/ListStyles.h"
#include "Body.h"
#include "property.h"

namespace Odp
{
	namespace Content
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
			property<Odt::Logic::NamedStyles>	NamedStyles;
			property<Odt::Logic::ListStyles>	ListStyles;
			property<Body> body;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_FILE_INCLUDE_H_